#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/*#define FILE_INPUT*/
/* !!!! no one line comment !!!! */
int p, q, r, s, t, u;
double func(double x);

int main()
{
    #ifdef FILE_INPUT
    freopen("10341.in" , "r" , stdin);
    #endif

    double upper, lower;
    double middle;


    while(~scanf("%d %d %d %d %d %d", &p , &q , &r , &s , &t , &u))
    {
        if(func(0)*func(1)>0){
            printf("No solution\n");
            continue;
        }

        lower=0;
        upper=1;
        while(upper-lower >= 0.000000001)
        {
            middle = (lower+upper)/2;
            double ans = func(middle);
            if (ans==0) upper = lower = middle;
            else if(ans>0) lower = middle;
            else upper = middle;
        }

        printf("%.4f\n", upper);
    }

    return 0;
}

double func(double x)
{
    return p*exp((-1)*x) + q*sin(x) + r*cos(x) + s*tan(x) + t*x*x + u;
}
