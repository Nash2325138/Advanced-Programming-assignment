#include <stdio.h>
#include <stdlib.h>

typedef struct Vertex
{
	int exist;
	int inDegree;
	int outDegree;
	int adjList[20];
	int adjListAvailable[20];
	int adjSize;

	int visited;
}Vertex;

/* from 0 to 9 */
Vertex v[10];

int pathStack[200];
int ps_size;
void DFS(int vertexID)
{
	/*printf("vertexID: %d\n", vertexID);*/
	int i;
	for(i=0 ; i<v[vertexID].adjSize ; i++) {
		if(v[vertexID].adjListAvailable[i] == 0) continue;

		v[vertexID].adjListAvailable[i] = 0;
		DFS(v[vertexID].adjList[i]);
	}
	v[vertexID].visited = 1;
	pathStack[ps_size++] = vertexID;
}

void initial(void)
{
	int i;
	for(i=0 ; i<10 ; i++)
		{
		v[i].exist = v[i].visited = 0;
		v[i].adjSize = v[i].inDegree = v[i].outDegree = 0;
	}
	ps_size = 0;
}
void addEdge(int from, int to)
{
	v[from].adjList[v[from].adjSize] = to;
	v[from].adjListAvailable[v[from].adjSize] = 1;
	v[from].adjSize++;
	v[from].outDegree++;

	v[to].inDegree++;
}

void showGraph(void)
{
	int i;
	for(i=0 ; i<10 ; i++)
	{
		if(v[i].exist == 0) continue;
		printf("v[%d].inDegree: %d, outDegree: %d, adjList: ", i, v[i].inDegree, v[i].outDegree);
		int j;
		for(j=0 ; j < v[i].adjSize ; j++) {
			printf(" %d", v[i].adjList[j]);
		}
		printf("\n");
	}
}



int main(int argc, char const *argv[])
{
	setbuf(stdout, NULL);
	/* left close, right close */
	int lower, upper;
	while (scanf("%d %d", &lower, &upper) != EOF )
	{
		initial();
		int i;
		for (i=lower ; i<=upper ; i++) {
			if( i < 10 ) {
				v[i].exist = 1;
			} else {
				v[i/10].exist = v[i%10].exist = 1;
				addEdge(i/10, i%10);
			}
		}
		/*showGraph();*/


		/* classify into two groups. One group's member has inDegree > outDegree. Another group has outDegree > inDegree */
		/* we don't care those whose inDegree == outDegree. They don't need to be modified */
		int moreIn[10];
		int mI_size = 0;
		int moreOut[10];
		int mO_size = 0;

		for(i=0 ; i<10 ; i++) {
			if(v[i].exist == 0) continue;
			if(v[i].inDegree > v[i].outDegree) moreIn[mI_size++] = i;
			else if(v[i].inDegree < v[i].outDegree) moreOut[mO_size++] = i;
		}

		/* addEdge to make Euler's path */
		
		if(mI_size > 0 && mO_size > 0)
		{	
			int mI_iter = 0, mO_iter = 0;
			int target;
			while(1)
			{
				if( mI_iter >= mI_size-1 && mO_iter >= mO_size-1 ) {
					target = moreIn[mI_size-1];
					if(v[target].inDegree - v[target].outDegree == 1) /* it will implies v[moreOut[mO_size]]'s outDegree - inDgree == 1 */
					{
						break;
					}
				}
				addEdge( moreIn[mI_iter] , moreOut[mO_iter]);

				target = moreIn[mI_iter];
				if(v[target].inDegree == v[target].outDegree) mI_iter++;
				target = moreOut[mO_iter];
				if(v[target].inDegree == v[target].outDegree) mO_iter++;
			}
		}
		/*showGraph();*/
		for(i=0 ; i<10 ; i++) {
			if(v[i].exist == 1){
				if(v[i].outDegree == v[i].inDegree + 1) DFS(i);
			}
		}
		for(i=0 ; i<10 ; i++) {
			if(v[i].exist && v[i].visited == 0) DFS(i); 
		}

		/*printf("%d", ps_size);*/
		for(i=ps_size-1 ; i>=0 ; i--) printf("%d", pathStack[i]);
		printf("\n");
	}
	return 0;
}