#include <iostream>

using namespace std;

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
    int **rojo = new int *[5];
    for (int i = 0; i < 5; ++i) {
        rojo[i] = new int[5];
    }

    int **verde = new int *[5];
    for (int i = 0; i < 5; ++i) {
        verde[i] = new int[5];
    }

    int **azul = new int *[5];
    for (int i = 0; i < 5; ++i){
        azul[i] = new int[5];
    }

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
            rojo[i][j] = 0;
            verde[i][j] = 1;
            azul[i][j] = 2;
        }
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