#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*#include <assert.h>*/

/* int adjMatrix[30][30];*/

char names[30][30];
int connected[30];
int names_size;
int name_to_id(char *name)
{
    /*assert(names_size <= 25);*/
    int i;
    for(i=0 ; i<names_size ; i++) {
        if(strcmp(names[i], name) == 0) {
            return i;
        }
    }
    strcpy(names[names_size++], name);
    return names_size-1;
}

int all_pairs[30][30];
void floyd_warshall(int num_people)
{
    int i, j, k;
    for(k=0 ; k<num_people ; k++) {
        for(i=0 ; i<num_people ; i++) {
            for(j=0 ; j<num_people ; j++) {
                if(all_pairs[i][k] == -1 || all_pairs[k][j] == -1 ) continue;
                if( all_pairs[i][j] < 0) {
                    all_pairs[i][j] = all_pairs[i][k] + all_pairs[k][j];
                } else {
                    int new_distance = all_pairs[i][k] + all_pairs[k][j];
                    if(all_pairs[i][j] > new_distance) all_pairs[i][j] = new_distance;
                }
            }
        }
    }
}

void initial()
{
    names_size = 0;
    int i, j;
    for(i=0 ; i<30 ; i++) {
        connected[i] = 0;
        for(j=0 ; j<30 ; j++) {
            /*adjMatrix[i][j] = 0;*/
            all_pairs[i][j] = -1;
        }
    }
}
int main()
{
    int set_count = 0;
    int num_people, num_line;
    scanf("%d %d", &num_people, &num_line);
    while(num_people != 0 || num_line != 0)
    {
        set_count++;
        initial();

        int i;
        char from[30];
        char to[30];
        for(i=0 ; i<num_line ; i++) {
            scanf("%s %s", from, to);
            /*int from_id = name_to_id(from);
            int to_id = name_to_id(to);
            adjMatrix[ from_id ][ to_id ] = 1;*/
            all_pairs[ name_to_id(from) ][ name_to_id(to) ] = 1;
        }

        floyd_warshall(num_people);

        printf("Calling circles for data set %d:\n", set_count);
        int j;
        for(i=0 ; i<num_people ; i++) {
            if (connected[i]) continue;
            connected[i] = 1;
            printf("%s", names[i]);
            for(j=i+1 ; j<num_people ; j++) {
                if(all_pairs[i][j] != -1 && all_pairs[j][i] != -1) {
                    connected[j] = 1;
                    printf(", %s", names[j]);
                }
            }
            printf("\n");
        }

        scanf("%d %d", &num_people, &num_line);
    }
    return 0;
}
