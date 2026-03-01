#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <stdbool.h>

// TREE NODE
struct Node{

    int value;
    int size;
    struct Node* left;
    struct Node* right;
};





int get_size(struct Node* x){
    return x ? x->size: 0;

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

struct Node* build_balanced_tree(struct Node** list, int left, int right){

    if (left > right){
        return NULL;
    }
    int middle = (left + right)/2;
    struct Node* root = list[middle];
    root->left =  build_balanced_tree(list, left, middle - 1);
    root->right =  build_balanced_tree(list, middle + 1, right);

    root->size = 1 + get_size(root->left) + get_size(root->right);

    return root;

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
    if(is_balanced(root, 1)){
        return root;
    }

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

    if (is_star_bst(root)){
        root = rebalance_insert(root);
    }

    return root;
}

void inorder_traversal_tree_print(struct Node* root){
    if(!root){\
        return;
    }

    inorder_traversal_tree_print(root->left);
  
    printf("%d ", root->value);
    

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
   
        printf("%d\n", root->value);
    
    print_tree(root->left, level + 1);
    print_tree(root->right, level + 1);
}


void free_tree(struct Node* root) {
  if (!root) return;
  free_tree(root->left);
  free_tree(root->right);
  free(root);
}

int main(void){
    struct Node* root = NULL;
    int values[] = {8,7,9,1,5,2,6,3,4,0};
    int val_size = (int)(sizeof(values)/sizeof(values[0]));

    for(int i = 0; i < val_size; i++){
        root = lazy_insert(root, values[i]);
    }

    printf("Final LBST:\n");
    print_tree(root, 0);
    free_tree(root);

    return 0;
}

