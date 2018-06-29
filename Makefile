all: *.cpp
	g++ -std=c++11 MainGurobi.cpp -o executable -I/home/david/gurobi801/linux64/include -L/home/david/gurobi801/linux64/lib -lgurobi_c++ -lgurobi80


run:
	./executable Instancias/1.txt
