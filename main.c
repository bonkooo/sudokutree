#include "game.h"

int main() {
    int n, **matrica, i, j, num;
    char opcija;

    struct tree *inicijalnoStablo = (struct tree*)malloc(sizeof(struct tree));
    if (!inicijalnoStablo) {
        printf("Greska u alokaciji memorije\n");
    }

    inicijalnoStablo->root = NULL;

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
                scanf("%d", &n);
                if (n != 4) {
                    printf("Ne podrzava se resavanje za dimenzije osim 4x4.\n");
                    return 0;
                }
                matrica = alocirajMatricuStanja(n);
                inicijalizujMatricu(matrica, n);
                break;

            case 'b':
                ispisMatrice(matrica, n);
                break;

            case 'c':
                printf("Unesite potez (i, j, broj):\n");
                scanf("%d %d %d", &i, &j, &num);
                odigrajPotez(matrica, n, i, j, num);
                break;

            case 'd':
                if (proveriDobarPut(matrica, inicijalnoStablo, n)){
                    printf("Bravo, na dobrom si putu\n");
                } else {
                    printf("Greska, nisi na dobrom putu.\n");
                }
                break;

            case 'f':
                Pomoc(matrica, inicijalnoStablo, n);
                break;

            case 'h':
                if(proveraValidneMatrice(matrica, n)) {
                    printf("Matrica je validna\n");
                } else {
                    printf("Matrica nije validna\n");
                }
                break;

            case 'i':
                kreirajStablo(matrica, inicijalnoStablo, n);
                break;

            case 'j':
                ispisStabla(inicijalnoStablo, n);
                break;

            case 'k':
                ispisiResenja(matrica, inicijalnoStablo, n);
                break;

            case 'e':
                printf("Izlaz\n");
                break;

            default:
                printf("Nepostojeca opcija\n");
                break;
        }
    } while (opcija != 'e');

    return 0;
}
