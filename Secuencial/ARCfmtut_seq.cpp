#include <iostream>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <cstring>
#include <omp.h>
#include <chrono>

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
pixel **generarMatrizPixeles(char *rutaEntrada) {
    ifstream archivo(rutaEntrada, ios::binary);
    pixel **matrizPixeles;
    if (archivo.is_open()) {
        archivo.seekg(0, ios::beg);
        archivo.read((char *) &ALTURA, 4);
        archivo.seekg(4, ios::beg);
        archivo.read((char *) &ANCHURA, 4);

        matrizPixeles = new pixel *[ALTURA];
        for (int i = 0; i < ALTURA; ++i) {
            matrizPixeles[i] = new pixel[ANCHURA];
        }

        for(int channel = 0; channel < 3; channel++) {
            for(int i=0; i< ALTURA; ++i){
                for(int j=0; j< ANCHURA; ++j){
                    if(!archivo.eof()){
                        switch(channel) {
                            case 0: archivo.read((char *) &matrizPixeles[i][j].r, 1); break;
                            case 1: archivo.read((char *) &matrizPixeles[i][j].g, 1); break;
                            case 2: archivo.read((char *) &matrizPixeles[i][j].b, 1); break;
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
    for (int i = 0; i < tramos; ++i) {
        result[i] = 0;
    }
    double valoresTramo = 256 / (double)tramos;
    int contador = 0;

    for (int i = 0; i < ALTURA; i++) {
        for (int j = 0; j < ANCHURA; j++) {
            while (contador < tramos) {
                if (escalagrises[i][j] >= contador * valoresTramo &&
                    escalagrises[i][j] < (contador + 1) * valoresTramo) {
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

    for (int i = 0; i < tramos; ++i) {
        outputFile << result[i];
        outputFile << " ";
    }

}

void escribirSalida(pixel **matrizPixeles, char *rutaSalida) {
    ofstream archivo(rutaSalida, ios::binary);
    if (archivo.is_open()) {
        archivo.seekp(0, ios::beg);
        archivo.write((char *) &ALTURA, 4);
        archivo.seekp(4, ios::beg);
        archivo.write((char *) &ANCHURA, 4);


        for(int channel = 0; channel < 3; channel++) {
            for(int i=0; i< ALTURA; ++i){
                for(int j=0; j< ANCHURA; ++j){
                    switch(channel) {
                        case 0: archivo.write((char *) &matrizPixeles[i][j].r, 1); break;
                        case 1: archivo.write((char *) &matrizPixeles[i][j].g, 1); break;
                        case 2: archivo.write((char *) &matrizPixeles[i][j].b, 1); break;
                    }

                }
            }
        }
    } else {
        cerr << "El fichero de salida no se ha creado correctamente." << endl;
        exit(-1);
    }
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

    escribirSalida(matriz, rutaSalida);
}

void mascara (pixel **imagen, pixel **mascara, char *rutaSalida){



    for(int i=0; i<ALTURA; ++i) {
        for(int j=0; j<ANCHURA; ++j) {
            imagen[i][j].r = imagen[i][j].r * mascara[i][j].r;
            imagen[i][j].g = imagen[i][j].g * mascara[i][j].g;
            imagen[i][j].b = imagen[i][j].b * mascara[i][j].b;
        }
    }

    for (int i = 0; i < ALTURA; ++i) {
        for (int j = 0; j < ANCHURA; ++j) {
            stringstream rs; // red stream
            if(imagen[i][j].r>=0 && imagen[i][j].r<16){
                stringTotal.append("0");
            }
            rs << hex << imagen[i][j].r;
            stringTotal.append(rs.str());
        }
    }

    for (int i = 0; i < ALTURA; ++i) {
        for (int j = 0; j < ANCHURA; ++j) {
            stringstream gs; // green stream
            if(imagen[i][j].g>=0 && imagen[i][j].g<16){
                stringTotal.append("0");
            }
            gs << hex << imagen[i][j].g;
            stringTotal.append(gs.str());
        }
    }
    for (int i = 0; i < ALTURA; ++i) {
        for (int j = 0; j < ANCHURA; ++j) {
            stringstream bs; //blue stream
            if(imagen[i][j].b>=0 && imagen[i][j].b<16){
                stringTotal.append("0");
            }
            bs << hex << imagen[i][j].b;
            stringTotal.append(bs.str());
        }
    }

    ofstream ficheroSalida;
    ficheroSalida.open (rutaSalida);

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
    ficheroSalida << stringTotal;
    ficheroSalida.close();

}

void rotacion(pixel **imagen, double grados, char *rutaSalida){
    double yMax= ALTURA-1;
    double xMax= ANCHURA-1;
    int filaCentro= ceil(yMax/2);
    int colCentro= ceil(xMax/2);
    int coorX;
    int coorY;
    int coorXrotada;
    int coorYrotada;

    double radianes=grados*3.14159/180;

    pixel **rotada= new pixel*[ALTURA];
    for(int i=0; i<ALTURA; i++) {
        rotada[i] = new pixel[ANCHURA];
    }

    for(int i=0; i<ALTURA; ++i){
        for(int j=0; j<ANCHURA; ++j) {
            rotada[i][j].r= 0;
            rotada[i][j].g= 0;
            rotada[i][j].b= 0;
        }
    }

    for(int i=0; i<ALTURA; ++i){
        for(int j=0; j<ANCHURA; ++j) {
            coorX= j - colCentro;
            coorY= i - filaCentro;

            coorXrotada = round(cos(radianes)*coorX - sin(radianes)*coorY);
            coorYrotada = round(sin(radianes)*coorX + cos(radianes)*coorY);

            coorX= coorX +colCentro;
            coorY= coorY +filaCentro;
            coorXrotada= coorXrotada+colCentro;
            coorYrotada= coorYrotada+filaCentro;

            if(coorXrotada<0 || coorXrotada> ANCHURA-1 || coorYrotada<0 || coorYrotada> ALTURA-1){
            }
            else{
                rotada[coorYrotada][coorXrotada].r= imagen[coorY][coorX].r;
                rotada[coorYrotada][coorXrotada].g= imagen[coorY][coorX].g;
                rotada[coorYrotada][coorXrotada].b= imagen[coorY][coorX].b;
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

    switch (ejecucion) {
        case 0: {
            double **resultado = escalaGrises(generarMatrizPixeles(rutaEntrada));
            histograma(resultado, atoi(parametroExtra), rutaSalida);
            break;
        }
        case 1: {
            calcularMaximosYMinimos(generarMatrizPixeles(rutaEntrada), rutaSalida);
            break;
        }
        case 2: {
            break;
        }
        case 3: {
            rotacion(generarMatrizPixeles(rutaEntrada), atoi(parametroExtra), rutaSalida);
            break;
        }
        case 4: {
            filtroBN(generarMatrizPixeles(rutaEntrada), atoi(parametroExtra), rutaSalida);

            break;
        }
        default:
            cerr << "El parámetro que indica la acción no es correcto. Insertar valores entre 0 - 4.";
    }
    auto elapsed = std::chrono::high_resolution_clock::now() - start;
    long long microseconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
    cout << "Tiempo transcurrido: "<< microseconds << " microsegundos\n";
    return 0;
}