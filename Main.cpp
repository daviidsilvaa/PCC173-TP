#include <chrono>
#include <iostream>
#include <string>
#include <vector>
#include <time.h>
#include "stdlib.h"
using namespace std;

struct Adj{
    int v;
    int weight;
    vector<int> resource;
};

struct List{
    int v;
    vector<int> resource;
    vector<Adj> adj;
};

int openFile(FILE**, char*);
void getFirstLine(FILE**, int*, int*, int*);
void createAdjList(FILE**, vector<List>&, int, int, int, vector<int>*, vector<int>*);
void printAdjList(vector<List>);

int main(int argc, char *argv[]){
    FILE *file_in;
    char name[16];
    int N, // numero de vertices
        M, // numero de arcos
        K; // numero de recursos
    vector<int> *lowerBound;
    vector<int> *higherBound;

    sprintf(name, "Instancias/%s", argv[1]);
    openFile(&file_in, name);
    getFirstLine(&file_in, &N, &M, &K);
    vector<List> graph(N);

    createAdjList(&file_in, graph, N, M, K, lowerBound, higherBound);

    printAdjList(graph);

    return 0;
}

int openFile(FILE **file_in, char* name){
	if ((*file_in) = fopen(name, "r")){
		return 1;
	}
	return 0;
}

void printAdjList(vector<List> graph){
    for(int i = 1; i < graph.size(); i++){
        cout<< graph[i].v << "\t";
        for(int j = 0; j < graph[i].adj.size(); j++){
            cout<< graph[i].adj[j].v << " ";
        }
        cout << endl;
    }
}

void getFirstLine(FILE **file_in, int *N, int *M, int *K){
    char enter;
    fscanf(*file_in, "%d %d %d %[\n]", N, M, K, &enter);
    (*N)++;
}

void createAdjList(FILE **file_in, vector<List>& graph, int N, int M, int K, vector<int> *lowerBound, vector<int> *higherBound){
	int tmp, v1, v2;
	char enter;
    Adj edge_tmp;

    lowerBound = new vector<int>();
    higherBound = new vector<int>();
    // leitura dos limites inferiores
    for(int i = 0; i < K; i++){
        fscanf(*file_in, "%d", &tmp);
        (*lowerBound).push_back(tmp);
    }

    fscanf(*file_in, "%[\n]", &enter);

    // leitura dos limites superiores
    for(int i = 0; i < K; i++){
        fscanf(*file_in, "%d", &tmp);
        (*higherBound).push_back(tmp);
    }

    fscanf(*file_in, "%[\n]", &enter);

    // leitura dos limites dos vertices
    for(int i = 1; i < N; i++){
        for(int j = 0; j < K; j++){
            fscanf(*file_in, "%d", &tmp);
            (graph)[i].resource.push_back(tmp);
        }
        fscanf(*file_in, "%[\n]", &enter);
    }

    // leitura dos arcos
    for(int i = 0; i < M; i++){
        fscanf(*file_in, "%d %d %d", &v1, &v2, &tmp);
        edge_tmp.v = v2;
        edge_tmp.weight = tmp;
        for(int j = 0; j < K; j++){
            fscanf(*file_in, "%d", &tmp);
            edge_tmp.resource.push_back(tmp);
        }
        (graph)[v1].v = v1;
        (graph)[v1].adj.push_back(edge_tmp);
        edge_tmp.resource.clear();
        fscanf(*file_in, "%[\n]", &enter);
    }
    fclose(*file_in);
}
