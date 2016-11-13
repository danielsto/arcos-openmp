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

int **stringToMatrizR(){
	int **matrizRoja= new int *[ALTURA];
	for(int i=0; i< ALTURA; ++i){
		matrizRoja[i]=new int[ANCHURA];
	}

	int columna=0;
    int filas=0;
	int decimal=0;

	int length= stringRed.length();

	for(int i=0; i <length; i= i + 2){
		if(columna==ANCHURA){
            columna=0;
            filas++;
        }
		stringstream ss;
        ss << hex << stringRed.substr(i,2);
        ss >> decimal;
        matrizRoja[filas][columna] = decimal;
		columna++;
	}

	return matrizRoja;
}

int **stringToMatrizG(){
	int **matrizVerde= new int *[ALTURA];
	for(int i=0; i< ALTURA; ++i){
		matrizVerde[i]=new int[ANCHURA];
	}

	int columna=0;
    int filas=0;
	int decimal=0;

	int length= stringGreen.length();

	for(int i=0; i <length; i= i + 2){
		if(columna==ANCHURA){
            columna=0;
            filas++;
        }
		stringstream ss;
        ss << hex << stringGreen.substr(i,2);
        ss >> decimal;
        matrizVerde[filas][columna] = decimal;
		columna++;
	}


	return matrizVerde;
}


int **stringToMatrizB(){
	int **matrizAzul= new int *[ALTURA];
	for(int i=0; i< ALTURA; ++i){
		matrizAzul[i]=new int[ANCHURA];
	}

	int columna=0;
    int filas=0;
	int decimal=0;

	int length= stringBlue.length();

	for(int i=0; i <length; i= i + 2){
		if(columna==ANCHURA){
            columna=0;
            filas++;
        }
		stringstream ss;
        ss << hex << stringBlue.substr(i,2);
        ss >> decimal;
        matrizAzul[filas][columna] = decimal;
		columna++;
	}


	return matrizAzul;
}


int mascara (int **rojoImg, int **verdeImg, int **azulImg, int **rojoMascara, int **verdeMascara, int **azulMascara)
{
    ofstream ficheroSalida;
    ficheroSalida.open ("salidaMascara.txt");
    for(int i=0; i<ALTURA; ++i) {
        for(int j=0; j<ANCHURA; ++j) {
            rojoImg[i][j] = rojoImg[i][j] * rojoMascara[i][j];
            verdeImg[i][j] = verdeImg[i][j] * verdeMascara[i][j];
            azulImg[i][j] = azulImg[i][j] * azulMascara[i][j];
        }
    }

    for (int i = 0; i < ALTURA; ++i) {
        for (int j = 0; j < ANCHURA; ++j) {
            stringstream rs; // red stream
            if(rojoImg[i][j]>=0 && rojoImg[i][j]<16){
                stringTotal.append("0");
            }
            rs << hex << rojoImg[i][j];
            stringTotal.append(rs.str());
        }
    }

    for(int i=0; i<ALTURA; ++i){
        for(int j=0; j<ANCHURA; ++j) {
            cout << rojoImg[i][j];
            cout << ' ';
        }
        cout << endl;
    }

    for (int i = 0; i < ALTURA; ++i) {
        for (int j = 0; j < ANCHURA; ++j) {
            stringstream gs; // green stream
            if(verdeImg[i][j]>=0 && verdeImg[i][j]<16){
                stringTotal.append("0");
            }
            gs << hex << verdeImg[i][j];
            stringTotal.append(gs.str());
        }
    }
    for (int i = 0; i < ALTURA; ++i) {
        for (int j = 0; j < ANCHURA; ++j) {
            stringstream bs; //blue stream
            if(azulImg[i][j]>=0 && azulImg[i][j]<16){
                stringTotal.append("0");
            }
            bs << hex << azulImg[i][j];
            stringTotal.append(bs.str());
        }
    }

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


    return 0;
}


int rotacion(int **matriz){
    double yMax= ALTURA-1;
    double xMax= ANCHURA-1;
    int filaCentro= ceil(yMax/2);
    int colCentro= ceil(xMax/2);
    int coorX;
    int coorY;
    int coorXrotada;
    int coorYrotada;
    int grados=90;
    double radianes=grados*3.14159/180;
    int ** rRotada= new int*[ALTURA];

    for(int i=0; i<ALTURA; i++) {
        rRotada[i] = new int[ANCHURA];
    }

    for(int i=0; i<ALTURA; ++i){
        for(int j=0; j<ANCHURA; ++j) {
           rRotada[i][j]= 6;
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
                rRotada[coorYrotada][coorXrotada]= matriz[coorY][coorX];
            }
        }
    }

    for(int i=0; i<ALTURA; ++i){
        for(int j=0; j<ANCHURA; ++j) {
            cout << rRotada[i][j];
            cout << ' ';
        }
        cout << endl;
    }

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

    int **resultado= stringToMatrizR();

    int **resultado2= stringToMatrizG();

    int **resultado3= stringToMatrizB();

    dimensiones(parametroExtra);
    imagenToString(parametroExtra);


    int ** rojoMascara= stringToMatrizR();

    int ** verdeMascara= stringToMatrizG();

    int ** azulMascara= stringToMatrizB();



    mascara(resultado, resultado2, resultado3, rojoMascara, verdeMascara, azulMascara);


    //rotacion(resultado);


    return 0;
}