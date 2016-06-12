#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

int bookNum, stackNum, pages[600];
/*
make test() more fast by using binary search
int searchRightIndex(int* pages, int bookNum, int stackNum, int maxStack, int left)
{

}
*/

/* whether it can use maxStackSize to make a arrangement */
int test(int maxStackSize)
{
	/* can be accelerate by usting  searchRightIndex()*/
	int i=0, size=0, stackUse=1;
	while(i<bookNum){
		if(pages[i] > maxStackSize || stackUse > stackNum) return 0;
		if(size+pages[i] <= maxStackSize){
			size+=pages[i];
		} else {
			stackUse++;
			size = pages[i];
		}
		i++;
	}
	if(stackUse > stackNum) return 0;
	else return 1;
}

int main()
{
	int testCases;
	scanf("%d", &testCases);
	while(testCases--)
	{
		long upper=0 , lower=0;

		int i;
		scanf("%d %d", &bookNum, &stackNum);
		for(i=0; i<bookNum ; i++) {
			scanf("%d", &pages[i]);
			upper += pages[i];
		}

		/* find the min number such that the maxStack <= that number*/
		while(lower+1 < upper)
		{
			int middle = (lower+upper)/2;
			if( test(middle)==1 ) upper = middle;
			else lower = middle;
		}

		/*printf("maxStackSize==%d\n", upper);*/
		long maxStackSize = upper , size=0;
		int stackUse = 1;
		int ans[1100];
		int j=0;
		for( i=bookNum-1 ; i>=0 ; i-- )
		{
			/*printf("stackUse==%d , stackNum==%d\n", stackUse , stackNum);*/
			if(size+pages[i] <= maxStackSize){
				size += pages[i];
			}
			else{
				size = pages[i];
				ans[j++] = -1;
				stackUse++;
				assert(stackUse <= stackNum);
			}
			ans[j++] = pages[i];

			if( stackNum - stackUse >= i ){
				for(i=i-1 ; i>=0 ; i--)
				{
					ans[j++] = -1;
					ans[j++] = pages[i];
				}
			}

		}
		for(j=j-1 ; j>0 ; j--){
			if(ans[j]==-1) printf("/ ");
			else printf("%d ", ans[j]);
		}
		printf("%d\n", ans[0]);
	}
}
