#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
/*#define DEBUG*/
#if defined DEBUG
#define debug(fmt, args...) printf(fmt, ##args)
#else
#define debug(fmt, args...)
#endif
#define MAXLEN 24

const char end[] = {0, 3, 4, 3, 0, 5, 6, 5, 0, 1, 2, 1, 0, 7, 8, 7, 0, 9, 10, 9, 0, 1, 2, 1};
void print_content(char *content)
{
	int i;
	for(i=0 ; i<MAXLEN ; i++) printf("%hhd ", content[i]);
	printf("\n");
}
typedef struct State {
	char content[MAXLEN];
	char process[20];
	int step;
}State;
/* node of binary search tree */
typedef struct Node {
	struct State *state;
	struct Node *parent;
	struct Node *left;
	struct Node *right;
}Node;
Node *startRoot;
Node *endRoot;
void addBST(Node **root, State *toAdd)
{
	if((*root) == NULL) {
		(*root) = (Node *)malloc(sizeof(Node));
		(*root)->state = toAdd;
		(*root)->left = (*root)->right = NULL;
		(*root)->parent = NULL;
		return;
	}
	Node *current = *root;
	Node *last;
	for( ; ; ) {
		last = current;
		int result = memcmp(current->state->content, toAdd->content, sizeof(char) * MAXLEN);
		if( result < 0 ) {
			if(current->left == NULL) {
				current->left = (Node*)malloc(sizeof(Node));
				current = current->left;

				current->state = toAdd;
				current->left = current->right = NULL;
				current->parent = last;
				return;
			}
			else {
				current = current->left;
			}
		}
		else {
			if(current->right == NULL) {
				current->right = (Node*)malloc(sizeof(Node));
				current = current->right;

				current->state = toAdd;
				current->left = current->right = NULL;
				current->parent = last;
				return;
			}
			else {
				current = current->right;
			}
		}
	}
}
void bruteFind(Node *root, char *tested, int *isFind, State **store)
{
	if(root == NULL) return;
	if(*isFind == 1) return;
	if(memcmp(tested, root->state->content, sizeof(char) * MAXLEN) == 0){
		*isFind = 1;
		if(store != NULL) (*store) = root->state;
		return;
	} 
	bruteFind(root->left, tested, isFind, store);
	bruteFind(root->right, tested, isFind, store);
}
int hasNode(Node *root, char *tested, State **store)
{
	/*if(0) {
		int isFind = 0;
		bruteFind(root, tested, &isFind, store);
		return isFind;
	}*/
	Node *current = root;
	for( ; ; )
	{
		if(current == NULL) return 0;
		int result = memcmp(current->state->content, tested, sizeof(char) * MAXLEN);
		if (result == 0) {
			if( store != NULL ) (*store) = current->state;
			return 1;
		}
		else if (result < 0)
			current = current->left;
		else
			current = current->right;
	}
	return 0;
}

State *Qstart[655550];
int startHead, startTail;

State *Qend[655550];
int endHead, endTail;

void initial(void)
{
	startRoot = NULL;
	startHead = startTail = 0;
}

void leftClock(char *from, char *to)
{
	int i;
	for(i=2 ; i<12 ; i++) to[i] = from[i-2];
	to[0] = from[10];
	to[1] = from[11];
	for(i=12 ; i<21 ; i++) to[i] = from[i];
	to[21] = to[9];
	to[22] = to[10];
	to[23] = to[11];
}
void leftCounter(char *from, char *to)
{
	int i;
	for(i=0 ; i<10 ; i++) to[i] = from[i+2];
	to[10] = from[0];
	to[11] = from[1];
	for(i=12 ; i<21 ; i++) to[i] = from[i];
	to[21] = to[9];
	to[22] = to[10];
	to[23] = to[11];
}
void rightClock(char *from, char *to)
{
	int i;
	for(i=12 ; i<22 ; i++) to[i] = from[i+2];
	to[22] = from[12];
	to[23] = from[13];
	for(i=0 ; i<9 ; i++) to[i] = from[i];
	to[9] = to[21];
	to[10] = to[22];
	to[11] = to[23];
}
void rightCounter(char *from, char *to)
{
	int i;
	for(i=14 ; i<24 ; i++) to[i] = from[i-2];
	to[12] = from[22];
	to[13] = from[23];
	for(i=0 ; i<9 ; i++) to[i] = from[i];
	to[9] = to[21];
	to[10] = to[22];
	to[11] = to[23];
}
int compare(const char *s1, const char *s2)
{
	int i;
	for(i=0 ; i<MAXLEN ; i++) {
		if(s1[i] != s2[i]) return 0;
	}
	return 1;
}

void QstartAdd_4way(State *now)
{
	State *next;
					/* 1: left clock */
					next = (State *) malloc(sizeof(State));
					strncpy(next->process, now->process, now->step);
					next->process[now->step] = 1;
					next->step = now->step+1;
					leftClock(now->content, next->content);
					if( hasNode(startRoot, next->content, NULL) ) {
						free(next);
					}
					else {
						addBST(&startRoot, next);
						Qstart[startTail++] = next;
					}

					/* 2: right clock */
					next = (State *) malloc(sizeof(State));
					strncpy(next->process, now->process, now->step);
					next->process[now->step] = 2;
					next->step = now->step+1;
					rightClock(now->content, next->content);
					if( hasNode(startRoot, next->content, NULL) ) {
						free(next);
					}
					else {
						addBST(&startRoot, next);
						Qstart[startTail++] = next;
					}

					/* 3: left counter-clock */
					next = (State *) malloc(sizeof(State));
					strncpy(next->process, now->process, now->step);
					next->process[now->step] = 3;
					next->step = now->step+1;
					leftCounter(now->content, next->content);
					if( hasNode(startRoot, next->content, NULL) ) {
						free(next);
					}
					else {
						addBST(&startRoot, next);
						Qstart[startTail++] = next;
					}

					/* 4: right counter-clock */
					next = (State *) malloc(sizeof(State));
					strncpy(next->process, now->process, now->step);
					next->process[now->step] = 4;
					next->step = now->step+1;
					rightCounter(now->content, next->content);
					if( hasNode(startRoot, next->content, NULL) ) {
						free(next);
					}
					else {
						addBST(&startRoot, next);
						Qstart[startTail++] = next;
					}

}
void QendAdd_4way(State *now)
{
	State *next;
					/* 3: left counter-clock */
					next = (State *) malloc(sizeof(State));
					strncpy(next->process, now->process, now->step);
					next->process[now->step] = 1;
					next->step = now->step+1;
					leftCounter(now->content, next->content);
					if( hasNode(endRoot, next->content, NULL) ) {
						free(next);
					}
					else {
						addBST(&endRoot, next);
						Qend[endTail++] = next;
					}

					/* 4: right counter-clock */
					next = (State *) malloc(sizeof(State));
					strncpy(next->process, now->process, now->step);
					next->process[now->step] = 2;
					next->step = now->step+1;
					rightCounter(now->content, next->content);
					if( hasNode(endRoot, next->content, NULL) ) {
						free(next);
					}
					else {
						addBST(&endRoot, next);
						Qend[endTail++] = next;
					}

					/* 1: left clock */
					debug("!");
					next = (State *) malloc(sizeof(State));
					strncpy(next->process, now->process, now->step);
					next->process[now->step] = 3;
					next->step = now->step+1;
					leftClock(now->content, next->content);
					if( hasNode(endRoot, next->content, NULL) ) {
						free(next);
					}
					else {
						addBST(&endRoot, next);
						Qend[endTail++] = next;
					}

					/* 2: right clock */
					next = (State *) malloc(sizeof(State));
					strncpy(next->process, now->process, now->step);
					next->process[now->step] = 4;
					next->step = now->step+1;
					rightClock(now->content, next->content);
					if( hasNode(endRoot, next->content, NULL) ) {
						free(next);
					}
					else {
						addBST(&endRoot, next);
						Qend[endTail++] = next;
					}


}
void back_BFS(void)
{
	State *now;
	do {
		now = Qend[endHead++];
		/* the four ways to generate new State */
		QendAdd_4way(now);
	} while(now->step <= 7);
}
int nodeCount;
void print_BST(Node *root, int depth)
{
	if(root == NULL) return;
	nodeCount++;
	print_BST(root->left, depth+1);
	print_BST(root->right, depth+1);
	printf("depth %d, step %d, content: ", depth, root->state->step);
	print_content(root->state->content);
}
int main(int argc, char const *argv[])
{
	setbuf(stdout, NULL);
	int testCases;
	scanf("%d ", &testCases);
	
	State *now;
	endHead = endTail = 0;
	endRoot = NULL;
	now = (State *) malloc(sizeof(State));
	memcpy(now->content, end, sizeof(char) * MAXLEN);
	now->step = 0;
	Qend[endTail++] = now;
	addBST(&endRoot, now);
	back_BFS();
	/*nodeCount = 0;
	print_BST(endRoot, 0);
	printf("nodeCount: %d\n", nodeCount);

	printf("endQ up to %d\n", endTail);*/
	while(testCases--)
	{
		initial();
		char start[MAXLEN];
		int i;
		for(i=0 ; i<MAXLEN ; i++) {
			scanf("%hhd", &start[i]);
		}

		if(memcmp(start, end, sizeof(char) * MAXLEN) == 0) {
			printf("PUZZLE ALREADY SOLVED\n");
		}
		else {
			State *ansFromStart = NULL;
			State *ansFromEnd = NULL;

			now = (State *) malloc(sizeof(State));
			memcpy(now->content, start, sizeof(char) * MAXLEN);
			now->step = 0;
			Qstart[startTail++] = now;
			addBST(&startRoot, now);

			int ans = 0;
			/* from start */
			do {
				now = Qstart[startHead++];
				if(hasNode(endRoot, now->content, &ansFromEnd)) {
					/*printf("now->step %d, ansFromEnd->step %d\n", now->step, ansFromEnd->step);*/
					ans = 1;
					ansFromStart = now;
					break;
				}
				if(ans != 0) break;
				/* the four ways to generate new State */
				if( now->step < 8 ) {
					QstartAdd_4way(now);
				}
			} while(startHead < startTail);

			if(ans == 0) {
				printf("NO SOLUTION WAS FOUND IN 16 STEPS\n");
			} else {
				int i;
				for(i=0 ; i<ansFromStart->step ; i++) {
					printf("%hhd", ansFromStart->process[i]);
				}
				for(i = ansFromEnd->step - 1 ; i >= 0 ; i--) {
					printf("%hhd", ansFromEnd->process[i]);
				}
				printf("\n");
			}
		}

	}
	return 0;
}