#include <iostream>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <iomanip>

using namespace std;

int ALTURA;
int ANCHURA;

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
    int ** matrizR= new int*[ALTURA];
    int ** matrizG= new int*[ALTURA];
    int ** matrizB= new int*[ALTURA];
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
    for(i; i < ALTURA * ANCHURA * 2 * 2; i= i + 2){


        if(columna>ANCHURA){
            columna=0;
            filas++;
        }
        stringstream ss;
        ss << hex << imagen.substr(i,2);
        ss >> decimal;
        matrizG[filas][columna] = decimal;
        //cout << matrizG[filas][columna];
        columna++;


    }

    columna=0;
    filas=0;
    for(i; i < ALTURA * ANCHURA * 2 * 3; i= i + 2){


        if(columna>ANCHURA){
            columna=0;
            filas++;
        }
        stringstream ss;
        ss << hex << imagen.substr(i,2);
        ss >> decimal;
        matrizB[filas][columna] = decimal;
        columna++;


    }






}



int mascara (int **rojoImg, int **verdeImg, int **azulImg, int **rojoMascara, int **verdeMascara, int **azulMascara)
{
    for(int i=0; i<5; ++i) {
        for(int j=0; j<5; ++j) {
            rojoImg[i][j] = rojoImg[i][j] * rojoMascara[i][j];
            verdeImg[i][j] = verdeImg[i][j] * verdeMascara[i][j];
            azulImg[i][j] = azulImg[i][j] * azulMascara[i][j];
        }
    }
    return 0;
}

int main() {

    dimensiones();
    crearImagen();

    int **rojoMascara = new int *[5];
    for (int i = 0; i < 5; ++i) {
        rojoMascara[i] = new int[5];
    }

    int **verdeMascara = new int *[5];
    for (int i = 0; i < 5; ++i) {
        verdeMascara[i] = new int[5];
    }

    int **azulMascara = new int *[5];
    for (int i = 0; i < 5; ++i){
        azulMascara[i] = new int[5];
    }


    for(int i=0; i<5; ++i){
        for(int j=0; j<5; ++j) {
            rojoMascara[i][j] = 0;
            verdeMascara[i][j] = 0;
            azulMascara[i][j] = 0;
        }
    }


    for(int i=0; i<5; ++i){
        for(int j=0; j<5; ++j) {
            cout << rojo[i][j];
            cout << verde[i][j];
            cout << azul[i][j];
        }
        cout << endl;
    }

    mascara(rojo, verde, azul, rojoMascara, verdeMascara, azulMascara);

    for(int i=0; i<5; ++i){
        for(int j=0; j<5; ++j) {
            cout << rojo[i][j];
            cout << verde[i][j];
            cout << azul[i][j];
        }
        cout << endl;
    }

    return 0;
}