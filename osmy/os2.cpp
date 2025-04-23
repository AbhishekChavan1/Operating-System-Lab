#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <climits>
using namespace std;

struct Process {
    int id, remaining_time, arrival_time, burst_time, priority;
    int completion_time, waiting_time, turnaround_time;    
};

void printsfun(vector<Process>& proc, bool show_priority) {
    int n = proc.size();
    int asum = 0, atat = 0;
    cout << "Process ID\tArrival Time\tBurst Time";
    if (show_priority) cout << "\tPriority";
    cout << "\tCompletion Time\tWaiting Time\tTurnaround Time\n";
    
    for(auto& p : proc) {
        asum += p.waiting_time;
        atat += p.turnaround_time;
        cout << p.id << "\t\t" << p.arrival_time << "\t\t"<< p.burst_time;
        if (show_priority) cout << "\t\t" << p.priority;
        cout << "\t\t" << p.completion_time << "\t\t" << p.waiting_time << "\t\t" <<p.turnaround_time << endl;
    }
    cout << "Average waiting time: " << (double)asum/n << endl;
    cout << "Average turnaround time: " << (double)atat/n << endl;
}

void FCFS(vector<Process>& proc) {
    int n = proc.size();
    sort(proc.begin(), proc.end(), [](Process a, Process b) { return a.arrival_time < b.arrival_time; });
    proc[0].completion_time = proc[0].arrival_time + proc[0].burst_time;
    proc[0].waiting_time = 0;
    proc[0].turnaround_time = proc[0].burst_time;

    for (int i = 1; i < n; i++) {
        int start_time = max(proc[i].arrival_time, proc[i-1].completion_time);
        proc[i].completion_time = start_time + proc[i].burst_time;
        proc[i].turnaround_time = proc[i].completion_time - proc[i].arrival_time;
        proc[i].waiting_time = proc[i].turnaround_time - proc[i].burst_time;
    }    
    printsfun(proc, false);
}

void SJF(vector<Process>& proc) {    
    int n = proc.size(), current_time = 0, completed = 0;
    vector<bool> is_completed(n, false);

    while (completed != n) {
        int shortest_job = -1, min_burst = INT_MAX;        
        for (int i = 0; i < n; i++) {
            if (!is_completed[i] && proc[i].arrival_time <= current_time) {
                if (proc[i].burst_time < min_burst) {
                    min_burst = proc[i].burst_time;
                    shortest_job = i;
                }                
                else if (proc[i].burst_time == min_burst) {
                    if (proc[i].arrival_time < proc[shortest_job].arrival_time) {
                        shortest_job = i;
                    }
                }
            }
        }
        if (shortest_job == -1) { current_time++; } 
        else {            
            proc[shortest_job].completion_time = current_time + proc[shortest_job].burst_time;
            proc[shortest_job].turnaround_time = proc[shortest_job].completion_time - proc[shortest_job].arrival_time;
            proc[shortest_job].waiting_time = proc[shortest_job].turnaround_time - proc[shortest_job].burst_time;                        
            current_time = proc[shortest_job].completion_time;
            is_completed[shortest_job] = true;
            completed++;
        }
    }
    printsfun(proc, false);
}

void SJFPreemptive(vector<Process>& proc) {
    int n = proc.size();   
    vector<int> remaining_time(n);
    vector<bool> is_completed(n, false);    
    for (int i = 0; i < n; i++)     remaining_time[i] = proc[i].burst_time;    
    int current_time = 0, completed = 0;    

    while (completed != n) {
        int shortest_job = -1, min_remaining = INT_MAX;        
        for (int i = 0; i < n; i++) {
            if (!is_completed[i] && proc[i].arrival_time <= current_time) {
                if (remaining_time[i] < min_remaining) {
                    min_remaining = remaining_time[i];
                    shortest_job = i;
                }                
                else if (remaining_time[i] == min_remaining) {
                    if (proc[i].arrival_time < proc[shortest_job].arrival_time) {
                        shortest_job = i;
                    }
                }
            }
        }
        
        if (shortest_job == -1) {  current_time++; } 
        else {            
            remaining_time[shortest_job]--;
            current_time++;
            if (remaining_time[shortest_job] == 0) {
                proc[shortest_job].completion_time = current_time;
                proc[shortest_job].turnaround_time = proc[shortest_job].completion_time - proc[shortest_job].arrival_time;
                proc[shortest_job].waiting_time = proc[shortest_job].turnaround_time - proc[shortest_job].burst_time;                
                is_completed[shortest_job] = true;
                completed++;
            }
        }
    }
    printsfun(proc, false);
}

void PriorityNonPreemptive(vector<Process>& proc) {
    int n = proc.size(), current_time = 0, completed_count = 0;
    vector<bool> completed(n, false);        
    while (completed_count < n) {
        int idx = -1,    highest_priority = INT_MAX;
        for (int i = 0; i < n; i++) {
            if (!completed[i] && proc[i].arrival_time <= current_time) {
                if (proc[i].priority < highest_priority) {
                    highest_priority = proc[i].priority;
                    idx = i;
                } else if (proc[i].priority == highest_priority) {
                    if (proc[i].arrival_time < proc[idx].arrival_time)
                        idx = i;
                }
            }
        }
        if (idx == -1) { current_time++; } 
        else {
            proc[idx].waiting_time = current_time - proc[idx].arrival_time;
            proc[idx].completion_time = current_time + proc[idx].burst_time;
            proc[idx].turnaround_time = proc[idx].completion_time - proc[idx].arrival_time;
            current_time += proc[idx].burst_time;
            completed[idx] = true;
            completed_count++;
        }
    }
    printsfun(proc,true);
}

void PriorityPreemptive(vector<Process>& proc) {
    int n = proc.size(), completed = 0, time = 0, idx = -1;
    for (int i = 0; i < n; i++)        proc[i].remaining_time = proc[i].burst_time;
            
    while (completed < n) {
        int highest_priority = INT_MAX;
        idx = -1;
        for (int i = 0; i < n; i++) {
            if (proc[i].arrival_time <= time && proc[i].remaining_time > 0) {
                if (proc[i].priority < highest_priority) {
                    highest_priority = proc[i].priority;
                    idx = i;
                } else if (proc[i].priority == highest_priority) {
                    if (proc[i].arrival_time < proc[idx].arrival_time)
                        idx = i;
                }
            }
        }
        if (idx == -1) {
            time++;
            continue;
        }
        proc[idx].remaining_time--;
        if (proc[idx].remaining_time == 0) {
            completed++;
            int finish_time = time + 1;
            proc[idx].completion_time = finish_time;
            proc[idx].turnaround_time = finish_time - proc[idx].arrival_time;
            proc[idx].waiting_time = proc[idx].turnaround_time - proc[idx].burst_time;
        }
        time++;
    }
    printsfun(proc,true);
}

/*
void RoundRobin(vector<Process>& proc, int quantum) {
    int n = proc.size(), time = 0, min_arrival = INT_MAX;
    for (int i = 0; i < n; i++)     proc[i].remaining_time = proc[i].burst_time;
        
    queue<int> q;
    vector<bool> inQueue(n, false);    
    for (int i = 0; i < n; i++) {
        if (proc[i].arrival_time < min_arrival)
            min_arrival = proc[i].arrival_time;
    }
    time = min_arrival;    
    for (int i = 0; i < n; i++) {
        if (proc[i].arrival_time <= time) {
            q.push(i);
            inQueue[i] = true;
        }
    }    
    while (!q.empty()) {
        int idx = q.front();
        q.pop();
        int exec_time = min(quantum, proc[idx].remaining_time);
        proc[idx].remaining_time -= exec_time;
        time += exec_time;
        
        for (int i = 0; i < n; i++) {
            if (!inQueue[i] && proc[i].arrival_time <= time) {
                q.push(i);
                inQueue[i] = true;
            }
        } 
        if (proc[idx].remaining_time > 0) { q.push(idx); } 
        else {
            proc[idx].completion_time = time;
            proc[idx].turnaround_time = time - proc[idx].arrival_time;
            proc[idx].waiting_time = proc[idx].turnaround_time - proc[idx].burst_time;
        }
        if (q.empty()) {
            for (int i = 0; i < n; i++) {
                if (proc[i].remaining_time > 0) {
                    q.push(i);
                    inQueue[i] = true;
                    break;
                }
            }
        }
    }
    printsfun(proc, false);
}
*/
int main() {
    int n, choice,quantum = 0;
    cout << "Enter the number of proc: ";   cin >> n;    
    cout << "\nScheduling Options:" << endl << "1. First Come First Serve (FCFS)" << endl << "2. SJF Non-Preemptive" << endl;
    cout << "3. SJF Preemptive" << endl << "4. Priority Scheduling Non-Preemptive" << endl;
    cout << "5. Priority Scheduling Preemptive" << endl<< "6. Round Robin" << endl << "Enter your choice: ";
    cin >> choice;
    
    vector<Process> proc(n);    
    for (int i = 0; i < n; i++) {
        cout << "\nEnter details for Proc"<<i+1<<endl;      proc[i].id = i + 1;
        cout << "Arrival Time: ";                           cin>>proc[i].arrival_time;
        cout << "Burst Time: ";                             cin>>proc[i].burst_time;        
        if (choice == 4 || choice == 5) {
            cout << "Priority (lower number means higher priority): ";
            cin >> proc[i].priority;
        }
        else     proc[i].priority = 0;        
    }        
    if (choice == 6) {
        cout << "Enter Time Quantum for Round Robin: ";
        cin >> quantum;
    }
    cout << "\nScheduling Results:\n" << endl;
    switch(choice) {
        case 1: FCFS(proc);                            break;
        case 2: SJF(proc);                             break;
        case 3: SJFPreemptive(proc);                   break;
        case 4: PriorityNonPreemptive(proc);           break;
        case 5: PriorityPreemptive(proc);              break;
//        case 6: RoundRobin(proc, quantum);             break;
        default:cout << "Invalid choice." << endl;     break;
    }
    return 0;
}