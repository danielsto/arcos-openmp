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


/**
 * Método que lee el archivo de imagen y extrae los datos de altura y anchura de los 8 primeros bytes.
 * El método comprueba si el archivo se abre correctamente, mostrando un mensaje de error si no lo hace.
 * Previa lectura de los bytes, se eliminan los espacios para facilitar la operación de extracción.
 * Durante la lectura de los bytes, se convierten los valores de notación hexadecimal a decimal,
 * guardándose estos valores en variables globales.
 *
 * @param rutaEntrada Indica la ruta del archivo de entrada a leer.
 */
void dimensiones(char *rutaEntrada) {
    string primeraLinea;
    ifstream archivo(rutaEntrada);

    if (archivo.is_open()) {
        stringstream stringAltura;
        stringstream stringAnchura;

        getline(archivo, primeraLinea);

        primeraLinea.erase(remove(primeraLinea.begin(), primeraLinea.end(), ' '), primeraLinea.end());

        stringAltura << hex << primeraLinea.substr(6, 2) + primeraLinea.substr(4, 2) + primeraLinea.substr(2, 2) +
                               primeraLinea.substr(0, 2);
        stringAltura >> ALTURA;

        stringAnchura << hex << primeraLinea.substr(14, 2) + primeraLinea.substr(12, 2) + primeraLinea.substr(10, 2) +
                                primeraLinea.substr(8, 2);
        stringAnchura >> ANCHURA;

        archivo.close();

    } else {
        cerr << "El fichero de entrada no existe en la ruta especificada." << endl;
        exit(-1);
    }

}

/**
 * Método que lee el archivo de imagen y extrae tres strings que se corresponden a los datos de las
 * tres matrices de colores RGB que contiene el archivo.
 * El método comprueba si el archivo se abre correctamente, mostrando un mensaje de error si no lo hace.
 * El contenido del archivo se inserta en un string, que posteriormente será dividido en tres partes:
 * una para cada matriz de colores. Se eliminan los retornos de carro y los espacios para facilitar la
 * lectura del contenido del string.
 * Los strings resultantes de cada matriz serán guardados como variables globales.
 *
 * @param rutaEntrada Indica la ruta del archivo de entrada a leer.
 */
void imagenToString(char *rutaEntrada) {
    string lineaLeida;
    string stringImagenCompleta;

    ifstream archivo(rutaEntrada);

    if (archivo.is_open()) {
        while (getline(archivo, lineaLeida)) {
            lineaLeida.erase(remove(lineaLeida.begin(), lineaLeida.end(), '\r'), lineaLeida.end());
            stringImagenCompleta += lineaLeida;
        }
        archivo.close();

        stringImagenCompleta.erase(remove(stringImagenCompleta.begin(), stringImagenCompleta.end(), ' '),
                                   stringImagenCompleta.end());

        stringRed = stringImagenCompleta.substr(16, ALTURA * ANCHURA * 2);
        stringGreen = stringImagenCompleta.substr(16 + (ALTURA * ANCHURA * 2), ALTURA * ANCHURA * 2);
        stringBlue = stringImagenCompleta.substr(16 + (ALTURA * ANCHURA * 2 * 2), ALTURA * ANCHURA * 2);
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
 * Función que devuelve en un array el valor máximo de cada matriz de color pasada por
 * parámetros.
 * @param matrizR
 * @param matrizG
 * @param matrizB
 * @return Array de tres posiciones con los valores máximos de cada matriz
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
 * Función que devuelve en un array el valor mínimo de cada matriz de color pasada por
 * parámetros.
 * @param matrizR
 * @param matrizG
 * @param matrizB
 * @return Array de tres posiciones con los valores mínimos de cada matriz
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
 * @param matrizR
 * @param matrizG
 * @param matrizB
 * @param rutaSalida Archivo en el que se escribirá el resultado.
 */
void calcularMaximosYMinimos(int **matrizR, int **matrizG, int **matrizB, char *rutaSalida) {
    array<int, 3> maximos = maximo(matrizR, matrizG, matrizB);
    array<int, 3> minimos = minimo(matrizR, matrizG, matrizB);
    ofstream outputFile(rutaSalida);
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