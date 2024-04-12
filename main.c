#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdlib.h>
#define MAX 100
#define LEN 10

typedef struct treeNode{
    int** info;
    int childrenSize;
    struct treeNode** children;
    int dubina;
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
    cvor->dubina = 0;

    return cvor;
}

int** alocirajMatricuStanja (int n){
    int **matrix = (int**)malloc(n * sizeof(int*));
    for (int i = 0; i < n; i++){
        *(matrix + i) = (int*)malloc(n * sizeof(int));
    }
    return matrix;
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
        //underflow
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

void ispisStabla(struct tree* tree, int n){
    stack* ispisStek = inicijalizujStek();

    push(ispisStek, tree->root);
    while (!stack_empty(ispisStek)){
        struct treeNode* next = pop(ispisStek);
        next->dubina = 0;
        while (next != NULL){
            for (int i = 0; i < n; i++){
                for (int k = 0; k < next->dubina; k++){
                    putchar('_');
                }
                for (int j = 0; j < n; j++){
                    printf("%d ", next->info[i][j]);
                }
                putchar('\n');
            }
            putchar('\n');

            for (int i = 0; i < next->childrenSize; i++){
                next->children[i]->dubina = next->dubina + 1;
                push(ispisStek, next->children[i]);
            }

            next = pop(ispisStek);
        }
    }
}

int proveriDobarPut(int **trenutnoStanje, struct tree* tree, int n){
    stack* stekPut = inicijalizujStek();
    push (stekPut, tree->root);
    int trenPoklapanja = 0;

    while(!stack_empty(stekPut)){
        struct treeNode *next = pop(stekPut);
        while (next != NULL){
            trenPoklapanja = 0;
            for (int i = 0; i < n; i++){
                for (int j = 0; j < n; j++){
                    if (trenutnoStanje[i][j] == next->info[i][j] || trenutnoStanje[i][j] == 0){
                        trenPoklapanja++;
                    }
                }
            }

            if (trenPoklapanja == n*n && proveraValidneMatrice(next->info, n)){
                return 1;
            }

            for (int i = 0; i < next->childrenSize; i++){
                next->children[i]->dubina = next->dubina + 1;
                push(stekPut, next->children[i]);
            }

            next = pop(stekPut);
        }
    }

    return 0;

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

int proveriResenje(int** matrica, int n){
    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
            if (matrica[i][j] == 0){
                return 0;
            }
        }
    }
    return 1;
}

int istaMatrica(int **matrica1, int **matrica2, int n){
    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
            if (matrica1[i][j] != matrica2[i][j]){
                return 0;
            }
        }
    }
    return 1;
}

int proveriDuplikate(int **resenja[MAX], int **trenutnaMatrica, int len, int n){
    for (int i = 0; i < len; i++){
        if (istaMatrica(trenutnaMatrica, resenja[i], n)){
            return 0;
        }
    }
    return 1;
}

void ispisiResenja(int **trenutnoStanje, struct tree* tree, int n) {
    int** resenja[MAX];
    int len = 0;
    stack *stekPut = inicijalizujStek();
    push(stekPut, tree->root);

    while (!stack_empty(stekPut)) {
        struct treeNode *next = pop(stekPut);
        while (next != NULL) {
            if (proveriResenje(next->info, n) && proveriDuplikate(resenja, next->info, len, n)) {
                resenja[len++] = next->info;
            }

            for (int i = 0; i < next->childrenSize; i++) {
                next->children[i]->dubina = next->dubina + 1;
                push(stekPut, next->children[i]);
            }

            next = pop(stekPut);
        }
    }

    for (int i = 0; i < len; i++){
        ispisMatrice(resenja[i], n);
        putchar('\n');
    }

}

void predloziPotez (int** trenutnoStanje, struct tree* tree, int n){
    if (!proveraValidneMatrice(trenutnoStanje, n)){
        printf("Stanje nije validno.\n");
        return;
    }

    struct tree *novoStablo = (struct tree*)malloc(sizeof(struct tree));
    kreirajStablo(trenutnoStanje, novoStablo, n);

    printf("Predlozen potez je: \n");
    ispisMatrice(novoStablo->root->children[0]->info, n);

}

int main() {
    int n, **matrica, i, j, num;
    char opcija;

    struct tree *inicijalnoStablo = (struct tree*)malloc(sizeof(struct tree));
    if (!inicijalnoStablo) {
        printf("Greska u alokaciji memorije\n");
    }


    inicijalnoStablo ->root = NULL;
    do {
        printf("Meni: \n");
        printf("a - Unesi inicijalno stanje\n");
        printf("b - Ispis matrice\n");
        printf("c - Odigraj potez\n");
        printf("d - Proveri da li si na dobrom putu\n");
        printf("f - Trazi pomoc\n");
        printf("h - Proveri validno stanje\n");
        printf("i - Formiraj stablo\n");
        printf("j - Ispisi stablo\n");
        printf("k - Ispisi resenja\n");
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
                printf("Izaberite polje u koje zelite da upisete broj [i, j]:\n");
                scanf(" %d%d", &i, &j );
                putchar('\n');
                printf("Izaberite broj koji zelite da upisete:\n");
                scanf("%d", &num);
                putchar('\n');
                odigrajPotez(matrica, n, i, j, num);
                break;
            case 'd':
                if (proveriDobarPut(matrica, inicijalnoStablo, n)){
                    printf("Na dobrom ste putu. \n");
                }
                else{
                    printf("Niste na dobrom putu. \n");
                }
                break;
            case 'f':
                predloziPotez(matrica, inicijalnoStablo, n);
                break;
            case 'e':
                printf("Izlaz\n");
                break;
            case 'h':
                if (proveraValidneMatrice(matrica, n)){
                    printf("Matrica je validna. \n");
                }
                else{
                    printf("Matrica nije validna. \n");
                }
                break;
            case 'i':
                if (!inicijalnoStablo->root){
                    kreirajStablo(matrica, inicijalnoStablo, n);
                    printf("Stablo formirano.\n");
                }
                break;
            case 'j':
                if (!inicijalnoStablo->root){
                    printf("Stablo nije formirano.\n");
                }
                ispisStabla(inicijalnoStablo, n);
                break;
            case 'k':
                ispisiResenja(matrica, inicijalnoStablo, n);
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
