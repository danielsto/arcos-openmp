#include <iostream>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <cstring>
#include <omp.h>
#include <chrono>
#include <vector>
#include <array>
#include <unistd.h>

using namespace std;

int ALTURA;
int ANCHURA;

char *stringTotal;
char *stringRoja;
char *stringVerde;
char *stringAzul;


void escribirSalida(int **matrizRoja, int **matrizVerde, int **matrizAzul, char *rutaSalida) {
    ofstream archivo(rutaSalida, ios::binary);
    if (archivo.is_open()) {
        archivo.write((char *) &ALTURA, 4);
        archivo.write((char *) &ANCHURA, 4);
        archivo.write((char *) &matrizRoja, ALTURA * ANCHURA);
        archivo.write((char *) &matrizVerde, ALTURA * ANCHURA);
        archivo.write((char *) &matrizAzul, ALTURA * ANCHURA);
        archivo.close();
    } else {
        cerr << "El fichero de salida no se ha creado correctamente." << endl;
        exit(-1);
    }
}

/**
 * Método que lee el archivo de imagen y extrae tres strings que se corresponden a los datos de las
 * tres matrices de colores RGB que contiene el archivo.
 * El método comprueba si el archivo se abre correctamente, mostrando un mensaje de error si no lo hace.
 * El contenido del archivo se inserta en un string, que posteriormente será dividido en cuatro partes:
 * una para cada matriz de colores y otra para las dimensiones de la imagen. Se eliminan los retornos de
 * carro y los espacios para facilitar la lectura del contenido del string.
 * Los strings resultantes de cada matriz serán guardados como variables globales.
 *
 * @param rutaEntrada Indica la ruta del archivo de entrada a leer.
 */
void imagenToString(char *rutaEntrada) {
    ifstream archivo(rutaEntrada, ios::binary);
    if (archivo.is_open()) {
        archivo.read((char *) &ALTURA, 4);
        archivo.read((char *) &ANCHURA, 4);

        int fileSize = (int) archivo.seekg(0, ios::end).tellg();

        char *buffer = new char[fileSize - 8];

        archivo.seekg(8, ios::beg);
        archivo.read(buffer, fileSize - 8);

        if (!archivo.eof()) {
            archivo.read((char *) &stringRoja, ALTURA * ANCHURA);
            archivo.read((char *) &stringVerde, ALTURA * ANCHURA);
            archivo.read((char *) &stringAzul, ALTURA * ANCHURA);
            archivo.close();
        } else {
            cerr << "El fichero de entrada no existe en la ruta especificada." << endl;
            exit(-1);
        }
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
    void histograma(double **escalagrises, char *rutaSalida, int tramos) {

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
    int filtroBN(int **matrizR, int **matrizG, int **matrizB, double radio) {

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

        return 0;
    }

    int main(int argv, char **argc) {
        char *rutaEntrada = NULL;
        char *rutaSalida = NULL;
        char *parametroExtra = NULL;
        int ejecucion = -1;
        for (int i = 1; i < argv; ++i) {
            if (strcmp(argc[i], "-u") == 0) {
                try {
                    ejecucion = stoi(argc[i + 1]);
                    i++;
                    continue;
                } catch (const std::invalid_argument) {
                    cerr << "El parámetro que indica la acción no es correcto. Insertar valores entre 0 - 4." << endl;
                    exit(-1);
                }
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

        if (rutaEntrada == NULL) {
            cerr << "No se ha especificado el fichero de entrada. Inserte el parámetro -i seguido de la ruta." << endl;
            exit(-1);
        }
        if (rutaSalida == NULL) {
            cerr << "No se ha especificado el fichero de salida. Inserte el parámetro -o seguido de la ruta." << endl;
            exit(-1);
        }
        if (ejecucion == -1) {
            cerr << "No se ha especificado la acción a realizar. Inserte el parámetro -u seguido de la acción (0-4)."
                 << endl;
            exit(-1);
        }

        imagenToString(rutaEntrada);

        switch (ejecucion) {
            case 0: {
                if (parametroExtra == NULL) {
                    cerr
                            << "No se ha especificado el número de tramos. Inserte el parámetro -t seguido del número de tramos."
                            << endl;
                    exit(-1);
                }
                double **resultado = escalaGrises(stringToMatrizR(), stringToMatrizG(), stringToMatrizB());
                try {
                    histograma(resultado, rutaSalida, stoi(parametroExtra));
                } catch (const std::invalid_argument) {
                    cerr << "El parámetro indicado por -t tiene que ser un número entero." << endl;
                    exit(-1);
                }
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
                if (parametroExtra == NULL) {
                    cerr
                            << "No se ha especificado el radio del filtro B/N. Inserte el parámetro -r seguido del radio del filtro."
                            << endl;
                    exit(-1);
                }
                try {
                    filtroBN(stringToMatrizR(), stringToMatrizG(), stringToMatrizB(), stod(parametroExtra));
                } catch (const std::invalid_argument) {
                    cerr << "El parámetro indicado por -r tiene que ser un número decimal." << endl;
                    exit(-1);
                }
                break;
            }
            default:
                cerr << "El parámetro que indica la acción no es correcto. Insertar valores entre 0 - 4.";
                exit(-1);
        }
        return 0;
    }