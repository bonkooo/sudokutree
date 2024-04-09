#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdlib.h>
#define LEN 10

typedef struct treeNode{
    int** info;
    int childrenSize;
    struct treeNode** children;
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

    return cvor;
}

int** alocirajMatricuStanja (int n){
    int **matrix = (int**)malloc(n * sizeof(int*));
    for (int i = 0; i < n; i++){
        *(matrix + i) = (int*)malloc(n * sizeof(int));
    }
    return matrix;
}

void dealocirajMatricuStanja (int** matrix, int n){
    for (int i = 0; i < n; i++){
        free(matrix[i]);
    }
    free(matrix);
}

void push(stack *stek, struct treeNode *cvor) {
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
    if (stek->sp == NULL) {
        return 1;
    }
    else{
        return 0;
    }
}

struct treeNode *pop(stack *stek) {
    if (stack_empty(stek)) {
        printf("Underflow\n");
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

    // Provera redova
    for(int i = 0; i < n; i++){
        int niz[LEN] = {0}; // Resetovanje niza
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

    // Provera kolona
    for(int j = 0; j < n; j++){
        int niz[LEN] = {0}; // Resetovanje niza
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

    // Provera blokova
    for(int i = 0; i < n; i += velicinaBloka){
        for(int j = 0; j < n; j += velicinaBloka){
            int niz[LEN] = {0}; // Resetujemo niz za svaki blok
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

void dodajDete(stack* stek, struct treeNode* curr, int n, int** novoStanje) {
    if (proveraValidneMatrice(novoStanje, n)) {
        struct treeNode* novoDete = kreirajCvor(novoStanje);
        int childNum = curr->childrenSize;
        struct treeNode** childArr = curr->children;
        childNum++;
        childArr = (struct treeNode**)realloc(childArr,  childNum * sizeof(struct treeNode*));
        if (!childArr) {
            printf("Greska u alokaciji memorije. \n");
            return;
        }
        childArr[childNum - 1] = novoDete;
        curr->children = childArr;
        curr->childrenSize = childNum;

        push(stek, novoDete);
    }
}


void razvijStanja(stack* stek, struct treeNode* curr, int n){
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
                    dealocirajMatricuStanja(novoStanje, n);
                }
            }
        }
    }
}


void kreirajStablo(int** inicijalnaMatrica, struct tree *tree, int n){
    stack *stek = inicijalizujStek();
    tree->root = kreirajCvor(inicijalnaMatrica);
    push(stek, tree->root);

    while(!stack_empty(stek)){
        TreeNode *curr = pop(stek);
        if (curr != NULL)
        razvijStanja(stek, curr, n);
    }

}


void ispisMatrice(int **matrix, int n){
    for (int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            if (j != n - 1){
                printf("%d ", matrix[i][j]);
            }
            else{
                printf("%d", matrix[i][j]);
            }
        }
        putchar('\n');
    }
}

void odigrajPotez(int **matrix, int n, int i, int j, int num){
    if(i > n || j > n || num > n || i < 0 || j < 0 || num < 0){
        printf("Pozicija ili broj nisu validni!\n");
    }
    else if (matrix[i][j] != 0){
        printf("Na dato polje je vec upisan broj!\n");
    }
    else{
        matrix[i][j] = num;
        printf("Potez odigran.\n");
    }
}


int main() {
    int n, **matrica, i, j, num;
    char opcija;

    do {
        printf("Meni: \n");
        printf("a - Unesi inicijalno stanje\n");
        printf("b - Ispis matrice\n");
        printf("c - Odigraj potez\n");
        printf("d - Ispisi matricu\n");
        printf("f - Trazi pomoc\n");
        printf("g - Proveri da li si na dobrom putu\n");
        printf("h - Proveri validno stanje\n");
        printf("e - Izlaz\n");
        scanf(" %c", &opcija);

        switch (opcija) {
            case 'a':
                printf("Unesite dimenzije matrice\n");
                scanf("%d",&n);
                if (n != 4){
                    printf("Neispravne dimenzije matrice, pokusajte ponovo.\n");
                }
                else{
                    matrica = (int**)malloc(n * sizeof(int*));
                    for (int i = 0; i < n; i++){
                        *(matrica + i) = (int*)malloc(n * sizeof(int));
                    }
                    if (!matrica){
                        printf("Greska u alokaciji memorije.\n");
                        break;
                    }

                    inicijalizujMatricu(matrica, n);
                    if (!proveraValidneMatrice(matrica, n)){
                        printf("Matrica nije validna. Ponovo unesite inicijalno stanje.\n");
                        for (int i = 0; i < n; i++) {
                            free(matrica[i]);
                        }
                        free(matrica);
                        matrica = NULL;
                    }

                }

                break;
            case 'b':
                ispisMatrice(matrica, n);
                break;
            case 'c':
                ispisMatrice(matrica, n);
                printf("Izaberite polje u koje zelite da upisete broj [i, j]: ");
                scanf(" %d%d", &i, &j );
                putchar('\n');
                printf("Izaberite broj koji zelite da upisete: ");
                scanf("%d", &num);
                putchar('\n');
                odigrajPotez(matrica, n, i, j, num);
                break;
            case 'd':
                break;
            case 'e':
                printf("Izlaz\n");
                break;
            case 'h':
                if (proveraValidneMatrice(matrica, n)){
                    printf("Matrica je validna. ");
                }
                else{
                    printf("Matrica nije validna. ");
                }
                break;
            default:
                printf("Nepoznata opcija\n");
        }
    } while (opcija != 'e');

    if (matrica){
        for (int i = 0; i < n; i++){
            free(*(matrica + i));
        }
        free(matrica);
    }

    return 0;
}
