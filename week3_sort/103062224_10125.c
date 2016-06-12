#include <stdio.h>
#include <stdlib.h>

typedef struct Sum
{
    long a, b;
    long sum;
}Sum;
Sum X[1000000];
int index_X[1000000];

int cmp_int(const void *a, const void *b)
{
    long c = *((long*)a);
    long d = *((long*)b);
    return (c>d) - (c<d);
}

int cmp_Sum(const void *a, const void *b)
{
    int ia = *((int*)a);
    int ib = *((int*)b);
    return( X[ia].sum > X[ib].sum ) - ( X[ia].sum < X[ib].sum );
}

int overlap(Sum *one, int c, int d)
{
    if(one->a==c || one->a==d || one->b==c || one->b==d )return 1;
    return 0;
}

int main()
{
    int size;
    scanf("%d", &size);

    while(size!=0)
    {
        long input[1100];

        int i, j;
        for(i=0 ; i<size ; i++)scanf("%ld", &input[i]);
        qsort(input, size, sizeof(long), cmp_int);

        /*for(i=0 ; i<size; i++) printf("%ld ", input[i]);
        printf("\n");*/

        int count=0;
        for(i=0 ; i<size-1 ; i++){
            for(j=i+1 ; j<size ; j++){
                X[count].a = input[i];
                X[count].b = input[j];
                X[count].sum = input[i]+input[j];
                count++;
            }
        }

        for(i=0 ; i<count ; i++) index_X[i] = i;
        qsort(index_X, count, sizeof(int), cmp_Sum);
        /*for(i=0 ; i<count ; i++) printf("%d %d %ld\n", X[index_X[i]].a , X[index_X[i]].b , X[index_X[i]].sum);
        printf("\n");*/

        int find = 0;
        long ans;
        for(i=size-1 ; i>=0 ; i--)
        {
            for(j=size-1 ; j>=0 ; j--)
            {
                if(i==j) continue;
                long Y = input[i]-input[j];
                /*printf("input[i]==%d input[j]==%d\n", input[i], input[j]);*/
                int lower=0 , upper=count;
                while(lower+1<upper)
                {
                    /*printf("%d %d\n", lower, upper);*/
                    int middle = (upper+lower)/2;
                    long target = X[index_X[middle]].sum;
                    if( target > Y ) upper = middle;
                    else if( target < Y ) lower = middle;
                    else upper = lower = middle;
                }
                if( X[index_X[lower]].sum != Y ) continue;

                int shot = lower;
                int k=0;
                /*printf("X[index[shot].sum==%d shot==%d Y==%ld\n", X[index_X[shot]].sum, shot, Y);*/

                /*for(k=-2 ; k<3 ; k++){
                    printf("%ld ", X[index_X[shot+k]].sum);
                }
                printf("\n");*/

                for(k=-2; k<3; k++){
                    if(shot+k < 0 || shot+k>=count ) continue;
                    if(X[index_X[shot+k]].sum!=Y) continue;

                    if(overlap( &(X[index_X[shot+k]]), input[i], input[j] )==0){
                        find = 1;
                        ans = input[i];
                        break;
                    }
                }
                if(find) break;
            }
            if(find) break;
        }

        if(!find) printf("no solution\n");
        else printf("%ld\n", ans);

        scanf("%d", &size);
    }

    return 0;
}
