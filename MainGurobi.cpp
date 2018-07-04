#include <chrono>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include "stdlib.h"
#include "gurobi_c++.h"
using namespace std;
using namespace std::chrono;

#define INF 999999

string itos(int i) {
    stringstream s; s << i;
    return s.str();
}

struct Tupla{
	int i;
	int j;
};

struct Arco{
    int custo;
    int *consumo;
};

int abrirArquivo(FILE**, char*);
void lerArquivo(FILE **arq_entrada, Arco ***adjacencia, int ***consumo, int *N, int *M, int *K, int **limitesInferiores, int **limitesSuperiores);
void imprimirMatriz(Arco **adjacencia, int N, int K);
void deletarEstruturas(Arco ***adjacencia, int ***consumo, int **limitesInferiores, int **limitesSuperiores, int *N, int *K);

int main(int argc, char *argv[]){
    duration<double> time_span2;
    Arco **adjacencia; // matriz de adjacencia
    int **consumo; // matriz de consumo de recursos dos arcos

    FILE *arq_entrada; // arquivo de entrada de dados
    int N, // numero de vertices
    M, // numero de arcos
    K; // numero de recursos
    int *limitesInferiores; // limites inferiores de recursos
    int *limitesSuperiores; // limites superiores de recursos

    if(!abrirArquivo(&arq_entrada, argv[1])){
        cout << "Nao foi possivel abrir o arquivo!" << endl;

        return 0;
    }

    lerArquivo(&arq_entrada, &adjacencia, &consumo, &N, &M, &K, &limitesInferiores, &limitesSuperiores);

    fclose(arq_entrada);

    high_resolution_clock::time_point t1 = high_resolution_clock::now();

    // cria ambiente gurobi
    GRBEnv *env = NULL;

    // cria variaveis do modelo
    GRBVar **vars = NULL;

    vars = new GRBVar*[N];
    for (int i = 0; i < N; i++) {
        vars[i] = new GRBVar[N];
    }

    try {
        env = new GRBEnv();
        GRBModel model = GRBModel(*env);

        model.set(GRB_IntParam_LazyConstraints, 1);

        // adiciona variaveis de decisao ao modelo
        for(int i = 1; i < N; i++){
            for(int j = 1; j < N; j++){
				if(adjacencia[i][j].custo != INF){
					vars[i][j] = model.addVar(0.0, 1.0, adjacencia[i][j].custo, GRB_BINARY, itos(i) + ">" + itos(j));
				} else {
					vars[i][j] = model.addVar(0.0, 0.0, adjacencia[i][j].custo, GRB_INTEGER, itos(i) + ">" + itos(j));
				}
            }
        }
        model.update();

        // cria expressao linear que representa a funcao objetivo
        GRBLinExpr fo = 0;
        for(int i = 1; i < N; i++){
            for(int j = 1; j < N; j++){
                fo += model.getVarByName(itos(i)+">"+itos(j)) * adjacencia[i][j].custo;
            }
        }

        // define a função objetivo como sendo de minimização e a adiciona ao modelo
        model.setObjective(fo, GRB_MINIMIZE);

        model.update();

        // restricoes

        // restricao de limite de recursos
        for(int b = 0; b < K ; b++){
            GRBLinExpr consumido = 0;
            for (int i = 1; i < N; i++){
                for (int j = 1; j < N; j++){
                    consumido += model.getVarByName(itos(i) + ">" + itos(j)) * (consumo[j][b] + adjacencia[i][j].consumo[b]);
                }
            }
            model.addConstr(consumido >= limitesInferiores[b], "Gasto minimo de recursos");
            model.addConstr(consumido <= limitesSuperiores[b], "Gasto maximo de recursos");
        }

        // restricao que obriga a comecar do primeiro vertice
        GRBLinExpr somatorio = 0;
        for(int i = 1; i < N; i++){
            somatorio += model.getVarByName(itos(1)+">"+itos(i));
        }
        model.addConstr(somatorio == 1, "Comeca no vertice 1");

        // restricao que obriga a terminar no ultimo vertice
        somatorio = 0;
        for(int i = 1; i < N; i++){
            somatorio += model.getVarByName(itos(i)+">"+itos(N-1));
        }
        model.addConstr(somatorio == 1, "Termina no vertice N-1");

        // restricao de conservacao de fluxo
        for(int k = 2; k < N-1; k++){
            GRBLinExpr somatorio_i = 0;
            GRBLinExpr somatorio_j = 0;
            for(int i = 1; i < N; i++){
                somatorio_i += model.getVarByName(itos(i) + ">" + itos(k));
            }
            for(int j = 1; j < N; j++){
                somatorio_j += model.getVarByName(itos(k) + ">" + itos(j));
            }
            model.addConstr((somatorio_i - somatorio_j) == 0, "Conservacao de fluxo " + itos(k));

        }

        model.update();
        model.optimize();

        //Verifica o status do modelo
        int status = model.get(GRB_IntAttr_Status);

        //Imprime o status do modelo
        if (status == GRB_UNBOUNDED){
            cout << "O modelo nao pode ser resolvido porque e ilimitado" << endl;
            return 0;
        }
        if (status == GRB_OPTIMAL){
            cout << "Solucao otima encontrada!" << endl;
            //Acessa e imprime o valor da funcao objetivo
            cout << "O valor da solucao otima e: " << model.get(GRB_DoubleAttr_ObjVal) << endl;
        }
        if (status == GRB_INFEASIBLE){
            cout << "O modelo nao pode ser resolvido, porque e inviavel" << endl;
            return 0;
        }

        //Acessa as variáveis do modelo depois de resolvido
        GRBVar* v = model.getVars();
        char varname[100];


		// estrutura que armazena o caminho resultado
		vector<Tupla> caminho;
		Tupla tupla_auxiliar;

        //De maneira alternativa, imprime o valor de cada uma das variáveis
        for (int index = 0; index < model.get(GRB_IntAttr_NumVars); ++index) {
            double sol = v[index].get(GRB_DoubleAttr_X);

            sscanf(v[index].get(GRB_StringAttr_VarName).c_str(), "%s", varname);
			// caso a variavel tenha sido usada na solucao
            if(sol == 1){
                char *i_c = strtok(varname, ">");
                char *j_c = strtok(NULL, ">");
                int i = atoi(i_c);
                int j = atoi(j_c);
				tupla_auxiliar.i = i;
				tupla_auxiliar.j = j;
				// empilhamos a Tupla de vertices num vetor que armazena o caminho resultado
				caminho.push_back(tupla_auxiliar);
            }
        }

		cout << "Vertices que compoem o caminho solucao: 1 ";
		for(int i = 0; i < caminho.size(); i++){
			for(int j = 0; j < caminho.size(); j++){
				if(caminho[i].j == caminho[j].i){
					cout << caminho[i].j << " ";
				}
			}
		}

		cout << "100" << endl;

    } catch (GRBException e) {
        cout << "Erro numero: " << e.getErrorCode() << endl;
        cout << e.getMessage() << endl;
    } catch (...) {
        cout << "Erro durante a construcao ou solucao do modelo" << endl;
    }
    high_resolution_clock::time_point t2 = high_resolution_clock::now();
    time_span2 = duration_cast<duration<double>>(t2-t1);
    cout << "Tempo total gasto: " << time_span2.count() << "s" << endl;
    return 0;
}

int abrirArquivo(FILE **arq_entrada, char* path){
    if ((*arq_entrada) = fopen(path, "r")){
        return 1;
    }
    return 0;
}

void lerArquivo(FILE **arq_entrada, Arco ***adjacencia, int ***consumo, int *N, int *M, int *K, int **limitesInferiores, int **limitesSuperiores){
    char enter;

    // le N, M e K do arquivo de entrada
    fscanf(*arq_entrada, "%d %d %d %[\n]", N, M, K, &enter);
    (*N)++;

    // instancia a matriz de adjacencia
    (*adjacencia) = new Arco *[(*N)];
    for(int i = 1; i < *N; i++){
        (*adjacencia)[i] = new Arco[(*N)];
        for(int j = 1; j < *N; j++){
            (*adjacencia)[i][j].consumo = new int[*K];
        }
    }

    // inicializa matriz com custos maximos
    for(int i = 1; i < (*N); i++){
        for(int j = 1; j < (*N); j++){
            (*adjacencia)[i][j].custo = INF;
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
    (*limitesInferiores) = new int[(*K)];
    (*limitesSuperiores) = new int[(*K)];

    // le limites inferiores do arquivo de entrada
    for(int i = 0; i < (*K); i++){
        fscanf(*arq_entrada, "%d", &tmp);
        (*limitesInferiores)[i] = tmp;
    }
    cout<<endl;

    fscanf(*arq_entrada, "%[\n]", &enter);

    // le limites superiores do arquivo de entrada
    for(int i = 0; i < (*K); i++){
        fscanf(*arq_entrada, "%d", &tmp);
        (*limitesSuperiores)[i] = tmp;
    }

    fscanf(*arq_entrada, "%[\n]", &enter);

    // le consumos dos vertices
    for(int i = 1; i < (*N); i++){
        for(int j = 0; j < (*K); j++){
            fscanf(*arq_entrada, "%d", &tmp);
            (*consumo)[i][j] = tmp;
        }
        fscanf(*arq_entrada, "%[\n]", &enter);
    }

    // leitura dos arcos
    for(int i = 0; i < (*M); i++){
        fscanf(*arq_entrada, "%d %d %d", &v1, &v2, &tmp);

        // adiciona peso ao arco (v1 > v2)
        (*adjacencia)[v1][v2].custo = tmp;

        // adiciona consumo de recursos ao arco (v1 > v2)
        for(int j = 0; j < (*K); j++){
            fscanf(*arq_entrada, "%d", &tmp);
            (*adjacencia)[v1][v2].consumo[j] = tmp;
        }
        fscanf(*arq_entrada, "%[\n]", &enter);
    }
}

void imprimirMatriz(Arco **adjacencia, int N, int K){
    for(int i = 1; i < (N); i++){
        for(int j = 1; j < (N); j++){
            if (adjacencia[i][j].custo != INF){
                for(int l = 0; l < K; l++){
                    cout << adjacencia[i][j].consumo[l];
                }
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
