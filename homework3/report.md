
# 第九組
# 41343128 陳廷鑫 、41343119 金宗翰
## 解題說明

### 解題策略和步驟
1. 作業要求先分別完成各種排序法的基本版本，用小資料確認結果正確
2. Quick Sort 的 pivot 規定使用 median-of-three，取 left 、mid 、right 三者的中位數來降低遇到接近排序好資料時，會變慢的問題
3. Merge Sort 規定使用迭代，每合併完一輪，合併的區間寬度就乘以2，直到整條陣列都合併完。    
4. worst-case 測資部分：
   - Insertion Sort：使用反向序列 (n, n-1, ..., 1)。
   - Merge Sort：用 generateWorstCase() 產生讓 merge 過程比較吃力的排列。
   - Heap Sort：課本使用 random permutation 產生多組測資，測很多次後取最大值來近似 worst-case。
5. average-case 測資部分：使用 random permutation（隨機排列）多次測試，再取平均。

## 程式實作
以下是程式碼：
```cpp

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
<img width="1047" height="125" alt="image" src="https://github.com/user-attachments/assets/48100456-0b7f-4226-ab5c-b3496dca14d8" />

<img width="998" height="655" alt="image" src="https://github.com/user-attachments/assets/9541b7de-08c4-4e38-9b69-6010bf3a48a5" />

### 結論



## 申論及開發報告
1. 本階段先以「先能跑、先能驗證正確」為目標，把各排序法分開寫成獨立檔案，方便逐一除錯。
2. 測資部分先完成 random permutation（Permute），並依作業說明用「多次抽樣取最大值」的方式近似 Heap Sort 的 worst-case。
3. 報告中效能部分目前先整理理論複雜度；實測數據（表格/圖）會在整合 benchmark 完成後補上。
