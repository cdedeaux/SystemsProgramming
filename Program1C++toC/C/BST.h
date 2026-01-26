#pragma once
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct Node{
  char *value;
  struct Node *left;
  struct Node *right;
};

struct BST{
  struct Node *root;
};

void BST_init(struct BST *tree);
int BST_insert(struct BST *tree, const char *value);
int BST_search(struct BST *tree, const char *value);
int BST_remove(struct BST *tree, const char *value);
void BST_postOrder(struct BST *tree);
void BST_destroy(struct BST *tree);

int Node_insert(struct Node **node, const char *value);
int Node_search(struct Node *node, const char *value);
int Node_remove(struct Node **node, const char *value);
void Node_postOrder(struct Node *node);
void Node_dealloc(struct Node **node);