// basic file operations
#include <iostream>
#include <fstream>
using namespace std;

int main () {
    char cadena[128];
    // Crea un fichero de salida
    ofstream fs("nombre.txt");

    // Enviamos una cadena al fichero de salida:
    fs << "Hola, mundo" << endl;
    // Cerrar el fichero,
    // para luego poder abrirlo para lectura:
    fs.close();

    // Abre un fichero de entrada
    ifstream fe("nombre.txt");

    // Leeremos mediante getline, si lo hiciéramos
    // mediante el operador << sólo leeríamos
    // parte de la cadena:
    fe.getline(cadena, 128);

    cout << cadena << endl;
    return 0;
}

