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
#include <algorithm>
#include <chrono>
#include <climits>
#include <cstdlib>

using namespace std;

// Insertion Sort
void insertionSort(int data[], int left, int right)
{
    for (int i = left + 1; i <= right; i++)
    {
        int current = data[i];
        int j = i - 1;

        while (j >= left && data[j] > current)
        {
            data[j + 1] = data[j];
            j--;
        }

        data[j + 1] = current;
    }
}

// Quick Sort
int pickPivot(int data[], int left, int right)
{
    int mid = (left + right) / 2;

    if (data[mid] < data[left])
        swap(data[mid], data[left]);

    if (data[right] < data[left])
        swap(data[right], data[left]);

    if (data[right] < data[mid])
        swap(data[right], data[mid]);

    swap(data[left], data[mid]);

    return data[left];
}

void quickSort(int data[], int left, int right)
{
    if (left >= right)
        return;

    int pivot = pickPivot(data, left, right);

    int i = left;
    int j = right + 1;

    do
    {
        do i++;
        while (data[i] < pivot);

        do j--;
        while (data[j] > pivot);

        if (i < j)
            swap(data[i], data[j]);

    } while (i < j);

    swap(data[left], data[j]);

    quickSort(data, left, j - 1);
    quickSort(data, j + 1, right);
}

// Heap Sort
void heapAdjust(int data[], int root, int size)
{
    int saved = data[root];
    int child;

    for (child = 2 * root; child <= size; child *= 2)
    {
        if (child < size && data[child] < data[child + 1])
            child++;

        if (saved >= data[child])
            break;

        data[child / 2] = data[child];
    }

    data[child / 2] = saved;
}

void heapSort(int data[], int size)
{
    for (int i = size / 2; i >= 1; i--)
        heapAdjust(data, i, size);

    for (int i = size - 1; i >= 1; i--)
    {
        swap(data[1], data[i + 1]);
        heapAdjust(data, 1, i);
    }
}

// Merge Sort
void mergeTwo(int data[], int left, int mid, int right)
{
    int leftSize = mid - left + 1;
    int rightSize = right - mid;

    int* leftPart = new int[leftSize];
    int* rightPart = new int[rightSize];

    for (int i = 0; i < leftSize; i++)
        leftPart[i] = data[left + i];

    for (int i = 0; i < rightSize; i++)
        rightPart[i] = data[mid + 1 + i];

    int i = 0;
    int j = 0;
    int k = left;

    while (i < leftSize && j < rightSize)
    {
        if (leftPart[i] <= rightPart[j])
            data[k++] = leftPart[i++];
        else
            data[k++] = rightPart[j++];
    }

    while (i < leftSize)
        data[k++] = leftPart[i++];

    while (j < rightSize)
        data[k++] = rightPart[j++];

    delete[] leftPart;
    delete[] rightPart;
}

void mergeSort(int data[], int size)
{
    for (int width = 1; width < size; width *= 2)
    {
        for (int left = 1; left <= size - width; left += 2 * width)
        {
            int mid = min(left + width - 1, size);
            int right = min(left + 2 * width - 1, size);

            if (mid < right)
                mergeTwo(data, left, mid, right);
        }
    }
}

// Composite Sort
void compositeSort(int data[], int left, int right)
{
    int threshold = 20;

    if (right - left + 1 <= threshold)
        insertionSort(data, left, right);
    else
        heapSort(data, right);
}

// Shuffle
void randomShuffle(int data[], int size)
{
    for (int i = size; i >= 2; i--)
    {
        int j = rand() % i + 1;
        swap(data[i], data[j]);
    }
}

// 印結果
void printResult(string name, long long times[], int testSizes[])
{
    cout << name << " (us): ";

    for (int i = 0; i < 6; i++)
    {
        cout << "N=" << testSizes[i]
            << ":" << times[i];

        if (i != 5)
            cout << ", ";
    }

    cout << endl;
}

int main()
{
    srand(42);

    const int RUNS = 100;
    int testSizes[6] = { 500, 1000, 2000, 3000, 4000, 5000 };

    long long timesMerge[6];
    long long timesQuick[6];
    long long timesHeap[6];
    long long timesInsertion[6];
    long long timesComposite[6];

    for (int s = 0; s < 6; s++)
    {
        int n = testSizes[s];

        int** batches = new int* [RUNS];

        for (int i = 0; i < RUNS; i++)
            batches[i] = new int[n + 2];

        for (int t = 0; t < RUNS; t++)
        {
            for (int i = 1; i <= n; i++)
                batches[t][i] = i;

            randomShuffle(batches[t], n);
        }

        int* data = new int[n + 2];
        data[n + 1] = INT_MAX;

        chrono::high_resolution_clock::time_point start;
        chrono::high_resolution_clock::time_point end;
        double total;

        // Merge
        start = chrono::high_resolution_clock::now();

        for (int t = 0; t < RUNS; t++)
        {
            for (int i = 1; i <= n; i++)
                data[i] = batches[t][i];

            mergeSort(data, n);
        }

        end = chrono::high_resolution_clock::now();
        total = chrono::duration<double, micro>(end - start).count();
        timesMerge[s] = total / RUNS;

        // Quick
        start = chrono::high_resolution_clock::now();

        for (int t = 0; t < RUNS; t++)
        {
            for (int i = 1; i <= n; i++)
                data[i] = batches[t][i];

            quickSort(data, 1, n);
        }

        end = chrono::high_resolution_clock::now();
        total = chrono::duration<double, micro>(end - start).count();
        timesQuick[s] = total / RUNS;

        // Heap
        start = chrono::high_resolution_clock::now();

        for (int t = 0; t < RUNS; t++)
        {
            for (int i = 1; i <= n; i++)
                data[i] = batches[t][i];

            heapSort(data, n);
        }

        end = chrono::high_resolution_clock::now();
        total = chrono::duration<double, micro>(end - start).count();
        timesHeap[s] = total / RUNS;

        // Insertion
        start = chrono::high_resolution_clock::now();

        for (int t = 0; t < RUNS; t++)
        {
            for (int i = 1; i <= n; i++)
                data[i] = batches[t][i];

            insertionSort(data, 1, n);
        }

        end = chrono::high_resolution_clock::now();
        total = chrono::duration<double, micro>(end - start).count();
        timesInsertion[s] = total / RUNS;

        // Composite
        start = chrono::high_resolution_clock::now();

        for (int t = 0; t < RUNS; t++)
        {
            for (int i = 1; i <= n; i++)
                data[i] = batches[t][i];

            compositeSort(data, 1, n);
        }

        end = chrono::high_resolution_clock::now();
        total = chrono::duration<double, micro>(end - start).count();
        timesComposite[s] = total / RUNS;

        delete[] data;

        for (int i = 0; i < RUNS; i++)
            delete[] batches[i];

        delete[] batches;
    }

    printResult("Merge", timesMerge, testSizes);
    printResult("Quick", timesQuick, testSizes);
    printResult("Heap", timesHeap, testSizes);
    printResult("Insertion", timesInsertion, testSizes);
    printResult("Composite", timesComposite, testSizes);

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
3. Merge Sort：先產生 worst-case 資料，印出資料後進行 iterativeMergeSort，再印出確認已排序。
4. Heap Sort：
   - 測試 n = 500, 1000, 2000, 3000, 4000, 5000。
   - 每個 n 產生 10 組 random permutation，分別計時 HeapSort，取最大時間作為 worst-case 的近似值。

<img width="600" height="77" alt="image" src="https://github.com/user-attachments/assets/2abee0df-0ae9-4d1a-8147-d2fcf9bbb291" />

<img width="563" height="469" alt="image" src="https://github.com/user-attachments/assets/77f983c7-8b2e-42bb-84a0-6f5b2094f97d" />

<img width="596" height="79" alt="image" src="https://github.com/user-attachments/assets/af6c1d92-b697-4ae9-8cb5-79a70bfb6c59" />

<img width="817" height="344" alt="image" src="https://github.com/user-attachments/assets/a11c6758-231c-468c-94db-1dfb465acfe9" />


<img width="984" height="653" alt="image" src="https://github.com/user-attachments/assets/7d037245-fd3b-4414-b2fb-1d982c87e165" />

紀錄不同排序的平均時間

<img width="590" height="153" alt="image" src="https://github.com/user-attachments/assets/b483d1f0-514f-43e4-8496-d7709ff3440c" />

<img width="977" height="655" alt="image" src="https://github.com/user-attachments/assets/b05d9293-0b61-499f-b446-e940fe9b9400" />


## 結論
作業做了Insertion Sort、Quick Sort、Merge Sort、Heap Sort還有Composite Sorting，並在不同資料n下，對Worst Case和Average Cas進行了測試還有畫圖。
### 分工

| 負責人 | 實作部分 |報告部分|
|--------|----------|---------|
| 陳廷鑫 | Quick Sort、Heap Sort |說明、時間複雜度|
| 金宗翰 | Insertion Sort、Merge Sort |圖表製作、測試案例|

### 一起做的

- **程式碼整合：** Composite Sorting



## 申論及開發報告
41343119:我們一開始是先各自一個人負責兩個排序的程式，各自做好後再合併。在做的時候merge的worst case其實有點不太好理解上網查了一些資料才找到方法。然後在合併的時候才意識到insertion、merge和quick、heap讀取索引的開頭不太一樣，所以後來有對這部分做些修改才有辦法套用同一個輸入。為了要符合需求，我們各自做的也經過了許多的改版，一開始最主要測試能夠正確排序，然後再增加測資數量與紀錄時間。
41343128:在本次排序法作業的過程中，從最基本的排序程開始修改，一開始我先把Quick Sort和 Heap Sort 的基礎完成，確認可以正常排序之後，再慢慢加入效能測試與資料生成的功能，在設計測試時，用作業提供的函式來生成亂數，來跑出需要使用的時間。
