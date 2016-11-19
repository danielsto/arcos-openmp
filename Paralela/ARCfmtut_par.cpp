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

double t1 = 0;
double t2 = 0;

struct pixel {
    unsigned char r;
    unsigned char g;
    unsigned char b;
};


void escribirSalida(pixel **matrizPixeles, char *rutaSalida) {

//    t1 = t1 + omp_get_wtime();

    ofstream archivo(rutaSalida, ios::binary);
    if (archivo.is_open()) {

        unsigned char *buffer = new unsigned char[ALTURA * ANCHURA * 3];

        archivo.write((char *) &ALTURA, 4);
        archivo.write((char *) &ANCHURA, 4);

        //#pragma omp parallel for
        for (int channel = 0; channel < 3; channel++) {
        #pragma omp parallel for collapse(2)
            for (int i = 0; i < ALTURA; ++i) {
                for (int j = 0; j < ANCHURA; ++j) {
                    switch (channel) {
                        case 0:
                            buffer[i * ANCHURA + j] = matrizPixeles[i][j].r;
                            break;
                        case 1:
                            buffer[i * ANCHURA + j + ALTURA * ANCHURA] = matrizPixeles[i][j].g;
                            break;
                        case 2:
                            buffer[i * ANCHURA + j + ALTURA * ANCHURA * 2] = matrizPixeles[i][j].b;
                            break;
                    }

                }
            }
        }
        archivo.write((const char *) buffer, ALTURA * ANCHURA * 3);
        archivo.close();
    } else {
        cerr << "El fichero de salida no se ha creado correctamente." << endl;
        exit(-1);
    }

//    t2 = t2 + omp_get_wtime();

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

//    t1 = t1 + omp_get_wtime();


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


        //#pragma omp parallel for
        for (int channel = 0; channel < 3; channel++) {
        #pragma omp parallel for collapse(2)
            for (int i = 0; i < ALTURA; ++i) {
                for (int j = 0; j < ANCHURA; ++j) {
                    if (!archivo.eof()) {
                        switch (channel) {
                            case 0:
                                matrizPixeles[i][j].r = (unsigned char) buffer[i * ANCHURA + j];

                                break;
                            case 1:
                                matrizPixeles[i][j].g = (unsigned char) buffer[i * ANCHURA + j + ANCHURA * ALTURA];

                                break;
                            case 2:
                                matrizPixeles[i][j].b = (unsigned char) buffer[i * ANCHURA + j + ANCHURA * ALTURA * 2];

                                break;
                        }
                    }

                }

            }
        }

        archivo.close();
    } else {
        cerr << "El fichero de entrada no existe en la ruta especificada." << endl;
        exit(-1);
    }


//    t2 = t2 + omp_get_wtime();


    return matrizPixeles;
}

/**
 * Escribe en un archivo los valores máximos y mínimos de todas las matrices de colores, en el
 * orden de Rojo, Verde y Azul.
 * Se crea un array de tamaño 6 que contiene en sus primeras 3 posiciones los valores máximos de
 * las tres matrices y en sus siguientes 3 posiciones los valores mínimos. Recorre los tres canales
 * de color RGB, comparando los valores leídos con los anteriores valores máximos y mínimos,
 * actualizándolos si es necesario.
 * @param matriz Matriz de píxeles que se corresponde con la imagen a analizar
 * @param rutaSalida Archivo en el que se escribirá el resultado.
 */
/*
void calcularMaximosYMinimos(pixel **matriz, char *rutaSalida) {
    array<int, 6> maximosYMinimos = {0, 0, 0, 0, 0, 0};

    #pragma omp parallel for collapse(2) shared(maximosYMinimos)
    for (int i = 0; i < ALTURA; ++i) {
        for (int j = 0; j < ANCHURA; ++j) {

            omp_set_num_threads(6);
            #pragma omp sections
            {
                #pragma omp section
                if (matriz[i][j].r > maximosYMinimos[0]) {
                    maximosYMinimos[0] = matriz[i][j].r;

                }
                #pragma omp section
                if (matriz[i][j].r < maximosYMinimos[3]) {
                    maximosYMinimos[3] = matriz[i][j].r;

                }
                #pragma omp section
                if (matriz[i][j].g > maximosYMinimos[1]) {
                    maximosYMinimos[1] = matriz[i][j].g;

                }
                #pragma omp section
                if (matriz[i][j].g < maximosYMinimos[4]) {
                    maximosYMinimos[4] = matriz[i][j].g;

                }
                #pragma omp section
                if (matriz[i][j].b > maximosYMinimos[2]) {
                    maximosYMinimos[2] = matriz[i][j].b;

                }
                #pragma omp section
                if (matriz[i][j].b < maximosYMinimos[5]) {
                    maximosYMinimos[5] = matriz[i][j].b;

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
}
*/

void calcularMaximosYMinimos(pixel **matriz, char *rutaSalida) {

    //t1 = t1 + omp_get_wtime();
    array<int, 6> maximosYMinimos = {0, 0, 0, 0, 0, 0};

    //#pragma omp parallel for
    for (int canal = 0; canal < 3; ++canal) {
        #pragma omp parallel for collapse(2)
        for (int i = 0; i < ALTURA; ++i) {
            for (int j = 0; j < ANCHURA; ++j) {
                switch (canal) {
                    case 0:
                        if (matriz[i][j].r > maximosYMinimos[0]) {
                            maximosYMinimos[0] = matriz[i][j].r;
                            break;
                        }
                        if (matriz[i][j].r < maximosYMinimos[3]) {
                            maximosYMinimos[3] = matriz[i][j].r;
                            break;
                        }
                        break;

                    case 1:
                        if (matriz[i][j].g > maximosYMinimos[1]) {
                            maximosYMinimos[1] = matriz[i][j].g;
                            break;
                        }
                        if (matriz[i][j].g < maximosYMinimos[4]) {
                            maximosYMinimos[4] = matriz[i][j].g;
                            break;
                        }
                        break;
                    case 2:
                        if (matriz[i][j].b > maximosYMinimos[2]) {
                            maximosYMinimos[2] = matriz[i][j].b;
                            break;
                        }
                        if (matriz[i][j].b < maximosYMinimos[5]) {
                            maximosYMinimos[5] = matriz[i][j].b;
                            break;
                        }
                        break;
                    default:
                        break;
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

    //t2 = t2 + omp_get_wtime();

}


/**
 * Construye una matriz con los valores de los píxeles de la imagen transformados a escala de
 * grises necesaria por parámetro para la función histograma()
 * @param rojo
 * @param verde
 * @param azul
 * @return
 */

void histograma(pixel **matrizPixeles, char *rutaSalida, int tramos) {

    //t1 = t1 + omp_get_wtime();
    if (tramos <= 0) {
        cerr << "El número de tramos debería ser mayor que 0.";
        exit(-1);
    }

    vector<int> histogramaFinal(tramos);
    double grises = 0;
    double intervaloSize = 256 / (double) tramos;

    vector<int *> arrayHistogramasPrivados(omp_get_max_threads());
    #pragma omp parallel
    {
        int hilos = omp_get_num_threads();
        vector<int> histogramaPrivadoHilo(tramos);
        arrayHistogramasPrivados[omp_get_thread_num()] = histogramaPrivadoHilo.data();

        #pragma omp for
        for (int i = 0; i < ALTURA; i++) {
            for (int j = 0; j < ANCHURA; j++) {
                grises = matrizPixeles[i][j].r * 0.3 + matrizPixeles[i][j].g * 0.59 + matrizPixeles[i][j].b * 0.11;
                int intervalo = (int) floor(grises / intervaloSize);
                histogramaPrivadoHilo[intervalo] += 1;
            }
        }
        #pragma omp for
            for (int j = 0; j < hilos; ++j) {
                for (int i = 0; i < tramos; ++i) {
                histogramaFinal[i] += arrayHistogramasPrivados[j][i];
            }
        }
    }
        ofstream outputFile(rutaSalida);
        if (!outputFile.is_open()) {
            cerr << "El fichero de salida no se ha creado correctamente."
                    "Es posible que la ruta de salida no sea correcta." << endl;
            exit(-1);
        }

        for (int i = 0; i < tramos; ++i) {
            outputFile << histogramaFinal[i];
            if (i != tramos - 1) {
                outputFile << " ";
            }
        }
        delete[] matrizPixeles;
        //t2 = t2 + omp_get_wtime();
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

    t1 = t1 + omp_get_wtime();

    int centroX = ANCHURA / 2;
    int centroY = ALTURA / 2;

    #pragma omp parallel for collapse(2)
    for (int i = 0; i < ALTURA; ++i) {
        for (int j = 0; j < ALTURA; ++j) {
            double suma = pow(i - centroY, 2) + pow(j - centroX, 2);
            if (suma > pow(radio, 2)) {
                matriz[i][j].r = (unsigned char) (matriz[i][j].r * 0.3);
                matriz[i][j].g = (unsigned char) (matriz[i][j].g * 0.59);
                matriz[i][j].b = (unsigned char) (matriz[i][j].b * 0.11);
            }
        }

    }

    t2 = t2 +omp_get_wtime();

    escribirSalida(matriz, rutaSalida);
}


/*
 * Se realiza un parallel for
 */
void mascara(pixel **imagen, pixel **mascara, char *rutaSalida) {

    //t1 = t1 + omp_get_wtime();

    #pragma omp parallel for
    for (int i = 0; i < ALTURA; ++i) {
        for (int j = 0; j < ANCHURA; ++j) {
            imagen[i][j].r = imagen[i][j].r * mascara[i][j].r;
            imagen[i][j].g = imagen[i][j].g * mascara[i][j].g;
            imagen[i][j].b = imagen[i][j].b * mascara[i][j].b;

        }
    }

    //t2 = t2 +omp_get_wtime();

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
    #pragma omp parallel for
    for (int i = 0; i < ALTURA; i++) {
        rotada[i] = new pixel[ANCHURA];
    }

    #pragma omp parallel for collapse(2)
    for (int i = 0; i < ALTURA; ++i) {
        for (int j = 0; j < ANCHURA; ++j) {
            rotada[i][j].r = 0;
            rotada[i][j].g = 0;
            rotada[i][j].b = 0;
        }
    }

    //#pragma omp parallel for collapse(2) private()
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
//    double t1 = omp_get_wtime();

    char *rutaEntrada = NULL;
    char *rutaSalida = NULL;
    char *parametroExtra = NULL;
    char *parametroLetra = NULL;
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
            parametroLetra = argc[i];
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

    switch (ejecucion) {
        case 0: {
            if (parametroExtra == NULL || strcmp(parametroLetra, "-t") != 0) {
                cerr << "No se ha especificado el número de tramos. "
                        "Inserte el parámetro -t seguido del número de tramos."
                     << endl;
                exit(-1);
            }

            try {
                histograma(generarMatrizPixeles(rutaEntrada),rutaSalida, stoi(parametroExtra));
            } catch (const std::invalid_argument) {
                cerr << "El parámetro indicado por -t tiene que ser un número entero." << endl;
                exit(-1);
            }
            break;
        }
        case 1: {
            for (int i = 0; i <10; ++i) {
                calcularMaximosYMinimos(generarMatrizPixeles(rutaEntrada), rutaSalida);
            }

            break;
        }
        case 2: {
            if (parametroExtra == NULL || strcmp(parametroLetra, "-f") != 0) {
                cerr << "No se ha especificado la dirección del fichero de máscara. "
                        "Inserte el parámetro -f seguido del ángulo de la rotación de la imagen."
                     << endl;
                exit(-1);
            }
            for (int i = 0; i < 10 ; ++i) {
                mascara(generarMatrizPixeles(rutaEntrada), generarMatrizPixeles(parametroExtra), rutaSalida);
            }

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
                for (int i = 0; i < 10 ; ++i) {
                    rotacion(generarMatrizPixeles(rutaEntrada), stod(parametroExtra), rutaSalida);
                }
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
                for (int i = 0; i <10 ; ++i) {
                    filtroBN(generarMatrizPixeles(rutaEntrada), stod(parametroExtra), rutaSalida);
                }

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

//    double t2 = omp_get_wtime();
      double diff = (t2-t1)*pow(10,6);
      cout << "Tiempo transcurrido: " << diff/10 << " microsegundos" << endl;


    return 0;
}