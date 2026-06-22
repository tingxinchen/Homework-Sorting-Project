#include <iostream>
#include <algorithm>
#include <chrono>
#include <climits>
#include <numeric>
#include <random>

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

// Composite Sort: Quick Sort + Insertion Sort
void compositeSort(int data[], int left, int right)
{
    const int threshold = 20;

    if (right - left + 1 <= threshold)
    {
        insertionSort(data, left, right);
        return;
    }

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

    compositeSort(data, left, j - 1);
    compositeSort(data, j + 1, right);
}

// Shuffle
void randomShuffle(int data[], int size, mt19937& generator)
{
    int* temp = new int[size];

    iota(temp, temp + size, 1);
    shuffle(temp, temp + size, generator);

    for (int i = 1; i <= size; i++)
        data[i] = temp[i - 1];

    delete[] temp;
}

// Print result
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
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    mt19937 generator(seed);

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
            randomShuffle(batches[t], n, generator);

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
