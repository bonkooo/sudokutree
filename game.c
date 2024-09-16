#include "game.h"

stack *inicijalizujStek() {
    stack *stek = (stack *)malloc(sizeof(stack));
    stek->sp = NULL;
    return stek;
}

TreeNode *kreirajCvor(int** matrix){
    TreeNode *cvor = (TreeNode*)malloc(sizeof(TreeNode));
    cvor->childrenSize = 0;
    cvor->children = NULL;
    cvor->info = matrix;
    cvor->dubina = 0;
    cvor->dobarPut = 0;
    cvor->otac = NULL;
    return cvor;
}

int** alocirajMatricuStanja(int n){
    int **matrix = (int**)malloc(n * sizeof(int*));
    for (int i = 0; i < n; i++){
        *(matrix + i) = (int*)malloc(n * sizeof(int));
    }
    return matrix;
}

void push(stack *stek, TreeNode *cvor) {
    stackElement *p = (stackElement *)malloc(sizeof(stackElement));
    if (!p) {
        printf("Greska u memorijskoj aloakaciji\n");
        return;
    }

    p->node = cvor;
    p->next = stek->sp;
    stek->sp = p;
}

int stack_empty(stack *stek){
    return stek->sp == NULL;
}

TreeNode *pop(stack *stek) {
    if (stack_empty(stek)) {
        return NULL;
    }
    stackElement *p = stek->sp;
    TreeNode *x = p->node;
    stek->sp = p->next;
    free(p);
    return x;
}

void inicijalizujMatricu(int **matrix, int n){
    printf("Unosite elemente matrice red po red\n");
    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
            scanf("%d", &matrix[i][j]);
        }
    }
}

int proveraValidneMatrice(int **matrix, int n){
    int velicinaBloka = (int)sqrt(n);
    for(int i = 0; i < n; i++){
        int niz[LEN] = {0};
        for(int j = 0; j < n; j++){
            int num = matrix[i][j];
            if (num != 0) {
                if (niz[num - 1] == 1) {
                    return 0;
                }
                niz[num - 1] = 1;
            }
        }
    }

    for(int j = 0; j < n; j++){
        int niz[LEN] = {0};
        for(int i = 0; i < n; i++){
            int num = matrix[i][j];
            if (num != 0) {
                if (niz[num - 1] == 1) {
                    return 0;
                }
                niz[num - 1] = 1;
            }
        }
    }

    for(int i = 0; i < n; i += velicinaBloka){
        for(int j = 0; j < n; j += velicinaBloka){
            int niz[LEN] = {0};
            for(int m = 0; m < velicinaBloka; m++){
                for(int n = 0; n < velicinaBloka; n++){
                    int num = matrix[i+m][j+n];
                    if (num != 0) {
                        if (niz[num - 1] == 1) {
                            return 0;
                        }
                        niz[num - 1] = 1;
                    }
                }
            }
        }
    }
    return 1;
}

void dodajDete(stack* stek, TreeNode* curr, int n, int** novoStanje) {
    if (proveraValidneMatrice(novoStanje, n)) {
        TreeNode* novoDete = kreirajCvor(novoStanje);
        int childNum = curr->childrenSize;
        TreeNode** childArr = curr->children;
        novoDete->otac = curr;
        childNum++;
        childArr = (TreeNode**)realloc(childArr, childNum * sizeof(TreeNode*));
        if (!childArr) {
            printf("Greska u alokaciji memorije.\n");
            return;
        }
        childArr[childNum - 1] = novoDete;
        curr->children = childArr;
        curr->childrenSize = childNum;
        push(stek, novoDete);
    }
}

void razvijStanja(stack* stek, TreeNode* curr, int n){
    int** trenutnaMatrica = curr->info;
    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
            if (trenutnaMatrica[i][j] == 0){
                for(int val = 1; val <= n; val++){
                    int** novoStanje = alocirajMatricuStanja(n);
                    for (int p = 0; p < n; p++){
                        for (int q = 0; q < n; q++){
                            novoStanje[p][q] = trenutnaMatrica[p][q];
                        }
                    }
                    novoStanje[i][j] = val;
                    dodajDete(stek, curr, n, novoStanje);
                }
            }
        }
    }
}

// Other functions go here...

