#define _CRT_SECURE_NO_WARNINGS

#include <chrono>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <chrono>
#include "stdlib.h"
#include "gurobi_c++.h"
using namespace std;
using namespace std::chrono;

#define INF 999999


string itos(int i) {
	stringstream s; s << i;
	return s.str();
}

struct Arco {
	int custo;
	int *consumo;
};

int abrirArquivo(FILE**, char*);
void lerArquivo(FILE **arq_entrada, Arco ***adjacencia, int ***consumo, int *N, int *M, int *K, int **limitesInferiores, int **limitesSuperiores);
void imprimirMatriz(Arco **adjacencia, int N, int K);
void deletarEstruturas(Arco ***adjacencia, int ***consumo, int **limitesInferiores, int **limitesSuperiores, int *N, int *K);

int main(int argc, char *argv[]) {
	duration<double> time_span2;
	Arco **adjacencia; // matriz de adjacencia
	int **consumo; // matriz de consumo de recursos dos arcos

	FILE *arq_entrada; // arquivo de entrada de dados
	int nVertices, // numero de vertices
		nArcos, // numero de arcos
		nRecursos; // numero de recursos
	int *limitesInferiores; // limites inferiores de recursos
	int *limitesSuperiores; // limites superiores de recursos

	if (!abrirArquivo(&arq_entrada, argv[1])) {
		std::cout << "Cant open file" << endl;
		std::cout << "\n\n" << argv[1] << "\n";
		std::cin >> nVertices;
		return 0;
	}

	ofstream saida;
	saida.open("saida.txt", ofstream::app);
	saida << "\n\n";
	saida << argv[1] << "\n";

	lerArquivo(&arq_entrada, &adjacencia, &consumo, &nVertices, &nArcos, &nRecursos, &limitesInferiores, &limitesSuperiores);
	std::cout << endl;

	fclose(arq_entrada);

	high_resolution_clock::time_point t1 = high_resolution_clock::now();

	//imprimirMatriz(adjacencia, N, K);

	// cria ambiente gurobi

	GRBEnv *env = NULL;

	// adiciona variaveis ao modelo
	GRBVar **vars = NULL;

	vars = new GRBVar*[nVertices];
	for (int i = 0; i < nVertices; i++) {
		vars[i] = new GRBVar[nVertices];
	}

	

	try {
		env = new GRBEnv();
		GRBModel model = GRBModel(*env);

		model.set(GRB_IntParam_LazyConstraints, 1);

		// adiciona variaveis de decisao ao modelo
		for (int i = 1; i < nVertices; i++) {
			for (int j = 1; j < nVertices; j++) {
				if (adjacencia[i][j].custo != INF) {
					vars[i][j] = model.addVar(0.0, 1.0, adjacencia[i][j].custo, GRB_BINARY, itos(i) + ">" + itos(j));
				}
				else {
					vars[i][j] = model.addVar(0.0, 0.0, adjacencia[i][j].custo, GRB_INTEGER, itos(i) + ">" + itos(j));
				}
			}
		}
		model.update();

		// cria expressao linear que representa a funcao objetivo
		GRBLinExpr fo = 0;
		for (int i = 1; i < nVertices; i++) {
			for (int j = 1; j < nVertices; j++) {
				if (adjacencia[i][j].custo != INF) {
					fo += model.getVarByName(itos(i) + ">" + itos(j)) * adjacencia[i][j].custo;
				}
			}
		}

		// define a função objetivo como sendo de minimização e a adiciona ao modelo
		model.setObjective(fo, GRB_MINIMIZE);

		model.update();

		// restricoes

		// restricao de limite de recursos
		for (int b = 0; b < nRecursos; b++) {
			GRBLinExpr consumido = 0;
			for (int i = 1; i < nVertices; i++) {
				for (int j = 1; j < nVertices; j++) {
					if (adjacencia[i][j].custo != INF) {
						consumido += model.getVarByName(itos(i) + ">" + itos(j)) * (consumo[j][b] + adjacencia[i][j].consumo[b]);
					}
				}
			}
			model.addConstr(consumido >= limitesInferiores[b], "Gasto minimo de recursos");
			model.addConstr(consumido <= limitesSuperiores[b], "Gasto maximo de recursos");
		}

		// restricao que obriga a comecar do primeiro vertice
		GRBLinExpr somatorio = 0;
		GRBLinExpr arcosInexistentes = 0;
		for (int i = 1; i < nVertices; i++) {
			somatorio += model.getVarByName(itos(1) + ">" + itos(i));
		}
		model.addConstr(somatorio == 1, "Comeca no vertice 1");

		// restricao que obriga a terminar no ultimo vertice
		somatorio = 0;
		for (int i = 1; i < nVertices; i++) {
			somatorio += model.getVarByName(itos(i) + ">" + itos(nVertices - 1));
		}
		model.addConstr(somatorio == 1, "Termina no vertice N-1");

		// restricao de conservacao de fluxo
		for (int k = 2; k < nVertices - 1; k++) {
			GRBLinExpr somatorio_i = 0;
			GRBLinExpr somatorio_j = 0;
			for (int i = 1; i < nVertices; i++) {
					somatorio_i += model.getVarByName(itos(i) + ">" + itos(k));
			}
			for (int j = 1; j < nVertices; j++) {
					somatorio_j += model.getVarByName(itos(k) + ">" + itos(j));
			}
			model.addConstr((somatorio_i - somatorio_j) == 0, "Conservacao de fluxo " + itos(k));

		}

		model.update();
		model.optimize();

		//Verifica o status do modelo
		int status = model.get(GRB_IntAttr_Status);

		model.write("saida.lp");		

		//Imprime o status do modelo
		if (status == GRB_UNBOUNDED)
		{
			std::cout << "O modelo nao pode ser resolvido porque e ilimitado" << endl;
			saida << "O modelo nao pode ser resolvido porque e ilimitado" << endl;
			return 0;
		}
		if (status == GRB_OPTIMAL)
		{
			std::cout << "Solucao otima encontrada!" << endl;
			saida << "Solucao otima encontrada!\n";
			//Acessa e imprime o valor da funcao objetivo
			std::cout << "O valor da solucao otima e: " << model.get(GRB_DoubleAttr_ObjVal) << endl;
			saida << "O valor da solucao otima e: " << model.get(GRB_DoubleAttr_ObjVal) << "\n";
		}
		if (status == GRB_INFEASIBLE)
		{
			std::cout << "O modelo nao pode ser resolvido porque e inviavel" << endl;
			saida << "O modelo nao pode ser resolvido porque e inviavel\n";
			return 0;
		}

		//Acessa as variáveis do modelo depois de resolvido
		GRBVar* v = model.getVars();
		char varname[100];

		std::cout << "Arcos escolhidos, consumos dos arcos e dos vertices no caminho" << endl;
		saida << "Arcos escolhidos, consumos dos arcos e dos vertices no caminho" << endl;
		//De maneira alternativa, imprime o valor de cada uma das variáveis
		for (int index = 0; index < model.get(GRB_IntAttr_NumVars); ++index) {
			double sol = v[index].get(GRB_DoubleAttr_X);

			sscanf(v[index].get(GRB_StringAttr_VarName).c_str(), "%s", varname);
			if (sol == 1) {
				char *i_c = strtok(varname, ">");
				char *j_c = strtok(NULL, ">");
				int i = atoi(i_c);
				int j = atoi(j_c);
				std::cout << i << " " << j << " ";
				saida << i << " " << j << " ";
				for (int k = 0; k<nRecursos; k++) {
					std::cout << adjacencia[i][j].consumo[k] << " " << consumo[j][k] << " ";
					saida << adjacencia[i][j].consumo[k] << " " << consumo[j][k] << " ";
				}
				std::cout << endl;
				saida << "\n";
				//printf("%s = %.2lf\n", varname, sol);
			}
		}

	}
	catch (GRBException e) {
		std::cout << "Erro numero: " << e.getErrorCode() << endl;
		std::cout << e.getMessage() << endl;
	}
	catch (...) {
		std::cout << "Erro durante a construcao ou solucao do modelo" << endl;
	}
	high_resolution_clock::time_point t2 = high_resolution_clock::now();
	time_span2 = duration_cast<duration<double>>(t2 - t1);
	std::cout << "Tempo total de programa: " << time_span2.count() << endl;
	saida << "Tempo total de programa: " << time_span2.count() << "\n";
	saida.close();	
	return 0;
}

int abrirArquivo(FILE **arq_entrada, char* path) {
	if ((*arq_entrada) = fopen(path, "r")) {
		return 1;
	}
	return 0;
}

void lerArquivo(FILE **arq_entrada, Arco ***adjacencia, int ***consumo, int *nVertices, int *nArcos, int *nRecursos, int **limitesInferiores, int **limitesSuperiores) {
	char enter;

	ofstream saida;
	saida.open("saida.txt", ofstream::app);

	// le N, M e K do arquivo de entrada
	fscanf(*arq_entrada, "%d %d %d %[\n]", nVertices, nArcos, nRecursos, &enter);
	(*nVertices)++;

	// instancia a matriz de adjacencia
	(*adjacencia) = new Arco *[(*nVertices)];
	for (int i = 1; i < *nVertices; i++) {
		(*adjacencia)[i] = new Arco[(*nVertices)];
		for (int j = 1; j < *nVertices; j++) {
			(*adjacencia)[i][j].consumo = new int[*nRecursos];
		}
	}

	// inicializa matriz com custos maximos
	for (int i = 1; i < (*nVertices); i++) {
		for (int j = 1; j < (*nVertices); j++) {
			(*adjacencia)[i][j].custo = INF;
		}
	}

	// inicializa a matriz de consumo de recursos
	(*consumo) = new int *[(*nVertices)];
	for (int i = 1; i < *nVertices; i++) {
		(*consumo)[i] = new int[(*nRecursos)];
	}

	int tmp, v1, v2;

	// vetores de limites inferios e superiores do problema
	(*limitesInferiores) = new int[*nRecursos];
	(*limitesSuperiores) = new int[*nRecursos];

	// le limites inferiores do arquivo de entrada
	for (int i = 0; i < (*nRecursos); i++) {
		fscanf(*arq_entrada, "%d", &tmp);
		(*limitesInferiores)[i] = tmp;
	}
	cout << endl;

	fscanf(*arq_entrada, "%[\n]", &enter);

	// le limites superiores do arquivo de entrada
	for (int i = 0; i < (*nRecursos); i++) {
		fscanf(*arq_entrada, "%d", &tmp);
		(*limitesSuperiores)[i] = tmp;
	}

	fscanf(*arq_entrada, "%[\n]", &enter);

	// le consumos dos vertices
	for (int i = 1; i < (*nVertices); i++) {
		for (int j = 0; j < (*nRecursos); j++) {
			fscanf(*arq_entrada, "%d", &tmp);
			(*consumo)[i][j] = tmp;
		}
		fscanf(*arq_entrada, "%[\n]", &enter);
	}

	// leitura dos arcos
	for (int i = 0; i < (*nArcos); i++) {
		fscanf(*arq_entrada, "%d %d %d", &v1, &v2, &tmp);

		// adiciona peso ao arco (v1 > v2)
		(*adjacencia)[v1][v2].custo = tmp;

		// adiciona consumo de recursos ao arco (v1 > v2)
		for (int j = 0; j < (*nRecursos); j++) {
			fscanf(*arq_entrada, "%d", &tmp);
			(*adjacencia)[v1][v2].consumo[j] = tmp;
		}
		fscanf(*arq_entrada, "%[\n]", &enter);
	}
}

void imprimirMatriz(Arco **adjacencia, int N, int K) {
	for (int i = 1; i < (N); i++) {
		for (int j = 1; j < (N); j++) {
			if (adjacencia[i][j].custo != 999999) {
				for (int l = 0; l < K; l++) {
					std::cout << adjacencia[i][j].consumo[l];
				}
				std::cout << endl;
			}
		}
	}
}

void deletarEstruturas(Arco ***adjacencia, int ***consumo, int **limitesInferiores, int **limitesSuperiores, int N, int K) {
	for (int i = 0; i < N; i++) {
		delete (*adjacencia)[i];
	}
	delete (**adjacencia);

	for (int i = 0; i < N; i++) {
		delete (*consumo)[i];
	}
	delete (**consumo);

	delete (*limitesInferiores);
	delete (*limitesSuperiores);

}
