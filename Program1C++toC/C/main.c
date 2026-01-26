#include <string.h>
#include "BST.h"

int main(){
  struct BST store;
  char temp[100];


  printf("Please enter a series of words to store in a BST\n");
  printf("Type 'STOP' to end the series and output the BST\n");
  printf("In postorder\n");

  BST_init(&store);
  while (1) {
    printf("Word = ");
    scanf("%s", temp);
    if (strcmp(temp, "STOP") == 0) break;
    char *newWord = malloc(strlen(temp) + 1);
    if (newWord == NULL) return 1;
    strcpy(newWord, temp);

    BST_insert(&store, newWord);
  }
  
  printf("The BST ouput in postorder is \n");
  BST_postOrder(&store);
  printf("\n");

  BST_destroy(&store);

  return 0;
}
