#define CW 10
#define CL 11
#define CCW 12

#include "ETSP.h"

static float inf = numeric_limits<float>::infinity();
Vertex initial;

void ETSP::twoOpt(int start, int end){

}

void ETSP::closestFirst(int vertexRoot){
	
	this->minPath.push_back(vertexRoot);
	this->totalDist = 0.0;

	while (vertexRoot > 0){
		this->descobertos[vertexRoot-1] = 1;
		float smallest = inf;
		int v2 = 0;
		for (int j = 0; j < this->vertexNum; j++){
			if (!(this->descobertos[j]) && smallest > this->getDistance(vertexRoot-1, j)){
				smallest = this->getDistance(vertexRoot-1, j);
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

int orientation(Vertex v1, Vertex v2, Vertex v3){
	float det = (v1.posX - v2.posX)*(v3.posY - v2.posY) - (v1.posY - v2.posY)*(v3.posX - v2.posX);
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
	sort(this->coordinates.begin()+1, this->coordinates.end(), polarOrder);
	
	hull.push(this->coordinates[0]);	
	hull.push(this->coordinates[1]);
	hull.push(this->coordinates[2]);
    
	for (int i = 3; i < this->coordinates.size(); i++) {
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

void ETSP::cheapInsertion(){
	
	vector<int> chosen;
	vector<Comb> smallest;
	int newVertex = 0;

	stack<Vertex> hull = this->grahamScan();
	while(!hull.empty()){
		this->minPath.push_back(hull.top().id);
		this->descobertos[hull.top().id-1] = 1;
		hull.pop();
	}

	for (int i = 0; i < this->vertexNum; i++) if (!(this->descobertos[i])) chosen.push_back(i+1);
	
	for (int j = 0; j < chosen.size(); j++){
		
		float cab, car, crb;

		float dist = inf;
		Comb c;
		c.indexR = j;

		for (int i = 0; i < this->minPath.size()-1; i++){
			
			cab = getDistance(this->minPath[i]-1, this->minPath[i+1]-1);
			car = getDistance(this->minPath[i]-1, chosen[j]-1);
			crb = getDistance(chosen[j]-1, this->minPath[i+1]-1);

			if (dist > car + crb - cab){
				dist = cab + crb - cab;
				c.indexA = i;
				c.indexB = i+1;
			}
		}
		this->heapInsert(&smallest, chosen, c);
		newVertex = j;
		this->minPath.insert(minPath.begin() + min.indexB, chosen[min.indexR]);
		this->descobertos[chosen[min.indexR]-1];
	}

	while (this->minPath.size() < this->vertexNum){
		//DEVELOP

		Comb min = heapRemove(&smallest);
		this->minPath.insert(minPath.begin() + min.indexB, chosen[min.indexR]);
		this->descobertos[chosen[min.indexR]-1];
		chosen.erase(chosen.begin() + min.indexR);
	}
}
