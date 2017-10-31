#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include "ETSP.h"

using namespace std;

void swap(vector<Tuple>* vec, int index1, int index2){
	Tuple temp = (*vec)[index1];
	(*vec)[index1] = (*vec)[index2];
	(*vec)[index2] = temp;
}

void ETSP::heapifyBottomUp(vector<Tuple>* heap, vector<int> chosen, int son){
	if (son == 0) return;
	
	int father;
	if (son % 2 == 1) father = (son-1)/2;
	else father = (son-2)/2;

	Tuple sonC = (*heap)[son];
	Tuple fatherC = (*heap)[father];

	if (sonC.dist < fatherC.dist){
		swap(heap, father, son);
		heapifyBottomUp(heap, chosen, father);
	}
}

void ETSP::heapInsert(vector<Tuple>* heap, vector<int> chosen, Tuple c){
	int index = heap->size();
	heap->push_back(c);
	if (index != 0) heapifyBottomUp(heap, chosen, index);
}
