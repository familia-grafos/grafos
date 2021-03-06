#include "ETSP.h"

void recPrint(stack<Vertex> hull){
	if (hull.empty()) return;
	int top = hull.top().id;
	hull.pop();
	recPrint(hull);
	printf("%d ", top);
}

int main(){
	ETSP* grafo = new ETSP();

	int method;
	printf("Which of these methods would you like to use?\n\nNearest Neighbour - 0\nConvex Hull Insertion - 1\n\n");
	cin >> method;

	if (!method){	
		int root;
		printf("\nEnter 1st vertex:");
		cin >> root;
		printf("\n");
		
		grafo->closestFirst(root);
		grafo->twoOpt();
	}
	else grafo->cheapInsertion();

	FILE* arq1;
	arq1 = fopen("vertices.txt", "w");

	grafo->printPath(arq1);

	/*
	int search;
	printf("\nWhich search method would you like?\n\n");
	printf("Closest-First - 0\nConvex Hull - 1\nChristofides' Algorithm - 2\n\n");
	cin >> search;

	if (!grafo->dijkstra && (search == 2 || search == 0)){
		printf("Cannot calculate Dijkstra because of negative edge weights.\n");
		return 1;
	}

	int path;		
	int root;
	
	if (search != 2){	
		printf("\nEnter the 1st tree's root (the valid interval is [1, %d]): ", grafo->vertexNum);
		cin >> root;
		printf("\n");		
		if (search == 0){
			printf("Would you like to calculate:\n0 - Every minimum path?; or\n[1, %d] - Only the chosen vertex's?: ", grafo->vertexNum);
			cin >> path;
			printf("\n");
		}
	}

	if (search == 0){
		StartCounter();
		dijkstra(grafo, arq1, root, false, path);
		cout << GetCounter() << "s.\n";
		fclose(arq1);
	}

	if (search == 1){
		StartCounter();
		prim(grafo, arq1, root);
		cout << GetCounter() << "s.\n";
		fclose(arq1);	
	}

	if (search == 2){
		StartCounter();
		avgDistance(grafo, arq1);
		cout << GetCounter() << "s.\n";
		fclose(arq1);
	}
	*/
	return 0;
}
