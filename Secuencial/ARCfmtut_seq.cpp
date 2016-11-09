#include <iostream>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <cstring>

using namespace std;

int ALTURA;
int ANCHURA;

string stringRed;
string stringGreen;
string stringBlue;
string stringDim;


void dimensiones(char *rutaEntrada) {
    string line;
    ifstream myfile(rutaEntrada); // Fichero de entrada
    if (myfile.is_open()) { // Si existe o lo encuentra{
        // objeto strin string en el que se almacena la conversion de string a hexadecimal
        stringstream hs; //ALTURAstream
        stringstream ws; //ANCHURAstream

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

    } else {
        cerr << "El fichero de entrada no existe en la ruta especificada." << endl;
        exit(-1);
    }

}

void imagenToString(char *rutaEntrada) {
    string line;
    string stringImagen;

    ifstream myfile(rutaEntrada); // Fichero de entrada

    if (myfile.is_open()) // Si existe o lo encuentra
    {
        while (getline(myfile, line)) {
            line.erase(remove(line.begin(), line.end(), '\r'), line.end());
            stringImagen += line;
        }
        myfile.close();

        stringImagen.erase(remove(stringImagen.begin(), stringImagen.end(), ' '), stringImagen.end());

        stringDim = stringImagen.substr(0, 16);
        stringRed = stringImagen.substr(16, ALTURA * ANCHURA * 2);
        stringGreen = stringImagen.substr(16 + (ALTURA * ANCHURA * 2), ALTURA * ANCHURA * 2);
        stringBlue = stringImagen.substr(16 + (ALTURA * ANCHURA * 2 * 2), ALTURA * ANCHURA * 2);
    } else {
        cerr << "El fichero de entrada no existe en la ruta especificada." << endl;
        exit(-1);
    }
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
void calcularMaximosYMinimos(int **matrizR, int **matrizG, int **matrizB, char *rutaEntrada) {
    array<int, 3> maximos = maximo(matrizR, matrizG, matrizB);
    array<int, 3> minimos = minimo(matrizR, matrizG, matrizB);
    ofstream outputFile(rutaEntrada);
    outputFile << maximos[0] << " " << minimos[0] << " "
               << maximos[1] << " " << minimos[1] << " "
               << maximos[2] << " " << minimos[2];
}

double **escalaGrises(int **rojo, int **verde, int **azul) {


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

void histograma(double **escalagrises, int tramos) {

    int result[tramos];
    for (int k = 0; k < tramos; ++k) {
        result[k] = 0;
    }
    double valores_tramo = 256 / tramos;
    int contador = 0;

    for (int i = 0; i < ALTURA; i++) {
        for (int j = 0; j < ANCHURA; j++) {
            while (contador < tramos) {
                if (escalagrises[i][j] >= contador * valores_tramo &&
                    escalagrises[i][j] < (contador + 1) * valores_tramo) {
                    result[contador] = result[contador] + 1;
                    contador = 0;
                    break;
                } else {
                    contador++;
                }
            }
        }
    }

    ofstream outputFile("histogram.txt");

    for (int i = 0; i < tramos; i++) {
        outputFile << result[i];
        outputFile << " ";
    }

}


int filtroBN(int **matrizR, int **matrizG, int **matrizB, int radio) {

    int centroX = ANCHURA / 2;
    int centroY = ALTURA / 2;

    for (int i = 0; i < ALTURA; ++i) {
        for (int j = 0; j < ALTURA; ++j) {
            float suma = pow(static_cast< double >(i) - centroY, 2) + pow(j - centroX, 2);
            if (suma > pow(static_cast< double >( radio ), 2)) {
                matrizR[i][j] = matrizR[i][j] * 0.3;
                matrizG[i][j] = matrizG[i][j] * 0.59;
                matrizB[i][j] = matrizB[i][j] * 0.11;
            }
        }

    }
//    for (int k = 0; k < ALTURA; ++k) {
//        for (int i = 0; i < ANCHURA; ++i) {
//            cout << matrizR[k][i] << " | ";
//        }
//        cout << '\n';
//    }
//
//    cout << '\n';
//    cout << '\n';
//
//    for (int k = 0; k < ALTURA; ++k) {
//        for (int i = 0; i < ANCHURA; ++i) {
//            cout << matrizG[k][i] << " | ";
//        }
//        cout << '\n';
//    }
//
//    cout << '\n';
//    cout << '\n';
//
//    for (int k = 0; k < ALTURA; ++k) {
//        for (int i = 0; i < ANCHURA; ++i) {
//            cout << matrizB[k][i] << " | ";
//        }
//        cout << '\n';
//    }
    return 0;
}

int main(int argv, char **argc) {
    if (strcmp(argc[1], "-u") == 0) {
        if (strcmp(argc[3], "-i") == 0) {
            dimensiones(argc[4]);
            imagenToString(argc[4]);
        } else {
            cerr << "Es necesario indicar la ruta de la imagen con el parámetro -i." << endl;
            exit(-1);
        }
        if (strcmp(argc[5], "-o") != 0) {
            cerr << "Es necesario indicar la ruta de la imagen con el parámetro -i." << endl;
            exit(-1);
        }
        if (strcmp(argc[2], "0") == 0) {
            double **resultado = escalaGrises(stringToMatrizR(), stringToMatrizG(), stringToMatrizB());
            histograma(resultado, 25);
        }
        if (strcmp(argc[2], "1") == 0) {
            calcularMaximosYMinimos(stringToMatrizR(), stringToMatrizG(), stringToMatrizB(), argc[6]);
        }
        if (strcmp(argc[2], "2") == 0) {
        }
        if (strcmp(argc[2], "3") == 0) {
        }
        if (strcmp(argc[2], "4") == 0) {
            filtroBN(stringToMatrizR(), stringToMatrizG(), stringToMatrizB(), 5);
        }
    } else {
        cerr << "Es necesario indicar las acciones con el parámetro -u." << endl;
        exit(-1);
    }
    return 0;
}