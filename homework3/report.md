# 第九組 Sorting Project
# 41343128 陳廷鑫 、41343119 金宗翰
## 解題說明
這次作業的主要目的是設計一個組合式排序函式，比較不同排序演算法在最差情況與平均情況下的執行效率，組合式排序函式是把兩種以上的排序法結合在一起，根據資料大小狀況，來選擇較適合的排序方式來提高效率還有速度，Quick Sort在資料很多時通常比較快，資料量變小後，Insertion Sort比較有效率，所以可以在 Quick Sort 分割到某個大小後改用 Insertion Sort，這樣能減少遞迴造成的額外負擔，也能提升實際執行速度。
作業中需要先用 C++ 實作4種排序法，例如 Insertion Sort、Quick Sort、Merge Sort 與 Heap Sort，之後再利用這些設計自己的 composite sorting function，做完之後，要利用不同數量的資料進行測試，比較它們在執行的時間，分成worst-case還有average-case兩種，worst-case 是排序法遇到會算最慢的情況，在Insertion Sort 在資料完全反向排列時會變得非常慢，因為每次插入都需要移動大量數字，average-case則是利用隨機排列的資料進行測試，用來模擬一般使用情況下的表現。
在測試過程中，需要使用計時工具紀錄各排序法的執行時間，並觀察它們在不同資料大小下的差異。除了比較誰比較快之外，也要分析哪一種排序法在什麼資料量開始有優勢，資料比較少的時候 Insertion Sort 可能比 Quick Sort 還快，但當資料量變大後，Quick Sort 的效率就會逐漸超越它。最後需要把測試結果整理成表格與圖表，並對結果進行比較，說明各排序法的特性、優缺點，以及組合式排序函式是否真的能提升整體效能。
### 解題策略和步驟
1. 作業要求先分別完成各種排序法的基本版本，用小資料確認結果正確
2. Quick Sort 的 pivot 規定使用 median-of-three，取 left 、mid 、right 三者的中位數來降低遇到接近排序好資料時，會變慢的問題
3. Merge Sort 規定使用迭代，每合併完一輪，合併的區間寬度就乘以2，直到整條陣列都合併完。    
4. worst-case 測資的部分：
   - Insertion Sort：使用反向序列 (n, n-1, ..., 1)。
   - Merge Sort：用 generateWorstCase() 產生讓 merge 過程比較吃力的排列。
   - Heap Sort：課本使用 random permutation 產生多組測資，測很多次後取最大值來近似 worst-case。
5. average-case 測資的部分：使用 random permutation（隨機排列）多次測試，再取平均。

## 程式實作
以下是程式碼：
```cpp
#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <climits>
#include <cstdlib>
#include <Windows.h>
#include <Psapi.h>

using namespace std;

// Merge Sort
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

// Quick Sort
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

// Insertion Sort
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

// Composite Sort
static void compositeSort(int arr[], int left, int right)
{
    int threshold = 20;
    if ((right - left + 1) <= threshold) {
        insertionSortRange(arr, left, right);
    }
    else {
        // this compositeSort chooses HeapSort for bigger input
        HeapSort(arr, right);
    }
}

// 隨機數值
template <class T>
static void Permute(T* a, int n)
{
    for (int i = n; i >= 2; i--) {
        int j = rand() % i + 1;
        swap(a[j], a[i]);
    }
}



//記憶體
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
int main()
{
    srand(42);

    vector<int> sizes = { 500, 1000, 2000, 3000, 4000, 5000 };
    const int RUNS = 100;
    vector<long long> mem_merge, mem_quick, mem_heap, mem_insertion, mem_composite; //記憶體
    vector<long long> res_merge, res_quick, res_heap, res_insertion, res_composite;

    for (int n : sizes) {
        // 先產生同一批測資給所有排序法使用（average-case）
        vector<vector<int>> test_batches(RUNS, vector<int>(n + 2));
        for (int t = 0; t < RUNS; t++) {
            for (int k = 1; k <= n; k++) test_batches[t][k] = k;
            Permute(test_batches[t].data(), n);
        }

        int* arr = new int[n + 2];
        arr[n + 1] = INT_MAX; // QuickSort 的哨兵

        // ---------------- Merge Sort ----------------
        {
            size_t mem_before = getWorkingSetKB();

            auto start = chrono::high_resolution_clock::now();
            for (int t = 0; t < RUNS; t++) {
                for (int i = 1; i <= n; i++) arr[i] = test_batches[t][i];
                iterativeMergeSort(arr, n);
            }
            double t_merge = chrono::duration<double, micro>(chrono::high_resolution_clock::now() - start).count();

            size_t mem_after = getWorkingSetKB();

            res_merge.push_back((long long)(t_merge / RUNS));
            mem_merge.push_back((long long)((long long)mem_after - (long long)mem_before));
        }

        // ---------------- Quick Sort ----------------
        {
            size_t mem_before = getWorkingSetKB();

            auto start = chrono::high_resolution_clock::now();
            for (int t = 0; t < RUNS; t++) {
                for (int i = 1; i <= n; i++) arr[i] = test_batches[t][i];
                QuickSort(arr, 1, n);
            }
            double t_quick = chrono::duration<double, micro>(chrono::high_resolution_clock::now() - start).count();

            size_t mem_after = getWorkingSetKB();

            res_quick.push_back((long long)(t_quick / RUNS));
            mem_quick.push_back((long long)((long long)mem_after - (long long)mem_before));
        }

        // ---------------- Heap Sort ----------------
        {
            size_t mem_before = getWorkingSetKB();

            auto start = chrono::high_resolution_clock::now();
            for (int t = 0; t < RUNS; t++) {
                for (int i = 1; i <= n; i++) arr[i] = test_batches[t][i];
                HeapSort(arr, n);
            }
            double t_heap = chrono::duration<double, micro>(chrono::high_resolution_clock::now() - start).count();

            size_t mem_after = getWorkingSetKB();

            res_heap.push_back((long long)(t_heap / RUNS));
            mem_heap.push_back((long long)((long long)mem_after - (long long)mem_before));
        }

        // ---------------- Insertion Sort ----------------
        {
            size_t mem_before = getWorkingSetKB();

            auto start = chrono::high_resolution_clock::now();
            for (int t = 0; t < RUNS; t++) {
                for (int i = 1; i <= n; i++) arr[i] = test_batches[t][i];
                insertionSort(arr, n);
            }
            double t_insertion = chrono::duration<double, micro>(chrono::high_resolution_clock::now() - start).count();

            size_t mem_after = getWorkingSetKB();

            res_insertion.push_back((long long)(t_insertion / RUNS));
            mem_insertion.push_back((long long)((long long)mem_after - (long long)mem_before));
        }

        // ---------------- Composite Sort ----------------
        {
            size_t mem_before = getWorkingSetKB();

            auto start = chrono::high_resolution_clock::now();
            for (int t = 0; t < RUNS; t++) {
                for (int i = 1; i <= n; i++) arr[i] = test_batches[t][i];
                compositeSort(arr, 1, n);
            }
            double t_composite = chrono::duration<double, micro>(chrono::high_resolution_clock::now() - start).count();

            size_t mem_after = getWorkingSetKB();

            res_composite.push_back((long long)(t_composite / RUNS));
            mem_composite.push_back((long long)((long long)mem_after - (long long)mem_before));
        }

        delete[] arr;
    }

    // 直接 5 個 cout 輸出（us）
    cout << "Merge { N=500:" << res_merge[0]
        << ", N=1000:" << res_merge[1]
        << ", N=2000:" << res_merge[2]
        << ", N=3000:" << res_merge[3]
        << ", N=4000:" << res_merge[4]
        << ", N=5000:" << res_merge[5] << " }\n";

    cout << "Quick { N=500:" << res_quick[0]
        << ", N=1000:" << res_quick[1]
        << ", N=2000:" << res_quick[2]
        << ", N=3000:" << res_quick[3]
        << ", N=4000:" << res_quick[4]
        << ", N=5000:" << res_quick[5] << " }\n";

    cout << "Heap { N=500:" << res_heap[0]
        << ", N=1000:" << res_heap[1]
        << ", N=2000:" << res_heap[2]
        << ", N=3000:" << res_heap[3]
        << ", N=4000:" << res_heap[4]
        << ", N=5000:" << res_heap[5] << " }\n";

    cout << "Insertion { N=500:" << res_insertion[0]
        << ", N=1000:" << res_insertion[1]
        << ", N=2000:" << res_insertion[2]
        << ", N=3000:" << res_insertion[3]
        << ", N=4000:" << res_insertion[4]
        << ", N=5000:" << res_insertion[5] << " }\n";

    cout << "Composite { N=500:" << res_composite[0]
        << ", N=1000:" << res_composite[1]
        << ", N=2000:" << res_composite[2]
        << ", N=3000:" << res_composite[3]
        << ", N=4000:" << res_composite[4]
        << ", N=5000:" << res_composite[5] << " }\n";



    cout << "\nPeakWorkingSet(KB): " << getPeakWorkingSetKB() << "\n\n";

    return 0;
}
```
## 效能分析

* **時間複雜度**：
  - Insertion Sort：平均/最差 O(n^2)。資料接近排序好時會比較快。
  - Quick Sort（median-of-three）：平均 O(n log n)，但最差仍可能是 O(n^2)
  - Merge Sort（iterative）：平均/最差都是 O(n log n)，但需要額外空間。
  - Heap Sort：平均/最差都是 O(n log n)。

## 測試與驗證

### 測試案例
1. Insertion Sort：使用小陣列 {5,4,3,2,1}，排序後輸出確認為遞增。
2. Quick Sort：先建立 1..n 的陣列，Permute 後再 QuickSort，最後輸出確認結果已排序。
3. Merge Sort：先產生 worst-case 資料（n=16），印出資料後進行 iterativeMergeSort，再印出確認已排序。
4. Heap Sort：
   - 測試 n = 500, 1000, 2000, 3000, 4000, 5000。
   - 每個 n 產生 10 組 random permutation，分別計時 HeapSort，取最大時間作為 worst-case 的近似值。
<img width="590" height="153" alt="image" src="https://github.com/user-attachments/assets/b483d1f0-514f-43e4-8496-d7709ff3440c" />

<img width="977" height="655" alt="image" src="https://github.com/user-attachments/assets/b05d9293-0b61-499f-b446-e940fe9b9400" />


### 結論
作業做了Insertion Sort、Quick Sort、Merge Sort、Heap Sort還有Composite Sorting，並在不同資料n下，對Worst Case和Average Cas進行了測試還有畫圖。
## 分工
由兩人共同完成，如下表所示：
| 負責人 |  負責項目(Core Contributions) | 實作內容|
| :---: | :--- | :--- | :---: |
| **金宗翰** | **演算法實作與基礎架構** | 1. 實作基礎排序演算法：`Insertion Sort` 與 `Merge Sort`（非遞迴反向版本）。<br>2. 設計與實作 `Composite Sorting` 混合排序法（小資料切換 `Insertion Sort`，大資料切換 `Heap Sort`）。<br>3. 撰寫主要計時與測試框架（包含亂數產生器與記憶體配置優化）。 |
| **陳廷鑫** | **演算法優化與效能分析** | 1. 實作高效排序演算法：`Quick Sort`（包含 `Median-of-three` 三數取中法優化）與 `Heap Sort`。<br>2. 負責進行專案第五題（Exercise 5）實驗：針對不同資料量 $n$ 的「隨機排列（Random Permutations）」進行平均執行時間測試。<br>3. 收集與整理 Worst-case / Average-case 的測試數據，並撰寫實驗心得與討論報告。 |



## 申論及開發報告
