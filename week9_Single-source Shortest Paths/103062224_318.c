#include <stdio.h>
#include <stdlib.h>
typedef struct Row{
    int to;
    int seconds;
}Row;
Row rows[510][510];
int rSize[510];

typedef struct Edge{
    int endPoint1, endPoint2;
    int seconds;
    int falling_time_2;
}Edge;

Edge edges[250000/2 +1];
int edges_num;

int shortest_distance[510];
int determined[510];

void initial(int num_key, int num_row);
void dijkstra(int num_key, int num_row);
int find_next_shortest(int num_key);
void calc_edges_falling(void)
{
    int i;
    for( i=0 ; i<edges_num ; i++){
        int end1 = edges[i].endPoint1;
        int end2 = edges[i].endPoint2;
        if( shortest_distance[end1] < shortest_distance[end2] ){
            int temp = end2;
            end2 = end1;
            end1 = temp;
        }
        int diff = shortest_distance[end1] - shortest_distance[end2];
        if( diff >= edges[i].seconds) {
            edges[i].falling_time_2 = (shortest_distance[end2] + edges[i].seconds)*2;
        } else {
            edges[i].falling_time_2 = shortest_distance[end1]*2 + (edges[i].seconds - diff) ;
        }
        /*printf("edge(%d, %d : %d): %.1f\n", end1, end2, edges[i].seconds, edges[i].falling_time);*/
    }
}

int main()
{
    int num_key, num_row;
    int system_count = 0;
    /*freopen("318.in", "r", stdin);
    freopen("my.out", "w", stdout);*/
    scanf("%d %d", &num_key, &num_row);
    while( !(num_key==0 && num_row==0) )
    {
        system_count ++;

        initial(num_key, num_row);
        dijkstra(num_key, num_row);
        /*int i; for(i=1 ; i<=num_key ; i++) printf("%d : %d\n", i, shortest_distance[i]);*/
        calc_edges_falling();

        int max_2 = -1;
        int chosen_key;
        int chosen_edge = -1;
        int i;
        for(i=1 ; i<=num_key ; i++){
            if(shortest_distance[i]*2 > max_2){
                max_2 = shortest_distance[i]*2;
                chosen_key = i;
            }
        }
        for(i=0 ; i<edges_num ; i++){
            if(edges[i].falling_time_2 > max_2){
                max_2 = edges[i].falling_time_2;
                chosen_edge = i;
            }
        }
        printf("System #%d\n", system_count);
        if(chosen_edge==-1){
            printf("The last domino falls after %.1f seconds, at key domino %d.\n", (float)max_2/2, chosen_key);
        } else {
            printf("The last domino falls after %.1f seconds, between key dominoes %d and %d.\n",
                   (float)edges[chosen_edge].falling_time_2/2,
                   edges[chosen_edge].endPoint1,
                   edges[chosen_edge].endPoint2 );
        }
        printf("\n");

        scanf("%d %d", &num_key, &num_row);
    }
    return 0;
}
void initial(int num_key, int num_row)
{
    int i;
    edges_num = 0;
    for(i=0 ; i<=num_key ; i++){
        rSize[i] = 0;
        determined[i] = 0;
        shortest_distance[i] = -1;
    }

    int from, to, seconds;
    for(i=0 ; i<num_row ; i++){
        scanf("%d %d %d", &from, &to, &seconds);

        rows[from][rSize[from]].to = to;
        rows[from][rSize[from]].seconds = seconds;
        rSize[from]++;

        rows[to][rSize[to]].to = from;
        rows[to][rSize[to]].seconds = seconds;
        rSize[to]++;

        edges[edges_num].endPoint1 = from;
        edges[edges_num].endPoint2 = to;
        edges[edges_num].seconds = seconds;
        edges_num++;
    }
    shortest_distance[1] = 0;
    determined[1] = 1;

    for(i=0 ; i<rSize[1] ; i++){
        int neighbor = rows[1][i].to;
        shortest_distance[neighbor] = rows[1][i].seconds;
    }
}
void dijkstra(int num_key, int num_row)
{
    int i, j, next;
    for(i=0 ; i<num_key ; i++){
        if( (next = find_next_shortest(num_key)) == -1 ) return;
        determined[next] = 1;

        /*printf("next==%d (%d): ",next, shortest_distance[next]);*/
        for(j=0 ; j<rSize[next] ; j++){
            int neighbor = rows[next][j].to;
            /*printf("(%d, %d) ", neighbor, rows[next][j].seconds);*/
            if( !determined[neighbor] ){
                int new_distance = shortest_distance[next] + rows[next][j].seconds;
                if(shortest_distance[neighbor] > new_distance || shortest_distance[neighbor]==-1){
                    shortest_distance[neighbor] = new_distance;
                }
            }
        }
    }
}
int find_next_shortest(int num_key)
{
    int i, min = -1, chosen = -1;
    /*printf("next scanning ( candidate , shortest_distance[candidate] )");*/
    for(i=1 ; i<=num_key ; i++){
        if( shortest_distance[i]==-1 ) continue;
        /*printf("\n(%d, %d) ", i, shortest_distance[i]);*/
        if( determined[i] ) continue;
        /*printf("!");*/
        if( shortest_distance[i] < min || min==-1 ){
            chosen = i;
            min = shortest_distance[chosen];
        }
    }
    /*printf("\n\n");*/
    return chosen;
}
