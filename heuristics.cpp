//------------------------------------------------------------------------------------------------------------------------------------------------
//  TODO LIST:
//	=> TSP implementation{ http://www2.isye.gatech.edu/~mgoetsch/cali/VEHICLE/TSP/TSP017__.HTM
//		-> steps 2 through 5
//		}
//	=>
//------------------------------------------------------------------------------------------------------------------------------------------------

#include "ETSP.h"

static float inf = numeric_limits<float>::infinity();
Vertex initial;

void ETSP::twoOpt(int start, int end){

}

void ETSP::closestFirst(int vertexRoot){
	
	this->minPath.push_back(vertexRoot);
	this->totalDist = 0.0;

	if (this->representation == VECTOR){
		while (vertexRoot > 0){
			this->descobertos[vertexRoot-1] = 1;
			float smallest = inf;
			int v2 = 0;
			for (int j = 0; j < vertexRoot; j++){
				if (!(this->descobertos[j]) && smallest > this->AdjVector[vertexRoot-1][j]){
					smallest = this->AdjVector[vertexRoot-1][j];
					v2 = j+1;
				}
			}
			for (int j = vertexRoot+1; j < this->vertexNum; j++){
				if (!(this->descobertos[j]) && smallest > this->AdjVector[j][vertexRoot-1]){
					smallest = this->AdjVector[j][vertexRoot-1];
					v2 = j+1;
				}
			}
			if (v2){
				totalDist += smallest;
				this->minPath.push_back(v2);
			}
			vertexRoot = v2;
		}
	}

	if (this->representation == MATRIX){
		while (vertexRoot > 0){
			this->descobertos[vertexRoot-1] = 1;
			float smallest = inf;
			int v2 = 0;
			for (int j = 0; j < vertexRoot; j++){
				if (!(this->descobertos[j]) && smallest > this->AdjMatrix[vertexRoot-1][j]){
					smallest = this->AdjMatrix[vertexRoot-1][j];
					v2 = j+1;
				}
			}
			for (int j = vertexRoot+1; j < this->vertexNum; j++){
				if (!(this->descobertos[j]) && smallest > this->AdjMatrix[j][vertexRoot-1]){
					smallest = this->AdjMatrix[j][vertexRoot-1];
					v2 = j+1;
				}
			}
			if (v2){
				this->totalDist += smallest;
				this->minPath.push_back(v2);
			}
			vertexRoot = v2;
		}
	}
}

int orientation(Vertex v1, Vertex v2, Vertex v3){
	int det = (v2.posY - v1.posY) * (v3.posX - v2.posX) - (v2.posX - v1.posX) * (v3.posY - v2.posY);
	if (det == 0) return CL;
	return (det > 0) ? CW : CCW;
}

int sqrDist(Vertex a, Vertex b){
	int dx = a.posX - b.posX;
	int dy = a.posY - b.posY;
	return dx*dx + dy*dy;
}

bool polarOrder(Vertex a, Vertex b){
	int order = orientation(initial, a, b);
	if (order == CL) return sqrDist(initial, a) < sqrDist(initial, b);
	return (order == CCW);
}

stack<Vertex> ETSP::grahamScan(){
	stack<Vertex> hull;
	if (this->vertexNum < 3) return hull;

	int least = 0;
	for (int i = 1; i < this->vertexNum; i++){
		if (this->coordinates[i] < this->coordinates[least]) least = i;
	}

	Vertex temp = this->coordinates[least];
	this->coordinates[least] = this->coordinates[0];
	this->coordinates[0] = temp;

	initial = coordinates[0];
	sort(this->coordinates.begin()+1, this->coordinates.end()-1, polarOrder);
	
	hull.push(this->coordinates[0]);
	hull.push(this->coordinates[1]);
	hull.push(this->coordinates[2]);
    
	for (int i = 3; i < this->vertexNum; i++) {
		Vertex top = hull.top();
		hull.pop();
		while (orientation(hull.top(), top, this->coordinates[i]) != CCW){
			top = hull.top();
			hull.pop();
		}
		hull.push(top);
		hull.push(this->coordinates[i]);
	}
	return hull;
}

void ETSP::insertion(){
	stack<Vertex> hull = this->grahamScan();
	while(!hull.empty()){
		this->minPath.push_back(hull.top().id);
		hull.pop();
	}
 



}