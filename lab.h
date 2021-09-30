#ifndef __LAB_H
#define __LAB_H

#ifdef __cplusplus
extern "C" {
#endif

#define TRUE 1
#define FALSE 0

// function prototypes
void *parallel_mergesort(void *arg);
void p_mergesort(int A[], int r, int threadcount);
void serial_mergesort(int A[], int p, int r);
void merge(int A[], int p, int q, int r);
void insertion_sort(int A[], int p, int r);
void final_merge(int A[], int npt, int amplitude);

const int INSERTION_SORT_THRESHOLD = 100; //based on trial and error

#ifdef __cplusplus
}
#endif
#endif //__LAB_H
