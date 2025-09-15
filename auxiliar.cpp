#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <random>
#include <ctime>

using namespace std;

int main(int argc, char* argv[]) {
    int n1 = stoi(argv[1]);
    int m1 = stoi(argv[2]);
    int n2 = stoi(argv[3]);
    int m2 = stoi(argv[4]);

    //checando numero de entradas e se existe produto possível
    if (argc != 5) {
        cerr << "Uso: " << argv[0] << " <n1> <m1> <n2> <m2>" << endl;
        return 1;
    }
    
    // cout << "n1: "<< n1 << endl; 
    // cout << "m1: "<< m1 << endl; 
    srand(time(0));

    ofstream matriz1("./resultados/resultadoM1.csv");
    matriz1 << argv[1] << " " << argv[2] << endl;
    // gerar a matriz 1
    for(int altura = 1; altura <= n1; altura++) {
        for(int largura = 1; largura <= m1; largura++) {
            matriz1 << "c" << altura << "x" << largura << " " << rand() % 11 << endl;
        }
    }
    matriz1.close();

    ofstream matriz2("./resultados/resultadoM2.csv");
    matriz2 << argv[3] << " " << argv[4] << endl;
    // clock_t antes = clock();
    // gerar a matriz 2
    for(int altura = 1; altura <= n2; altura++) {
        for(int largura = 1; largura <= m2; largura++) {
            matriz2 << "c" << altura << "x" << largura << " " << rand() % 11 << endl;
        }
    }
    matriz2.close();
    
    return 0;
}