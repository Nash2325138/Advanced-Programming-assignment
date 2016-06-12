#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define MAX 35000

int main(int argc, char const *argv[])
{
	char isPrime[MAX];
	int i, j;
	memset(isPrime, 1, sizeof(isPrime));
	isPrime[0] = isPrime[1] = 0;
	for(i=2 ; i<MAX ; i++){
		if(isPrime[i]){
			for(j=i+i ; j<MAX ; j+=i)
				isPrime[j] = 0;
		}
	}

	int primeList[3800];
	int listSize=0;
	for(i=2 ; i<MAX ; i++){
		if(isPrime[i]){
			primeList[listSize++] = i;
			/*printf("%d ", i);*/
		}
	}
	/*printf("%d", listSize);*/

	int next[3510];
	int ans[3510];
	memset(ans, 0, sizeof(ans));
	int input;
	scanf("%d", &input);
	while(input > 0)
	{
		if(ans[input]!=0) {
			printf("%d\n", ans[input]);
			scanf("%d", &input);
			continue;
		}

		for(i=1 ; i<input ; i++) next[i] = i+1;
		next[input] = 1;

		int listCount=0;
		int current=input, remain=input;
		while(remain > 1){
			int toKill = primeList[listCount++];
			toKill = (toKill%remain==0) ? remain-1 : toKill%remain-1;
			for(i=0 ; i<toKill ; i++) {
				current = next[current];
			}
			next[current] = next[next[current]];
			remain--;
		}
		/*assert(next[current]==current);*/
		printf("%d\n", current);
		ans[input] = current;

		scanf("%d", &input);
	}
	return 0;
}