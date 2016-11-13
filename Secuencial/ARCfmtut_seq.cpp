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

struct pixel {
    int r;
    int g;
    int b;
};


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

        stringTotal = stringImagenCompleta.substr(0, 16);
        stringRed = stringImagenCompleta.substr(16, ALTURA * ANCHURA * 2);
        stringGreen = stringImagenCompleta.substr(16 + (ALTURA * ANCHURA * 2), ALTURA * ANCHURA * 2);
        stringBlue = stringImagenCompleta.substr(16 + (ALTURA * ANCHURA * 2 * 2), ALTURA * ANCHURA * 2);
    } else {
        cerr << "El fichero de entrada no existe en la ruta especificada." << endl;
        exit(-1);
    }
}


/**
 * Método que devuelve una matriz de estructuras tipo pixel en el que cada posición contiene
 * tres valores para los tres colores RGB de la imagen.
 * El método utiliza las variables globales de los string extraídos del archivo y convierte
 * sus valores a notación hexadecimal, para posteriormente guardar el valor en el color que
 * corresponda al píxel de la matriz.
 */
pixel **generarMatrizPixeles() {
    pixel **matrizPixeles = new pixel *[ALTURA];
    for (int i = 0; i < ALTURA; ++i) {
        matrizPixeles[i] = new pixel[ANCHURA];
    }

    int columna = 0;
    int fila = 0;
    int rojo = 0;
    int verde = 0;
    int azul = 0;

    // Se coge únicamente la longitud del string verde porque es igual a lo de los demás
    long length = stringGreen.length();

    for (int i = 0; i < length; i = i + 2) {
        if (columna == ANCHURA) {
            columna = 0;
            fila++;
        }

        stringstream streamRojo;
        stringstream streamVerde;
        stringstream streamAzul;
        streamRojo << hex << stringRed.substr(i, 2);
        streamRojo >> rojo;

        streamVerde << hex << stringGreen.substr(i, 2);
        streamVerde >> verde;

        streamAzul << hex << stringBlue.substr(i, 2);
        streamAzul >> azul;

        matrizPixeles[fila][columna].r = rojo;
        matrizPixeles[fila][columna].g = verde;
        matrizPixeles[fila][columna].b = azul;

        columna++;
    }

    return matrizPixeles;
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

void calcularMaximosYMinimos(pixel **matriz, char *rutaSalida) {
    array<int, 6> maximosYMinimos = {0, 0, 0, 0, 0, 0};
    for (int i = 0; i < ALTURA; ++i) {
        for (int j = 0; j < ANCHURA; ++j) {
            if (matriz[i][j].r > maximosYMinimos[0]) {
                maximosYMinimos[0] = matriz[i][j].r;
            }
            if (matriz[i][j].g > maximosYMinimos[1]) {
                maximosYMinimos[1] = matriz[i][j].g;
            }
            if (matriz[i][j].b > maximosYMinimos[2]) {
                maximosYMinimos[2] = matriz[i][j].b;
            }
            if (matriz[i][j].r < maximosYMinimos[3]) {
                maximosYMinimos[3] = matriz[i][j].r;
            }
            if (matriz[i][j].g < maximosYMinimos[4]) {
                maximosYMinimos[4] = matriz[i][j].g;
            }
            if (matriz[i][j].b < maximosYMinimos[5]) {
                maximosYMinimos[5] = matriz[i][j].b;
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
double **escalaGrises(pixel **matriz) {


    double **grises = new double *[ALTURA];
    for (int k = 0; k < ALTURA; ++k) {
        grises[k] = new double[ANCHURA];
    }


    for (int i = 0; i < ALTURA; i++) {
        for (int j = 0; j < ANCHURA; j++) {
            grises[i][j] = matriz[i][j].r * 0.3 + matriz[i][j].g * 0.59 + matriz[i][j].b * 0.11;
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
void histograma(double **escalagrises, int tramos, char *rutaSalida) {

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

    ofstream outputFile(rutaSalida);

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
void filtroBN(pixel **matriz, int radio, char *rutaSalida) {

    int centroX = ANCHURA / 2;
    int centroY = ALTURA / 2;

    for (int i = 0; i < ALTURA; ++i) {
        for (int j = 0; j < ALTURA; ++j) {
            float suma = pow(i - centroY, 2) + pow(j - centroX, 2);
            if (suma > pow(radio, 2)) {
                matriz[i][j].r = matriz[i][j].r * 0.3;
                matriz[i][j].g = matriz[i][j].g * 0.59;
                matriz[i][j].b = matriz[i][j].b * 0.11;
            }
        }

    }

    ofstream outputFile(rutaSalida);

    for (int i = 0; i < ALTURA; ++i) {
        for (int j = 0; j < ANCHURA; ++j) {
            stringstream rs; // red stream
            rs << hex << matriz[i][j].r;
            stringTotal.append(rs.str());
        }
    }
    for (int i = 0; i < ALTURA; ++i) {
        for (int j = 0; j < ANCHURA; ++j) {
            stringstream gs; // green stream
            gs << hex << matriz[i][j].g;
            stringTotal.append(gs.str());
        }
    }
    for (int i = 0; i < ALTURA; ++i) {
        for (int j = 0; j < ANCHURA; ++j) {
            stringstream bs; //blue stream
            bs << hex << matriz[i][j].b;
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
            double **resultado = escalaGrises(generarMatrizPixeles());
            histograma(resultado, atoi(parametroExtra), rutaSalida);
            break;
        }
        case 1: {
            calcularMaximosYMinimos(generarMatrizPixeles(), rutaSalida);
            break;
        }
        case 2: {
            break;
        }
        case 3: {
            break;
        }
        case 4: {
            filtroBN(generarMatrizPixeles(), atoi(parametroExtra), rutaSalida);

            break;
        }
        default:
            cerr << "El parámetro que indica la acción no es correcto. Insertar valores entre 0 - 4.";
    }
    return 0;
}