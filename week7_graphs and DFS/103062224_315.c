#include <stdlib.h>
#include <stdio.h>

typedef struct Node{
	int id;
	struct Node *next;
}Node;
void insert_after(Node **now, int id);
void print_list(int listId, Node *head);

struct Node * (adjList)[110];
int visited[110];
void DFS(int root, int toIgnore)
{
	if(visited[root]==1) return;
	if(root==toIgnore) return;		/* not neccesary */
	visited[root] = 1;
	/*printf("-> %d", root);*/

	Node *current = adjList[root];
	while(current!=NULL)
	{
		DFS(current->id, toIgnore);
		current = current->next;
	}
	return;
}

int main(int argc, char const *argv[])
{
	/* code */
	int i;

	int places;
	scanf("%d", &places);
	while(places > 0)
	{
		for(i=0 ; i<110 ; i++) adjList[i] = NULL;
		
		int from;
		scanf("%d",& from);
		while(from > 0){
			/*printf("\nfrom==%d, ", from);*/
			int to;
			while(getchar()!='\n'){
				scanf("%d", &to);
				insert_after(&adjList[from], to);
				insert_after(&adjList[to], from);
				/*printf("%d ", to);*/
			}
			scanf("%d", &from);
		}

		/*for(i=1 ; i<=places ; i++) print_list(i, adjList[i]);*/
		
		int critical = 0;
		int toIgnore;

		for(toIgnore=1 ; toIgnore<=places ; toIgnore++)
		{
			for(i=1 ; i<=places ; i++) visited[i] = 0; 	/* initialize */
			visited[toIgnore] = 1;
			
			if(toIgnore==1)	DFS(2, toIgnore);
			else			DFS(1, toIgnore);

			for(i=1 ; i<=places ; i++){					/* is there any not connected node? */
				if(visited[i]==0){
					critical++;
					break;
				}
			}
			/*printf("\n");*/
		
		}
		printf("%d\n", critical);
		scanf("%d", &places);
	}
	return 0;
}

void insert_after(Node **now, int id)
{
	if((*now)==NULL) {
		/*fprintf(stdout, "Warning: insert_after a NULL pointer\n");*/
		/*printf("1");*/
		(*now) = (Node *)malloc(sizeof(Node));
		if(now==NULL) printf("what?");
		(*now)->id = id;
		(*now)->next = NULL;
		return;
	}

	if((*now)->next==NULL){
		/*printf("2");*/
		(*now)->next = (Node *)malloc(sizeof(Node));
		(*now)->next->id = id;
		(*now)->next->next = NULL;
		return;
	}

	/*printf("3");*/
	Node *temp = (*now)->next;
	(*now)->next = (Node *)malloc(sizeof(Node));
	(*now)->next->id = id;
	(*now)->next->next = temp;
	return;
}

void print_list(int listId, Node *head)
{
	/*printf("!");
	if(head==NULL) return;
	printf("?");*/
	printf("%d : ", listId);
	Node *current = head;
	while(current!=NULL)
	{
		printf("%d ", current->id);
		current = current->next;
	}
	printf("\n");
	return;
}