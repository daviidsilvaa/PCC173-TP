all: *.cpp
	g++ -std=c++11 main.cpp -o main -I/home/david/gurobi801/linux64/include -L/home/david/gurobi801/linux64/lib -lgurobi_c++ -lgurobi80


run:
	./main Instancias/1.txt
