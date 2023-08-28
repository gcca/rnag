#include <stdio.h>

void quick_sort (int p, int r, float *data)
{
    if (p < r) {
            int q = partition (p, r, data);
            #pragma omp task
            quick_sort (p, q-1, data, low_limit);
            #pragma omp task
            quick_sort (q+1, r, data, low_limit);
        }
    }
}

void par_quick_sort (int n, float *data)
{
    #pragma omp parallel
    {
        #pragma omp single nowait
        quick_sort (0, n, data);
    }
}

main(){
	float aa[10000];
	float p=10000;
	
	register i;

	for(i=0;i<10000;i++)
		aa[i]=p--;



	printf();
}
