
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "stdlib.h"
using namespace std;

// Edge.hpp
struct Edge{
    int v;
    int weight;
    vector<int> resource;
};

// List.hpp
struct List{
    int v;
    vector<int> resource;
    vector<Edge> adj;
};


int abreArquivo(FILE **entrada, char* nome);
int carregaArquivo(FILE **file_in, vector<List> *graph, int *N, int *M, int *K, vector<int> *LOW, vector<int> *HIGH);

int main(int argc, char *argv[]){

    FILE *file_in;
    int N, // numero de vertices
        M, // numero de arcos
        K; // numero de recursos
    vector<int> *LOW;
    vector<int> *HIGH;
    vector<List> *graph;

    abreArquivo(&file_in, argv[1]);
    carregaArquivo(&file_in, graph, &N, &M, &K, LOW, HIGH);

    for(int i = 1; i < N; i++){
        cout<< (*graph)[i].v << "\t";
        for(int j = 0; j < (*graph)[i].resource.size(); j++){
            cout<< (*graph)[i].resource[j] << " ";
        }
        cout << endl;
    }

    return 0;
}
int abreArquivo(FILE **entrada, char* nome){

	if ((*entrada) = fopen(nome, "r")){
		cout<<"Arquivo aberto"<<endl;
		return 1;
	}
	return 0;
}

int carregaArquivo(FILE **file_in, vector<List> *graph, int *N, int *M, int *K, vector<int> *LOW, vector<int> *HIGH){
	int tmp, v1, v2;
	char enter;
    Edge edge_tmp;

    fscanf(*file_in, "%d %d %d %[\n]", N, M, K, &enter);
    (*N)++;
    graph = new vector<List>(*N);
    cout << (*graph).size() << endl;
    LOW = new vector<int>();
    HIGH = new vector<int>();


    // leitura dos limites inferiores
    for(int i = 0; i < *K; i++){
        fscanf(*file_in, "%d", &tmp);
        (*LOW).push_back(tmp);
    }

    fscanf(*file_in, "%[\n]", &enter);

    // leitura dos limites superiores
    for(int i = 0; i < *K; i++){
        fscanf(*file_in, "%d", &tmp);
        (*HIGH).push_back(tmp);
    }

    fscanf(*file_in, "%[\n]", &enter);

    // leitura dos limites dos vertices
    for(int i = 1; i < *N; i++){
        for(int j = 0; j < *K; j++){
            fscanf(*file_in, "%d", &tmp);
            (*graph)[i].resource.push_back(tmp);
        }
        fscanf(*file_in, "%[\n]", &enter);
    }

    // leitura dos arcos
    for(int i = 0; i < *M; i++){
        fscanf(*file_in, "%d %d %d", &v1, &v2, &tmp);

        edge_tmp.v = v2;
        edge_tmp.weight = tmp;

        for(int j = 0; j < *K; j++){
            fscanf(*file_in, "%d", &tmp);
            edge_tmp.resource.push_back(tmp);
        }

        (*graph)[v1].v = v1;
        (*graph)[v1].adj.push_back(edge_tmp);
        cout << (*graph)[v1].v << endl;
        edge_tmp.resource.clear();

        //fscanf(*file_in, "%[\n]", &enter);
        // cout << i << endl;
    }

    fclose(*file_in);

    return 1;
}
