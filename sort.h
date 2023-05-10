#ifndef SORT_H_
#define SORT_H_

void selectionSort(unsigned char* A, int n);  // in-place selection sort

// in-place selection sort
// Input: n-element array A with values between 0 and 255
// Output: sorted array A in increasing order
void selectionSort(unsigned char* A, int n){

    int j;            
    unsigned char t;  // temporary variable for swapping

    for (int i = 0; i < n - 1; i++){  // iterate from 0 to n-1
        j = i;
        for (int k = i + 1; k < n; k++){
            if (A[k] < A[j]) j = k;  // store current smallest element index in j
        }
        t = A[i]; A[i] = A[j]; A[j] = t;  // swap A[i] and A[j]
    }
} 

#endif