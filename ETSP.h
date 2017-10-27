#include <cmath>
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
#define CLOSESTFIRST 7
#define HULL 8
#define CHRISTOFIDES 9

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
} Comb;

void StartCounter();
double GetCounter();

class ETSP{
	
	public:
		
		vector<Vertex> coordinates;
		vector<bool> descobertos;
		vector<vector<float> > AdjVector; // Vetor de Adjacência
		float** AdjMatrix; // Matriz de Adjacência; vector<bool>
		vector<int> minPath;
		float totalDist;

		ETSP();
		
		int initEssentials(int size, int x);
		
		void init();
		void addDistance(Vertex v1, Vertex v2);
		float getDistance(int v1, int v2);

		void printPath(FILE* file);
		
		void twoOpt(int start, int end);
		stack<Vertex> grahamScan();
		void closestFirst(int vertexRoot);
		void cheapInsertion();
		void christofides();

		void heapInsert(vector<Comb>* heap, vector<int> chosen, Comb c);
		void heapifyBottomUp(vector<Comb>* heap, vector<int> chosen, int son);
		void heapRemove(vector<Comb>* heap, vector<int> chosen);
		void heapifyTopDown(vector<Comb>* heap, vector<int> chosen, int root);

	private:
		int representation;
		int vertexNum; // Número de Vértices
};
