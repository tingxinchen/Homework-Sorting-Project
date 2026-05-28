#include <iostream>
#include <vector>
using namespace std;


void merge(int arr[], int left, int mid, int right)
{
    int n1 = mid - left + 1;

    int n2 = right - mid;

    int* L = new int[n1];
    int* R = new int[n2];

    for (int i = 0; i < n1; i++)
        L[i] = arr[left + i];

    for (int i = 0; i < n2; i++)
        R[i] = arr[mid + 1 + i];

    int i = 0, j = 0, k = left;

    while (i < n1 && j < n2)
    {
        if (L[i] <= R[j])
            arr[k++] = L[i++];
        else
            arr[k++] = R[j++];
    }

    
    while (i < n1)
        arr[k++] = L[i++];

    while (j < n2)
        arr[k++] = R[j++];

    delete[] L;
    delete[] R;
}

// Iterative Merge Sort
void iterativeMergeSort(int arr[], int n)
{
    for (int size = 1; size < n; size *= 2)
    {
        for (int left = 0; left < n - 1; left += 2 * size)
        {
            int mid = min(left + size - 1, n - 1);

            int right = min(left + 2 * size - 1, n - 1);

            if (mid < right)
                merge(arr, left, mid, right);
        }
    }
}

void generateWorstCase(vector<int>& arr, int left, int right)
{
    if (right - left <= 1)
        return;

    // 暫存陣列
    vector<int> temp;

    for (int i = left; i < right; i += 2)
        temp.push_back(arr[i]);

    for (int i = left + 1; i < right; i += 2)
        temp.push_back(arr[i]);

    for (int i = left; i < right; i++)
        arr[i] = temp[i - left];

    int mid = (left + right) / 2;

    generateWorstCase(arr, left, mid);

    generateWorstCase(arr, mid, right);
}

int main()
{

    int n = 16;

    vector<int> worstCase;

    for (int i = 1; i <= n; i++)
        worstCase.push_back(i);

    generateWorstCase(worstCase, 0, n);

    cout << "Worst Case Data:\n";

    for (int x : worstCase)
        cout << x << " ";

    cout << "\n\n";


    int* arr = new int[n];

    for (int i = 0; i < n; i++)
        arr[i] = worstCase[i];


    iterativeMergeSort(arr, n);

    cout << "After Merge Sort:\n";

    for (int i = 0; i < n; i++)
        cout << arr[i] << " ";


    delete[] arr;

    return 0;
}
