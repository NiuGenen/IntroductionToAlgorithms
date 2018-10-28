#include<stdio.h>
//¶¨Òåmax×÷Îª´æ½á¹ûµÄ±äÁ¿

int Longestincreasingsquence(int *nums, int n){
	if (n==0){
		return 0;
	}
	
	int max=1;
	int numslength = n;
	int i;
	int k;
	int LIS[100000];
	
	LIS[1]= nums[0];
	for( i = 1; i < numslength; ++i){
		if(LIS[max]<nums[i]){
			max=max+1;
			LIS[max]=nums[i];
		}
		else{
			//
			// LIS records nums[0][...][i-1]
			// LIS[1][2][...][max]
			// new element [i]
			//
			if(LIS[1]>nums[i]){
				LIS[1]=nums[i];
			}
			else{
				for( k = 2; k <= max; ++k){
					// LIS[k-1] < nums[i] < LIS[k]
					// LIS[k-3] < nums[i]
					// LIS[k-2] < nums[i]
                    if( LIS[k-1] < nums[i] && nums[i] < LIS[k] ){
				        LIS[k]=nums[i];
						break;
                    }
				}
			}
		}
	}
	
	return max;
}
	
int main()
{
	int T=0;
	int i,j;
	int n;
	int num[100000];
	int q1;

	scanf("%d",&T);

	for(i=0;i<T;i++){
        // 0  1 2 3 ... T : ¿¿¿T+1¿
        // ¿¿¿T¿
		n=0;
		scanf("%d",&n);
	
		for(j=0;j<n;++j)
			scanf("%d",&num[j]);
		
		q1=Longestincreasingsquence(num,n);
		
		printf("%d\n",q1);
	}

	return 0;
}
		
