#include <iostream>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <cmath>
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

    /*unsigned long da problemas con -Werror*/ int i;
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

    for(i=16; i < ALTURA * ANCHURA * 2; i= i + 2){
        if(columna>ANCHURA){
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
    for(j = i; j < ALTURA * ANCHURA * 2 * 2; j= j + 2){
        if(columna>ANCHURA){
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
    for(k = j; k < ALTURA * ANCHURA * 2 * 3; k= k + 2){
        if(columna>ANCHURA){
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


void filtroBN() {

    int centroX = ANCHURA / 2;
    int centroY = ALTURA / 2;
    int radio = 16;

    for (int k = 0; k < ALTURA; ++k) {
        for (int i = 0; i < ANCHURA; ++i)
            matrizR[k][i] = 1;
    }
    for (int k = 0; k < ALTURA; ++k) {
        for (int i = 0; i < ANCHURA; ++i)
            matrizG[k][i] = 1;
    }
    for (int k = 0; k < ALTURA; ++k){
        for (int i = 0; i < ANCHURA; ++i)
            matrizB[k][i] = 1;
    }

    for (int i = 0; i < ALTURA; ++i) {
        for (int j = 0; j < ANCHURA; ++j) {
            float suma = pow(i - centroY, 2) + pow(j - centroX, 2);
            if (suma > pow(radio, 2)) {
                matrizR[i][j] = matrizR[i][j] * 3;
            }
        }
    }

    /*for (int i = 0; i < ALTURA; ++i) {
        for (int j = 0; j < ANCHURA; ++j) {
            float suma = pow(i - centroY, 2) + pow(j - centroX, 2);
            if (suma > pow(radio, 2)) {
                matrizG[i][j] = matrizG[i][j] * 2;
            }
        }
    }

    for (int i = 0; i < ALTURA; ++i) {
        for (int j = 0; j < ANCHURA; ++j) {
            float suma = pow(i - centroY, 2) + pow(j - centroX, 2);
            if (suma > pow(radio, 2)) {
                matrizB[i][j] = matrizB[i][j] * 3;
            }
        }
    }*/

    for (int k = 0; k < ALTURA; ++k) {
        for (int i = 0; i < ANCHURA; ++i) {
            cout << matrizR[k][i] << " | ";
        }
        cout << '\n';
    }

    cout << '\n';
    cout << '\n';

    /*for (int k = 0; k < ALTURA; ++k) {
        for (int i = 0; i < ANCHURA; ++i) {
            cout << matrizG[k][i] << " | ";
        }
        cout << '\n';
    }

    cout << '\n';
    cout << '\n';

    for (int k = 0; k < ALTURA; ++k) {
        for (int i = 0; i < ANCHURA; ++i) {
            cout << matrizB[k][i] << " | ";
        }
        cout << '\n';
    }*/


}

int main() {
    dimensiones();
    crearImagen();


    filtroBN();

}