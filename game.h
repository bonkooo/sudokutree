#ifndef GAME_H
#define GAME_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX 100
#define LEN 10

typedef struct treeNode{
    int** info;
    int childrenSize;
    struct treeNode** children;
    int dubina;
    int dobarPut;
    struct treeNode *otac;
} TreeNode;

typedef struct tree{
    TreeNode *root;
} Tree;

typedef struct stackElement{
    TreeNode  *node;
    struct stackElement *next;
} stackElement;

typedef struct stack{
    stackElement *sp;
} stack;

// Function declarations
stack *inicijalizujStek();
TreeNode *kreirajCvor(int** matrix);
int** alocirajMatricuStanja (int n);
void push(stack *stek, TreeNode *cvor);
int stack_empty(stack *stek);
TreeNode *pop(stack *stek);
void inicijalizujMatricu(int **matrix, int n);
int proveraValidneMatrice(int **matrix, int n);
void dodajDete(stack* stek, TreeNode* curr, int n, int** novoStanje);
void razvijStanja(stack* stek, TreeNode* curr, int n);
void kreirajStablo(int** inicijalnaMatrica, Tree *tree, int n);
void ispisStabla(Tree* tree, int n);
int proveriDobarPut(int **trenutnoStanje, Tree* tree, int n);
void ispisMatrice(int **matrix, int n);
void odigrajPotez(int **matrix, int n, int i, int j, int num);
int proveriResenje(int** matrica, int n);
int istaMatrica(int **matrica1, int **matrica2, int n);
int proveriDuplikate(int **resenja[MAX], int **trenutnaMatrica, int len, int n);
void ispisiResenja(int **trenutnoStanje, Tree* tree, int n);
void Pomoc(int** trenutnoStanje, Tree* tree, int n);
void predloziPotez (int** trenutnoStanje, Tree* tree, int n);

#endif
