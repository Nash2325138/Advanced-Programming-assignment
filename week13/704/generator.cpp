#include <cstdio>
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
int main(int argc, char const *argv[])
{
	printf("1\n");
	char now[] = {0, 3, 4, 3, 0, 5, 6, 5, 0, 1, 2, 1, 0, 7, 8, 7, 0, 9, 10, 9, 0, 1, 2, 1};
	char test[2][24];
	rightClock(now, test[0]);
	rightClock(test[0], test[1]);
	leftCounter(test[1], test[0]);
	rightCounter(test[0], test[1]);
	rightCounter(test[1], test[0]);
	rightCounter(test[0], test[1]);
	leftClock(test[1], test[0]);
	rightCounter(test[0], test[1]);
	for(int i=0 ; i<24 ; i++) {
		printf("%d ", test[1][i]);
	}
	printf("\n");
	return 0;
}