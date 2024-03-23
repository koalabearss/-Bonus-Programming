#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int extraMemoryAllocated;

void *Alloc(size_t sz)
{
    extraMemoryAllocated += sz;
    size_t* ret = malloc(sizeof(size_t) + sz);
    *ret = sz;
    printf("Extra memory allocated, size: %ld\n", sz);
    return &ret[1];
}

void DeAlloc(void* ptr)
{
    size_t* pSz = (size_t*)ptr - 1;
    extraMemoryAllocated -= *pSz;
    printf("Extra memory deallocated, size: %ld\n", *pSz);
    free((size_t*)ptr - 1);
}

size_t Size(void* ptr)
{
    return ((size_t*)ptr)[-1];
}

int parseData(char *inputFileName, int **ppData) {
    FILE* inFile = fopen(inputFileName, "r");
    int dataSz = 0, i = 0;
    if (inFile) {
        fscanf(inFile, "%d", &dataSz);
        *ppData = (int *)Alloc(sizeof(int) * dataSz);
        for (i = 0; i < dataSz; ++i) {
            fscanf(inFile, "%d", &((*ppData)[i]));
        }
        fclose(inFile);
    }
    return dataSz;
}

void selectionSort(int* pData, int n) {
    int i, j, min_idx;

    for (i = 0; i < n-1; i++) {
        min_idx = i;
        for (j = i+1; j < n; j++)
          if (pData[j] < pData[min_idx])
            min_idx = j;

        if (min_idx != i) {
            int temp = pData[i];
            pData[i] = pData[min_idx];
            pData[min_idx] = temp;
        }
    }
}

void insertionSort(int* pData, int n) {
    int i, key, j;
    for (i = 1; i < n; i++) {
        key = pData[i];
        j = i - 1;

        while (j >= 0 && pData[j] > key) {
            pData[j + 1] = pData[j];
            j = j - 1;
        }
        pData[j + 1] = key;
    }
}

void bubbleSort(int* pData, int n) {
   int i, j;
   for (i = 0; i < n-1; i++)       
       for (j = 0; j < n-i-1; j++) 
           if (pData[j] > pData[j+1]) {
              int temp = pData[j];
              pData[j] = pData[j+1];
              pData[j+1] = temp;
           }
}

void merge(int arr[], int l, int m, int r) {
    int i, j, k;
    int n1 = m - l + 1;
    int n2 =  r - m;

    int *L = (int *)Alloc(n1 * sizeof(int)), *R = (int *)Alloc(n2 * sizeof(int));

    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1+ j];

    i = 0;
    j = 0;
    k = l;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }

    DeAlloc(L);
    DeAlloc(R);
}

void mergeSort(int arr[], int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}

void heapify(int arr[], int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && arr[left] > arr[largest])
        largest = left;

    if (right < n && arr[right] > arr[largest])
        largest = right;

    if (largest != i) {
        int swap = arr[i];
        arr[i] = arr[largest];
        arr[largest] = swap;

        heapify(arr, n, largest);
    }
}

void heapSort(int arr[], int n) {
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(arr, n, i);

    for (int i = n - 1; i > 0; i--) {
        int temp = arr[0];
        arr[0] = arr[i];
        arr[i] = temp;

        heapify(arr, i, 0);
    }
}

void printArray(int pData[], int dataSz) {
    int i, sz = dataSz > 100 ? dataSz - 100 : 0;
    printf("\tData:\n\t");
    for (i=0; i < 100 && i < dataSz; ++i) {
        printf("%d ", pData[i]);
    }
    if (dataSz > 100) printf("\n\t...\n\t");
    for (i=sz; i<dataSz; ++i) {
        printf("%d ", pData[i]);
    }
    printf("\n\n");
}

int main(void) {
    clock_t start, end;
    int i;
    double cpu_time_used;
    char* fileNames[] = {"input1.txt", "input2.txt", "input3.txt"};

    for (i=0; i<3; ++i) {
        int *pDataSrc, *pDataCopy;
        int dataSz = parseData(fileNames[i], &pDataSrc);

        if (dataSz <= 0)
            continue;

        pDataCopy = (int *)Alloc(sizeof(int)*dataSz);

        // Selection Sort
        memcpy(pDataCopy, pDataSrc, sizeof(int)*dataSz);
        start = clock();
        selectionSort(pDataCopy, dataSz);
        end = clock();
        cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
        printf("Selection Sort Time: %f\n", cpu_time_used);
        printArray(pDataCopy, dataSz);

        // Insertion Sort
        memcpy(pDataCopy, pDataSrc, sizeof(int)*dataSz);
        start = clock();
        insertionSort(pDataCopy, dataSz);
        end = clock();
        cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
        printf("Insertion Sort Time: %f\n", cpu_time_used);
        printArray(pDataCopy, dataSz);

        // Bubble Sort
        memcpy(pDataCopy, pDataSrc, sizeof(int)*dataSz);
        start = clock();
        bubbleSort(pDataCopy, dataSz);
        end = clock();
        cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
        printf("Bubble Sort Time: %f\n", cpu_time_used);
        printArray(pDataCopy, dataSz);

        // Merge Sort
        memcpy(pDataCopy, pDataSrc, sizeof(int)*dataSz);
        start = clock();
        mergeSort(pDataCopy, 0, dataSz-1);
        end = clock();
        cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
        printf("Merge Sort Time: %f\n", cpu_time_used);
        printArray(pDataCopy, dataSz);

        // Heap Sort
        memcpy(pDataCopy, pDataSrc, sizeof(int)*dataSz);
        start = clock();
        heapSort(pDataCopy, dataSz);
        end = clock();
        cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
        printf("Heap Sort Time: %f\n", cpu_time_used);
        printArray(pDataCopy, dataSz);

        DeAlloc(pDataSrc);
        DeAlloc(pDataCopy);
    }

    return 0;
}
