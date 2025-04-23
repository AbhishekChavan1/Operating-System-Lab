#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <limits.h>
using namespace std;

void printMemory(vector<int> &memory){
    cout << "[ ";
    for (int page : memory) {
        cout << page << " ";
    }
    cout << "]" << endl;
}

int getValidInput(bool allowZero = false){
    int value;
    while (true) {
        cin >> value;
        if (cin.fail() || value < 0 || (!allowZero && value == 0)){
            cin.clear();                                         
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
            cout<< "Invalid input! Please enter a valid positive number: "; 
        } else {
            return value;
        }
    }
}

int FCFS(vector<int> pages, int frames){
    vector<int> memory;
    int page_faults = 0;
    cout << "\nFCFS Page Placement:" << endl;
    for (int page : pages)  {
        if (find(memory.begin(), memory.end(), page) == memory.end()) { 
            if (memory.size() < frames){
                memory.push_back(page);
            }else{
                memory.erase(memory.begin());
                memory.push_back(page);
            }
            page_faults++;
        }
        printMemory(memory);
    }
    return page_faults;
}

int LRU(vector<int> pages, int frames){
    vector<int> memory;
    unordered_map<int, int> last_used;
    int page_faults = 0;
    cout << "\nLRU Page Placement:" << endl;

    for (int i = 0; i < pages.size(); i++) {
        int page = pages[i];
        if (find(memory.begin(), memory.end(), page) == memory.end()) { 
            if (memory.size() < frames) {                
                memory.push_back(page);
            }else{                                
                int lru_page = memory[0];
                int min_last_used = last_used[lru_page];
                for (int p : memory){
                    if (last_used[p] < min_last_used){
                        lru_page = p;
                        min_last_used = last_used[p];
                    }
                }                                
                memory.erase(remove(memory.begin(), memory.end(),lru_page),memory.end());
                memory.push_back(page);
            }
            page_faults++;
        }        
        last_used[page] = i;
        printMemory(memory); 
    }
    return page_faults;
}

int Optimal(vector<int> pages, int frames){
    vector<int> memory;
    int page_faults = 0;
    cout << "\nOptimal Page Placement:" << endl;
    for (int i = 0; i < pages.size(); i++){
        int page = pages[i];
        if (find(memory.begin(), memory.end(), page) == memory.end()){
            if (memory.size() < frames){
                memory.push_back(page);
            }else{       
                int farthest = -1, page_to_replace;
                for (int p : memory){
                    auto it = find(pages.begin() + i + 1, pages.end(), p);
                    int next_use = (it == pages.end()) ? INT_MAX : distance(pages.begin(), it);
                    if (next_use > farthest){
                        farthest = next_use;
                        page_to_replace = p;
                    }
                }
                memory.erase(remove(memory.begin(), memory.end(),page_to_replace),memory.end());
                memory.push_back(page);
            }
            page_faults++;
        }
        printMemory(memory);
    }
    return page_faults;
}

int main(){
    int n, frames;
    cout << "Enter number of pages: ";
    n = getValidInput();
    vector<int> pages(n);
    cout << "Enter page sequence: ";
    for (int i = 0; i < n; i++)     pages[i] = getValidInput();

    cout << "Enter number of frames: ";
    frames = getValidInput();
    int fcfs_faults = FCFS(pages, frames);
    int lru_faults = LRU(pages, frames);
    int opt_faults = Optimal(pages, frames);
    cout << "\nPage Faults Count:" << endl<< "FCFS: " << fcfs_faults << endl;
    cout << "LRU: " << lru_faults << endl<< "Optimal: " << opt_faults << endl;
    int min_faults = min({fcfs_faults, lru_faults, opt_faults});
    
    cout << "\nBest Algorithm: ";
    if (min_faults == opt_faults)       cout << "Optimal";
    else if (min_faults == lru_faults)  cout << "LRU";
    else                                cout << "FCFS";
    cout << " (Least Page Faults: " << min_faults << ")\n";
    return 0;
}