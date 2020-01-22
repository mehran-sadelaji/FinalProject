#include<iostream>
#include<vector>

using namespace std;

void swap(int &large, int &small){
    int temp = large;
    large = small;
    small = temp;
}

int partitionFn(vector<int>& myArr, int start, int end){
    int pivot = myArr[end];
    int i = start;
    for(int j = start; j <= end-1; j++){
        if(myArr[j] <= pivot){
            swap(myArr[j], myArr[i]);
            i++;
        }
    }
    swap(myArr[i], myArr[end]);
    return i;
}

void quickSort(vector<int>& myArr, int start, int end){
    if(end > start){
        int partition = partitionFn(myArr, start, end);
        quickSort(myArr, start, partition-1);
        quickSort(myArr, partition+1, end);
    }
}

void quickSort(vector<int>& myArr){
    int start = 0;
    int end = myArr.size()-1;
    quickSort(myArr, start, end);
}