#include<iostream>
#include<algorithm>
#include<vector>
#include<climits>
#include<cmath>
using namespace std;

void fcfs(vector<int> request,int head){
    int seek = 0;
    cout<<"Sequence of requests: ";
    for(auto i: request){
        cout<<i<<" ";
        seek += abs(head-i);
        head=i;
    }
    cout<<"\nTotal seek time: "<<seek<<endl;
}

void sstf(vector<int> request,int head){
    int seek=0;
    vector<bool> visited(request.size(), false);
    cout<<"Sequence of requests: ";
    for(int i=0;i<request.size(); i++){
        int minDistance = INT_MAX, index=-1;
        for(int j=0;j<request.size();j++){
            if(!visited[j] && abs(request[j]-head)<minDistance){
                minDistance=abs(request[j]-head);
                index=j;
            }
        }
        visited[index]=true;
        seek+=abs(head-request[index]);
        head=request[index];
        cout<<head<<" ";
    }
    cout<<"\nTotal seek time: "<<seek<<endl;
}

void scan(vector<int> request, int head,int diskSize ,string direction){
    int seek = 0;
    vector<int> left, right, seekSequence;
    if (direction == "left") left.push_back(0);
    else right.push_back(diskSize - 1);
    for(int r: request){
        if(r < head) left.push_back(r);
        else right.push_back(r);
    }
    
    sort(left.begin(), left.end());
    sort(right.begin(), right.end());
    cout<<"Sequence of requests: ";

    if(direction == "left"){
        for(int i=left.size()-1; i>=0; i--) seekSequence.push_back(left[i]);
        for(int i=0; i<right.size(); i++) seekSequence.push_back(right[i]);
    } else {
        for(int i=0; i<right.size(); i++) seekSequence.push_back(right[i]);
        for(int i=left.size()-1; i>=0; i--) seekSequence.push_back(left[i]);
    }
    for(int r: seekSequence){
        cout<<r<<" ";
        seek += abs(head - r);
        head = r;
    }
    cout<<"\nTotal seek time: "<<seek<<endl;
}

void cscan(vector<int> request, int head, int diskSize){
    int seek = 0;
    vector<int> left, right, seekSequence;
    right.push_back(diskSize - 1);
    left.push_back(0);
    for(int r: request){
        if(r < head)    left.push_back(r);
        else            right.push_back(r);
    }
    sort(left.begin(), left.end());
    sort(right.begin(), right.end());
    cout<<"Sequence of requests: ";

    for(int i=0; i<right.size(); i++) seekSequence.push_back(right[i]);
    for(int i=0; i<left.size(); i++)  seekSequence.push_back(left[i]);
    for(int r: seekSequence){
        cout<<r<<" ";
        seek += abs(head - r);
        head = r;
        if(head == diskSize - 1){
            seek += abs(diskSize - 1);
            head = 0;
        } 
    }
    cout<<"\nTotal seek time: "<<seek<<endl;
}

int main(){
    int n, head, choice, diskSize;
    string direction;
    vector<int> request(n);

    cout<<"Enter the number of requests: ";     cin>>n;    
    cout<<"Enter the requests: ";
    for(int i=0;i<n;i++)    cin>>request[i];
        
    cout<<"Enter the initial head position: ";  cin>>head;
    cout<<"1: FCFS Disk Scheduling Algorithm\n2: SSTF Disk Scheduling Algorithm\n";
    cout<<"3: Scan Scheduling Algorithm\n4: C-Scan Disk Scheduling Algorithm\n";
    cin>>choice; 

    switch (choice){
        case 1: cout<<"FCFS Disk Scheduling Algorithm\n";
                fcfs(request, head);
                break;
        case 2: cout<<"SSTF Disk Scheduling Algorithm\n";
                sstf(request, head);
                break;
        case 3: cout<<"Scan Scheduling Algorithm\n";
                cout<<"Enter the disk size: ";
                cin>>diskSize;
                cout<<"Enter the direction (left/right): ";
                cin>>direction;
                scan(request, head, diskSize, direction);
                break;
        case 4: cout<<"C-Scan Disk Scheduling Algorithm\n";
                cout<<"Enter the disk size: ";
                cin>>diskSize;
                cscan(request, head, diskSize);
                break;
        default:cout<<"Invalid choice"<<endl;                
    }
    return 0;
}