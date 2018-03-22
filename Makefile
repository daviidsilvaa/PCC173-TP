all: *.cpp
	g++ -o cmc Main.cpp -std=c++11

run:
	./cmc 1.txt

