#include <iostream>
using namespace std;

// Insertion Sort 函式
void insertionSort(int arr[], int n)
{
    // 從第二個元素開始
    for (int i = 1; i < n; i++)
    {
        // 儲存目前要插入的值
        int key = arr[i];

        // j 指向前一個元素
        int j = i - 1;

        // 將比 key 大的元素往右移
        while (j >= 0 && arr[j] > key)
        {
            arr[j + 1] = arr[j];
            j--;
        }

        // 將 key 插入正確位置
        arr[j + 1] = key;
    }
}

int main()
{
    // 測試資料
    int arr[] = { 5, 4, 3, 2, 1 };

    // 陣列大小
    int n = 5;

    // 呼叫 Insertion Sort
    insertionSort(arr, n);

    // 輸出排序結果
    for (int i = 0; i < n; i++)
        cout << arr[i] << " ";

    return 0;
}
