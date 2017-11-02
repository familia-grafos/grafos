#include "ETSP.h"

using namespace std;

void swap(vector<Tuple>* vec, int index1, int index2){
	Tuple temp = (*vec)[index1];
	(*vec)[index1] = (*vec)[index2];
	(*vec)[index2] = temp;
}

void ETSP::heapifyBottomUp(vector<Tuple>* heap, int son){
	if (son == 0) return;

	int father;
	if (son % 2 == 1) father = (son-1)/2;
	else father = (son-2)/2;

	Tuple sonC = (*heap)[son];
	Tuple fatherC = (*heap)[father];

	if (sonC.dist < fatherC.dist){
		swap(heap, father, son);
		heapifyBottomUp(heap, father);
	}
}

void ETSP::heapifyTopDown(vector<Tuple>* heap, int root){
	int smallest = root;
	int leftSon = 2*root+1;
	int rightSon = 2*root+2;

	Tuple fatherTuple = (*heap)[smallest];
	Tuple leftTuple, rightTuple;

	float distance = fatherTuple.dist;

	if (leftSon < heap->size()){
		leftTuple = (*heap)[leftSon];

		if (leftTuple.dist < distance){
			distance = leftTuple.dist;
			smallest = leftSon;
		}
		
		if (rightSon < heap->size()){
			rightTuple = (*heap)[rightSon];

			if (rightTuple.dist < distance){
				distance = rightTuple.dist;
				smallest = rightSon;
			}
		}
	}

	if (smallest != root){
		swap(heap, smallest, root);
		heapifyTopDown(heap, smallest);
	}
}

void ETSP::heapInsert(vector<Tuple>* heap, Tuple c){
	int index = heap->size();
	heap->push_back(c);
	if (index != 0) heapifyBottomUp(heap, index);
}

void ETSP::heapRemove(vector<Tuple>* heap){
	int index = heap->size()-1;
	if (index < 0) return;

	vector<Tuple> temp;
	temp.swap(*heap);
	swap(&temp, index, 0);
	temp.pop_back();
	heap->swap(temp);
	heapifyTopDown(heap, 0);
}
