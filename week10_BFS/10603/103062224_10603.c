#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <assert.h>

typedef struct Candidate {
	int cost;
	int amount[3];
} Candidate;
Candidate cans[40010];
int can_size;
int limit[3];

void pushCan(int *amount, int cost)
{
	cans[can_size].cost = cost;
	memcpy(cans[can_size].amount, amount, sizeof(int) * 3);
	can_size++;
}

/* only to know the first two jugs' volume then we'll know the third one's */
/* so vertex id can be denoted by the first two jugs' volume */
int shortestDistance[201][201];

int extract_min(void)
{
	int i, min = -1, chosen = -1;
	for(i=0 ; i<can_size ; i++) {
		if(shortestDistance[ cans[i].amount[0] ][ cans[i].amount[1] ] != -1) {
			/* discard this determined vertex by swapping with last one and can_size-- */
			memcpy(&cans[i], &cans[can_size-1], sizeof(Candidate));
			can_size--;
			
			/* we need to examine the original last one at next cycle */
			i--;
			continue;
		}
		if(min == -1 || cans[i].cost < min) {
			min = cans[i].cost;
			chosen = i;
		}
	}
	return chosen;
}


/* return the vertex id with the exact desired volume (or the closest smaller one) in one of three jugs */
int* dijkstra(int desired)
{
	int *amount = (int *)malloc(sizeof(int) * 3);
	int closest_diff = 1000; 	/* bigger than 200 is enough */
	int *closest_amount = (int *)malloc(sizeof(int) * 3);
	for( ; ; )
	{
		int next = extract_min();
		/* printf("next: %d, ", next); */
		if(next == -1) break;
		/* printf("extracted Candidate: (%d, %d, %d), cost %d\n", cans[next].amount[0], cans[next].amount[1], cans[next].amount[2], cans[next].cost); */

		memcpy(amount, cans[next].amount, sizeof(int) * 3);

		shortestDistance[ amount[0] ][ amount[1] ] = cans[next].cost;
		int k;
		for(k=0 ; k<3 ; k++) if(amount[k] == desired) {
			free(closest_amount);
			return amount;
		}
		for(k=0 ; k<3 ; k++) {
			if(desired < amount[k]) continue;
			if(desired - amount[k] < closest_diff) {
				closest_diff = desired - amount[k];
				memcpy(closest_amount, amount, sizeof(int) * 3);
			}
		}

		int i, j, cost;
		int newAmmount[3];
		for(i=0 ; i<3 ; i++) {
			for(j=0 ; j<3 ; j++) {
				if(i==j) continue;
				if(amount[i] == limit[i] || amount[j] == 0) continue;
				memcpy(newAmmount, amount, sizeof(int) * 3);
				/* pour j to i */
				cost = cans[next].cost;
				if(amount[i] + amount[j] > limit[i]) {
					cost += limit[i] - amount[i];
					newAmmount[i] = limit[i];
					newAmmount[j] = amount[j] - (limit[i] - amount[i]);
				} else {
					cost += amount[j];
					newAmmount[i] = amount[i] + amount[j];
					newAmmount[j] = 0;
				}
				/* printf("Push: (%d, %d, %d), cost %d\n", newAmmount[0], newAmmount[1], newAmmount[2], cost); */
				pushCan(newAmmount, cost);
			}
		}
	}

	/* if not return in the for loop, then there's no exact desired volume for all vertexs' three jugs */
	/* so we need to find the closest smaller one and return its vertex id */
	free(amount);
	return closest_amount;
}

void initial(void)
{
	can_size = 0;
	int i, j;
	for(i=0 ; i<201 ; i++) {
		for(j=0 ; j<201 ; j++) {
			shortestDistance[i][j] = -1;
		}
	}
}
int main(int argc, char const *argv[])
{
	/*freopen("10603.in", "r", stdin);
	freopen("my.out", "w", stdout);
	*/int testCases;
	scanf("%d", &testCases);
	while(testCases--) 
	{
		initial();
		int desired;
		scanf("%d %d %d %d", &limit[0], &limit[1], &limit[2], &desired);
		int amount[3] = {0, 0, limit[2]};
		pushCan(amount, 0);
		
		/* calculate the shortest path with source vertex: (0) */
		/* and find the vertex id with the exact desired volume (or the closest smaller one) in one of three jugs */
		int *find_amount;
		find_amount = dijkstra(desired);
		int closest_diff = 1000; /* bigger than 200 is enough */
		int i;
		/* printf("desired: %d, find_amount: %d %d %d\n", desired, find_amount[0], find_amount[1], find_amount[2]); */
		for(i=0 ; i<3 ; i++) {
			if(find_amount[i] <=	 desired) {
				int temp = desired - find_amount[i];
				if( temp < closest_diff) closest_diff = temp;
			}
		}
		printf("%d %d\n", shortestDistance[ find_amount[0] ][ find_amount[1] ], desired - closest_diff);

		free(find_amount);
	}
	return 0;
}