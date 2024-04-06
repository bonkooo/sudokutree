#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdlib.h>
#define LEN 10

typedef struct treeNode{
    int** matrica;
    struct cvor *left, *right;
} TreeNode;

typedef struct tree{
    TreeNode *root;
} Tree;

void inicijalizujMatricu(int **matrix, int n){
    printf("Unosite elemente matrice red po red\n");
    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
            scanf("%d", &matrix[i][j]);
        }
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


int main() {
    int n, **matrica;
    char opcija;

    do {
        printf("Meni: \n");
        printf("a - Unesi inicijalno stanje\n");
        printf("b - Ispis matrice\n");
        printf("c - Odigraj potez\n");
        printf("d - Ispisi matricu\n");
        printf("f - Trazi pomoc\n");
        printf("g - Proveri da li si na dobrom putu\n");
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
                break;
            case 'd':
                break;
            case 'e':
                printf("Izlaz\n");
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
