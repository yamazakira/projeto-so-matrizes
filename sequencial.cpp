#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <vector>
#include <string>
#include <chrono>

using namespace std;

vector<vector<int>> ler_matriz(const string &nome_arquivo, int &linhas, int &colunas)
{
    ifstream arquivo(nome_arquivo);
    if (!arquivo)
    {
        cerr << "Erro ao abrir arquivo da matriz: " << nome_arquivo << endl;
        exit(1);
    }

    string linha_atual;
    getline(arquivo, linha_atual);
    stringstream ss(linha_atual);
    ss >> linhas >> colunas;

    vector<vector<int>> matriz(linhas, vector<int>(colunas));

    for (int item_linha = 0; item_linha < linhas; item_linha++)
    {
        for (int item_coluna = 0; item_coluna < colunas; item_coluna++)
        {
            getline(arquivo, linha_atual);
            stringstream ss2(linha_atual);
            string lixo;
            int valor;
            ss2 >> lixo >> valor;
            matriz[item_linha][item_coluna] = valor;
        }
    }

    arquivo.close();

    return matriz;
}

void salvar_matriz(const vector<vector<int>>& matriz, double tempo, const string& nome_output)
{
    ofstream fout(nome_output);
    if (!fout)
    {
        cerr << "Erro ao criar arquivo: " << nome_output << endl;
        exit(1);
    }

    int linhas = matriz.size();
    int colunas = matriz[0].size();

    fout << linhas << " " << colunas << endl;

    for (int linha_atual = 0; linha_atual < linhas; linha_atual++) {
        for (int coluna_atual = 0; coluna_atual < colunas; coluna_atual++) {
            fout << "c" << linha_atual+1 << "x" << coluna_atual+1 << " " << matriz[linha_atual][coluna_atual] << endl;
        }
    }

    fout << tempo << endl;
    fout.close();
}

int main(int argc, char *argv[])
{
    // checando numero de entradas e se existe produto possível
    if (argc != 4)
    {
        cerr << "Uso: " << argv[0] << " <arquivoM1> <arquivoM2> <arquivoResultado>" << endl;
        return 1;
    }

    int n1, m1, n2, m2;
    vector<vector<int>> M1 = ler_matriz(argv[1], n1, m1);
    vector<vector<int>> M2 = ler_matriz(argv[2], n2, m2);

    if (m1 != n2)
    {

        cerr << "Impossível!! m1 != n2; (" << m1 << "," << n2 << ")" << endl;
        return 1;
    }

    // resultado
    vector<vector<int>> resultado(n1, vector<int>(m2, 0));

    auto inicio = chrono::high_resolution_clock::now();

    for (int i = 0; i < n1; i++)
    {
        for (int j = 0; j < m2; j++)
        {
            for (int k = 0; k < m1; k++)
            {
                resultado[i][j] += M1[i][k] * M2[k][j];
            }
        }
    }

    auto fim = chrono::high_resolution_clock::now();
    double tempo_ms = chrono::duration<double, milli>(fim - inicio).count();

    salvar_matriz(resultado, tempo_ms, argv[3]);

    cout << "Concluido. Tempo: " << tempo_ms << " ms" << endl;

    return 0;
}