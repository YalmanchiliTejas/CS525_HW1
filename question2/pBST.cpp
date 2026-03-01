#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#define MAX_THREAD_COUNT 2
//for workers
enum class op_t{
  OP_INSERT,
  OP_DELETE,
  OP_LOOKUP,
};

struct op_v{
  op_t typ;
  int val;
};


// TREE NODE
struct Node{

    int value;
    struct Node* left;
    struct Node* right;
    pthread_mutex_t lock;
};
//initialisation
Node* root = NULL;
pthread_mutex_t root_l = PTHREAD_MUTEX_INITIALIZER;
int thread_c = 0;
pthread_mutex_t act_l = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t act_cond = PTHREAD_COND_INITIALIZER;

//helpers
Node* new_node(int value){
    Node* node = (struct Node*) malloc(sizeof(Node));

    if(node == NULL){
        printf("The error is the malloc issue");
        exit(1);

    }
    node->value = value;
    node->left = NULL;
    node->right = NULL;
    pthread_mutex_init(&node->lock,NULL);
    return node;

}
void free_tree(Node* root) {
  if (!root) return;
  free_tree(root->left);
  free_tree(root->right);
  pthread_mutex_destroy(&root->lock);
  free(root);
}

void print_tree(Node* root, int l){
  if(!root) return;
  printf("|");
  for(int i = 0; i<l;i++) printf("-");
  printf("%d\n",root->value);
  if(root->left)printf("L");
  print_tree(root->left,l+1);
  if(root->right)printf("R");
  print_tree(root->right, l+1);
}

//2.1 parallel insert
void pinsert(int val){
  pthread_mutex_lock(&root_l);
  if(root ==NULL){
    root= new_node(val);
    pthread_mutex_unlock(&root_l);
    return;
  }
  Node* curr = root;
  pthread_mutex_lock(&curr->lock);
  pthread_mutex_unlock(&root_l);

  while(true){
    if(val<curr->value){
      if(curr->left==NULL){
        curr->left = new_node(val);
        pthread_mutex_unlock(&curr->lock);
        return;
      } else{
        Node* next = curr->left;
        pthread_mutex_lock(&next->lock);
        pthread_mutex_unlock(&curr->lock);
        curr= next;
      }
    } else if (val > curr->value){
      if(curr->right == NULL){
        curr->right = new_node(val);
        pthread_mutex_unlock(&curr->lock);
        return;
      } else{
        Node* next = curr->right;
        pthread_mutex_lock(&next->lock);
        pthread_mutex_unlock(&curr->lock);
        curr= next;
      }
    } else{
      pthread_mutex_unlock(&curr->lock);
      return;
    }
  }
}

//2.2 parallel delete
void set_ch(Node* parent, Node* old_ch, Node* new_ch){
  if(!parent) return;
  if(parent->left == old_ch) parent->left = new_ch;
  else if(parent->right == old_ch) parent->right = new_ch;
}
int pdelete(int val){
  pthread_mutex_lock(&root_l);
  Node* parent = NULL;
  Node* curr = root;
  if(curr) pthread_mutex_lock(&curr->lock);
  pthread_mutex_unlock(&root_l);
  while(curr && curr->value != val){
    Node *next;
    if(val <curr->value){
      next= curr->left;
    } else{
      next = curr->right;
    }
    if(next) pthread_mutex_lock(&next->lock);
    if(parent) pthread_mutex_unlock(&parent->lock);
    parent = curr;
    curr= next;

  }
  if(!curr){
    if(parent) pthread_mutex_unlock(&parent->lock);
    return 0;
  }
  //2 ch
  if(curr->left && curr->right){
    Node *succ_p = curr;
    Node* succ = curr->right;
    pthread_mutex_lock(&succ->lock);
    while(succ->left){
      Node* next = succ->left;
      pthread_mutex_lock(&next->lock);
      if(succ_p!=curr) pthread_mutex_unlock(&succ_p->lock);
      succ_p = succ;
      succ=next;
    }
    curr->value = succ->value;
    Node * ch = succ->right;
    if(succ_p == curr) curr->right = ch;
    else set_ch(succ_p,succ,ch);
    pthread_mutex_unlock(&curr->lock);
    pthread_mutex_unlock(&succ_p->lock);
    pthread_mutex_destroy(&succ->lock);
    free(succ);
    if(parent) pthread_mutex_unlock(&parent->lock);
    return 1;
  }
  //1 or no ch
  Node * ch = curr->left ? curr->left:curr->right;
  pthread_mutex_lock(&root_l);
  if(!parent){
    root = ch;
    pthread_mutex_unlock(&root_l);
    pthread_mutex_unlock(&curr->lock);
    pthread_mutex_destroy(&curr->lock);
    free(curr);
    return 1;
  } else{
    pthread_mutex_unlock(&root_l);
    set_ch(parent, curr, ch);
    pthread_mutex_unlock(&curr->lock);
    pthread_mutex_unlock(&parent->lock);
    pthread_mutex_destroy(&curr->lock);
    free(curr);
    return 1;

  }
}

//2.3 parallel lookup
bool plookup(int val){
  pthread_mutex_lock(&root_l);
  Node* curr = root;
  if(curr) pthread_mutex_lock(&curr->lock);
  pthread_mutex_unlock(&root_l);

  while(curr){
    if(val==curr->value){
      pthread_mutex_unlock(&curr->lock);
      return true;
    } else if(val<curr->value){
      Node* next = curr->left;
      if(next) pthread_mutex_lock(&next->lock);
      pthread_mutex_unlock(&curr->lock);
      curr=next;
    } else{
      Node* next = curr->right;
      if(next) pthread_mutex_lock(&next->lock);
      pthread_mutex_unlock(&curr->lock);
      curr=next;

    }
  }
  return false;
}
//workers
void *worker(void * arg){
  op_v * ov = (op_v*) arg;
  if(ov->typ == op_t::OP_INSERT){
    pinsert(ov->val);
  } else if(ov->typ == op_t::OP_DELETE){
    pdelete(ov->val);
  } else{
    bool ans = plookup(ov->val);
    printf("lookup for %d is %d\n",ov->val,ans);
  }
  free(ov);
  pthread_mutex_lock(&act_l);
  thread_c--;
  pthread_cond_signal(&act_cond);
  pthread_mutex_unlock(&act_l);
  return NULL;
}

void start_t(op_t typ, int val){
  pthread_t t;
  op_v* ov = (op_v*)malloc(sizeof(op_v));
  ov->typ = typ;
  ov->val = val;
  pthread_mutex_lock(&act_l);
  while(thread_c == MAX_THREAD_COUNT){
    pthread_cond_wait(&act_cond, &act_l);
  }
  thread_c++;
  pthread_mutex_unlock(&act_l);
  pthread_create(&t,NULL, worker, ov);
  pthread_detach(t);

}

int main(){

  //2.1
  int ins[] ={5,2,8,1,9,3,6,7,4,10};
  for (int i = 0 ;i<10;i++){
    start_t(op_t::OP_INSERT,ins[i]);
  }
  pthread_mutex_lock(&act_l);
  while(thread_c>0) pthread_cond_wait(&act_cond,&act_l);
  pthread_mutex_unlock(&act_l);
  printf("inserting 10 elements for 2.1\n");
  print_tree(root,0); 

  //2.2
  int del[] = {7,0,6,2,10};
  for(int i = 0; i<5;i++){
    start_t(op_t::OP_DELETE,del[i]);
  }
  pthread_mutex_lock(&act_l);
  while(thread_c>0) pthread_cond_wait(&act_cond,&act_l);
  pthread_mutex_unlock(&act_l);
  printf("del 5 elements for 2.2\n");
  print_tree(root,0); 

  int look[] ={1,6,10,5,8,0};
  for(int i = 0; i<6;i++){
    start_t(op_t::OP_LOOKUP,look[i]);
  }
  pthread_mutex_lock(&act_l);
  while(thread_c>0) pthread_cond_wait(&act_cond,&act_l);
  pthread_mutex_unlock(&act_l);
  free_tree(root);
  return 0;
}
