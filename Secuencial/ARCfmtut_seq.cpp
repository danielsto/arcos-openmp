#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <cstring>

using namespace std;

int ALTURA;
int ANCHURA;

string stringRed;
string stringGreen;
string stringBlue;


void dimensiones() {
    string line;
    ifstream myfile("..\\imagen_entrada"); // Fichero de entrada
    if (myfile.is_open()) { // Si existe o lo encuentra{
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

    } else cout << "No se puede abrir el fichero";

}

void imagenToString() {

    string line;
    string stringImagen;

    ifstream myfile("..\\imagen_entrada"); // Fichero de entrada

    if (myfile.is_open()) // Si existe o lo encuentra
    {
        while (getline(myfile, line)) {
            line.erase(remove(line.begin(), line.end(), '\r'), line.end());
            stringImagen += line;
        }
        myfile.close();
    } else cout << "No se puede abrir el fichero";

    stringImagen.erase(remove(stringImagen.begin(), stringImagen.end(), ' '), stringImagen.end());

    stringRed = stringImagen.substr(16, ALTURA * ANCHURA * 2);
    stringGreen = stringImagen.substr(16 + (ALTURA * ANCHURA * 2), ALTURA * ANCHURA * 2);
    stringBlue = stringImagen.substr(16 + (ALTURA * ANCHURA * 2 * 2), ALTURA * ANCHURA * 2);

}


int **stringToMatrizR() {
    int **matrizRoja = new int *[ALTURA];
    for (int i = 0; i < ALTURA; ++i) {
        matrizRoja[i] = new int[ANCHURA];
    }

    int columna = 0;
    int filas = 0;
    int decimal = 0;

    int length = stringRed.length();

    for (int i = 0; i < length; i = i + 2) {
        if (columna == ANCHURA) {
            columna = 0;
            filas++;
        }
        stringstream ss;
        ss << hex << stringRed.substr(i, 2);
        ss >> decimal;
        matrizRoja[filas][columna] = decimal;
        columna++;
    }


    return matrizRoja;
}

int **stringToMatrizG() {
    int **matrizVerde = new int *[ALTURA];
    for (int i = 0; i < ALTURA; ++i) {
        matrizVerde[i] = new int[ANCHURA];
    }

    int columna = 0;
    int filas = 0;
    int decimal = 0;

    int length = stringGreen.length();

    for (int i = 0; i < length; i = i + 2) {
        if (columna == ANCHURA) {
            columna = 0;
            filas++;
        }
        stringstream ss;
        ss << hex << stringGreen.substr(i, 2);
        ss >> decimal;
        matrizVerde[filas][columna] = decimal;
        columna++;
    }
    return matrizVerde;
}


int **stringToMatrizB() {
    int **matrizAzul = new int *[ALTURA];
    for (int i = 0; i < ALTURA; ++i) {
        matrizAzul[i] = new int[ANCHURA];
    }

    int columna = 0;
    int filas = 0;
    int decimal = 0;

    int length = stringBlue.length();

    for (int i = 0; i < length; i = i + 2) {
        if (columna == ANCHURA) {
            columna = 0;
            filas++;
        }
        stringstream ss;
        ss << hex << stringBlue.substr(i, 2);
        ss >> decimal;
        matrizAzul[filas][columna] = decimal;
        columna++;
    }
    return matrizAzul;
}

/**
 * Función que devuelve los valores máximos de píxel dentro de las matrices de color
 * @return
 */
array<int, 3> maximo(int **matrizR, int **matrizG, int **matrizB) {
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
array<int, 3> minimo(int **matrizR, int **matrizG, int **matrizB) {
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
void calcularMaximosYMinimos(int **matrizR, int **matrizG, int **matrizB) {
    array<int, 3> maximos = maximo(matrizR, matrizG, matrizB);
    array<int, 3> minimos = minimo(matrizR, matrizG, matrizB);
    ofstream outputFile("maxmin.txt");
    outputFile << maximos[0] << " " << minimos[0] << " "
               << maximos[1] << " " << minimos[1] << " "
               << maximos[2] << " " << minimos[2];
}


int main(int argv, char **argc) {
    dimensiones();
    imagenToString();

    if (strcmp(argc[1], "-u") == 0) {
        if (strcmp(argc[2], "1") == 0) {
            calcularMaximosYMinimos(stringToMatrizR(), stringToMatrizG(), stringToMatrizB());
        }
    } else {
        cout << "Es necesario indicar las acciones con el parámetro -u";
    }
    return 0;
}