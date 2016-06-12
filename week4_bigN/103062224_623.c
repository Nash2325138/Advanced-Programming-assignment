#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LEN 3000
int answer[1001][MAX_LEN];

int main()
{
    int i, j, k;
    memset(answer, 0, sizeof(int)*1001*MAX_LEN);
    answer[0][0] = 1;

    for(i=1 ; i<=999 ; i++){
        int num[4];
        num[0] = i%10;
        num[1] = (i/10)%10;
        num[2] = (i/100)%10;
        num[3] = 0;

        for(j=0 ; j<MAX_LEN ; j++){
            if(answer[i-1][j] == 0) continue;
            int carry = 0;
            for(k=0 ; k<4 ; k++){
                if(j+k >= MAX_LEN) continue;
                int temp = answer[i][j+k] + answer[i-1][j]*num[k] + carry;
                answer[i][j+k] = temp%10;
                carry = temp/10;
            }
        }
    }
    answer[1000][0] = answer[1000][1] = answer[1000][2] = 0;
    for(i=3 ; i<MAX_LEN ; i++){
        answer[1000][i] = answer[999][i-3];
    }

    int required;
    while(~scanf("%d", &required)){
        printf("%d!\n", required);
        int flag = 0;
        for(i=MAX_LEN-1 ; i>=0 ; i--){
            int temp = answer[required][i];
            if( flag==0 ){

                if( temp==0 ) continue;
                else flag = 1;
            }
            printf("%d", temp);
        }
        printf("\n");
    }
    return 0;
}
