#include <stdio.h>
#include <stdlib.h>

int adj[10];
int need[10];
int in[10];
int out[10];

int vertex[10004];
int next[10004];
int used[10004];
int Top;

int color[10];
int stack[10004];
int stackTop;

void DFS(int v)
{
    color[v] = 1;
    /* empty ? */
    if(adj[v] == -1)
    {
        stack[stackTop++] = v;
        return;
    }

    /* DFS */
    int index;
    for(index = adj[v]; ; index = next[index])
    {
        if(used[index] == 0) /* edge is not used yet */
        {
            used[index] = 1;
            DFS(vertex[index]);
        }
        if(next[index] == -1)   break;
    }

    /* Push */
    stack[stackTop++] = v;
    return;
}

void addEdge(int from,int to)
{
    /* Maybe multi edge */

    /* Empty ? */
    if(adj[from] == -1)
    {
        adj[from] = Top;
        vertex[Top] = to;
        next[Top] = -1;
        Top++;
    }
    else
    {
        int prev = adj[from];
        adj[from] = Top;
        vertex[Top] = to;
        next[Top] = prev;
        Top++;
    }

    /* Degree */
    out[from]++;
    in[to]++;
}

int main()
{
    int m,n;
    int i,j;
    while(scanf("%d %d",&m,&n) != EOF)
    {
        /* Reset */
        Top = 0;
        for(i=0 ; i<10 ; i++)
        {
            need[i] = adj[i] = -1; /* Not exist */
            in[i] = out[i] = 0;
        }

        /* Make a directed graph */
        for(i=m ; i<=n ; i++)
        {
            need[i%10] = 1; /* Node exist */
            if( i/10 > 0)/* Need an edge ? */
            {
                need[i/10] = 1;/* Node exist */
                addEdge(i/10, i%10); /* Directed graph , no need to add edge in both way */
            }
        }

        /*
        int index;
        for(i=0 ; i<10 ; i++)
        {
            if(need[i] != -1)
            {
                printf("Vertex %d in-degree %d, out-degree %d, neighbors: " ,i,in[i],out[i] );
                if(adj[i] != -1)
                {
                    for(index = adj[i] ; ; index = next[index])
                    {
                        printf(" %d", vertex[index]);
                        if(next[index] == -1)   break;
                    }
                }
                puts("");
            }
        }*/

        /* Add edge, make the graph Eulerian path */
        int a[10]; /* vertex that in > out */
        int b[10]; /* vertex that out > in */
        int aTop = -1, bTop = -1;
        for(i=0 ; i<10 ; i++)
        {
            if(in[i] > out[i])  a[++aTop] = i;
            if(in[i] < out[i])  b[++bTop] = i;
        }

        int aIndex = 0, bIndex = 0;
        if( (aTop > -1) && (bTop > -1) ) /* Not empty */
        {
            while(1)
            {
                /* This vertex is done, next edge */
                if(in[a[aIndex]] == out[a[aIndex]]) aIndex++;
                if(in[b[bIndex]] == out[b[bIndex]]) bIndex++;

                if(aIndex == aTop && bIndex == bTop)
                    if( (in[a[aIndex]] - out[a[aIndex]]) == 1)
                        break;

                /* Add edge */
                addEdge(a[aIndex] , b[bIndex]);
            }
        }

        /*puts("\nAfter adding edge");
        for(i=0 ; i<10 ; i++)
        {
            if(need[i] != -1)
            {
                printf("Vertex %d in-degree %d, out-degree %d, neighbors: " ,i,in[i],out[i] );
                if(adj[i] != -1)
                {
                    for(index = adj[i] ; ; index = next[index])
                    {
                        printf(" %d", vertex[index]);
                        if(next[index] == -1)   break;
                    }
                }
                puts("");
            }
        }*/

        /* Find Circuit */
        /* reset*/
        for(i=0 ; i<10 ; i++)
        {
            if(need[i] != -1)   color[i] = 0;
            else color[i] = 1; /* Not exist */
        }
        stackTop = 0;/* empty stack */
        for(i=0 ; i<Top ; i++)  used[i] = 0;/* All edges not used */

        /* Find the start of Eulerian path */
        for(i=0 ; i<10 ; i++)
            if(color[i] == 0 && (out[i] - in[i] == 1))
                DFS(i);
        /* Remain of that is not in the path or a Eulerian circuit*/
        for(i=0 ; i<10 ; i++)
            if(color[i] == 0)
                DFS(i);

        for(i=stackTop-1 ; i>=0 ; i--)
            printf("%d",stack[i]);
        puts("");
    }
    return 0;
}
