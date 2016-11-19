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

int ALTURA = 0;
int ANCHURA = 0;

struct pixel {
    unsigned char r;
    unsigned char g;
    unsigned char b;
};


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
    int alturaMatrizActual = 0;
    int anchuraMatrizActual = 0;
    pixel **matrizPixeles;
    if (archivo.is_open()) {
        archivo.read((char *) &alturaMatrizActual, 4);
        archivo.read((char *) &anchuraMatrizActual, 4);

        if (ALTURA != 0 && ANCHURA != 0 && (ALTURA != alturaMatrizActual || ANCHURA != anchuraMatrizActual)) {
            cerr << "Las dimensiones de la máscara no coinciden con las dimensiones de la imagen original." << endl
                 << "Es posible que la ruta del fichero de máscara no sea correcta." << endl;
            exit(-1);
        } else {
            ALTURA = alturaMatrizActual;
            ANCHURA = anchuraMatrizActual;
        }

        struct stat st;
        stat(rutaEntrada, &st);
        int fileSize = (int) st.st_size;

        if (fileSize != ALTURA * ANCHURA * 3 + 8) {
            cerr << "El fichero de entrada no tiene un formato correcto." << endl;
            exit(-1);
        }

        matrizPixeles = new pixel *[ALTURA];
        for (int i = 0; i < ALTURA; ++i) {
            matrizPixeles[i] = new pixel[ANCHURA];
        }

        char *buffer = new char[fileSize - 8];

        archivo.seekg(8, ios::beg);
        archivo.read(buffer, fileSize - 8);

        for (int canal = 0; canal < 3; canal++) {
            for (int i = 0; i < ALTURA; ++i) {
                for (int j = 0; j < ANCHURA; ++j) {
                    switch (canal) {
                        case 0:
                            matrizPixeles[i][j].r = (unsigned char) buffer[i * ANCHURA + j];
                            break;
                        case 1:
                            matrizPixeles[i][j].g = (unsigned char) buffer[i * ANCHURA + j + ANCHURA * ALTURA];
                            break;
                        case 2:
                            matrizPixeles[i][j].b = (unsigned char) buffer[i * ANCHURA + j + ANCHURA * ALTURA * 2];
                            break;
                        default:
                            break;
                    }
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
 * Método que escribe en un fichero binario la matriz de píxeles recibida por parámetros.
 * La escritura comienza escribiendo las variables globales ALTURA y ANCHURA, para después recorrer los canales
 * de la matriz de píxeles guardando en una cadena de caracteres los colores uno detrás de otro. Esta cadena será
 * escrita en la ruta pasada por parámetros, creando el fichero si no existe.
 * Se comprueba que el archivo se ha creado correctamente antes de intentar escribir en él.
 * @param matrizPixeles Matriz que será escrita en el archivo binario.
 * @param rutaSalida Ruta del archivo binario que será escrito.
 */
void escribirSalida(pixel **matrizPixeles, char *rutaSalida) {
    ofstream archivo(rutaSalida, ios::binary);
    if (archivo.is_open()) {
        unsigned char *buffer = new unsigned char[ALTURA * ANCHURA * 3];

        archivo.write((char *) &ALTURA, 4);
        archivo.write((char *) &ANCHURA, 4);

        for (int channel = 0; channel < 3; channel++) {
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
                        default:
                            break;
                    }
                }
            }
        }
        archivo.write((const char *) buffer, ALTURA * ANCHURA * 3);
        archivo.close();
        delete[] matrizPixeles;
    } else {
        cerr << "El fichero de salida no se ha creado correctamente."
                "Es posible que la ruta de salida no sea correcta." << endl;
        exit(-1);
    }
}


/**
 * Escribe en un archivo los valores máximos y mínimos de todas las matrices de colores, en el
 * orden de Rojo, Verde y Azul.
 * Se crea un array de tamaño 6 que contiene en sus primeras 3 posiciones los valores máximos de
 * las tres matrices y en sus siguientes 3 posiciones los valores mínimos. Recorre los tres canales
 * de color RGB en un for independiente, comparando los valores leídos con los anteriores valores
 * máximos y mínimos,
 * actualizándolos si es necesario.
 * @param matrizPixeles Matriz de píxeles que se corresponde con la imagen a analizar
 * @param rutaSalida Archivo en el que se escribirá el resultado.
 */

void calcularMaximosYMinimos(pixel **matrizPixeles, char *rutaSalida) {
    auto start = std::chrono::high_resolution_clock::now();
    array<int, 6> maximosYMinimos = {0, 0, 0, 0, 0, 0};
    for (int i = 0; i < ALTURA; ++i) {
        for (int j = 0; j < ANCHURA; ++j) {
            if (matrizPixeles[i][j].r > maximosYMinimos[0]) {
                maximosYMinimos[0] = matrizPixeles[i][j].r;
                break;
            }
            if (matrizPixeles[i][j].r < maximosYMinimos[3]) {
                maximosYMinimos[3] = matrizPixeles[i][j].r;
                break;
            }
        }
    }
    for (int i = 0; i < ALTURA; ++i) {
        for (int j = 0; j < ANCHURA; ++j) {
            if (matrizPixeles[i][j].g > maximosYMinimos[1]) {
                maximosYMinimos[1] = matrizPixeles[i][j].g;
                break;
            }
            if (matrizPixeles[i][j].g < maximosYMinimos[4]) {
                maximosYMinimos[4] = matrizPixeles[i][j].g;
                break;
            }
        }
    }
    for (int i = 0; i < ALTURA; ++i) {
        for (int j = 0; j < ANCHURA; ++j) {
            if (matrizPixeles[i][j].b > maximosYMinimos[2]) {
                maximosYMinimos[2] = matrizPixeles[i][j].b;
                break;
            }
            if (matrizPixeles[i][j].b < maximosYMinimos[5]) {
                maximosYMinimos[5] = matrizPixeles[i][j].b;
                break;
            }
        }
    }

    ofstream outputFile(rutaSalida);
    if (!outputFile.is_open()) {
        cerr << "El fichero de salida no se ha creado correctamente."
                "Es posible que la ruta de salida no sea correcta." <<
             endl;
        exit(-1);
    }
    outputFile << maximosYMinimos[0] << " " << maximosYMinimos[3] << " "
               << maximosYMinimos[1] << " " << maximosYMinimos[4] << " "
               << maximosYMinimos[2] << " " << maximosYMinimos[5];

    delete[] matrizPixeles;
    auto elapsed = std::chrono::high_resolution_clock::now() - start;
    long long microseconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
    cout << "Tiempo transcurrido: " << microseconds << " microsegundos\n";
}

/**
 *La función histograma recibe como parametros una matriz de estrucutras pixel que contiene la imagen sobre la que se tiene
 * que realizar el histograma, una ruta a un fichero en el que se escribirá el histograma y el número de tramos que deberá tener.
 *
 * Primero se comprueba que el número de tramos pasado como parametro es mayor que 0. Despueés se crea un array donde se almacenará
 * el histograma, se calcula el rango de valores que tendrá cada tramo y el valor medio del histograma.
 *
 * En segundo lugar se recorre la matriz de pixeles para transformarla a escala de grises, y se coloca cada valor en el tramo
 * correspondiente. Para ello, primero se comprueba si el valor en escala de grises es mayor o menor que el valor medio del
 * histograma. Una vez que se realiza esta comprobación, se compara el valor en escala de grises con los valores extremos de
 * cada tramo. En caso de que el valor se encuentre entre estos dos extremos, significa que está contenido en ese tramo, y se
 * suma 1 al número de valores que hay en ese tramo.
 *
 * Por último, se escribe el array resultante en el fichero de salida, especifcado como parametro,que contiene
 * el histograma completo.
 *
 * @param matrizPixeles
 * @param rutaSalida
 * @param tramos
 */
void histograma(pixel **matrizPixeles, char *rutaSalida, int tramos) {
    if (tramos <= 0) {
        cerr << "El número de tramos debería ser mayor que 0.";
        exit(-1);
    }
    vector<int> result(tramos);
    double grises;
    double valoresTramo = 256 / (double) tramos;
    double m = (tramos / 2) * valoresTramo;

    for (int i = 0; i < ALTURA; i++) {
        for (int j = 0; j < ANCHURA; j++) {
            grises = matrizPixeles[i][j].r * 0.3 + matrizPixeles[i][j].g * 0.59 + matrizPixeles[i][j].b * 0.11;

            if (grises < m) {
                for (int contador = 0; contador < tramos; contador++) {
                    if (grises >= contador * valoresTramo &&
                        grises < (contador + 1) * valoresTramo) {
                        result[contador] = result[contador] + 1;
                        break;
                    }
                }
            } else {
                for (int contador = tramos / 2; contador < tramos; contador++) {
                    if (grises >= contador * valoresTramo &&
                        grises < (contador + 1) * valoresTramo) {
                        result[contador] = result[contador] + 1;
                        break;
                    }
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

    for (int i = 0; i < tramos; ++i) {
        outputFile << result[i];
        if (i != tramos - 1) {
            outputFile << " ";
        }
    }
    delete[] matrizPixeles;
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
void filtroBN(pixel **matrizPixeles, double radio, char *rutaSalida) {
    int centroX = ANCHURA / 2;
    int centroY = ALTURA / 2;

    for (int i = 0; i < ALTURA; ++i) {
        for (int j = 0; j < ALTURA; ++j) {
            double suma = pow(i - centroY, 2) + pow(j - centroX, 2);
            if (suma > pow(radio, 2)) {
                matrizPixeles[i][j].r = (unsigned char) (matrizPixeles[i][j].r * 0.3);
                matrizPixeles[i][j].g = (unsigned char) (matrizPixeles[i][j].g * 0.59);
                matrizPixeles[i][j].b = (unsigned char) (matrizPixeles[i][j].b * 0.11);
            }
        }
    }
    escribirSalida(matrizPixeles, rutaSalida);
}

/**
 * Aplica una mascara a la imagen de entrada.
 *
 * En esta función se recorre la matriz de la imagen de entrada ("imagen") con dos bucles for anidados multiplicando
 * cada valor RGB de cada píxel de la matriz "imagen" por el valor RGB correspondiente de la matriz "mascara".
 *
 * El resultado se escribe en un fichero de salida llamando a la función "escribirSalida"
 *
 * @param matrizPixeles
 * @param mascara
 * @param rutaSalida
 */

void mascara(pixel **matrizPixeles, pixel **matrizMascara, char *rutaSalida) {
    for (int i = 0; i < ALTURA; ++i) {
        for (int j = 0; j < ANCHURA; ++j) {
            matrizPixeles[i][j].r = matrizPixeles[i][j].r * matrizMascara[i][j].r;
            matrizPixeles[i][j].g = matrizPixeles[i][j].g * matrizMascara[i][j].g;
            matrizPixeles[i][j].b = matrizPixeles[i][j].b * matrizMascara[i][j].b;
        }
    }
    escribirSalida(matrizPixeles, rutaSalida);
    delete[] matrizMascara;
}

/**
 * Rota la imagen un número de grados indicado por parámetro.
 *
 * La rotación se realiza tomando como centro de la rotación el centro de la imagen.
 *
 * Para poder llevar a cabo la función se crea una matriz auxiliar llamada "rotada" que es la
 * matriz en la que se irán colocando los píxeles una vez han sido rotados.
 *
 * Para cambiar los píxeles de la matriz original a la matriz auxiliar se recorre la matriz original y en cada iteración
 * se calcula la posición a la que se va a rotar el pixel ("coorXrotada" y "coorYrotada").
 * Para calcularla se aplica la ecuación de rotación correpondiente a cada coordenada en la que i-filaCentro y j-colCentro hacen
 * referencia a la posición del píxel con respecto al centro de la imagen.
 *
 * Una vez calculada la posición que va a ocupar el píxel en la nueva matriz se comprueba si se sale del rango de
 * la matriz con un if. Si no se sale entonces se introduce el pixel en la matriz auxiliar.
 *
 * Una vez se ha llenado la matriz auxiliar se llama a la función "escribirSalida" para que la escriba en un fichero de salida.
 *
 * @param matrizPixeles
 * @param grados
 * @param rutaSalida
 * */

void rotacion(pixel **matrizPixeles, double grados, char *rutaSalida) {
    double filaCentro = ALTURA / 2;
    double colCentro = ANCHURA / 2;
    int coorXrotada = 0;
    int coorYrotada = 0;
    double radianes = grados * M_PI / 180;

    pixel **rotada = new pixel *[ALTURA]();
    for (int i = 0; i < ALTURA; i++) {
        rotada[i] = new pixel[ANCHURA]();
    }

    for (int i = 0; i < ALTURA; ++i) {
        for (int j = 0; j < ANCHURA; ++j) {

            coorXrotada = (int) ceil((cos(radianes) * (j - colCentro) - sin(radianes) * (i - filaCentro)) + colCentro);
            coorYrotada = (int) ceil((sin(radianes) * (j - colCentro) + cos(radianes) * (i - filaCentro)) + filaCentro);

            if (coorXrotada >= 0 && coorXrotada <= ANCHURA - 1 && coorYrotada >= 0 && coorYrotada <= ALTURA - 1) {
                rotada[coorYrotada][coorXrotada] = matrizPixeles[i][j];
            }
        }
    }
    escribirSalida(rotada, rutaSalida);
    delete[] matrizPixeles;
}

int main(int argv, char **argc) {
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
                cerr << "No se ha especificado correctamente la acción a realizar. "
                        "Inserte el parámetro -u seguido de la acción a realizar, "
                        "utilizando valores entre 0 - 4" << endl
                     << "0: Histograma blanco y negro." << endl
                     << "1: Máximos y Mínimos." << endl
                     << "2: Aplicación de máscara." << endl
                     << "3: Rotación de imagen." << endl
                     << "4: Filtro blanco y negro." << endl;;
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
                histograma(generarMatrizPixeles(rutaEntrada), rutaSalida, stoi(parametroExtra));
            } catch (const std::invalid_argument) {
                cerr << "El parámetro indicado por -t tiene que ser un NÚMERO ENTERO." << endl;
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
                cerr << "El parámetro indicado por -a tiene que ser un NÚMERO DECIMAL." << endl;
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
                if (stod(parametroExtra) < 0) {
                    cerr << "El parámetro indicado por -r tiene que ser un número decimal POSITIVO." << endl;
                    exit(-1);
                }
                filtroBN(generarMatrizPixeles(rutaEntrada), stod(parametroExtra), rutaSalida);
            } catch (const std::invalid_argument) {
                cerr << "El parámetro indicado por -r tiene que ser un NÚMERO DECIMAL positivo." << endl;
                exit(-1);
            }
            break;
        }
        default:
            cerr << "No se ha especificado la acción a realizar. "
                    "Inserte el parámetro -u seguido de la acción a realizar, "
                    "utilizando VALORES ENTRE 0 - 4:" << endl
                 << "0: Histograma blanco y negro." << endl
                 << "1: Máximos y Mínimos." << endl
                 << "2: Aplicación de máscara." << endl
                 << "3: Rotación de imagen." << endl
                 << "4: Filtro blanco y negro." << endl;
            exit(-1);
    }

    return 0;
}