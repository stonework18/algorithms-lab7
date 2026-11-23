#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>
#include <cmath>
using namespace std;
using namespace chrono;
int partition(int arr[], int low, int high) {
    int pivot = arr[high];
    int i = low - 1;
    for (int j = low; j < high; j++) {
        if (arr[j] <= pivot) {
            i++; 
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i + 1], arr[high]);
    return i + 1;
}
int medianOfThree(int arr[], int low, int high) {
    int mid = low + (high - low) / 2;
    if (arr[low] > arr[mid])  swap(arr[low], arr[mid]);
    if (arr[low] > arr[high]) swap(arr[low], arr[high]);
    if (arr[mid] > arr[high]) swap(arr[mid], arr[high]);
    swap(arr[mid], arr[high]);
    return arr[high];
}
void insertionSort(int arr[], int low, int high) {
    for (int i = low + 1; i <= high; i++) {
        int key = arr[i];
        int j = i - 1;
        while (j >= low && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}
void heapify(int arr[], int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;
    if (left  < n && arr[left]  > arr[largest]) largest = left;
    if (right < n && arr[right] > arr[largest]) largest = right;
    if (largest != i) {
        swap(arr[i], arr[largest]);
        heapify(arr, n, largest);
    }
}

void heapSort(int arr[], int n) {
    for (int i = n / 2 - 1; i >= 0; i--) heapify(arr, n, i);
    for (int i = n - 1; i > 0; i--) {
        swap(arr[0], arr[i]);
        heapify(arr, i, 0);
    }
}

void merge(int arr[], int left, int mid, int right) {
    int n1 = mid - left + 1, n2 = right - mid;
    int* L = new int[n1];
    int* R = new int[n2];
    for (int i = 0; i < n1; i++) L[i] = arr[left + i];
    for (int j = 0; j < n2; j++) R[j] = arr[mid + 1 + j];
    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) arr[k++] = (L[i] <= R[j] ? L[i++] : R[j++]);
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];
    delete[] L;
    delete[] R;
}

void mergeSort(int arr[], int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}

void quickSort(int arr[], int low, int high) {
    if (low < high) {
        if (high - low > 16) medianOfThree(arr, low, high);
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

void introsortUtil(int arr[], int low, int high, int depthLimit) {
    int size = high - low + 1;
    if (size <= 16) {
        insertionSort(arr, low, high);
        return;
    }
    if (depthLimit == 0) {
        heapSort(arr + low, size);
        return;
    }
    medianOfThree(arr, low, high);
    int pi = partition(arr, low, high);
    introsortUtil(arr, low, pi - 1, depthLimit - 1);
    introsortUtil(arr, pi + 1, high, depthLimit - 1);
}

void introSort(int arr[], int n) {
    if (n < 2) return;
    int depthLimit = 2 * (int)log2((double)n);
    introsortUtil(arr, 0, n - 1, depthLimit);
}

int main() {
    const int N = 100000;
    mt19937 rng(42);
    uniform_int_distribution<int> d(0, N * 10);

    vector<int> base(N);
    for (auto& x : base) x = d(rng);

    auto a = base, b = base, c = base, d2 = base, e = base;

    auto t0 = high_resolution_clock::now();
    quickSort(a.data(), 0, N - 1);
    auto t1 = high_resolution_clock::now();
    cout << "quick: " << duration<double, milli>(t1 - t0).count() << " ms\n";

    t0 = high_resolution_clock::now();
    mergeSort(b.data(), 0, N - 1);
    t1 = high_resolution_clock::now();
    cout << "merge: " << duration<double, milli>(t1 - t0).count() << " ms\n";

    t0 = high_resolution_clock::now();
    heapSort(c.data(), N);
    t1 = high_resolution_clock::now();
    cout << "heap:  " << duration<double, milli>(t1 - t0).count() << " ms\n";

    t0 = high_resolution_clock::now();
    introSort(d2.data(), N);
    t1 = high_resolution_clock::now();
    cout << "intro: " << duration<double, milli>(t1 - t0).count() << " ms\n";

    t0 = high_resolution_clock::now();
    sort(e.begin(), e.end());
    t1 = high_resolution_clock::now();
    cout << "std:   " << duration<double, milli>(t1 - t0).count() << " ms\n";

    bool ok = is_sorted(a.begin(), a.end())
        && is_sorted(b.begin(), b.end())
        && is_sorted(c.begin(), c.end())
        && is_sorted(d2.begin(), d2.end())
        && is_sorted(e.begin(), e.end());
    cout << (ok ? "ALL OK" : "FAIL!") << "\n";

    return 0;
}









