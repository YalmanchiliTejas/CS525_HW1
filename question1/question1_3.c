#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <stdbool.h>

// TREE NODE
struct Node{

    int value;
    int size;
    int marked;
    int count;
    struct Node* left;
    struct Node* right;
};





int get_size(struct Node* x){
    return x ? x->size: 0;

}

int get_marked_count(struct Node* x){
    return x ? x->count: 0;

}
int get_unmarked_count(struct Node* x){
    if(!x){
        return 0;
    }
    return (!x->marked) + get_unmarked_count(x->left) + get_unmarked_count(x->right);
}






struct Node* new_node(int value){

    struct Node* node = (struct Node*) malloc(sizeof(struct Node));

    if(node == NULL){
        printf("The error is the malloc issue");
        exit(1);

    }
    node->value = value;
    node->size = 1;
    node->left = NULL;
    node->right = NULL;
    node->count = 0;
    node->marked = 0;

    return node;

}

bool is_star_bst(struct Node* root){


    if (root == NULL){
        return false;
    }

    int left_size = get_size(root->left);
    int right_size = get_size(root->right);

    int root_size = root->size;

    int difference = abs(left_size - right_size);

    return ( difference >= root_size / 3);
}



void traverse_inorder(struct Node * root, struct Node** list, int*index){

    if (root == NULL){
        return;

    }

    traverse_inorder(root->left, list, index);

    list[(*index)++] = root;

    traverse_inorder(root->right, list, index);
}
void traverse_inorder_unmarked(struct Node * root, struct Node** list, int*index){

    if (root == NULL){
        return;

    }



    traverse_inorder_unmarked(root->left, list, index);
    if (!root->marked){
    list[(*index)++] = root;
    }
    traverse_inorder_unmarked(root->right, list, index);
}


void free_tree(struct Node* root) {
  if (!root) return;
  free_tree(root->left);
  free_tree(root->right);
  free(root);
}


void free_marked_nodes(struct Node* root){
    if(!root){
        return;
    }
    free_marked_nodes(root->left);
    free_marked_nodes(root->right);
    if(root->marked){
        free(root);
    }
}


struct Node* build_balanced_tree(struct Node** list, int left, int right){

    if (left > right){
        return NULL;
    }
    int middle = (left + right)/2;
    struct Node* root = list[middle];
    root->left =  build_balanced_tree(list, left, middle - 1);
    root->right =  build_balanced_tree(list, middle + 1, right);

    root->size = 1 + get_size(root->left) + get_size(root->right);
    root->count = root->marked + get_marked_count(root->left) + get_marked_count(root->right);

    return root;

}
struct Node* rebalance_delete(struct Node* root){
    if(!root){
        return NULL;
    }
    int keep = get_unmarked_count(root);

    if(!keep){
        free_tree(root);
        return NULL;
    }
    struct Node** arr = (struct Node**) malloc((size_t) keep * sizeof(struct Node*));
    if (!arr){
        printf("error in the malloc here\n");
        exit(1);
    }


    int index = 0;
    traverse_inorder_unmarked(root, arr, &index);
    free_marked_nodes(root);

    for(int i = 0; i < keep; i++){
        arr[i]->marked = 0;
        arr[i]->count = 0;
        arr[i]->right = NULL;
        arr[i]->left = NULL;
        arr[i]->size = 1;
    }

    struct Node* new_root = build_balanced_tree(arr, 0 , keep - 1);
    free(arr);
    return new_root;

}

bool is_balanced(struct Node* root, int c){

    if (root == NULL){
        return true;
    }

    int left = get_size(root->left);
    int right = get_size(root->right);

    int difference = abs(left - right);
    return difference <= c;
}

struct Node* rebalance_insert(struct Node* root){

    int nodes = root->size;

   

    struct Node** arr = (struct Node**) malloc((size_t) nodes * sizeof(struct Node*));
    if(arr == NULL){
        printf("The issue is in the rebalance_malloc");
        exit(1);
    }
    int index = 0;

    traverse_inorder(root, arr, &index);

    for(int i = 0; i < nodes; i++){
        arr[i]->left = NULL;
        arr[i]->right = NULL;
    }
    struct Node* new_root = build_balanced_tree(arr, 0, nodes - 1);
    free(arr);
    return new_root;
}



struct Node* lazy_insert(struct Node* root, int val){

    if (root == NULL){
        return new_node(val);
    }

    if (root->value >= val){
        root->left = lazy_insert(root->left, val);

    } else{
        root->right = lazy_insert(root->right, val);
    }
    root->size = 1 + get_size(root->left) + get_size(root->right);
    
    root->count = root->marked + get_marked_count(root->left) + get_marked_count(root->right);
    if (is_star_bst(root) && !is_balanced(root, 1)){
        root = rebalance_insert(root);
    }

    return root;
}
struct Node* lazy_delete(struct Node* root, int val){
    if(!root){
        return NULL;
    }
    if (root->value > val){
        root->left = lazy_delete(root->left, val);
    } else if (root->value < val){
        root->right = lazy_delete(root->right, val);
    }else{
        root->marked = 1;
    }
    root->size = 1 + get_size(root->left) + get_size(root->right);
    root->count = root->marked + get_marked_count(root->left) + get_marked_count(root->right);



    if (root->count * 2 > root->size){
        root = rebalance_delete(root);
    }
    return root;
}

void inorder_traversal_tree_print(struct Node* root){
    if(!root){\
        return;
    }

    inorder_traversal_tree_print(root->left);
    if (root->marked){
        printf("*%d ", root->value);
    } else{
        printf("%d ", root->value);
    }

    inorder_traversal_tree_print(root->right);
}
void print_tree(struct Node* root, int level){
    if(root == NULL){
        return;
    }
    printf("|");
    for (int i = 0; i < level; i++){
        printf("-");
    }
    if(root->marked){
        printf("*%d\n", root->value);
    } else{
        printf("%d\n", root->value);
    }
    print_tree(root->left, level + 1);
    print_tree(root->right, level + 1);
}
bool look_up(struct Node*root, int val){
    if(!root){
        return false;
    }
    if (root->value == val){
        return !root->marked;
    } else if (root->value > val){
        return look_up(root->left, val);
    } else{
        return look_up(root->right, val);
}
}
int main(void){
    struct Node* root = NULL;

    int values[] = {8,7,9,1,5,2,6,3,4,0};
    int val_size = (int)(sizeof(values)/sizeof(values[0]));

    for(int i = 0; i < val_size; i++){
        root = lazy_insert(root, values[i]);
    }

    // Print after all inserts
    printf("After inserts:\n");
    int n;
    print_tree(root, 0);
    
    // Deletion order
    int dels[] = {7, 0, 6, 2, 10};
    int del_size = (int)(sizeof(dels)/sizeof(dels[0]));

    for(int i = 0; i < del_size; i++){
        int x = dels[i];
        root = lazy_delete(root, x);

        printf("\nAfter delete %d:\n", x);

        printf("Inorder (marked with *): ");
        print_tree(root, 0);
   
    }
    int looks[] = {1, 6, 10, 5, 8, 9};
    int look_size = (int)(sizeof(looks)/sizeof(looks[0]));

    printf("\nLookup results:\n");
    for (int i = 0; i < look_size; i++){
        int x = looks[i];
        printf("look_up %d: %s\n", x, look_up(root, x) ? "true" : "false");
    }
    free_tree(root);
    return 0;
}

