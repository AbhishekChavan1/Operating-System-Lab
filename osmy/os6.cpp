#include<iostream>
#include <vector>
#include <limits.h>
using namespace std;

void printAllocation(const vector<int> &allocation, const vector<int> &processSize, vector<int> &blockSize) {
    cout << "ProcessID\tProcessSize\tBlockID\tBlockSize\tFragmentedMemory\n";
    for (int i = 0; i < processSize.size(); i++) {
        if (allocation[i] != -1) {
            cout<<i+1<<"\t\t"<<processSize[i]<<"\t\t"<<allocation[i]+1<<"\t\t"<<blockSize[allocation[i]]+processSize[i]<<"\t\t"<<blockSize[allocation[i]]<<endl;
        } else {
            cout << i + 1 << "\t\t" << processSize[i] << "\t\tNot Allocated\t-\t-" << endl;
        }
    }
}

void nextFit(vector<int> &blockSize, vector<int> &processSize) {
    int n = processSize.size(), m = blockSize.size();
    vector<int> allocation(n, -1);    
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (blockSize[j] >= processSize[i]) {
                allocation[i] = j;
                blockSize[j] -= processSize[i];                
                break;
            }
        }
    }    
    cout << "Next Fit Allocation:\n";
    printAllocation(allocation, processSize, blockSize);
}

void firstFit(vector<int> &blockSize, vector<int> &processSize) {
    int n = processSize.size(), m = blockSize.size();
    vector<int> allocation(n, -1);    
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (blockSize[j] >= processSize[i]) {
                allocation[i] = j;
                blockSize[j] -= processSize[i];
                break;
            }
        }
    }    
    cout << "First Fit Allocation:\n";
    printAllocation(allocation, processSize, blockSize);
}

void worstFit(vector<int> &blockSize, vector<int> &processSize) {
    int n = processSize.size(), m = blockSize.size();
    vector<int> allocation(n, -1);    
    for (int i = 0; i < n; i++) {
        int worstIdx = -1;
        for (int j = 0; j < m; j++) {
            if (blockSize[j] >= processSize[i]) {
                if (worstIdx == -1 || blockSize[j] > blockSize[worstIdx]) {
                    worstIdx = j;
                }
            }
        }
        if (worstIdx != -1) {
            allocation[i] = worstIdx;
            blockSize[worstIdx] -= processSize[i];
        }
    }    
    cout << "Worst Fit Allocation:\n";
    printAllocation(allocation, processSize, blockSize);
}

void bestFit(vector<int> &blockSize, vector<int> &processSize) {
    int n = processSize.size(), m = blockSize.size();
    vector<int> allocation(n, -1);    
    for (int i = 0; i < n; i++) {
        int bestIdx = -1;
        for (int j = 0; j < m; j++) {
            if (blockSize[j] >= processSize[i]) {
                if (bestIdx == -1 || blockSize[j] < blockSize[bestIdx]) {
                    bestIdx = j;
                }
            }
        }
        if (bestIdx != -1) {
            allocation[i] = bestIdx;
            blockSize[bestIdx] -= processSize[i];
        }
    }    
    cout << "Best Fit Allocation:\n";
    printAllocation(allocation, processSize, blockSize);
}

int main() {
    int choice, numBlocks, numProcesses;
    vector<int> blockSize, processSize;    
    cout << "Enter number of blocks: ";         cin >> numBlocks;
    cout << "Enter sizes for " << numBlocks << " blocks: ";
    for (int i = 0; i < numBlocks; i++) {
        int size;                           
        cin >> size;
        blockSize.push_back(size);
    }    
    cout << "Enter number of processes: ";      cin >> numProcesses;
    cout << "Enter sizes for " << numProcesses << " processes: ";
    for (int i = 0; i < numProcesses; i++) {
        int size;
        cin >> size;
        processSize.push_back(size);
    }
    
    cout << "Choose a Memory Allocation Strategy:\n1. First Fit\n2. Next Fit\n";
    cout << "3. Best Fit\n4. Worst Fit\nEnter your choice: ";
    cin >> choice;    
    switch(choice) {
        case 1: firstFit(blockSize, processSize);
                break;
        case 2: nextFit(blockSize, processSize);
                break;
        case 3: bestFit(blockSize, processSize);
                break;
        case 4: worstFit(blockSize, processSize);
                break;
        default: cout << "Invalid choice!" << endl;
    }
    return 0;
}