#include <iostream>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <iomanip>
#include <cstring>

using namespace std;

int ALTURA;
int ANCHURA;
int **matrizR = new int *[ALTURA];
int **matrizG = new int *[ALTURA];
int **matrizB = new int *[ALTURA];

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
 * Función que devuelve los valores máximos de píxel dentro de las matrices de color
 * @return
 */
array<int, 3> maximo() {
    array<int, 3> numerosMaximos = {0, 0, 0};
    for (int i = 0; i < ALTURA; ++i) {
        for (int j = 0; j < ANCHURA; ++j) {
            if (matrizR[i][j] > numerosMaximos[0]) {
                numerosMaximos[0] = matrizR[i][j];
            }
            if (matrizG[i][j] > numerosMaximos[1]) {
                numerosMaximos[1] = matrizG[i][j];
            }
            if (matrizB[i][j] > numerosMaximos[2]) {
                numerosMaximos[2] = matrizB[i][j];
            }
        }
    }

    return numerosMaximos;
}

/**
 * Función que devuelve los valores mínimos de píxel dentro de las matrices de color
 * @return
 */
array<int, 3> minimo() {
    array<int, 3> numerosMinimos = {0, 0, 0};
    for (int i = 0; i < ALTURA; ++i) {
        for (int j = 0; j < ANCHURA; ++j) {
            if (matrizR[i][j] < numerosMinimos[0]) {
                numerosMinimos[0] = matrizR[i][j];
            }
            if (matrizG[i][j] < numerosMinimos[1]) {
                numerosMinimos[1] = matrizG[i][j];
            }
            if (matrizB[i][j] < numerosMinimos[2]) {
                numerosMinimos[2] = matrizB[i][j];
            }
        }
    }
    return numerosMinimos;
}

/**
 * Escribe en un archivo los valores máximos y mínimos de todas las matrices de colores, en el
 * orden de Rojo, Verde y Azul
 * @param matrizRojo
 * @param matrizVerde
 * @param matrizAzul
 * @return
 */
void calcularMaximosYMinimos() {
    array<int, 3> maximos = maximo();
    array<int, 3> minimos = minimo();
    ofstream outputFile("maxmin.txt");
    outputFile << maximos[0] << " " << minimos[0] << " "
               << maximos[1] << " " << minimos[1] << " "
               << maximos[2] << " " << minimos[2];
}

void crearImagen() {
    int decimal;

    string line;
    string imagen;

    /*unsigned long da problemas con -Werror*/ int i;
    for (i = 0; i < ALTURA; i++) {
        matrizR[i] = new int[ANCHURA];
        matrizG[i] = new int[ANCHURA];
        matrizB[i] = new int[ANCHURA];
    }

    ifstream myfile("..\\imagen_entrada"); // Fichero de entrada
    if (myfile.is_open()) // Si existe o lo encuentra
    {
        while (getline(myfile, line)) {
            line.erase(remove(line.begin(), line.end(), '\r'), line.end());
            imagen += line;
        }
        myfile.close();
    } else cout << "Unable to open file";

    imagen.erase(remove(imagen.begin(), imagen.end(), ' '), imagen.end());

    int columna = 0;
    int filas = 0;

    for (i = 16; i < ALTURA * ANCHURA * 2; i = i + 2) {
        if (columna > ANCHURA) {
            columna = 0;
            filas++;
        }
        stringstream ss;
        ss << hex << imagen.substr(i, 2);
        ss >> decimal;
        matrizR[filas][columna] = decimal;

        columna++;
    }

    columna = 0;
    filas = 0;
    int j;
    for (j = i; j < ALTURA * ANCHURA * 2 * 2; j = j + 2) {
        if (columna > ANCHURA) {
            columna = 0;
            filas++;
        }
        stringstream ss;
        ss << hex << imagen.substr(j, 2);
        ss >> decimal;
        matrizG[filas][columna] = decimal;
        columna++;
    }

    columna = 0;
    filas = 0;
    int k;
    for (k = j; k < ALTURA * ANCHURA * 2 * 3; k = k + 2) {
        if (columna > ANCHURA) {
            columna = 0;
            filas++;
        }
        stringstream ss;
        ss << hex << imagen.substr(k, 2);
        ss >> decimal;
        matrizB[filas][columna] = decimal;
        columna++;
    }
}

int main(int argv, char **argc) {
    dimensiones();
    crearImagen();
    if (strcmp(argc[1], "-u") == 0) {
        if (strcmp(argc[2], "1") == 0) {
            calcularMaximosYMinimos();
        }
    } else {
        cout << "Es necesario indicar las acciones con el parámetro -u";
    }
    return 0;
}