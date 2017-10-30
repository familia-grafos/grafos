#include <cmath>
#include <list>
#include <algorithm>
#include <iostream>
#include <stdio.h>
#include <vector>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <fstream>
#include <stack>
#include <sstream>
#include <limits>
#include <omp.h>

using namespace std;

#define VECTOR 0
#define MATRIX 1
#define CLOSESTFIRST 2
#define HULL 3

class Vertex{
	public:
		int id;
		float posX;
		float posY;

		bool operator < (Vertex b){
			if (posY != b.posY) return posY < b.posY;
			return posX < b.posX;
		}
};

typedef struct{
	int indexA;
	int indexB;
	int indexR;
	float dist;
	float compare;
} Tuple;

void StartCounter();
double GetCounter();

class ETSP{

	public:

		vector<Vertex> coordinates;
		vector<bool> descobertos;
		vector<vector<float> > AdjVector; // Vetor de Adjacência
		float** AdjMatrix; // Matriz de Adjacência; vector<bool>
		vector<int> minPath;
		vector<int> minPath2;

		float totalDist;

		ETSP();

		int initEssentials(int size, int x);

		void init();
		void addDistance(Vertex v1, Vertex v2);
		float getDistance(int v1, int v2);

		void printPath(FILE* file);

		void twoOpt();
		vector<int> twoOptSwap(int i, int j, float* distance);
		stack<Vertex> grahamScan();
		void closestFirst(int vertexRoot);
		void cheapInsertion();
		void christofides();

		void heapInsert(vector<Tuple>* heap, vector<int> chosen, Tuple c);
		void heapifyBottomUp(vector<Tuple>* heap, vector<int> chosen, int son);
		void heapifyTopDown(vector<Tuple>* heap, vector<int> chosen, int root);
		void heapRemove(vector<Tuple>* heap, vector<int> chosen);

	private:
		int representation;
		int vertexNum; // Número de Vértices
};
