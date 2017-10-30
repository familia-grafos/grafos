//------------------------------------------------------------------------------------------------------------------------------------------------
//  TODO LIST:
//	=> TSP implementation{ http://www2.isye.gatech.edu/~mgoetsch/cali/VEHICLE/TSP/TSP017__.HTM
//		-> steps 2 through 5
//		}
//	=>
//------------------------------------------------------------------------------------------------------------------------------------------------
#define CW 10
#define CL 11
#define CCW 12

#include "ETSP.h"

static float inf = numeric_limits<float>::infinity();
Vertex initial;

void ETSP::twoOpt(){
	int improve = 0;
	
	while (improve < 20){
		float bestDistance = this->totalDist;

		for (int i = 0; i < this->minPath.size() - 1; i++){
			for (int j = i+1; j < this->		minPath.size(); j++){
				twoOptSwap(i, j);

				float newDistance = bestDistance;
			}
		}
	}
}

void ETSP::twoOptSwap(int i, int j){

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
	this->minPath.push_back(minPath[0]);
	int index = this->minPath.size()-1;
	this->totalDist += this->getDistance(this->minPath[index-1], this->minPath[index]);
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
	
	this->totalDist = 0.0;
	vector<int> chosen;

	stack<Vertex> hull = this->grahamScan();
	while(!hull.empty()){
		Vertex top = hull.top();
		this->minPath.push_back(top.id);
		this->descobertos[top.id-1] = 1;
		hull.pop();
		if (!hull.empty()) this->totalDist += getDistance(top.id-1, hull.top().id-1);
	}
	for (int i = 0; i < this->vertexNum; i++) if (!(this->descobertos[i])) chosen.push_back(i+1);

	while (this->minPath.size() < this->vertexNum){
		
		vector<Tuple> smallest;
		float cab, car, crb;
		
		for (int j = 0; j < chosen.size(); j++){

			Tuple c;
			c.dist = inf;
			c.indexR = j;

			for (int i = 0; i < this->minPath.size()-1; i++){
				
				cab = getDistance(this->minPath[i]-1, this->minPath[i+1]-1);
				car = getDistance(this->minPath[i]-1, chosen[j]-1);
				crb = getDistance(chosen[j]-1, this->minPath[i+1]-1);

				if (c.dist > car + crb - cab){
					c.dist = car + crb - cab;
					c.indexA = i;
					c.indexB = i+1;
				}
			}

			c.compare = (car + crb)/cab;
			this->heapInsert(&smallest, chosen, c);
		}
		Tuple min = smallest[0];
		totalDist += min.dist;
		this->minPath.insert(minPath.begin() + min.indexB, chosen[min.indexR]);
		this->descobertos[chosen[min.indexR]-1];
		chosen.erase(chosen.begin() + min.indexR);
	}

	this->minPath.push_back(minPath[0]);
	int index = this->minPath.size()-1;
	this->totalDist += this->getDistance(this->minPath[index-1], this->minPath[index]);
}
