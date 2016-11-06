// reading a text file
#include <iostream>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <iomanip>

using namespace std;

int altura;
int anchura;

int main () {

    string line;

    ifstream myfile ("C:\\Users\\luis\\ClionProjects\\ARCOS_OpenMP\\imagen_entrada"); // Fichero de entrada
    if (myfile.is_open()) // Si existe o lo encuentra
    {
        // objeto strin string en el que se almacena la conversion de string a hexadecimal
        stringstream ss;
        stringstream ss2;


        getline (myfile,line);
        //se eliminan los espacios de la primera linea
        line.erase(remove(line.begin(), line.end(), ' '), line.end());
        // Obtener a partir de la posición 0 una cadena de longitud 8
        ss << hex << line.substr(6,2) + line.substr(4,2) + line.substr(2,2) + line.substr(0,2);
        ss >> altura;


        ss2 << hex << line.substr(14,2) + line.substr(12,2) + line.substr(10,2) + line.substr(8,2);
        ss2 >> anchura;

        myfile.close();
    }



    else cout << "Unable to open file";

    return 0;
}

int dimensiones (){

}