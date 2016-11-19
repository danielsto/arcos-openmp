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
#include <math.h>
#include <sys/stat.h>

using namespace std;

int ALTURA;
int ANCHURA;

struct pixel {
    unsigned char r;
    unsigned char g;
    unsigned char b;
};


void escribirSalida(pixel **matrizPixeles, char *rutaSalida) {
    ofstream archivo(rutaSalida, ios::binary);
    if (archivo.is_open()) {

        unsigned char *buffer = new unsigned char[ALTURA * ANCHURA * 3];

        archivo.write((char *) &ALTURA, 4);
        archivo.write((char *) &ANCHURA, 4);

#pragma omp parallel
        {
#pragma omp for collapse(2)
            for (int i = 0; i < ALTURA; ++i) {
                for (int j = 0; j < ANCHURA; ++j) {
                    buffer[i * ANCHURA + j] = matrizPixeles[i][j].r;
                }
            }
#pragma omp for collapse(2)
            for (int i = 0; i < ALTURA; ++i) {
                for (int j = 0; j < ANCHURA; ++j) {
                    buffer[i * ANCHURA + j + ALTURA * ANCHURA] = matrizPixeles[i][j].g;

                }
            }
#pragma omp for collapse(2)
            for (int i = 0; i < ALTURA; ++i) {
                for (int j = 0; j < ANCHURA; ++j) {
                    buffer[i * ANCHURA + j + ALTURA * ANCHURA * 2] = matrizPixeles[i][j].b;
                }
            }
        }

        archivo.write((const char *) buffer, ALTURA * ANCHURA * 3);
        archivo.close();
    } else {
        cerr << "El fichero de salida no se ha creado correctamente." << endl;
        exit(-1);
    }
}

/**
 * Método que lee el archivo de imagen de entrada y vierte los primeros dos grupos de
 * 4 bytes en las variables globales ALTURA y ANCHURA respectivamente. El resto del fichero
 * se vierte en una matriz de estructuras tipo pixel {r, g, b} en su atributo de canal
 * correspondiente.
 *
 * @param rutaEntrada Indica la ruta del archivo de entrada a leer.
 */
pixel **generarMatrizPixeles(char *rutaEntrada) {
    ifstream archivo(rutaEntrada, ios::binary);
    pixel **matrizPixeles;
    if (archivo.is_open()) {
        archivo.read((char *) &ALTURA, 4);
        archivo.read((char *) &ANCHURA, 4);

        matrizPixeles = new pixel *[ALTURA];
        for (int i = 0; i < ALTURA; ++i) {
            matrizPixeles[i] = new pixel[ANCHURA];
        }

        struct stat st;
        stat(rutaEntrada, &st);


        int fileSize = (int) st.st_size;

        if (fileSize != ALTURA * ANCHURA * 3 + 8) {
            cerr << "El formato del fichero no es correcto" << endl;
        }

        char *buffer = new char[fileSize - 8];

        archivo.seekg(8, ios::beg);
        archivo.read(buffer, fileSize - 8);


#pragma omp parallel
        {
#pragma omp for
            for (int i = 0; i < ALTURA; ++i) {
                for (int j = 0; j < ANCHURA; ++j) {
                    matrizPixeles[i][j].r = (unsigned char) buffer[i * ANCHURA + j];
                }
            }
#pragma omp for
            for (int i = 0; i < ALTURA; ++i) {
                for (int j = 0; j < ANCHURA; ++j) {
                    matrizPixeles[i][j].g = (unsigned char) buffer[i * ANCHURA + j + ANCHURA * ALTURA];

                }
            }
#pragma omp for
            for (int i = 0; i < ALTURA; ++i) {
                for (int j = 0; j < ANCHURA; ++j) {
                    matrizPixeles[i][j].b = (unsigned char) buffer[i * ANCHURA + j + ANCHURA * ALTURA * 2];
                }
            }
        }


        archivo.close();
    } else {
        cerr << "El fichero de entrada no existe en la ruta especificada." << endl;
        exit(-1);
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

void calcularMaximosYMinimos(pixel **matrizPixeles, char *rutaSalida) {
    array<int, 6> maximosYMinimos = {0, 0, 0, 0, 0, 0};
#pragma omp parallel
    {
#pragma omp for
        for (int i = 0; i < ALTURA; ++i) {
            for (int j = 0; j < ANCHURA; ++j) {
                if (matrizPixeles[i][j].r > maximosYMinimos[0]) {
                    maximosYMinimos[0] = matrizPixeles[i][j].r;
                }
                if (matrizPixeles[i][j].r < maximosYMinimos[3]) {
                    maximosYMinimos[3] = matrizPixeles[i][j].r;
                }
            }
        }
#pragma omp for
        for (int i = 0; i < ALTURA; ++i) {
            for (int j = 0; j < ANCHURA; ++j) {
                if (matrizPixeles[i][j].g > maximosYMinimos[1]) {
                    maximosYMinimos[1] = matrizPixeles[i][j].g;
                }
                if (matrizPixeles[i][j].g < maximosYMinimos[4]) {
                    maximosYMinimos[4] = matrizPixeles[i][j].g;
                }
            }
        }
#pragma omp for
        for (int i = 0; i < ALTURA; ++i) {
            for (int j = 0; j < ANCHURA; ++j) {
                if (matrizPixeles[i][j].b > maximosYMinimos[2]) {
                    maximosYMinimos[2] = matrizPixeles[i][j].b;
                }
                if (matrizPixeles[i][j].b < maximosYMinimos[5]) {
                    maximosYMinimos[5] = matrizPixeles[i][j].b;
                }
            }
        }
    }

    ofstream outputFile(rutaSalida);
    if (!outputFile.is_open()) {
        cerr << "El fichero de salida no se ha creado correctamente."
                "Es posible que la ruta de salida no sea correcta." << endl;
        exit(-1);
    }
    outputFile << maximosYMinimos[0] << " " << maximosYMinimos[3] << " "
               << maximosYMinimos[1] << " " << maximosYMinimos[4] << " "
               << maximosYMinimos[2] << " " << maximosYMinimos[5];

    delete[] matrizPixeles;
}

/**
 * Construye una matriz con los valores de los píxeles de la imagen transformados a escala de
 * grises necesaria por parámetro para la función histograma()
 * @param rojo
 * @param verde
 * @param azul
 * @return
 */
void histograma(pixel **matriz, char *rutaSalida, int tramos) {
    auto start = std::chrono::high_resolution_clock::now();

    vector<int> result(tramos);
    double grises;
    double valoresTramo = 256 / (double) tramos;


    for (int i = 0; i < ALTURA; i++) {
        for (int j = 0; j < ANCHURA; j++) {
            grises = matriz[i][j].r * 0.3 + matriz[i][j].g * 0.59 + matriz[i][j].b * 0.11;
            for (int contador = 0; contador < tramos; contador++) {
                if (grises >= contador * valoresTramo &&
                    grises < (contador + 1) * valoresTramo) {
                    result[contador] = result[contador] + 1;
                    break;
                }
            }
        }
    }

    ofstream outputFile(rutaSalida);


    for (int i = 0; i < tramos; ++i) {
        outputFile << result[i];
        if (i != tramos - 1) {
            outputFile << " ";
        }
    }

    auto elapsed = std::chrono::high_resolution_clock::now() - start;
    long long microseconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
    cout << "Tiempo total transcurrido escalaGrises: " << microseconds << " microsegundos\n";


}

/**
 * Aplica un filtro blanco y negro a las regiones de la imagen que quedan fuera del circulo de
 * radio indicado por parámetro y con centro en el centro de la imagen. Las regiones dentro del
 * círculo quedan igual.
 * @param matrizR
 * @param matrizG
 * @param matrizB
 * @param radio Radio del círculo dentro del cual no se aplicará el filtro
 * */
void filtroBN(pixel **matriz, double radio, char *rutaSalida) {
    int centroX = ANCHURA / 2;
    int centroY = ALTURA / 2;

#pragma omp parallel for
    for (int i = 0; i < ALTURA; ++i) {
        for (int j = 0; j < ALTURA; ++j) {
            double suma = pow(i - centroY, 2) + pow(j - centroX, 2);
            if (suma > pow(radio, 2)) {
                matriz[i][j].r *= 0.3;
                matriz[i][j].g *= 0.59;
                matriz[i][j].b *= 0.11;
            }
        }

    }
    escribirSalida(matriz, rutaSalida);
}

void mascara(pixel **imagen, pixel **mascara, char *rutaSalida) {
    for (int i = 0; i < ALTURA; ++i) {
        for (int j = 0; j < ANCHURA; ++j) {
            imagen[i][j].r *= mascara[i][j].r;
            imagen[i][j].g *= mascara[i][j].g;
            imagen[i][j].b *= mascara[i][j].b;
        }
    }
    escribirSalida(imagen, rutaSalida);
}

void rotacion(pixel **imagen, double grados, char *rutaSalida) {
    double yMax = ALTURA;
    double xMax = ANCHURA;
    double filaCentro = yMax / 2;
    double colCentro = xMax / 2;
    int coorXrotada;
    int coorYrotada;


    double radianes = grados * M_PI / 180;

    pixel **rotada = new pixel *[ALTURA];
    for (int i = 0; i < ALTURA; i++) {
        rotada[i] = new pixel[ANCHURA];
    }

    for (int i = 0; i < ALTURA; ++i) {
        for (int j = 0; j < ANCHURA; ++j) {
            rotada[i][j].r = 0;
            rotada[i][j].g = 0;
            rotada[i][j].b = 0;
        }
    }

    for (int i = 0; i < ALTURA; ++i) {
        for (int j = 0; j < ANCHURA; ++j) {


            coorXrotada = ceil((cos(radianes) * (j - colCentro) - sin(radianes) * (i - filaCentro)) + colCentro);
            coorYrotada = ceil((sin(radianes) * (j - colCentro) + cos(radianes) * (i - filaCentro)) + filaCentro);


            if (coorXrotada < 0 || coorXrotada > ANCHURA - 1 || coorYrotada < 0 || coorYrotada > ALTURA - 1) {
            } else {
                rotada[coorYrotada][coorXrotada].r = imagen[i][j].r;
                rotada[coorYrotada][coorXrotada].g = imagen[i][j].g;
                rotada[coorYrotada][coorXrotada].b = imagen[i][j].b;
            }
        }
    }

    escribirSalida(rotada, rutaSalida);

}

int main(int argv, char **argc) {
    auto start = std::chrono::high_resolution_clock::now();
    char *rutaEntrada = NULL;
    char *rutaSalida = NULL;
    char *parametroExtra = NULL;
    char *parametroLetra = NULL;
    int ejecucion = -1;
    for (int i = 1; i < argv; ++i) {
        cout << omp_get_thread_num() << endl;
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
            parametroLetra = argc[i];
            parametroExtra = argc[i + 1];
            i++;
            continue;
        }
    }
    auto elapsed = std::chrono::high_resolution_clock::now() - start;
    long long microseconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
    cout << "Tiempo transcurrido: " << microseconds << " microsegundos\n";

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

    switch (ejecucion) {
        case 0: {
            if (parametroExtra == NULL || strcmp(parametroLetra, "-t") != 0) {
                cerr << "No se ha especificado el número de tramos. "
                        "Inserte el parámetro -t seguido del número de tramos."
                     << endl;
                exit(-1);
            }

            try {
                histograma(generarMatrizPixeles(rutaEntrada), rutaSalida, stoi(parametroExtra));
            } catch (const std::invalid_argument) {
                cerr << "El parámetro indicado por -t tiene que ser un número entero." << endl;
                exit(-1);
            }
            break;
        }
        case 1: {
            calcularMaximosYMinimos(generarMatrizPixeles(rutaEntrada), rutaSalida);
            break;
        }
        case 2: {
            if (parametroExtra == NULL || strcmp(parametroLetra, "-f") != 0) {
                cerr << "No se ha especificado la dirección del fichero de máscara. "
                        "Inserte el parámetro -f seguido del ángulo de la rotación de la imagen."
                     << endl;
                exit(-1);
            }
            mascara(generarMatrizPixeles(rutaEntrada), generarMatrizPixeles(parametroExtra), rutaSalida);
            break;
        }
        case 3: {
            if (parametroExtra == NULL || strcmp(parametroLetra, "-a") != 0) {
                cerr << "No se ha especificado el ángulo de rotación. "
                        "Inserte el parámetro -a seguido del ángulo de la rotación de la imagen."
                     << endl;
                exit(-1);
            }
            try {
                rotacion(generarMatrizPixeles(rutaEntrada), stod(parametroExtra), rutaSalida);
            } catch (const std::invalid_argument) {
                cerr << "El parámetro indicado por -a tiene que ser un número decimal." << endl;
                exit(-1);
            }
            break;
        }
        case 4: {
            if (parametroExtra == NULL || strcmp(parametroLetra, "-r") != 0) {
                cerr << "No se ha especificado el radio del filtro B/N. "
                        "Inserte el parámetro -r seguido del radio del filtro."
                     << endl;
                exit(-1);
            }
            try {
                filtroBN(generarMatrizPixeles(rutaEntrada), stod(parametroExtra), rutaSalida);
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