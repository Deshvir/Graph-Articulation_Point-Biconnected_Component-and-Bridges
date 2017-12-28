/*
Graph_Code.c
Author: Deshvir Malik
Date : Nov20,2017
Copyright @ 2017 Deshvir Malik.
All right reserved
Functionality: Implementation of DFS algorithm for articulation point, biconnected components and bridges

*/

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "stdbool.h"

static int time = 0;	//time variable used in assigning discovery and low time of num_v
int num_v = 0;				//num_v represent the total number of vertices
int top;				//top represent the current status of stack
int temp;				//stores the maximum value of top before finding articulation point
int count_BC = 0;		//count biconnected components
int arr_BP[20];			//stack to store biconnected components
int bridge_count = 0;
int edge_cost = 0;		//cost of using the edges
int vertex_cost = 0;	//cost of using the vertices
int biconnected_count = 0;

void insertionSortBP(int arr[], int n);
void printSortedBC(int arr[], int n, FILE *f);

/*
Function name : addEdge
Description : It will create adjacency matrix by connecting souce and destination
Parameters :	_In_ of type int -> take source vertex	 
				_In_ of type int -> take destination vertex
				_in_ of type int -> take adjacency matrix
Called function: None
*/
int addEdge(int source, int destination, int G[num_v][num_v])	{		
	for(int i = 1; i<=num_v ; i++){		
		//edge_cost++;
		for (int j = 1; j <= num_v; ++j)	{			
			if(i == source && destination == j) {				
				G[i][j] = 1;	//edge from vertex source(i) to destination (j)
				G[j][i] = 1;	//edge from vertex destination(j) to source(i)				
			}			
		}		
	}
	return 0;
}

/*
Function name : DFS
Description : This consist fo DFS algorithm, articulation point, biconnected components and bridges
Parameters :	_In_ of type int -> take adjacency matrix 
				_In_ of type int -> take vertex to be considered as parent 
				_In_ of type int -> take status of vertex as visited or not
				_In_ of type int -> stores parent of vertex
				_In_ of type int -> stores discovery time of vertx
				_In_ of type int -> stores low time of vertx
				_In_ of type int -> stores articulation point status of vertx
				_In_ of type int -> stores the vertices in stack 
				_In_ of type FILE* ->takes the file pointer for storing bridges
				_In_ of type FILE* ->takes the file pointer for storing biconnected components 
Called function: recussive DFS
*/
void DFS(int G[num_v][num_v], int V,int visited[num_v], int parent[num_v], int disc[num_v], int low[num_v], \
	int ap[num_v], int stack[num_v], FILE *f_Ab, FILE *f_Ac)	{

	vertex_cost = vertex_cost + 3;
	visited[V] = true;	
	disc[V] = low[V] = time;	
	++time;
	int children = 0;	//this is used to find number of childeren of a root node and helps in deciding articulation point
	stack[top] = V;	//store visited vertx in stack

	for (int i = 1; i <= num_v; ++i)	{
		edge_cost++;
		if (G[V][i] == 1)	{
			if (visited[i] == false)	{
				//vertex_cost++;
				children = children+1;
				parent[i] = V;							
				++top; //increase TOP of stack and store value of top in temp	
				temp = top;				
				DFS(G,i,visited, parent, disc, low, ap, stack, f_Ab, f_Ac);				
				--top;//decrease max index value TOP during backedge
				
				if(low[i] < low[V]){		//if adjacent vertex is having less low time, assign it to current vertx			
					low[V] = low[i];
				}

				if(parent[V] == -1 && children > 1){					
					ap[V] = true;
					
					//printf("Biconnected Components : "); //print connected graph values
					for(int i = temp; i >= top ; i--){
						//printf("%d\t", stack[i]);
						arr_BP[count_BC] = stack[i];
						count_BC++;
						vertex_cost++;
					}
					//printf("\n");
					insertionSortBP(arr_BP, count_BC);	
					printSortedBC(arr_BP, count_BC, f_Ac);				
					temp = top;
				}

				if(parent[V] != -1 && low[i] >= disc[V] ){					
					ap[V] = true;
										
					//printf("Biconnected Components :: "); //print connected graph values//
					for(int i = temp; i >= top ; i--){
						//printf(" %d ", stack[i]);
						arr_BP[count_BC] = stack[i];
						count_BC++;
						vertex_cost++;
					}
					//printf("\n");	
					insertionSortBP(arr_BP, count_BC);	
					printSortedBC(arr_BP, count_BC,f_Ac);				
					temp = top;
				}

				//Bridge Detection Condition
				if(low[i] > disc[V]){					
					if(V < i){	//for putting in sorting order
						//printf("Bridge: %d-%d\n", V, i);
						fprintf(f_Ab, "%d  %d\n",V,i);
						bridge_count++;
					}
					else{
						//printf("Bridge: %d-%d\n", i, V);
						fprintf(f_Ab, "%d  %d\n",i,V);
						bridge_count++;
					}
				}
			}
			else if(parent[V] != i)	{				
				if(low[i] < low[V]) {
					low[V] = low[i];
				}				
			}
		}
	}	
}

/*
Function name : insertionSortBP
Description : This will arrange the biconnected components in ascending order 
Parameters :	_In_ of type int arr[] -> take array of elements in biconnected components
				_In_ of type int -> take number of vertices in biconnected components
Called function: None
*/

void insertionSortBP(int arr[], int n) {
	int j, i, key;
	for(j = 1; j < n; j++) 	{
		key = arr[j];
		i = j-1;
		while(i>=0 && arr[i]>key) {
			arr[i+1] = arr[i];
			i = i-1;
		}		
		arr[i+1] = key;
	}
}

/*
Function name : printSortedBC
Description : This will arrange the biconnected components in ascending order 
Parameters :	_In_ of type int arr[] -> take array of elements in biconnected components
				_In_ of type FILE * -> put vertices of biconnected components in  file using file pointer
Called function: None
*/
void printSortedBC(int arr[], int n, FILE *f_Ac) {	
	//printf("Output Array : [");	
	for(int j = 0; j < n; j++) 	{
	//	printf("%d ", arr[j]);
		fprintf(f_Ac," %d ", arr[j]);		
	}
	fprintf(f_Ac,"\n");
	//printf("]\n");
	count_BC = 0;
	biconnected_count++;
}

int main(int argc, char *argv[])
{
	int count_AP = 0;
	int sort_AP[15];
	int count_BPR = 0;
	int edge_count = 0;
	int total_cost = 0;

	FILE *f_Aa;
	FILE *f_Ab;
	FILE *f_Ac;
	FILE *f_C;
	FILE *ifp;

	if (argc < 2)
	{
		printf("too few arguments !");
		return 0;
	}

	// read input from file
	ifp = fopen(argv[1], "r");

	// int num_v = 0;
	char l[256];
	while (fgets(l, sizeof l, ifp) != NULL) num_v++;
	rewind(ifp);

  	// TODO: please comment the following line
  	//printf("Total number of vertices = %d\n", num_v);

  	int G[num_v][num_v]; //represent the adjacency matrix of a graph
	int visited[num_v];	//store status of visited num_v
	int parent[num_v];	//store status of parent of num_v
	int low[num_v];	//stores the low time of num_v
	int disc[num_v];	//stores the discovery time of num_v
	int ap[num_v];	//store the articulation point of num_v
	int stack[num_v]; //Stack is used during backedge and biconnected components 

	for(int i = 1; i<=num_v ; i++)	{
		for (int j = 1; j <= num_v; ++j) {
			G[i][j] = 0;	//initialise the adjacency matrix with valur 0 
			edge_cost++;
		}
	}

	for(int i = 1; i<=num_v ; i++) {
		visited[i] = 0;	//visited num_v is initialized to 0
		parent[i] = -1;	//parent of all vertices are -1 initially
		low[i] = -1;	//low time of all vertices id -1 initially
		disc[i] = -1;	//discovery time of all vertices is -1 initially
		ap[i] = 0;	//articulation status of each num_v is 0 initially
		vertex_cost++;
	}
	//printf("vertex_cost : %d\n", vertex_cost);

	char line[1024];
	while (fgets(line, sizeof line, ifp) != NULL)	{
    int src_vertex, dst_vertex;
		char * pch;
		pch = strtok(line, " \n\r");
		sscanf(pch, "%d", &src_vertex);
		pch = strtok(NULL, " \n\r");

		while (pch != NULL)		{
			// there is an edge from src_vertex to dst_vertex
			sscanf(pch, "%d", &dst_vertex);
      		// TODO : please comment the following line
      		//printf("Edge exists between (%d,%d)\n", src_vertex, dst_vertex);
			pch = strtok(NULL, " \n\r");
			addEdge(src_vertex,dst_vertex,G);
			edge_count++;
			edge_cost++;
		}		
	}
	// fclose(ifp);

	f_Aa = fopen("Aa.txt","w");
	if (f_Aa == NULL)
	{
    	printf("Error opening file!\n");
    	exit(1);
	}

	f_Ab = fopen("Ab.txt","w");
	if(f_Ab == NULL){
		printf("Couldn't open file\n");
		return 1;
	}

	f_Ac = fopen("Ac.txt","w");
	if(f_Ac == NULL){
		printf("Couldn't open file\n");
		return 1;
	}

	f_C = fopen("C.txt","w");
	if(f_C == NULL){
		printf("Couldn't open file\n");
		return 1;
	}

	for(int i = 1 ; i <= num_v ; i++){
		if(visited[i] == false){
			DFS(G,1,visited, parent, disc, low, ap, stack, f_Ab, f_Ac);	
		}		
		vertex_cost++;
	}

	//stores the remaining elements of stack	
	if (temp != -1) {
		for(int i = temp; i >= top ; i--){
			count_BPR++;		
			vertex_cost++;	
		}		
	}
	
	if (temp != -1 && count_BPR > 1) {
		//printf("Biconnected Components last:");
		for(int i = temp; i >= top ; i--){			
			//printf(" %d ", stack[i]);
			fprintf(f_Ac," %d ", stack[i]);
		}
		biconnected_count++;
		//printf("\n");
	}

	//printf("Articulation Point :");
	for (int i = 1; i <= num_v; ++i)	{			
		if (ap[i] == 1)	{			
			sort_AP[count_AP] = i;
			++count_AP;
			//printf(" %d ", i);
			fprintf(f_Aa," %d ", i);
		}		
	}	
	vertex_cost = vertex_cost + 10;
	total_cost = edge_cost + vertex_cost;
	fprintf(f_C,"Total number of operation charged to all vertices: %d\n", vertex_cost );
	fprintf(f_C, "Total number of all operations charged to all edges: %d\n", edge_cost);
	fprintf(f_C, "Total number of operations: %d\n", total_cost);

	printf("Articulation Point count : %d\n", count_AP);	
	printf("Brigde Count: %d\n", bridge_count);
	printf("Biconnected Component Count : %d \n", biconnected_count);
	//printf("Edge Count : %d \n", edge_count);
	
	printf("vertex_cost : %d\n", vertex_cost);
	printf("edge_cost : %d\n",edge_cost);	
	printf("Total cost:%d\n",total_cost);

	fclose(ifp);
	fclose(f_Aa);
	fclose(f_Ac);
	fclose(f_Ab);
}
