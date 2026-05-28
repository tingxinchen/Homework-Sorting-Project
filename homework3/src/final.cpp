#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <climits>
#include <cstdlib>
#include <numeric>
#include <random>
#include <Windows.h>
#include <Psapi.h>

using namespace std;

//Merge Sort
static void merge(int arr[], int left, int mid, int right)
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

static void iterativeMergeSort(int arr[], int n)
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
static int Median(int* a, int left, int right)
{
    int mid = (left + right) / 2;
    if (a[mid] < a[left]) swap(a[left], a[mid]);
    if (a[right] < a[left]) swap(a[left], a[right]);
    if (a[right] < a[mid]) swap(a[mid], a[right]);

    swap(a[left], a[mid]);
    return a[left];
}

static void QuickSort(int* a, int left, int right)
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

// Heap Sort
static void Adjust(int* a, const int root, const int n)
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

static void HeapSort(int* a, const int n)
{
    for (int i = n / 2; i >= 1; i--) Adjust(a, i, n);
    for (int i = n - 1; i >= 1; i--) {
        swap(a[1], a[i + 1]);
        Adjust(a, 1, i);
    }
}

//Insertion Sort
static void insertionSortRange(int arr[], int left, int right)
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

static void insertionSort(int arr[], int n)
{
    insertionSortRange(arr, 1, n);
}

//Composite Sort
// 使用 Quick Sort 分割，當區間小於 threshold 時改用 Insertion Sort
static void compositeSortInternal(int arr[], int left, int right, int threshold)
{
    if ((right - left + 1) <= threshold) {
        insertionSortRange(arr, left, right);
    }
    else {
        if (left < right) {
            int pivot = Median(arr, left, right);
            int i = left, j = right + 1;
            do {
                do i++; while (arr[i] < pivot);
                do j--; while (arr[j] > pivot);
                if (i < j) swap(arr[i], arr[j]);
            } while (i < j);

            swap(arr[left], arr[j]);
            compositeSortInternal(arr, left, j - 1, threshold);
            compositeSortInternal(arr, j + 1, right, threshold);
        }
    }
}

static void compositeSort(int arr[], int left, int right)
{
    compositeSortInternal(arr, left, right, 20); // 門檻值設為 20
}

// ==================== 記憶體計量功能 ====================
static size_t getWorkingSetKB() {
    PROCESS_MEMORY_COUNTERS memInfo;
    GetProcessMemoryInfo(GetCurrentProcess(), &memInfo, sizeof(memInfo));
    return (size_t)(memInfo.WorkingSetSize / 1024);
}

static size_t getPeakWorkingSetKB() {
    PROCESS_MEMORY_COUNTERS memInfo;
    GetProcessMemoryInfo(GetCurrentProcess(), &memInfo, sizeof(memInfo));
    return (size_t)(memInfo.PeakWorkingSetSize / 1024);
}

// ==================== 主程式 ====================
int main()
{
    // 改用時間戳記種子，確保平均狀況的隨機性
    unsigned seed_val = chrono::system_clock::now().time_since_epoch().count();
    mt19937 g(seed_val);

    vector<int> sizes = { 500, 1000, 2000, 3000, 4000, 5000 };
    const int RUNS = 100;

    vector<long long> res_merge, res_quick, res_heap, res_insertion, res_composite;
    vector<long long> mem_merge, mem_quick, mem_heap, mem_insertion, mem_composite;

    for (int n : sizes) {
        // 每次 N 值不同時，產生 100 組全新的隨機排列
        vector<vector<int>> test_batches(RUNS, vector<int>(n + 2));
        for (int t = 0; t < RUNS; t++) {
            vector<int> temp(n);
            iota(temp.begin(), temp.end(), 1); // 產生 1~n
            shuffle(temp.begin(), temp.end(), g); // 真正的隨機打亂
            for (int k = 1; k <= n; k++) test_batches[t][k] = temp[k - 1];
        }

        int* arr = new int[n + 2];
        arr[n + 1] = INT_MAX;

        // ---------------- Merge Sort ----------------
        size_t m_before = getWorkingSetKB();
        auto start = chrono::high_resolution_clock::now();
        for (int t = 0; t < RUNS; t++) {
            for (int i = 1; i <= n; i++) arr[i] = test_batches[t][i];
            iterativeMergeSort(arr, n);
        }
        auto end = chrono::high_resolution_clock::now();
        res_merge.push_back(chrono::duration_cast<chrono::microseconds>(end - start).count() / RUNS);
        mem_merge.push_back((long long)getWorkingSetKB() - (long long)m_before);

        // ---------------- Quick Sort ----------------
        m_before = getWorkingSetKB();
        start = chrono::high_resolution_clock::now();
        for (int t = 0; t < RUNS; t++) {
            for (int i = 1; i <= n; i++) arr[i] = test_batches[t][i];
            QuickSort(arr, 1, n);
        }
        end = chrono::high_resolution_clock::now();
        res_quick.push_back(chrono::duration_cast<chrono::microseconds>(end - start).count() / RUNS);
        mem_quick.push_back((long long)getWorkingSetKB() - (long long)m_before);

        // ---------------- Heap Sort ----------------
        m_before = getWorkingSetKB();
        start = chrono::high_resolution_clock::now();
        for (int t = 0; t < RUNS; t++) {
            for (int i = 1; i <= n; i++) arr[i] = test_batches[t][i];
            HeapSort(arr, n);
        }
        end = chrono::high_resolution_clock::now();
        res_heap.push_back(chrono::duration_cast<chrono::microseconds>(end - start).count() / RUNS);
        mem_heap.push_back((long long)getWorkingSetKB() - (long long)m_before);

        // ---------------- Insertion Sort ----------------
        m_before = getWorkingSetKB();
        start = chrono::high_resolution_clock::now();
        for (int t = 0; t < RUNS; t++) {
            for (int i = 1; i <= n; i++) arr[i] = test_batches[t][i];
            insertionSort(arr, n);
        }
        end = chrono::high_resolution_clock::now();
        res_insertion.push_back(chrono::duration_cast<chrono::microseconds>(end - start).count() / RUNS);
        mem_insertion.push_back((long long)getWorkingSetKB() - (long long)m_before);

        // ---------------- Composite Sort ----------------
        m_before = getWorkingSetKB();
        start = chrono::high_resolution_clock::now();
        for (int t = 0; t < RUNS; t++) {
            for (int i = 1; i <= n; i++) arr[i] = test_batches[t][i];
            compositeSort(arr, 1, n);
        }
        end = chrono::high_resolution_clock::now();
        res_composite.push_back(chrono::duration_cast<chrono::microseconds>(end - start).count() / RUNS);
        mem_composite.push_back((long long)getWorkingSetKB() - (long long)m_before);

        delete[] arr;
    }

    //輸出結果 (平均時間 us)
    cout << "--- Average Case Runtimes (us) ---\n";
    cout << "Merge:     " << res_merge[0] << ", " << res_merge[1] << ", " << res_merge[2] << ", " << res_merge[3] << ", " << res_merge[4] << ", " << res_merge[5] << endl;
    cout << "Quick:     " << res_quick[0] << ", " << res_quick[1] << ", " << res_quick[2] << ", " << res_quick[3] << ", " << res_quick[4] << ", " << res_quick[5] << endl;
    cout << "Heap:      " << res_heap[0] << ", " << res_heap[1] << ", " << res_heap[2] << ", " << res_heap[3] << ", " << res_heap[4] << ", " << res_heap[5] << endl;
    cout << "Insertion: " << res_insertion[0] << ", " << res_insertion[1] << ", " << res_insertion[2] << ", " << res_insertion[3] << ", " << res_insertion[4] << ", " << res_insertion[5] << endl;
    cout << "Composite: " << res_composite[0] << ", " << res_composite[1] << ", " << res_composite[2] << ", " << res_composite[3] << ", " << res_composite[4] << ", " << res_composite[5] << endl;


    return 0;
}
