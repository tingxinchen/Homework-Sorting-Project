#include <iostream>
#include <vector>
#include <algorithm> 
#include <chrono>
#include <climits>
#include <cstdlib>

using namespace std;

//Merge Sort
void merge(int arr[], int left, int mid, int right)
{
    int n1 = mid - left + 1;
    int n2 = right - mid;

    int* L = new int[n1];
    int* R = new int[n2];

    for (int i = 0; i < n1; i++) L[i] = arr[left + i];
    for (int i = 0; i < n2; i++) R[i] = arr[mid + 1 + i];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) arr[k++] = L[i++];
        else arr[k++] = R[j++];
    }
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];

    delete[] L;
    delete[] R;
}

void iterativeMergeSort(int arr[], int n)
{
    for (int size = 1; size < n; size *= 2) {
        for (int left = 1; left <= n - size; left += 2 * size) {
            int mid = min(left + size - 1, n);
            int right = min(left + 2 * size - 1, n);
            if (mid < right) merge(arr, left, mid, right);
        }
    }
}

//Quick Sort
int Median(int* a, int left, int right)
{
    int mid = (left + right) / 2;
    if (a[mid] < a[left])  swap(a[left], a[mid]);
    if (a[right] < a[left]) swap(a[left], a[right]);
    if (a[right] < a[mid]) swap(a[mid], a[right]);

    swap(a[left], a[mid]);
    return a[left];
}

void QuickSort(int* a, int left, int right)
{
    if (left < right) {
        int pivot = Median(a, left, right);
        int i = left, j = right + 1;
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

//Heap Sort
void Adjust(int* a, const int root, const int n)
{
    int j;
    int e = a[root];
    for (j = 2 * root; j <= n; j *= 2) {
        if (j < n && a[j] < a[j + 1]) j++;
        if (e >= a[j]) break;
        a[j / 2] = a[j];
    }
    a[j / 2] = e;
}

void HeapSort(int* a, const int n)
{
    for (int i = n / 2; i >= 1; i--) Adjust(a, i, n);
    for (int i = n - 1; i >= 1; i--) {
        swap(a[1], a[i + 1]);
        Adjust(a, 1, i);
    }
}

//Insertion Sort
void insertionSortRange(int arr[], int left, int right)
{
    for (int i = left + 1; i <= right; i++) {
        int key = arr[i];
        int j = i - 1;
        while (j >= left && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

void insertionSort(int arr[], int n)
{
    insertionSortRange(arr, 1, n);
}

//Composite Sort
void compositeSort(int arr[], int left, int right)
{
    int threshold = 20;
    if ((right - left + 1) <= threshold) {
        insertionSortRange(arr, left, right);
    }
    else {
        HeapSort(arr, right);
    }
}

//隨機數值
template <class T>
void Permute(T* a, int n)
{
    for (int i = n; i >= 2; i--) {
        int j = rand() % i + 1;
        swap(a[j], a[i]);
    }
}

int main()
{
    srand(42);
    vector<int> sizes = { 500, 1000, 2000, 3000, 4000, 5000 };
    const int RUNS = 100;

    vector<long long> res_merge, res_quick, res_heap, res_insertion, res_composite;

    for (int n : sizes) {
        vector<vector<int>> test_batches(RUNS, vector<int>(n + 2));
        for (int t = 0; t < RUNS; t++) {
            for (int k = 1; k <= n; k++) test_batches[t][k] = k;
            Permute(test_batches[t].data(), n);
        }

        int* arr = new int[n + 2];
        arr[n + 1] = INT_MAX;

        //Merge Sort測試
        auto start = chrono::high_resolution_clock::now();
        for (int t = 0; t < RUNS; t++) {
            for (int i = 1; i <= n; i++) arr[i] = test_batches[t][i];
            iterativeMergeSort(arr, n);
        }
        double t_merge = chrono::duration<double, micro>(chrono::high_resolution_clock::now() - start).count();
        res_merge.push_back((long long)(t_merge / RUNS));

        //Quick Sort測試
        start = chrono::high_resolution_clock::now();
        for (int t = 0; t < RUNS; t++) {
            for (int i = 1; i <= n; i++) arr[i] = test_batches[t][i];
            QuickSort(arr, 1, n);
        }
        double t_quick = chrono::duration<double, micro>(chrono::high_resolution_clock::now() - start).count();
        res_quick.push_back((long long)(t_quick / RUNS));

        //Heap Sort測試
        start = chrono::high_resolution_clock::now();
        for (int t = 0; t < RUNS; t++) {
            for (int i = 1; i <= n; i++) arr[i] = test_batches[t][i];
            HeapSort(arr, n);
        }
        double t_heap = chrono::duration<double, micro>(chrono::high_resolution_clock::now() - start).count();
        res_heap.push_back((long long)(t_heap / RUNS));

        //Insertion Sort測試
        start = chrono::high_resolution_clock::now();
        for (int t = 0; t < RUNS; t++) {
            for (int i = 1; i <= n; i++) arr[i] = test_batches[t][i];
            insertionSort(arr, n);
        }
        double t_insertion = chrono::duration<double, micro>(chrono::high_resolution_clock::now() - start).count();
        res_insertion.push_back((long long)(t_insertion / RUNS));

        //Composite Sort測試
        start = chrono::high_resolution_clock::now();
        for (int t = 0; t < RUNS; t++) {
            for (int i = 1; i <= n; i++) arr[i] = test_batches[t][i];
            compositeSort(arr, 1, n);
        }
        double t_composite = chrono::duration<double, micro>(chrono::high_resolution_clock::now() - start).count();
        res_composite.push_back((long long)(t_composite / RUNS));

        delete[] arr;
    }

    cout << "Merge { N=500:" << res_merge[0] << ", N=1000:" << res_merge[1] << ", N=2000:" << res_merge[2] << ", N=3000:" << res_merge[3] << ", N=4000:" << res_merge[4] << ", N=5000:" << res_merge[5] << " }\n";

    cout << "Quick { N=500:" << res_quick[0] << ", N=1000:" << res_quick[1] << ", N=2000:" << res_quick[2] << ", N=3000:" << res_quick[3] << ", N=4000:" << res_quick[4] << ", N=5000:" << res_quick[5] << " }\n";

    cout << "Heap { N=500:" << res_heap[0] << ", N=1000:" << res_heap[1] << ", N=2000:" << res_heap[2] << ", N=3000:" << res_heap[3] << ", N=4000:" << res_heap[4] << ", N=5000:" << res_heap[5] << " }\n";

    cout << "Insertion { N=500:" << res_insertion[0] << ", N=1000:" << res_insertion[1] << ", N=2000:" << res_insertion[2] << ", N=3000:" << res_insertion[3] << ", N=4000:" << res_insertion[4] << ", N=5000:" << res_insertion[5] << " }\n";

    cout << "Composite { N=500:" << res_composite[0] << ", N=1000:" << res_composite[1] << ", N=2000:" << res_composite[2] << ", N=3000:" << res_composite[3] << ", N=4000:" << res_composite[4] << ", N=5000:" << res_composite[5] << " }\n";

    return 0;
}
