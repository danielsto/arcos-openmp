#include <iostream>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <iomanip>

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


double **escalaGrises(int **verde, int **rojo, int **azul) {


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

void histograma(double **escalagrises, int tramos){

    int result[tramos];
    for (int k = 0; k <tramos ; ++k) {
        result[k]=0;
    }
    double valores_tramo=256/tramos;
    int contador=0;

    for (int i = 0; i < ALTURA; i++) {
        for (int j = 0; j < ANCHURA; j++) {
            while(contador<tramos){
                if(escalagrises[i][j]>=contador*valores_tramo && escalagrises[i][j]<(contador+1)*valores_tramo ){
                    result[contador]=result[contador]+1;
                    contador=0;
                    break;
                }
                else{
                    contador++;
                }
            }
        }
    }

    for (int i = 0; i < tramos; i++) {
        cout << result[i];
        cout << " ";
    }

}

int main() {
    dimensiones();
    crearImagen();
    resultado = escalaGrises(verde, rojo, azul);


    /*IMPRESION MATRIZ CAMBIADA*/
    for (int i = 0; i < ALTURA; i++) {
        for (int j = 0; j < ANCHURA; j++) {
            cout << resultado[i][j];
            cout << " ";
        }
        cout << endl;
    }

    cout << endl;

    histograma(resultado,4);

    return 0;
}