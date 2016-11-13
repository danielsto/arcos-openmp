#include <iostream>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <cstring>
#include <omp.h>

using namespace std;

int ALTURA;
int ANCHURA;

string stringRed;
string stringGreen;
string stringBlue;
string stringTotal;

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
 * Escribe en un archivo los valores máximos y mínimos de todas las matrices de colores, en el
 * orden de Rojo, Verde y Azul.
 * Se crea un array de tamaño 6 que contiene en sus primeras 3 posiciones los valores máximos de
 * las tres matrices y en sus siguientes 3 posiciones los valores mínimos.
 * @param matrizR
 * @param matrizG
 * @param matrizB
 * @param rutaSalida Archivo en el que se escribirá el resultado.
 */
void calcularMaximosYMinimos(int **matrizR, int **matrizG, int **matrizB, char *rutaSalida) {
    array<int, 6> maximosYMinimos = {0, 0, 0, 0, 0, 0};
    for (int i = 0; i < ALTURA; ++i) {
        for (int j = 0; j < ANCHURA; ++j) {
            if (matrizR[i][j] > maximosYMinimos[0]) {
                maximosYMinimos[0] = matrizR[i][j];
            }
            if (matrizG[i][j] > maximosYMinimos[1]) {
                maximosYMinimos[1] = matrizG[i][j];
            }
            if (matrizB[i][j] > maximosYMinimos[2]) {
                maximosYMinimos[2] = matrizB[i][j];
            }
            if (matrizR[i][j] < maximosYMinimos[3]) {
                maximosYMinimos[3] = matrizR[i][j];
            }
            if (matrizG[i][j] < maximosYMinimos[4]) {
                maximosYMinimos[4] = matrizG[i][j];
            }
            if (matrizB[i][j] < maximosYMinimos[5]) {
                maximosYMinimos[5] = matrizB[i][j];
            }
        }
    }
    ofstream outputFile(rutaSalida);
    outputFile << maximosYMinimos[0] << " " << maximosYMinimos[3] << " "
               << maximosYMinimos[1] << " " << maximosYMinimos[4] << " "
               << maximosYMinimos[2] << " " << maximosYMinimos[5];
}

/**
 * Construye una matriz con los valores de los píxeles de la imagen transformados a escala de
 * grises necesaria por parámetro para la función histograma()
 * @param rojo
 * @param verde
 * @param azul
 * @return
 */
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

/**
 * Se realiza un histograma de la imagen leída (en escala de grises) según el nùmero de tramos
 * indicado por parámetros. El resultado se guarda en un fichero de texto plano.
 * @param escalagrises Matriz con los valores de los pixeles de la imagen en escala de grises
 * @param tramos Número de tramos deseados en los que se divide el histograma
 */
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

/**
 * Aplica un filtro blanco y negro a las regiones de la imagen que quedan fuera del circulo de
 * radio indicado por parámetro y con centro en el centro de la imagen. Las regiones dentro del
 * círculo quedan igual.
 * @param matrizR
 * @param matrizG
 * @param matrizB
 * @param radio Radio del círculo dentro del cual no se aplicará el
 * */
int filtroBN(int **matrizR, int **matrizG, int **matrizB, int radio) {

    int centroX = ANCHURA / 2;
    int centroY = ALTURA / 2;

    for (int i = 0; i < ALTURA; ++i) {
        for (int j = 0; j < ALTURA; ++j) {
            float suma = pow(i - centroY, 2) + pow(j - centroX, 2);
            if (suma > pow(radio, 2)) {
                matrizR[i][j] = matrizR[i][j] * 0.3;
                matrizG[i][j] = matrizG[i][j] * 0.59;
                matrizB[i][j] = matrizB[i][j] * 0.11;
            }
        }

    }

    ofstream outputFile("circle_out.txt");

    for (int i = 0; i < ALTURA; ++i) {
        for (int j = 0; j < ANCHURA; ++j) {
            stringstream rs; // red stream
            rs << hex << matrizR[i][j];
            stringTotal.append(rs.str());
        }
    }
    for (int i = 0; i < ALTURA; ++i) {
        for (int j = 0; j < ANCHURA; ++j) {
            stringstream gs; // green stream
            gs << hex << matrizG[i][j];
            stringTotal.append(gs.str());
        }
    }
    for (int i = 0; i < ALTURA; ++i) {
        for (int j = 0; j < ANCHURA; ++j) {
            stringstream bs; //blue stream
            bs << hex << matrizB[i][j];
            stringTotal.append(bs.str());
        }
    }

    /*
     * Formateo del contenido del fichero de salida para hacer
     *coincidir con el formato del fichero de entrada
     * */

    unsigned long l = 0;
    unsigned long k = 0;
    for (unsigned long i = 0; i < stringTotal.length(); ++i) {
        if (l == 4) {
            stringTotal.insert(i, 1, ' '); //pone un espacio cada cuatro caracteres
            l = 0;
            i++;
        }
        l++;
        if (k == 32) {
            stringTotal.insert(i, 1, '\n'); //pone un salto de linea cada 32 caracteres
            k = 0;
            i++;
        }
        k++;
    }
    outputFile << stringTotal;
    return 0;
}

int main(int argv, char **argc) {
    char *rutaEntrada = NULL;
    char *rutaSalida = NULL;
    char *parametroExtra = NULL;
    int ejecucion = 0;
    for (int i = 1; i < argv; ++i) {
        if (strcmp(argc[i], "-u") == 0) {
            ejecucion = atoi(argc[i + 1]);
            i++;
            continue;
        }
        if (strcmp(argc[i], "-i") == 0) {
            rutaEntrada = argc[i + 1];
            i++;
            continue;
        }
        if (strcmp(argc[i], "-o") == 0) {
            rutaSalida = argc[i + 1];
            i++;
            continue;
        }
        if (strcmp(argc[i], "-t") == 0 || strcmp(argc[i], "-r") == 0 || strcmp(argc[i], "-a") == 0 ||
            strcmp(argc[i], "-f") == 0) {
            parametroExtra = argc[i + 1];
            i++;
            continue;
        }
    }
    dimensiones(rutaEntrada);
    imagenToString(rutaEntrada);

    switch (ejecucion) {
        case 0: {
            double **resultado = escalaGrises(stringToMatrizR(), stringToMatrizG(), stringToMatrizB());
            histograma(resultado, atoi(parametroExtra));
            break;
        }
        case 1: {
            calcularMaximosYMinimos(stringToMatrizR(), stringToMatrizG(), stringToMatrizB(), rutaSalida);
            break;
        }
        case 2: {
            break;
        }
        case 3: {
            break;
        }
        case 4: {
            filtroBN(stringToMatrizR(), stringToMatrizG(), stringToMatrizB(), atoi(parametroExtra));
            break;
        }
        default:
            cerr << "El parámetro que indica la acción no es correcto. Insertar valores entre 0 - 4.";
    }
    return 0;
}