#include <iostream>

const int ALTURA = 5;
const int ANCHURA = 5;
using namespace std;

double **escalaGrises(int **verde, int **rojo, int **azul) {


    double **grises = new double *[ALTURA];
    for (int k = 0; k < ALTURA; ++k) {
        grises[k] = new double[ANCHURA];
    }


    for (int i = 0; i < ALTURA; i++) {
        for (int j = 0; j < ANCHURA; j++) {
            grises[i][j] = rojo[i][j] * 0.3 + verde[i][j] * 0.59 + azul[i][j] * 0.11;
        }
    }
    return grises;
}

void histograma(double **escalagrises, int tramos){

    int result[tramos];
    for (int k = 0; k <tramos ; ++k) {
        result[k]=0;
    }
    double valores_tramo=256/tramos;
    int contador=0;

    for (int i = 0; i < ALTURA; i++) {
        for (int j = 0; j < ANCHURA; j++) {
            while(contador<tramos){
                if(escalagrises[i][j]>=contador*valores_tramo && escalagrises[i][j]<(contador+1)*valores_tramo ){
                    result[contador]=result[contador]+1;
                    contador=0;
                    break;
                }
                else{
                    contador++;
                }
            }
        }
    }

    for (int i = 0; i < tramos; i++) {
        cout << result[i];
        cout << " ";
    }

}

int main() {

    /*PRUEBAS*/

    /*FIN PRUEBAS*/
    int **rojo = new int *[ALTURA];
    for (int k = 0; k < ALTURA; ++k) {
        rojo[k] = new int[ANCHURA];
    }

    int **azul = new int *[ALTURA];
    for (int k = 0; k < ALTURA; ++k) {
        azul[k] = new int[ANCHURA];
    }

    int **verde = new int *[ALTURA];
    for (int k = 0; k < ALTURA; ++k) {
        verde[k] = new int[ANCHURA];
    }

    for (int i = 0; i < ALTURA; i++) {
        for (int j = 0; j < ANCHURA; j++) {
            rojo[i][j] = 1;
            verde[i][j] = 2;
            azul[i][j] = 3;
        }

    }

    /*IMPRESION MATRIZ ROJO
    for (int i = 0; i < ALTURA; i++) {
        for (int j = 0; j < ANCHURA; j++) {
            cout << rojo[i][j];
            cout << " ";
        }
        cout << endl;
    }*/

    cout << endl;

    /*IMPRESION MATRIZ VERDE
    for (int i = 0; i < ALTURA; i++) {
        for (int j = 0; j < ANCHURA; j++) {
            cout << verde[i][j];
            cout << " ";
        }
        cout << endl;
    }*/

    cout << endl;

    /*IMPRESION MATRIZ AZUL
    for (int i = 0; i < ALTURA; i++) {
        for (int j = 0; j < ANCHURA; j++) {
            cout << azul[i][j];
            cout << " ";
        }
        cout << endl;
    }*/

    cout << endl;

    double **resultado = new double *[ALTURA];
    for (int k = 0; k < ALTURA; ++k) {
        resultado[k] = new double[ANCHURA];
    }

    resultado = escalaGrises(verde, rojo, azul);


    /*IMPRESION MATRIZ CAMBIADA*/
    for (int i = 0; i < ALTURA; i++) {
        for (int j = 0; j < ANCHURA; j++) {
            cout << resultado[i][j];
            cout << " ";
        }
        cout << endl;
    }

    cout << endl;

    histograma(resultado,4);

    return 0;
}