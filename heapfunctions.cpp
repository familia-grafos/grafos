#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include "ETSP.h"

using namespace std;

void swap(vector<Comb>* vec, int index1, int index2){
	Comb temp = (*vec)[index1];
	(*vec)[index1] = (*vec)[index2];
	(*vec)[index2] = temp;
}

void ETSP::heapifyBottomUp(vector<Comb>* heap, vector<int> chosen, int son){
	if (son == 0) return;
	
	int father;
	if (son % 2 == 1) father = (son-1)/2;
	else father = (son-2)/2;

	Comb sonC = (*heap)[son];
	Comb fatherC = (*heap)[father];

	float sonCab = getDistance(this->minPath[sonC.indexA]-1, this->minPath[sonC.indexB]-1);
	float sonCar = getDistance(this->minPath[sonC.indexA]-1, chosen[sonC.indexR]-1);
	float sonCrb = getDistance(chosen[sonC.indexR]-1, this->minPath[sonC.indexB]-1);

	float fatherCab = getDistance(this->minPath[fatherC.indexA]-1, this->minPath[fatherC.indexB]-1);
	float fatherCar = getDistance(this->minPath[fatherC.indexA]-1, chosen[fatherC.indexR]-1);
	float fatherCrb = getDistance(chosen[fatherC.indexR]-1, this->minPath[fatherC.indexB]-1);

	if (sonCar + sonCrb - sonCab < fatherCar + fatherCrb - fatherCab){

		swap(heap, father, son);
		heapifyBottomUp(heap, chosen, father);
	}
}

void ETSP::heapInsert(vector<Comb>* heap, vector<int> chosen, Comb c){
	int index = heap->size();
	heap->push_back(c);
	if (index != 0) heapifyBottomUp(heap, chosen, index);
}


void ETSP::heapifyTopDown(vector<Comb>* heap, vector<int> chosen, int root){
	int smallest = root;
	int leftSon = 2*root+1;
	int rightSon = 2*root+2;

	Comb lSonC = (*heap)[leftSon];
	Comb rSonC = (*heap)[rightSon];
	Comb fatherC = (*heap)[smallest];

	float lSonCab = getDistance(this->minPath[lSonC.indexA]-1, this->minPath[lSonC.indexB]-1);
	float lSonCar = getDistance(this->minPath[lSonC.indexA]-1, chosen[lSonC.indexR]-1);
	float lSonCrb = getDistance(chosen[lSonC.indexR]-1, this->minPath[lSonC.indexB]-1);

	float rSonCab = getDistance(this->minPath[rSonC.indexA]-1, this->minPath[rSonC.indexB]-1);
	float rSonCar = getDistance(this->minPath[rSonC.indexA]-1, chosen[rSonC.indexR]-1);
	float rSonCrb = getDistance(chosen[rSonC.indexR]-1, this->minPath[rSonC.indexB]-1);	

	float fatherCab = getDistance(this->minPath[fatherC.indexA]-1, this->minPath[fatherC.indexB]-1);
	float fatherCar = getDistance(this->minPath[fatherC.indexA]-1, chosen[fatherC.indexR]-1);
	float fatherCrb = getDistance(chosen[fatherC.indexR]-1, this->minPath[fatherC.indexB]-1);

	if (leftSon < heap->size() && lSonCar + lSonCrb - lSonCab < fatherCar + fatherCrb - fatherCab){
		smallest = leftSon;
		fatherCab = lSonCab;
		fatherCar = lSonCar;
		fatherCrb = lSonCrb;
	}

	if (rightSon < heap->size() && rSonCar + rSonCrb - rSonCab < fatherCar + fatherCrb - fatherCab) father = rightSon;

	if (smallest != root){
		swap(heap, smallest, root);
		heapifyTopDown(heap, chosen, smallest);
	}
}

Comb ETSP::heapRemove(vector<Comb>* heap, vector<int> chosen){
	int index = heap->size()-1;
	if (index < 0) return;
	Comb res = heap[0];

	vector<Comb> temp;
	swap(heap, index, 0);
	heap->swap(temp);
	temp.pop_back();
	temp.swap(*heap);

	this->heapifyTopDown(heap, chosen, 0);
	return res;
}
