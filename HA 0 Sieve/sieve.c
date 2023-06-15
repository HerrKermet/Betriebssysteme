#include <stdio.h>
#include <stdbool.h>



int main(int argc, char **argv) {

	const int MAX = 100;	
	bool prime[MAX + 1];
	
	//init 0 and 1 to false
	prime[0] = false;
	prime[1] = false;
	
	//init values >1 to true
	for(int i = 2; i <= MAX; i++){
		prime[i] = true;
	}
	
	//loop over array starting at 2
	for(int i = 2; i <= MAX; i++){
		
		//if current element is still true
		if(prime[i] && i * i <= MAX){
						
			//set all multiples of i to false
			for(int j = 2; i * j <= MAX; j++) {
				prime[i * j] = false;
			}
		}
	}

	//print remaining elements
	for(int i = 0; i <= MAX; i++){
		if (prime[i])	printf("%d \n", i);
	}
		
	
	
}


