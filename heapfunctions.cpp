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
