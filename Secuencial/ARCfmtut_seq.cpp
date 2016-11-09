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



    ifstream myfile ("imagen_entrada.txt");



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



    ifstream myfile ("imagen_entrada.txt"); // Fichero de entrada



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




double **escalaGrises(int **rojo,int **verde, int **azul) {


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

    ofstream outputFile("histogram.txt");

    for (int i = 0; i < tramos; i++) {
        outputFile << result[i];
        outputFile << " ";
    }

}



int main(){





    dimensiones();

    imagenToString();

    double **resultado= escalaGrises(stringToMatrizR(),stringToMatrizG(),stringToMatrizB());
    histograma(resultado,25);


    //Impresión de las tres matrices, pruebas





    /*for(int i=0; i < ALTURA; ++i){

        for(int j=0; j < ANCHURA; ++j){

            cout << resultado[i][j];

            cout << " ";

        }

        cout << "\n";

    }

    cout << "\n";

    cout << "\n";*/



    /*int **resultado2= stringToMatrizG();



    for(int i=0; i < ALTURA; ++i){

        for(int j=0; j < ANCHURA; ++j){

            cout << resultado2[i][j];

            cout << " ";

        }

        cout << "\n";

    }



    cout << "\n";

    cout << "\n";



    int **resultado3= stringToMatrizB();



    for(int i=0; i < ALTURA; ++i){

        for(int j=0; j < ANCHURA; ++j){

            cout << resultado3[i][j];

            cout << " ";

        }

        cout << "\n";

    }

    cout << "\n";

    cout << "\n";*/



}