#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define MAX_LEN 1010

/*make in = in+number, where 0 <= number <= 9*/
/* best: O(1)	worst: O(MAX_LEN) */
void bigN_add_1number( int* in, int number)
{
	assert ( number >= 0);
	assert ( number <10 );
	int carry, i;
	int temp = in[0]+number;
	in[0] = temp%10;
	carry = temp/10;
	for(i=1 ; i<MAX_LEN ; i++){
		if(carry==0) break;
		temp = in[i] + carry;
		in[i] = temp%10;
		carry = temp/10;
	}
}

/* make out = in * number, where 0 <= number <= 9*/
/* O(MAX_LEN) */
void bigN_mul_1number(int* in, int number, int* out)
{
	/*assert(number>=0 && number <10);*/
	int carry, temp, i;
	for(i=0 ; i<MAX_LEN ; i++){
		temp = in[i] * number + carry;
		out[i] = temp%10;
		carry = temp/10;
	}
	/*assert(carry == 0);*/		/* if not, it's overflow */
}

/* caller should guarantee that num1 >= num2
 make num1 = num1 - num2*/
/* O(MAX_LEN) */
void bigN_sub( int* num1, int* num2)	
{
	int i, borrow = 0;
	for(i=0; i<MAX_LEN ; i++){
		num1[i] = num1[i] - num2[i] + borrow;
		if(num1[i] < 0){
			num1[i] += 10;
			borrow = -1;
		} else borrow = 0;
	}
	/*assert(borrow==0);	*/	/* if not, num1 < num2 */
}

/* if num1 < num2 return 1
 else if num1==num2 return 0
 else return -1*/
 /* O(MAX_LEN) */
int isLessThan(int* num1, int* num2)
{
	int i;
	for(i=MAX_LEN-1 ; i>=0 ; i--){
		if( num1[i] < num2[i] ) return 1;
		else if (num1[i] > num2[i]) return -1;
	}
	return 0;
}

/* Just for debug */
void print_bigN(int *input)
{
	int i, flag=0;
	for(i=MAX_LEN-1 ; i>=0 ; i--){
		if(flag==0){
			if(input[i]==0) continue;
			else flag = 1;
		}
		printf("%d ", input[i]);
	}
	printf("\n");
}

int main(int argc, char const *argv[])
{
	int testCases;
	int base[ MAX_LEN ];
	int toSub[ MAX_LEN ];
	int out[ MAX_LEN ];
	scanf("%d", &testCases);
	while(testCases>0)
	{
		testCases--;
		char str[ MAX_LEN ];
		int input[ MAX_LEN ];
		memset(input , 0, sizeof(input));

		scanf(" %s", str);
		int i , j , size = strlen(str);
		for( j=0, i=size-1 ; i>=0 ; i-- , j++)	input[j] = str[i] - '0';
		
		
		memset(base, 0, sizeof(base));
		memset(toSub, 0 , sizeof(toSub));
		
		if(size%2==1) size++;

		int ans[ MAX_LEN ];
		int ansTop = 0;

		for(i=size ; i>0 ; i-=2){
			for(j=MAX_LEN-1 ; j>=2 ; j--) toSub[j] = toSub[j-2];
			toSub[1] = input[i-1];
			toSub[0] = input[i-2];
			/*printf("toSub==");
			print_bigN(toSub);*/

			for(j=MAX_LEN-1 ; j>=1 ; j--) base[j] = base[j-1];
			base[0] = 0;
			/*printf("base==");
			print_bigN(base);*/

			for(j=0; j<10 ; j++){
				base[0] = j;
				bigN_mul_1number(base, j, out);
				/*printf("out==");
				print_bigN(out);*/
				int result = isLessThan(out, toSub);
				if(result > 0) continue;
				else if(result==0) break;
				else{
					j--;
					break;
				}
			}
			if(j==10) j--;
			/*while(j<0);
			while(j>=10);*/
			ans[ansTop++] = j;
			base[0] = j;

			
			bigN_mul_1number(base, j, out);
			/*printf("base:");
			print_bigN(base);
			printf("mul_out:");
			print_bigN(out);*/
			bigN_sub(toSub, out);

			bigN_add_1number(base, j);
		}
		for(i=0 ; i<ansTop ; i++) printf("%d", ans[i]);
		printf("\n");
		if(testCases>0)printf("\n");
	}
	return 0;
}

/*985210221312300000002318462035478894213548165416432134605318797951031436589314513715445332153346864765548435135489652134879133235410212100524321003543034897
970639180178231145138699634990156653805296311080127461788450287232053744494759887224194523863014669001501525835995381250175353163307899972246035533415408435573378490947387265567103140928008718823505283969708923532333729237758501704581221184837448223023868057772006393768085497584907675699682613012970281359800609

97063918017823114513869963499015665380529631108012746178845028723205374449475988722419452386301466900150152583697183903419119340384364196518963241689926051282159787539460478093706948911904892702085179638546058732993485765335097821416918675933478810450892296255154976126573507197215362728997562823140410311181718566913840678835526189696111717564393768085497584907675699682613012970281359800609
*/
