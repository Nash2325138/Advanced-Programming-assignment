#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SQRT_MAX 65536

char isPrime[SQRT_MAX+1];
char bigIsPrime[1000001];
int primeList[6555];
int primeListTop;
int main()
{
    unsigned int i, j, k;
    memset(isPrime, 1, sizeof(isPrime));
    for(i=2 ; i<=256 ; i++){
        if(isPrime[i]==1){
            for(j=i*i ; j<=SQRT_MAX ; j+=i) isPrime[j] = 0;
        }
    }
    primeListTop = 0;
    for(i=2; i<=SQRT_MAX ; i++){
        if(isPrime[i]==1) primeList[primeListTop++] = i;
    }
    /*printf("%d", primeListTop);*/

    unsigned int lower, upper;
    while(scanf("%d %d", &lower, &upper)==2)
    {
        memset(bigIsPrime, 1, sizeof(bigIsPrime));
        if(lower==1) bigIsPrime[0] = 0;
        for(i=0 ; i<primeListTop ; i++){
            int p = primeList[i];
            j = p* ( lower/p + ((lower%p==0)? 0:1) );
            if(j==p) j+=p;
            for( ; j<= upper ; j+=p){

                /*printf("p==%d, j==%d\n", p, j);*/
                bigIsPrime[j-lower] = 0;
            }
        }

        int maxDistant = -1, minDistant = -1;
        int maxLeft = -1 , maxRight = -1;
        int minLeft = -1 , minRight = -1;
        int last = -1;
        for(i=lower ; i<=upper ; i++){
            if(bigIsPrime[i-lower]!=1) continue;
            if(last==-1)last = i;
            else{
                if(maxDistant==-1 || i-last > maxDistant){
                    maxDistant = i-last;
                    maxLeft = last;
                    maxRight = i;
                }
                if(minDistant==-1 || i-last < minDistant){
                    minDistant = i-last;
                    minLeft = last;
                    minRight = i;
                }
                last = i;
            }
        }
        if(maxDistant==-1)
            printf("There are no adjacent primes.\n");
        else
            printf("%d,%d are closest, %d,%d are most distant.\n", minLeft, minRight, maxLeft, maxRight);
    }

    return 0;
}
