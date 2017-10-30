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
	printf("\n");

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

			init();
			printf("Finished initializing.\n");
			for (int i = 0; i < n; i++){
				for (int j = 0; j <= i; j++) this->addDistance(this->coordinates[i], this->coordinates[j]);
			}
			printf("Finished assigning distances.\n");
		}

		time = GetCounter();
	}
	this->totalDist = 0.0;
	printf("Finished printing.\n\n");
	cout << time << "s.\n";
	fclose(in);
}

void ETSP::init(){
	
	if (this->representation == VECTOR){
		vector<float> v;
		this->AdjVector.resize(this->vertexNum, v);
	}
	
	if (this->representation == MATRIX){
		this->AdjMatrix = new float* [vertexNum];
		for (int i = 0; i < vertexNum; i++) this->AdjMatrix[i] = new float [i+1];
	}
}

int ETSP::initEssentials(int size, int x){
	if (size <= 0) return 1;
	
	this->representation = x;
	this->vertexNum = size; // Inicializando número de vértices
	
	this->descobertos.resize(vertexNum);
	return 0;
}

void ETSP::addDistance(Vertex v1, Vertex v2){
	
	if (this->representation == VECTOR){
		if (v1.id == v2.id){
			AdjVector[v1.id-1].push_back(0.0);
			return;
		}

		float dx = pow((v2.posX-v1.posX), 2);
		float dy = pow((v2.posY-v1.posY), 2);

		float distance = sqrt(dx+dy);

		this->AdjVector[v1.id-1].push_back(distance);
	}

	if (this->representation == MATRIX){
		if (v1.id == v2.id){
			this->AdjMatrix[v1.id-1][v2.id-1] = 0.0;
			return;
		}

		float dx = pow((v2.posX-v1.posX), 2);
		float dy = pow((v2.posY-v1.posY), 2);

		float distance = sqrt(dx+dy);

		this->AdjMatrix[v1.id-1][v2.id-1] = distance;
	}
}

float ETSP::getDistance(int v1, int v2){
	if (v1 == v2) return 0.0;
	
	if (this->representation == VECTOR){
		if (v1 > v2) return this->AdjVector[v1][v2];
		return this->AdjVector[v2][v1];
	}
	
	if (this->representation == MATRIX){
		if (v1 > v2) return this->AdjMatrix[v1][v2];
		return this->AdjMatrix[v2][v1];
	}
}

void ETSP::printPath(FILE* file){
	fprintf(file, "Minimum Distance:	%.10f\n", this->totalDist);
	//fprintf(file, "Time:	%d", cout);
	fprintf(file, "Minimum Path:	%d", this->minPath[0]);

	
	
	for (int i = 1; i < this->minPath.size(); i++) fprintf(file, " %d", this->minPath[i]);
}
