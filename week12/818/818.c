#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
int opened[16];
int linkLimit;

int minOpen;
int numOpenedNow;

int adjMatrix[16][16];
char visited[16];
char isCycled;

void graphDFS(int root)
{
	if(visited[root]) {
		isCycled = 1;
		return;
	}
	visited[root] = 1;
	int i;
	for(i=1 ; i<=linkLimit ; i++) {
		if(opened[i] == 1) continue;
		if(root == i) continue;
		if(adjMatrix[root][i] == 1) {
			adjMatrix[i][root] = 0;
			graphDFS(i);
			adjMatrix[i][root] = 1;
		}
	}
}

int countDegree(int root) {
	int i, count = 0;
	for(i=1 ; i<=linkLimit ; i++) {
		if(opened[i] == 1) continue;
		if(adjMatrix[root][i] == 1) count++;
	}
	return count;
}

int test(void)
{
	int i;
	for(i=1 ; i<=linkLimit ; i++) {
		if( opened[i] == 0 ) {
			if(countDegree(i) > 2) return 0;
		}
		visited[i] = 0;
	}
	int numComponent = 0;
	isCycled = 0;
	for(i=1 ; i<=linkLimit ; i++) {
		if( opened[i] == 1) continue;
		if( visited[i] == 0 ) {
			numComponent++;
			graphDFS(i);
			if(isCycled == 1) {
				return 0;
			}
		} else continue;
	}
	if(numComponent > numOpenedNow + 1) return 0;

	return 1;
}

void choiceDFS(int toOpen)
{
	if(numOpenedNow >= minOpen) return;
	if(toOpen == linkLimit + 1) {
		if(test() == 1) {
			if(numOpenedNow < minOpen) minOpen = numOpenedNow;
			return;
		}
		else return;
	}

	opened[toOpen] = 0;
	choiceDFS(toOpen+1);
	opened[toOpen] = 1;
	numOpenedNow++;
	choiceDFS(toOpen+1);
	numOpenedNow--;
}

void initial(void)
{
	minOpen = 20;
	int i, j;
	for(i=1 ; i<=linkLimit ; i++) {
		opened[i] = 0;
		for(j=1 ; j<=linkLimit ; j++) {
			adjMatrix[i][j] = 0;
		}
	}
	numOpenedNow = 0;
}

int main(int argc, char const *argv[])
{
	int testCases = 0;
	scanf("%d", &linkLimit);
	while(linkLimit != 0)
	{
		testCases++;
		initial();
		int from, to;
		scanf("%d %d", &from, &to);
		while(from != -1 && to != -1) {
			adjMatrix[from][to] = adjMatrix[to][from] = 1;
			scanf("%d %d", &from, &to);
		}
		choiceDFS(1);
		printf("Set %d: Minimum links to open is %d\n", testCases, minOpen);

		scanf("%d", &linkLimit);
	}
	return 0;
}