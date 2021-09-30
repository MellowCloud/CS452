/**
 * Name: Jane Doe
 * Email: janedoe@u.boisestate.edu
 * Github Username: jdoe
 */
#include "lab.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <pthread.h>

int threadtotal, a_size;
int num_per_thread;
int *arr;


/*
 * insertion_sort(int A[], int p, int r):
 *
 * description: Sort the section of the array A[p..r].
 */
void insertion_sort(int A[], int p, int r)
{
	int j;

	for (j=p+1; j<=r; j++) {
		int key = A[j];
		int i = j-1;
		while ((i > p-1) && (A[i] > key)) {
			A[i+1] = A[i];
			i--;
		}
		A[i+1] = key;
	}
}


/*
 * serial_mergesort(int A[], int p, int r):
 *
 * description: Sort the section of the array A[p..r].
 */
void serial_mergesort(int A[], int p, int r)
{
	if (r-p+1 <= INSERTION_SORT_THRESHOLD)  {
			insertion_sort(A,p,r);
	} else {
		int q =  r + (p - r)/2;
		serial_mergesort(A, p, q);
		serial_mergesort(A, q+1, r);
		merge(A, p, q, r);
	}
}

/*
*	parallel_mergesort(void *arg)
*	description: Calls serial_mergesort() as a base case 
*	after partitioned to the requested amount of threads
*/
void *parallel_mergesort(void *arg){
	int current_thread = (long)arg;
	int left = current_thread * (num_per_thread)+1;
	int right = (current_thread + 1) * num_per_thread;
	if (current_thread == threadtotal-1){
		right += a_size%threadtotal;
	}
	int middle = left + (right - left) / 2;
	if (left < right){
		serial_mergesort(arr, left, right);
		serial_mergesort(arr, left + 1, right);
		merge(arr, left, middle, right);
	}

return 0;
}


/*
*p_mergesort(int A[], int p, int r, int threadcount)
*	description: called from mytests.c with parameters to
*	use for thread creation
*/
void p_mergesort(int A[], int r, int threadcount){

	threadtotal = threadcount;
	a_size = r;
	arr = (int *) malloc(sizeof(int) * (r+1));
	arr = A;
	num_per_thread = r/threadcount;

	pthread_t threads[threadcount];
	for (long i = 0; i < threadcount; i++){
		pthread_create(&threads[i], NULL, parallel_mergesort, (void *)i);
	}
	for (int i = 0; i < threadcount; i++){
		pthread_join(threads[i], NULL);
	}
	final_merge(arr, threadcount,1);
}

void final_merge(int A[], int npt ,int amplitude){
	for(int i = 0; i < npt; i = i+2){
		int left = i * (num_per_thread * amplitude)+1;
		int right = (i+2)* amplitude * num_per_thread;
		int middle = left + amplitude * num_per_thread - 1;
		if (right >= a_size-1){
			right = a_size;
		}
		merge(A, left, middle, right);
	}
	if (npt / 2 >= 1){
		final_merge(A, npt / 2, amplitude * 2);
	}
}

/*
 * merge(int A[], int p, int q, int r):
 *
 * description: Merge two sorted sequences A[p..q] and A[q+1..r]
 *              and place merged output back in array A. Uses extra
 *              space proportional to A[p..r].
 */
void merge(int A[], int p, int q, int r)
{
	int *B = (int *) malloc(sizeof(int) * (r-p+1));

	int i = p;
	int j = q+1;
	int k = 0;
	int l;

	// as long as both lists have unexamined elements
	// this loop keeps executing.
	while ((i <= q) && (j <= r)) {
		if (A[i] < A[j]) {
			B[k] = A[i];
			i++;
		} else {
			B[k] = A[j];
			j++;
		}
		k++;
	}

	// now only at most one list has unprocessed elements.

	if (i <= q) {
		// copy remaining elements from the first list
		for (l=i; l<=q; l++) {
			B[k] = A[l];
			k++;
		}
	} else {
		// copy remaining elements from the second list
		for (l=j; l<=r; l++) {
			B[k] = A[l];
			k++;
		}
	}

	// copy merged output from array B back to array A
	k=0;
	for (l=p; l<=r; l++) {
		A[l] = B[k];
		k++;
	}

	free(B);
}
