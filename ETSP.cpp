#include "ETSP.h"
#include <windows.h>

double PCFreq = 0.0;
__int64 CounterStart = 0;

void StartCounter(){
    LARGE_INTEGER li;
    if(!QueryPerformanceFrequency(&li))
    cout << "QueryPerformanceFrequency failed!\n";

    PCFreq = double(li.QuadPart)/1.0;

    QueryPerformanceCounter(&li);
    CounterStart = li.QuadPart;
}

double GetCounter(){
    LARGE_INTEGER li;
    QueryPerformanceCounter(&li);
    return double(li.QuadPart-CounterStart)/PCFreq;
}

ETSP::ETSP(){

	FILE* in;
	string s;
	printf("\nEnter .txt filename:");
	cin >> s;
	string str = s+".txt";
	in = fopen(str.c_str(), "r");

	while(!in){
		printf("\nNo file found\n");
		printf("\nEnter .txt filename:");
		cin >> s;
		string str = s+".txt";
		in = fopen(str.c_str(), "r");
	}

	int n, m;
	fscanf(in, "%d", &n);
		
	printf("\nPlease choose one of the 2\nPossible graph representations:\n\n");
	printf("Adjacency Vector - 0\nAdjacency Matrix - 1\n\n");
		
	cin >> m;

	double time;

	if (!in) printf("Could not open file.\n");
	else{
		
		StartCounter();

		int x = this->initEssentials(n, m);
		
		if (x) printf("Couldn't create graph.");
		
		else{
			
			int a, b;
			int z = 1;
			while (fscanf(in, "%d %d", &a, &b) == 2){
				Vertex u;
				u.id = z;
				z++;
				u.posX = a;
				u.posY = b;
				this->coordinates.push_back(u);
			}

			// Vetor

			if (m == VECTOR){
				printf("Vector\n\n");
				
				initAdjVector();
				printf("Finished initializing.\n");

				for (int i = 0; i < n; i++){
					for (int j = 0; j <= i; j++) this->addDistanceVector(this->coordinates[i], this->coordinates[j], i);
				}
				printf("Finished assigning distances.\n");
			}

			// Matriz

			if (m == MATRIX){
				printf("Matrix\n\n");

				initAdjMatrix();
				printf("Finished initializing.\n");
				
				for (int i = 0; i < n; i++){
					for (int j = 0; j < i; j++) this->addDistanceMatrix(this->coordinates[i], this->coordinates[j], i, j);
				}

				printf("Finished assigning edges.\n");
			}
		}
		time = GetCounter();
	}

	printf("Finished printing.\n\n");
	cout << time << "s.\n";

	fclose(in);
}

int ETSP::getVertexNum(){
	return this->vertexNum;
}

int ETSP::initEssentials(int size, int x){
	if (size <= 0) return 1;
	
	this->representation = x;
	this->vertexNum = size; // Inicializando número de vértices
	
	this->descobertos.resize(vertexNum);
	return 0;
}

//-----------------Vetor de Adjacência-----------------------------------------------------------------//

void ETSP::initAdjVector(){
	vector<float> v;
	this->AdjVector.resize(this->vertexNum, v);
}

void ETSP::addDistanceVector(Vertex v1, Vertex v2, int id){
	if (id == AdjVector[id].size()){
		AdjVector[id].push_back(0.0);
		return;
	}

	float dx = pow((v2.posX-v1.posX), 2);
	float dy = pow((v2.posY-v1.posY), 2);

	float distance = sqrt(dx+dy);

	this->AdjVector[id].push_back(distance);
}

//-------------------Matriz de Adjacência; vector<float>[]----------------------------------------------//

void ETSP::initAdjMatrix(){
	this->AdjMatrix = new float* [vertexNum];
	for (int i = 0; i < vertexNum; i++){
		this->AdjMatrix[i] = new float [i+1];
		this->AdjMatrix[i][i] = 0.0;
	}
}

void ETSP::addDistanceMatrix(Vertex v1, Vertex v2, int id1, int id2){
	float dx = pow((v2.posX-v1.posX), 2);
	float dy = pow((v2.posY-v1.posY), 2);

	float distance = sqrt(dx+dy);

	this->AdjMatrix[id1][id2] = distance;
}

//-------------------Funções Adicionais-----------------------------------------------------------------//

void ETSP::printPath(FILE* file){
	fprintf(file, "Minimum Distance:	%.10f\n\n", this->totalDist);
	fprintf(file, "[%d", this->minPath[0]);
	for (int i = 1; i < this->vertexNum; i++) fprintf(file, ", %d", this->coordinates[i]);
	fprintf(file, "]");
}