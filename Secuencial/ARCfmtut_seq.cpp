#include <iostream>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <iomanip>

using namespace std;

int ALTURA;
int ANCHURA;

void dimensiones() {
    string line;
    ifstream myfile("..\\imagen_entrada"); // Fichero de entrada
    if (myfile.is_open()) // Si existe o lo encuentra
    {
        // objeto strin string en el que se almacena la conversion de string a hexadecimal
        stringstream hs; //heightstream
        stringstream ws; //widthstream


        getline(myfile, line);
        //se eliminan los espacios de la primera linea
        line.erase(remove(line.begin(), line.end(), ' '), line.end());
        // Obtener a partir de la posición 0 una cadena de longitud 8
        hs << hex << line.substr(6, 2) + line.substr(4, 2) + line.substr(2, 2) + line.substr(0, 2);
        hs >> ALTURA;

        // Obtener a partir de la posición 8 una cadena de longitud 8
        ws << hex << line.substr(14, 2) + line.substr(12, 2) + line.substr(10, 2) + line.substr(8, 2);
        ws >> ANCHURA;

        myfile.close();

    } else cout << "Unable to open file" << endl;

}

/**
 * Función que devuelve el valor máximo de píxel dentro de la matriz de color
 * pasada por parámetro.
 * @param matriz
 * @return
 */
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

/**
 * Función que devuelve el valor mínimo de píxel dentro de la matriz de color
 * pasada por parámetro.
 * @param matriz
 * @return
 */
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

/**
 * Imprime los valores máximos y mínimos de todas las matrices de colores, en el
 * orden de Rojo, Verde y Azul
 * @param matrizRojo
 * @param matrizVerde
 * @param matrizAzul
 * @return
 */
void calcularMaximosYMinimos(int **matrizRojo, int **matrizVerde, int **matrizAzul) {
    cout << "Matriz Roja: \n" << "  Máximo: " << maximo(matrizRojo) << endl << "  Mínimo: " << minimo(matrizRojo)
         << endl;
    cout << "Matriz Verde: \n" << "  Máximo: " << maximo(matrizVerde) << endl << "  Mínimo: " << minimo(matrizRojo)
         << endl;
    cout << "Matriz Azul: \n" << "  Máximo: " << maximo(matrizAzul) << endl << "  Mínimo: " << minimo(matrizRojo)
         << endl;
}

int main() {
    dimensiones();
    int **matriz = new int *[ALTURA];
    for (int i = 0; i < ALTURA; ++i) {
        matriz[i] = new int[ANCHURA];
    }
    calcularMaximosYMinimos(matriz, matriz, matriz);
}