#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <assert.h>

#define MAX_RANGE 10001
#define MAX_UPPER 1000000000

char isPrime[40001];
int primeList[4300];
int primeListTop;

/*int plist[MAX_RANGE][4300];
int ppow[MAX_RANGE][4300];*/
int remain[MAX_RANGE];
int num_divisor[MAX_RANGE];

int main()
{
    int i, j;
    memset(isPrime, 1, sizeof(isPrime));
    for(i=2; i<=200 ; i++){
        if(isPrime[i]==1){
            for(j=i*i ; j<=40000 ; j+=i) isPrime[j] = 0;
        }
    }

    primeListTop = 0;
    for(i=2; i<40001 ; i++) {
        if(isPrime[i]==1) primeList[primeListTop++] = i;
    }
    /*for(i=0 ; i<primeListTop; i++){
        printf("%d ", primeList[i]);
        if(primeList[i]==0) exit(8888);
    }
      primeListTop == 4203*/


    int testCases;
    scanf("%d", &testCases);
    while(testCases--)
    {
        int lower, upper;
        /*memset(plist, 0, sizeof(plist));
        memset(ppow, 0, sizeof(ppow));*/

        /*printf("%d", primeListTop);*/
        scanf("%d %d", &lower , &upper);
        for(i=0 ; i<upper-lower+1 ; i++){
            remain[i] = lower+i;
            num_divisor[i] = 1;
        }
        for(i=0 ; i<primeListTop ; i++){
            int p = primeList[i];
            /*printf("now i==%d, primeList[%d]==%d, p==%d\n", i, i, primeList[i], p);*/
            j = p*( lower/p + ((lower%p==0) ? 0:1) );
            /*printf("j==%d\n", j);*/
            for(; j<=upper ; j+=p)
            {
                int powerOfp = 0;
                do{
                    powerOfp++;

                    assert(j-lower >= 0);
                    assert(j-lower < MAX_RANGE);

                    remain[j-lower] /= p;
                }while(remain[j-lower]%p==0 && remain[j-lower]>1);
                num_divisor[j-lower] *= (1+powerOfp);
            }
        }

        int max_num_divisor=0;
        int that_number;
        for(i=0 ; i<upper-lower+1 ; i++){
            if(remain[i]>1) num_divisor[i] *= 2;

            if(num_divisor[i] > max_num_divisor){
                max_num_divisor = num_divisor[i];
                that_number = i+lower;
            }
        }
        printf("Between %d and %d, %d has a maximum of %d divisors.\n", lower, upper, that_number, max_num_divisor);
    }
    return 0;
}
