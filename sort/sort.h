#ifndef SORT_HEAD_H_2022_03_17
#define SORT_HEAD_H_2022_03_17

#include <algorithm>

namespace templates
{
    /** Определение типа функции сравнения элементов
    \return
    \li > 0  - pElem1 меньше pElem2
    \li 0   - элементы pElem1 и pElem2 равны
    \li < 0 - pElem1 больше pElem2
    */
    typedef  int (CompareSortType)(const void *pElem1, const void *pElem2);

    void merge(void** pArray, int p, int q, int r, CompareSortType pCompareFunc) {
        int n1 = q - p + 1;
        int n2 = r - q;

        void* L[n1];
        void* M[n2];

        for (int i = 0; i < n1; i++)
            L[i] = pArray[p + i];
        for (int j = 0; j < n2; j++)
            M[j] = pArray[q + 1 + j];

        int i, j, k;
        i = 0;
        j = 0;
        k = p;

        while (i < n1 && j < n2) {

            if (pCompareFunc(L[i], M[j]) >= 0) {
                pArray[k] = L[i];
                i++;
            } else {
                pArray[k] = M[j];
                j++;
            }
            k++;
        }

        while (i < n1) {
            pArray[k] = L[i];
            i++;
            k++;
        }

        while (j < n2) {
            pArray[k] = M[j];
            j++;
            k++;
        }
    }

    void mergeSort2(void** pArr, int l, int r, CompareSortType pCompareFunc) {
        if (l < r) {
            int m = l + (r - l) / 2;

            mergeSort2(pArr, l, m, pCompareFunc);
            mergeSort2(pArr, m + 1, r, pCompareFunc);
            merge(pArr, l, m, r, pCompareFunc);
        }
    }

    void mergeSort(void **ppArray, int length, CompareSortType pCompareFunc) {
        mergeSort2(ppArray, 0, length - 1, pCompareFunc);
    }


    void heapify(void **ppArray, int length, int i, CompareSortType pCompareFunc) {
        int largest = i;
        int l = 2 * i + 1;
        int r = 2 * i + 2;

        if (l < length && pCompareFunc(ppArray[l], ppArray[largest]) < 0)
            largest = l;

        if (r < length && pCompareFunc(ppArray[r], ppArray[largest]) < 0)
            largest = r;

        if (largest != i) {
            std::swap(ppArray[i], ppArray[largest]);
            heapify(ppArray, length, largest, pCompareFunc);
        }
    }


    void heapSort(void **ppArray, int length, CompareSortType pCompareFunc) {
        for (int i = length / 2 - 1; i >= 0; i--)
            heapify(ppArray, length, i, pCompareFunc);
        for (int i = length - 1; i > 0; i--) {
            std::swap(ppArray[0], ppArray[i]);
            heapify(ppArray, i, 0, pCompareFunc);
        }
    }


    template <class T>
    void mergeSort(T **ppArray, int length, int (pCompareFunc)(const T *pElem1, const T *pElem2))
    {
        mergeSort(reinterpret_cast<void**>(ppArray), length, (CompareSortType*)pCompareFunc);
    }
    template <class T>
    void heapSort(T **ppArray, int length, int (pCompareFunc)(const T *pElem1, const T *pElem2))
    {
        heapSort(reinterpret_cast<void**>(ppArray), length, (CompareSortType*)pCompareFunc);
    }
}; // namespace templates

#endif // SORT_HEAD_H_2022_03_17