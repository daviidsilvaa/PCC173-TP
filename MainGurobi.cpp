#include <chrono>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <time.h>
#include "stdlib.h"
using namespace std;

#define OITO_DEITADO 999999;

struct Arco{
    int custo;
    vector<int> consumo;
};

int abrirArquivo(FILE**, char*);
void lerArquivo(FILE **file_in, Arco ***adjacencia, int ***consumo, int *N, int *M, int *K, int **limitesInferiores, int **limitesSuperiores);
void imprimirMatriz(Arco **adjacencia, int N);
void deletarEstruturas(Arco ***adjacencia, int ***consumo, int **limitesInferiores, int **limitesSuperiores, int *N, int *K);

int main(int argc, char *argv[]){
    Arco **adjacencia; // matriz de adjacencia
    int **consumo; // matriz de consumo de recursos dos arcos

    FILE *file_in; // arquivo de entrada de dados
    int N, // numero de vertices
    M, // numero de arcos
    K; // numero de recursos
    int *limitesInferiores; // limites inferiores de recursos
    int *limitesSuperiores; // limites superiores de recursos

    if(!abrirArquivo(&file_in, argv[1])){
        cout << "Cant open file" << endl;

        return 0;
    }

    lerArquivo(&file_in, &adjacencia, &consumo, &N, &M, &K, &limitesInferiores, &limitesSuperiores);

    fclose(file_in);

    imprimirMatriz(adjacencia, N);

    return 0;
}

int abrirArquivo(FILE **file_in, char* path){
    if ((*file_in) = fopen(path, "r")){
        return 1;
    }
    return 0;
}

void lerArquivo(FILE **file_in, Arco ***adjacencia, int ***consumo, int *N, int *M, int *K, int **limitesInferiores, int **limitesSuperiores){
    char enter;

    // le N, M e K do arquivo de entrada
    fscanf(*file_in, "%d %d %d %[\n]", N, M, K, &enter);
    (*N)++;

    // instancia a matriz de adjacencia
    (*adjacencia) = new Arco *[(*N)];
    for(int i = 1; i < *N; i++){
        (*adjacencia)[i] = new Arco[(*N)];
    }

    // inicializa matriz com custos maximos
    for(int i = 1; i < (*N); i++){
        for(int j = 1; j < (*N); j++){
            (*adjacencia)[i][j].custo = OITO_DEITADO;
        }
    }

    // inicializa a matriz de consumo de recursos
    (*consumo) = new int *[(*N)];
    for(int i = 1; i < *N; i++){
        (*consumo)[i] = new int[(*K)];
    }

    int tmp, v1, v2;
    Arco edge_tmp;

    // vetores de limites inferios e superiores do problema
    (*limitesInferiores) = new int((*K));
    (*limitesSuperiores) = new int((*K));

    // le limites inferiores do arquivo de entrada
    for(int i = 0; i < (*K); i++){
        fscanf(*file_in, "%d", &tmp);
        (*limitesInferiores)[i] = tmp;
    }

    fscanf(*file_in, "%[\n]", &enter);

    // le limites superiores do arquivo de entrada
    for(int i = 0; i < (*K); i++){
        fscanf(*file_in, "%d", &tmp);
        (*limitesSuperiores)[i] = tmp;
    }

    fscanf(*file_in, "%[\n]", &enter);

    // le consumos dos vertices
    for(int i = 1; i < (*N); i++){
        for(int j = 0; j < (*K); j++){
            fscanf(*file_in, "%d", &tmp);
            (*consumo)[i][j] = tmp;
        }
        fscanf(*file_in, "%[\n]", &enter);
    }
    // leitura dos arcos
    // for(int i = 0; i < 78; i++){
    //     fscanf(*file_in, "%d %d %d", &v1, &v2, &tmp);
    //     for(int j = 0; j < (*K); j++){
    //         fscanf(*file_in, "%d", &tmp);
    //     }
    // }
    for(int i = 0; i < (*M); i++){
        fscanf(*file_in, "%d %d %d", &v1, &v2, &tmp);
        // adiciona peso ao arco (v1 > v2)
        (*adjacencia)[v1][v2].custo = tmp;
        (*adjacencia)[v1][v2].consumo.resize(*K);
        cout << i << " ";
        // cout << v1 << " " << v2 << " " << tmp << " ";
        // adiciona consumo de recursos ao arco (v1 > v2)
        for(int j = 0; j < (*K); j++){
            fscanf(*file_in, "%d", &tmp);
            // cout << tmp << " ";
            (*adjacencia)[v1][v2].consumo[j] = tmp;
        }
        // cout << (*adjacencia)[79][36].custo << endl;
        // cout << endl;
        fscanf(*file_in, "%[\n]", &enter);
    }
    cout << __LINE__ << endl;
}

void imprimirMatriz(Arco **adjacencia, int N){
    for(int i = 1; i < (N); i++){
        for(int j = 1; j < (N); j++){
            if (adjacencia[i][j].custo != 999999){
                cout << i << "\t" << j << "\t" << adjacencia[i][j].custo << "\t" << adjacencia[i][j].consumo.size();
                // for(int l = 0; l < adjacencia[i][j].consumo.size(); l++){
                //     cout << adjacencia[i][j].consumo[l];
                // }
                cout << endl;
            }
        }
    }
}

void deletarEstruturas(Arco ***adjacencia, int ***consumo, int **limitesInferiores, int **limitesSuperiores, int N, int K){
    for(int i = 0; i < N; i++){
        delete (*adjacencia)[i];
    }
    delete (**adjacencia);

    for(int i = 0; i < N; i++){
        delete (*consumo)[i];
    }
    delete (**consumo);

    delete (*limitesInferiores);
    delete (*limitesSuperiores);

}
