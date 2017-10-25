#include <vector>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

void intSwap(vector<int>* vec, int index1, int index2){
	int temp = (*vec)[index1];
	(*vec)[index1] = (*vec)[index2];
	(*vec)[index2] = temp;
}

void intHeapifyBottomUp(vector<int>* heap, vector<float>* dist, vector<int>* pos, int son){
	if (son == 0) return;
	
	int father;
	if (son % 2 == 1) father = (son-1)/2;
	else father = (son-2)/2;

	if ((*dist)[(*heap)[son]] < (*dist)[(*heap)[father]]){

		intSwap(heap, father, son);
		intSwap(pos, (*heap)[father], (*heap)[son]);
		intHeapifyBottomUp(heap, dist, pos, father);
	}
}

void intHeapifyTopDown(vector<int>* heap, vector<float>* dist, vector<int>* pos, int root){
	int smallest = root;
	int leftSon = 2*root+1;
	int rightSon = 2*root+2;

	if (leftSon < heap->size() && (*dist)[(*heap)[leftSon]] < (*dist)[(*heap)[smallest]]) smallest = leftSon;
	if (rightSon < heap->size() && (*dist)[(*heap)[rightSon]] < (*dist)[(*heap)[smallest]]) smallest = rightSon;

	if (smallest != root){
		intSwap(heap, smallest, root);
		intSwap(pos, (*heap)[smallest], (*heap)[root]);
		intHeapifyTopDown(heap, dist, pos, smallest);
	}
}

void intHeapInsert(vector<int>* heap, vector<float>* dist, vector<int>* pos, int vertex){
	int index = heap->size();
	heap->push_back(vertex);
	pos->push_back(vertex);
	if (index != 0) intHeapifyBottomUp(heap, dist, pos, index);
}

void intHeapRemove(vector<int>* heap, vector<float>* dist, vector<int>* pos){
	int index = heap->size()-1;
	if (index < 0) return;
	vector<int> temp;
	intSwap(heap, index, 0);
	intSwap(pos, (*heap)[index], (*heap)[0]);
	heap->swap(temp);
	temp.pop_back();
	temp.swap(*heap);

	intHeapifyTopDown(heap, dist, pos, 0);
}