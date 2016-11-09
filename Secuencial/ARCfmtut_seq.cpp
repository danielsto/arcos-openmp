#include <iostream>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <iomanip>
#include <tgmath.h>

using namespace std;

int ALTURA;
int ANCHURA;
int ** matrizR= new int*[ALTURA];
int ** matrizG= new int*[ALTURA];
int ** matrizB= new int*[ALTURA];

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

void crearImagen(){
    int decimal;

    string line;
    string imagen;

    unsigned long i;
    for(i=0; i<ALTURA; i++){
        matrizR[i]= new int[ANCHURA];
        matrizG[i]= new int[ANCHURA];
        matrizB[i]= new int[ANCHURA];
    }

    ifstream myfile ("..\\imagen_entrada"); // Fichero de entrada
    if (myfile.is_open()) // Si existe o lo encuentra
    {
        while(getline (myfile,line)) {
            line.erase(remove(line.begin(), line.end(), '\r'), line.end());
            imagen += line;
        }
        myfile.close();
    }
    else cout << "Unable to open file";

    imagen.erase(remove(imagen.begin(), imagen.end(), ' '), imagen.end());

    int columna=0;
    int filas=0;

    for(i=16; i < ALTURA * ANCHURA * 2 + 16; i= i + 2){
        if(columna>=ANCHURA){
            columna=0;
            filas++;
        }
        stringstream ss;
        ss << hex << imagen.substr(i,2);
        ss >> decimal;
        matrizR[filas][columna] = decimal;

        columna++;
    }

    columna=0;
    filas=0;
    int j;
    for(j=i; j < ALTURA * ANCHURA * 2 * 2+16; j= j + 2){
        if(columna>=ANCHURA){
            columna=0;
            filas++;
        }
        stringstream ss;
        ss << hex << imagen.substr(j,2);
        ss >> decimal;
        matrizG[filas][columna] = decimal;
        //cout << matrizG[filas][columna];
        columna++;


    }

    columna=0;
    filas=0;
    int k;
    for(k=j; k < ALTURA * ANCHURA * 2 * 3 +16; k= k + 2){
        if(columna>=ANCHURA){
            columna=0;
            filas++;
        }
        stringstream ss;
        ss << hex << imagen.substr(k,2);
        ss >> decimal;
        matrizB[filas][columna] = decimal;
        columna++;


    }
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
                rRotada[coorYrotada][coorXrotada]= matrizR[coorY][coorX];
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
    crearImagen();

    int **rojoMascara = new int *[ALTURA];
    for (int i = 0; i < ALTURA; ++i) {
        rojoMascara[i] = new int[ANCHURA];
    }

    int **verdeMascara = new int *[ALTURA];
    for (int i = 0; i < ALTURA; ++i) {
        verdeMascara[i] = new int[ANCHURA];
    }

    int **azulMascara = new int *[ALTURA];
    for (int i = 0; i < ALTURA; ++i){
        azulMascara[i] = new int[ANCHURA];
    }


    for(int i=0; i<ALTURA; ++i){
        for(int j=0; j<ANCHURA; ++j) {
            rojoMascara[i][j] = 0;
            verdeMascara[i][j] = 0;
            azulMascara[i][j] = 0;
        }
    }

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

    for(int i=0; i<ALTURA; ++i){
        for(int j=0; j<ANCHURA; ++j) {
            cout << matrizR[i][j];
            cout << ' ';
        }
        cout << endl;
    }
    cout << "MATRIZ ROTADA";
    cout << endl;
    rotacion(matrizR);
    return 0;
}