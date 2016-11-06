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



int main () {

    dimensiones();

    return 0;
}