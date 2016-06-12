#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node{
	int id;
	int to;
	struct Node *next;
}Node;
typedef struct Street{
    int from;
    int to;
    int id;
}Street;
struct Node *(adjList)[45];

void insert_after(Node **now, int to, int id);
void print_list(Node *head);
void find_circuit(int root);
void solve_print(int jonnyStart, int street_count);
void print_all_list_remain(void);
void remove_next(Node **current)
{
    if((*current)->next == NULL) return;
    Node *temp = (*current)->next->next;    /* may be a NULL */
    free((*current)->next);
    (*current)->next = temp;
    return;
}
int street_cmp(const void *a, const void *b)
{
    Street *pa = (Street *)a;
    Street *pb = (Street *)b;
    return ( (pa->id < pb->id) - (pa->id > pb->id) );
}

int deleted[2000];
int pathStack[2000];
int ps_size;
void solve_print(int jonnyStart, int street_count)
{
    int i;
    for(i=0 ; i<45 ; i++){
        Node *current = adjList[i];
        int count=0;
        while(current->next != NULL){
            current = current->next;
            count++;
        }
        if(count%2 != 0){
            printf("Round trip does not exist.\n");
            return;
        }
    }

    memset(deleted, 0, sizeof(deleted));
    ps_size = 0;
    find_circuit(jonnyStart);

    if(ps_size < street_count){
        printf("Round trip does not exist.\n");
        return;
    }

    printf("%d", pathStack[ps_size-1]);
    for(i=ps_size-2 ; i>=0 ; i--){
        printf(" %d", pathStack[i]);
    }
    printf("\n");
    return;
}

void find_circuit(int root)
{
    Node *current = adjList[root];
    /*printf("-> %d", root);*/
    while(current->next!=NULL)
    {
        if(deleted[current->next->id]){
            /*printf("\ndelete %d: (%d, %d)", root, current->next->to, current->next->id);*/
            remove_next(&current);
            /*print_all_list_remain();*/
            continue;
        }

        int use_id = current->next->id;
        deleted[use_id] = 1;
        find_circuit(current->next->to);
        pathStack[ps_size++] = use_id;
        /* print_all_list_remain(); */
    }
}

int main()
{
    int from, to, id;
    int jonnyStart;
    struct Street streets[2000];

    /*freopen("302.in", "r", stdin);
    freopen("my.out", "w", stdout);*/
    scanf("%d %d", &from, &to);
    do{
        memset(streets, 0, sizeof(streets));
        int street_count=0;
        jonnyStart = (from < to) ? from : to;
        while(!(from==0 && to==0))
        {
            scanf("%d", &id);
            streets[street_count].from = from;
            streets[street_count].to = to;
            streets[street_count].id = id;
            street_count++;

            scanf("%d %d", &from, &to);
        }
        qsort(streets, street_count, sizeof(streets[0]), street_cmp);

        int i;

        for(i=0 ; i<45 ; i++){
            adjList[i] = (Node *)malloc(sizeof(Node));
            adjList[i]->id = i;
            adjList[i]->to = -1;
            adjList[i]->next = NULL;
        }
        for(i=0 ; i<street_count ; i++){
            /*printf("%d <-> %d : %d\n", streets[i].from, streets[i].to, streets[i].id);*/
            insert_after( &adjList[streets[i].from], streets[i].to, streets[i].id);
            insert_after( &adjList[streets[i].to], streets[i].from, streets[i].id);
        }
        /*print_all_list_remain();*/
        solve_print(jonnyStart, street_count);
        printf("\n");
        scanf("%d %d", &from, &to);
    }while(!(from==0 && to==0));
    return 0;
}

void insert_after(Node **now, int to, int id)
{
	if((*now)==NULL) {
		fprintf(stderr, "Warning: insert_after a NULL pointer\n");
		/*printf("1");*/
		(*now) = (Node *)malloc(sizeof(Node));
		if(now==NULL) printf("what?");
		(*now)->to = to;
		(*now)->id = id;
		(*now)->next = NULL;
		return;
	}

	if((*now)->next==NULL){
		/*printf("2");*/
		(*now)->next = (Node *)malloc(sizeof(Node));
		(*now)->next->to = to;
		(*now)->next->id = id;
		(*now)->next->next = NULL;
		return;
	}

	/*printf("3");*/
	Node *temp = (*now)->next;
	(*now)->next = (Node *)malloc(sizeof(Node));
    (*now)->next->to = to;
	(*now)->next->id = id;
	(*now)->next->next = temp;
	return;
}

void print_list(Node *head)
{
	/*printf("!");
	if(head==NULL) return;
	printf("?");*/
	printf("%d : ", head->id);
	Node *current = head->next;
	while(current!=NULL)
	{
		printf("(%d, %d) ", current->to, current->id);
		current = current->next;
	}
	printf("\n");
	return;
}

void print_all_list_remain(void)
{
    printf("\n");
    int i;
    for(i=0 ; i<45 ; i++){
        if(adjList[i]->next==NULL) continue;
        print_list(adjList[i]);
    }
    printf("\n");
}
