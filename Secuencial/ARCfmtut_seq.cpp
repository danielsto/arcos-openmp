#include <iostream>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <cmath>
#include <iomanip>

using namespace std;
int ALTURA;
int ANCHURA;

string stringRed;
string stringGreen;
string stringBlue;

void dimensiones(){
    string line;
    ifstream myfile ("..\\imagen_entrada"); // Fichero de entrada

    if (myfile.is_open()) // Si existe o lo encuentra
    {

        // objeto strin string en el que se almacena la conversion de string a hexadecimal
        stringstream hs; //heightstream
        stringstream ws; //widthstream
        getline (myfile,line);
        //se eliminan los espacios de la primera linea
        line.erase(remove(line.begin(), line.end(), ' '), line.end());
        // Obtener a partir de la posición 0 una cadena de longitud 8
        hs << hex << line.substr(6,2) + line.substr(4,2) + line.substr(2,2) + line.substr(0,2);
        hs >> ALTURA;

        // Obtener a partir de la posición 8 una cadena de longitud 8
        ws << hex << line.substr(14,2) + line.substr(12,2) + line.substr(10,2) + line.substr(8,2);
        ws >> ANCHURA;

        myfile.close();

    }
    else cout << "Unable to open file";

}


void imagenToString(){

    string line;
    string stringImagen;
    ifstream myfile ("..\\imagen_entrada"); // Fichero de entrada
    if (myfile.is_open()) // Si existe o lo encuentra
    {
        while(getline (myfile,line)) {
            line.erase(remove(line.begin(), line.end(), '\r'), line.end());
            stringImagen += line;
        }
        myfile.close();
    }
    else cout << "Unable to open file";

    stringImagen.erase(remove(stringImagen.begin(), stringImagen.end(), ' '), stringImagen.end());

	stringRed= stringImagen.substr(16, ALTURA*ANCHURA*2);
	stringGreen= stringImagen.substr(16 +(ALTURA*ANCHURA*2), ALTURA*ANCHURA*2);
	stringBlue= stringImagen.substr(16 +(ALTURA*ANCHURA*2*2), ALTURA*ANCHURA*2);

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
    for(int i=0; i<ALTURA; ++i) {
        for(int j=0; j<ANCHURA; ++j) {
            rojoImg[i][j] = rojoImg[i][j] * rojoMascara[i][j];
            verdeImg[i][j] = verdeImg[i][j] * verdeMascara[i][j];
            azulImg[i][j] = azulImg[i][j] * azulMascara[i][j];
        }
    }
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


int main() {


    dimensiones();
	imagenToString();



    /*
    for(int i=0; i<ALTURA; ++i){
        for(int j=0; j<ANCHURA; ++j) {
            cout << matrizR[i][j];
            cout << matrizG[i][j];
            cout << matrizB[i][j];
        }
        cout << endl;
    }

    mascara(matrizR, matrizG, matrizB, rojoMascara, verdeMascara, azulMascara);

    for(int i=0; i<ALTURA; ++i){
        for(int j=0; j<ANCHURA; ++j) {
            cout << matrizR[i][j];
            cout << matrizG[i][j];
            cout << matrizB[i][j];
        }
        cout << endl;
    }
    */

    int **resultado= stringToMatrizR();

    for(int i=0; i < ALTURA; ++i){
        for(int j=0; j < ANCHURA; ++j){
            cout << resultado[i][j];
            cout << " ";
        }
        cout << "\n";
    }
    cout << "\n";

   rotacion(resultado);
    return 0;
}