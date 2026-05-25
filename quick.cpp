#include <iostream>
#include <algorithm> 
using namespace std;


template <class T>
T Median(T* a, int left, int right)
{
    int mid = (left + right) / 2;

    if (a[mid] < a[left])  swap(a[left], a[mid]);
    if (a[right] < a[left]) swap(a[left], a[right]);
    if (a[right] < a[mid]) swap(a[mid], a[right]);

   
    swap(a[left], a[mid]);
    return a[left];
}

template <class T>
void QuickSort(T* a, int left, int right)
{
    if (left < right)
    {
        T pivot = Median(a, left, right);

        int i = left,
            j = right + 1;

        do {
            do i++; while (a[i] < pivot); 
            do j--; while (a[j] > pivot); 
            if (i < j) swap(a[i], a[j]);   
        } while (i < j);

        swap(a[left], a[j]);  

        QuickSort(a, left, j - 1);   
        QuickSort(a, j + 1, right);
    }
}

void printArray(int* a, int n)
{
    for (int i = 1; i <= n; i++)
        cout << a[i] << " ";
    cout << endl;
}
void Permute(int* a, int n)
{
    for (int i = n; i >= 2; i--)
    {
        int j = rand() % i + 1;
        swap(a[j], a[i]);
    }
}
int main()
{
    srand(42);
    int size[6] = { 500, 1000, 2000, 3000, 4000, 5000 };
    for (int i = 0; i < 6; i++) {
        int* d = new int[size[i] + 1]; 

        for (int k = 1; k <= size[i]; k++)
            d[k] = k;

        cout << "Permute 前：";
        printArray(d, size[i]);

        Permute(d, size[i]);  

        cout << "Permute 後：";
        printArray(d, size[i]);

        QuickSort(d, 1, size[i]);
        cout << "排序後確認：";
        printArray(d, size[i]);
        delete[] d;

    }
}