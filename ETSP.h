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
#define CW 10
#define CL 11
#define CCW 12

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
		
		int getVertexNum();
		int initEssentials(int size, int x);
		
		void initAdjVector();
		void initAdjMatrix();
		
		void addDistanceVector(Vertex v1, Vertex v2, int id);
		void addDistanceMatrix(Vertex v1, Vertex v2, int id1, int id2);
		
		void printPath(FILE* file);
		
		void twoOpt(int start, int end);
		stack<Vertex> grahamScan();
		void closestFirst(int vertexRoot);
		void insertion();
		void christofides();

	private:
		int representation;
		int vertexNum; // Número de Vértices
};
