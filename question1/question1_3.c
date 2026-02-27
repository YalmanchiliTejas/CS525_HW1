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

    return (3 * difference >= root_size);
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

static int height(struct Node* r) {
  if (!r) return 0;
  int hl = height(r->left);
  int hr = height(r->right);
  return 1 + (hl > hr ? hl : hr);
}

static int pow2_int(int e) {
  int x = 1;
  while (e-- > 0) x <<= 1;
  return x;
}

static void fill_heap_array(struct Node* root, int* arr, int idx, int max_size) {
  if (!root || idx >= max_size) return;

  arr[idx] = root->value;
  fill_heap_array(root->left,  arr, 2*idx + 1, max_size);
  fill_heap_array(root->right, arr, 2*idx + 2, max_size);
}

// Returns heap-style array with -1 for missing nodes.
// out_n = array length (= 2^h - 1).
static int* tree_to_heap_array(struct Node* root, int* out_n) {
  *out_n = 0;
  if (!root) return NULL;

  int h = height(root);
  int n = pow2_int(h) - 1;

  int* arr = (int*)malloc((size_t)n * sizeof(int));
  if (!arr) { perror("malloc"); exit(1); }

  for (int i = 0; i < n; i++) arr[i] = -1;

  fill_heap_array(root, arr, 0, n);

  *out_n = n;
  return arr;
}


static int levels_from_size(int n) {
  // smallest h such that (2^h - 1) >= n
  int h = 0;
  int total = 0;
  while (total < n) {
    h++;
    total = (1 << h) - 1;
  }
  return h;
}

static void print_spaces(int k) {
  for (int i = 0; i < k; i++) putchar(' ');
}

// Prints a heap-indexed array (size = 2^h - 1 recommended) as a pyramid.
// Missing nodes should be -1.
static void print_tree_from_heap_array(const int* arr, int size) {
  if (!arr || size <= 0) {
    printf("(empty)\n");
    return;
  }

  int h = levels_from_size(size);
  int idx = 0;

  for (int level = 0; level < h; level++) {
    int nodes_this_level = 1 << level;

    // spacing (tuned for single-/double-digit values; OK for this assignment)
    int first_pad   = (1 << (h - level - 1)) * 2 - 1;
    int between_pad = (1 << (h - level)) * 2 - 1;

    print_spaces(first_pad);

    for (int j = 0; j < nodes_this_level; j++) {
      int v = (idx < size) ? arr[idx] : -1;

      if (v == -1) {
        putchar(' ');
      } else {
        printf("%d", v);
      }

      idx++;
      if (j != nodes_this_level - 1) print_spaces(between_pad);
    }
    putchar('\n');

    if (idx >= size) break;
  }
}

void print_inorder_with_marks(struct Node* root){
    if(!root) return;
    print_inorder_with_marks(root->left);
    printf("%d%s ", root->value, root->marked ? "*" : "");
    print_inorder_with_marks(root->right);
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
    int* a = tree_to_heap_array(root, &n);
    for(int i = 0; i < n; i++) printf("%d ", a[i]);
    printf("\n");
    print_tree_from_heap_array(a, n);
    free(a);

    // Deletion order
    int dels[] = {7, 0, 6, 2, 10};
    int del_size = (int)(sizeof(dels)/sizeof(dels[0]));

    for(int i = 0; i < del_size; i++){
        int x = dels[i];
        root = lazy_delete(root, x);

        printf("\nAfter delete %d:\n", x);

        printf("Inorder (marked with *): ");
        print_inorder_with_marks(root);
        printf("\n");
        int m;
        int* b = tree_to_heap_array(root, &m);
        for(int j = 0; j < m; j++) printf("%d ", b[j]);
        printf("\n");
        print_tree_from_heap_array(b, m);
        free(b);
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

