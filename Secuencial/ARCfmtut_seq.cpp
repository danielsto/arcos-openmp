#include <iostream>

using namespace std;
int ALTURA = 2;
int ANCHURA = 3;

int maximo(int **matriz) {
    int numeroMaximo = 0;
    for (int i = 0; i < ALTURA; ++i) {
        for (int j = 0; j < ANCHURA; ++j) {
            if (matriz[i][j] > numeroMaximo) {
                numeroMaximo = matriz[i][j];
            }
        }
    }
    return numeroMaximo;
}

int minimo(int **matriz) {
    int numeroMinimo = 0;
    for (int i = 0; i < ALTURA; ++i) {
        for (int j = 0; j < ANCHURA; ++j) {
            if (matriz[i][j] < numeroMinimo) {
                numeroMinimo = matriz[i][j];
            }
        }
    }
    return numeroMinimo;
}

int main() {
    int ** matriz = new int *[ALTURA];
    for (int i = 0; i < ALTURA; ++i) {
        matriz[i] = new int[ANCHURA];
    }
    matriz[0][0] = 1;
    matriz[0][1] = 5;
    matriz[0][2] = 6;
    matriz[1][0] = 3;
    matriz[1][1] = -1;
    matriz[1][2] = 11;
    int numeroMaximo = maximo(matriz);
    cout << numeroMaximo << endl;
}

