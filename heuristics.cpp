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
	vector<int> nMinPath;
	//#pragma omp parallel
	//{
		while (improve < 1){
			float bestDistance = this->totalDist;

			for (int i = 0; i < this->minPath.size() - 1; i++){
				for (int j = i+1; j < this->minPath.size(); j++){
					float newDistance = inf;
					nMinPath = twoOptSwap(i, j, &newDistance);
					if (newDistance < bestDistance){
						improve = 0;
						this->minPath = nMinPath;
						bestDistance = newDistance;
						this->totalDist = bestDistance;
					}
				}
			}
			improve++;
		}
	//}
}

vector<int> ETSP::twoOptSwap(int i, int j, float* distance){
	if (i == 0 && j == this->minPath.size()-1) return this->minPath;

	vector<int> newMinPath (this->minPath);
	(*distance) = this->totalDist;

	reverse(newMinPath.begin()+i, newMinPath.begin()+j+1);

	float dxy, dwz, dxw, dyz;

	if (i == 0){
		dxy = this->getDistance(this->minPath[this->minPath.size()-1]-1, this->minPath[i]-1);	
		dxw = this->getDistance(this->minPath[this->minPath.size()-1]-1, this->minPath[j]-1);
		dwz = this->getDistance(this->minPath[j]-1, this->minPath[j+1]-1);
		dyz = this->getDistance(this->minPath[i]-1, this->minPath[j+1]-1);
	}
	else{
		if (j == this->minPath.size()-1){
			dxy = this->getDistance(this->minPath[i-1]-1, this->minPath[i]-1);	
			dxw = this->getDistance(this->minPath[i-1]-1, this->minPath[j]-1);
			dwz = this->getDistance(this->minPath[j]-1, this->minPath[0]-1);
			dyz = this->getDistance(this->minPath[i]-1, this->minPath[0]-1);
		}
		else{
			dxy = this->getDistance(this->minPath[i-1]-1, this->minPath[i]-1);	
			dxw = this->getDistance(this->minPath[i-1]-1, this->minPath[j]-1);
			dwz = this->getDistance(this->minPath[j]-1, this->minPath[j+1]-1);
			dyz = this->getDistance(this->minPath[i]-1, this->minPath[j+1]-1);
		}		
	}

	float increase = dxw + dyz - dxy - dwz;
	(*distance) += increase;
	/*vector<int> newMinPath;
	newMinPath.push_back(this->minPath[0]);

	for (int c = 1; c <= i - 1; ++c){
		newMinPath.push_back(this->minPath[c]);
		//calcular distancia
		int index = newMinPath.size()-1;
		(*distance) += this->getDistance(newMinPath[index-1]-1, newMinPath[index]-1);
	}
	
	int dec = 0;
	
	for (int c = i; c <= j; ++c){
		newMinPath.push_back(this->minPath[j - dec]);
		int index = newMinPath.size()-1;
		(*distance) += this->getDistance(newMinPath[index-1]-1, newMinPath[index]-1);
		dec++;
		//calcular distancia
		}

	for (int c = j + 1; c < this->minPath.size(); ++c){
		newMinPath.push_back(this->minPath[c]);
		//calcular distancia
		int index = newMinPath.size()-1;
		(*distance) += this->getDistance(newMinPath[index-1]-1, newMinPath[index]-1);
	}
	*/
	return newMinPath;
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
		if (v2 > 0){
			this->totalDist += smallest;
			this->minPath.push_back(v2);
		}
		vertexRoot = v2;
	}
	int index = this->minPath.size()-1;
	this->totalDist += this->getDistance(this->minPath[index]-1, this->minPath[0]-1);
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

	stack<Vertex> hull = this->grahamScan();
	while(!hull.empty()){

		Vertex top = hull.top();
		this->minPath.push_back(top.id);
		this->descobertos[top.id-1] = 1;
		hull.pop();
		
		if (!hull.empty()) this->totalDist += getDistance(top.id-1, hull.top().id-1);
		else this->totalDist += getDistance(top.id-1, this->minPath[0]-1);
	}

	vector<Tuple> smallest;
	float cab, car, crb;

	for (int i = 0; i < this->minPath.size(); i++){

		Tuple c;
		c.dist = inf;
		c.verA = this->minPath[i];
		c.verB = this->minPath[(i+1) % this->minPath.size()];

		int j = 0;
		while (this->descobertos[j]) j++;
		
		while (j < this->vertexNum){
			cab = getDistance(c.verA-1, c.verB-1);
			car = getDistance(c.verA-1, j);
			crb = getDistance(j, c.verB-1);

			if (c.dist > car + crb - cab){
				c.dist = car + crb - cab;
				c.verR = j+1;
			}

			j++;
			while (this->descobertos[j]) j++;
		}
		this->heapInsert(&smallest, c);
	}

	Tuple min = smallest[0];
	this->heapRemove(&smallest);
	this->totalDist += min.dist;

	int index = 0;
	while (this->minPath[index] != min.verA) index++;

	this->minPath.insert(minPath.begin() + ((index+1) % this->minPath.size()), min.verR);
	this->descobertos[min.verR-1] = 1;

	float c1ar, c1rb, c1ab, c2ar, c2rb, c2ab;

	while (this->minPath.size() < this->vertexNum){
		
		Tuple c1, c2;
		c1.dist = inf;
		c2.dist = inf;

		c1.verA = this->minPath[index];
		c1.verB = this->minPath[(index+1) % this->minPath.size()];

		c2.verA = c1.verB;
		c2.verB = this->minPath[(index+2) % this->minPath.size()];

		int j = 0;
		while (this->descobertos[j]) j++;
		
		while (j < this->vertexNum){
			c1ar = getDistance(c1.verA-1, j);
			c1rb = getDistance(j, c1.verB-1);
			c1ab = getDistance(c1.verA-1, c1.verB-1);
			
			c2ar = getDistance(c2.verA-1, j);
			c2rb = getDistance(j, c2.verB-1);
			c2ab = getDistance(c2.verA-1, c2.verB-1);

			if (c1.dist > c1ar + c1rb - c1ab){
				c1.dist = c1ar + c1rb - c1ab;
				c1.verR = j+1;
			}

			if (c2.dist > c2ar + c2rb - c2ab){
				c2.dist = c2ar + c2rb - c2ab;
				c2.verR = j+1; 
			}

			j++;
			while (this->descobertos[j]) j++;
		}

		this->heapInsert(&smallest, c1);
		this->heapInsert(&smallest, c2);

		while (this->descobertos[(smallest[0].verR)-1]) this->heapRemove(&smallest);

		Tuple min = smallest[0];
		this->heapRemove(&smallest);
		this->totalDist += min.dist;

		index = 0;
		while (this->minPath[index] != min.verA) index++;
		if (index == this->minPath.size()-1) this->minPath.push_back(min.verR);
		else{
			this->minPath.insert(minPath.begin() + ((index+1) % this->minPath.size()), min.verR);
			this->descobertos[min.verR-1] = 1;
		}
	}
}
