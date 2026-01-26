#include "BST.h"

void BST_init(struct BST *tree){
  tree->root = NULL;
}

int BST_search(struct BST *tree, const char *value) { 
  return Node_search(tree->root, value);
}

int BST_remove(struct BST *tree, const char *value) {
  return Node_remove(&tree->root, value);
}

int BST_insert(struct BST *tree, const char *value){
  Node_insert(&tree->root, value);
}

void BST_postOrder(struct BST *tree){
  Node_postOrder(tree->root);
}

void BST_destroy(struct BST *tree){
  Node_dealloc(&tree->root);
}

int Node_insert(struct Node **node, const char *value){
  if (*node == NULL){
    *node = malloc(sizeof(struct Node));
    (*node)->value = malloc(strlen(value) + 1);
    strcpy((*node)->value, value);
    (*node)->left = NULL;
    (*node)->right = NULL;
    return 1;
  }
  
  if (strcmp(value, (*node)->value) < 0)
    return Node_insert(&(*node)->left, value);
  
  if (strcmp(value, (*node)->value) > 0)
    return Node_insert(&(*node)->right, value);
  
  return 0;
}

int Node_search(struct Node *node, const char *value)
{
  if (node == NULL){
      return 0;
  }
  if (strcmp(value, node->value) < 0) {return Node_search(node->left, value);}
  if (strcmp(value, node->value) > 0) {return Node_search(node->right, value);}
  return 1;
}

int Node_remove(struct Node **node, const char *value){
  return 1;
}

void Node_dealloc(struct Node **node){
  if (*node == NULL) return;
  Node_dealloc(&(*node)->left);
  Node_dealloc(&(*node)->right);
  free((*node)->value);
  free(*node);
  *node = NULL;
}

void Node_postOrder(struct Node *node){
  if (node == NULL) return;
  
  Node_postOrder(node->left);
  Node_postOrder(node->right);
  printf("%s ", node->value);
}

void Node_destroy(struct Node **node){
  if (*node == NULL) return;
  
  Node_destroy(&(*node)->left);
  Node_destroy(&(*node)->right);
  free((*node)->value);
  free(*node);
  *node = NULL;
}