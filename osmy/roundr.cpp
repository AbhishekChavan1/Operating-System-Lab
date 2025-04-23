#include <iostream>
#include <queue>
#include <algorithm>
using namespace std;

struct Process {
    int pid, arrival_time, burst_time, remaining_time, completion_time;
    int waiting_time , turnaround_time;
};

int main() {
    int n, time_quantum;
    cout << "Enter number of processes: ";      cin >> n;
    cout << "Enter time quantum: ";             cin >> time_quantum;

    Process p[n];
    queue<int> q;
    for (int i = 0; i < n; i++) {
        p[i].pid = i + 1;
        cout << "Enter arrival time and burst time for process " << p[i].pid << ": ";
        cin >> p[i].arrival_time >> p[i].burst_time;
        p[i].remaining_time = p[i].burst_time;
    }

    int time = 0, completed = 0;
    vector<bool> visited(n, false);    
    sort(p, p + n, [](const Process &a, const Process &b) {
        return a.arrival_time < b.arrival_time;
    });

    q.push(0);
    visited[0] = true;
    while (completed < n) {
        if (q.empty()) {
            for (int i = 0; i < n; i++) {
                if (!visited[i] && p[i].arrival_time <= time) {
                    q.push(i);
                    visited[i] = true;
                    break;
                }
            }
            if (q.empty()) {
                time++;
                continue;
            }
        }

        int idx = q.front();
        q.pop();
        if (p[idx].remaining_time > time_quantum) {
            time += time_quantum;
            p[idx].remaining_time -= time_quantum;
        } else {
            time += p[idx].remaining_time;
            p[idx].remaining_time = 0;
            p[idx].completion_time = time;
            p[idx].turnaround_time = p[idx].completion_time - p[idx].arrival_time;
            p[idx].waiting_time = p[idx].turnaround_time - p[idx].burst_time;
            completed++;
        }

        for (int i = 0; i < n; i++) {
            if (!visited[i] && p[i].arrival_time <= time) {
                q.push(i);
                visited[i] = true;
            }
        }
        if (p[idx].remaining_time > 0) q.push(idx);        
    }

    double total_wt = 0, total_tat = 0;
    cout << "\nProcess ID\tArrival Time\tBurst Time\tCompletion Time\tWaiting Time\tTurnaround Time\n";
    for (int i = 0; i < n; i++) {
        total_wt += p[i].waiting_time;
        total_tat += p[i].turnaround_time;
        cout<<p[i].pid<<"\t\t"<<p[i].arrival_time<<"\t\t"<<p[i].burst_time<<"\t\t"<<p[i].completion_time
            << "\t\t" << p[i].waiting_time<< "\t\t" << p[i].turnaround_time << "\n";
    }
    cout << "\nAverage Waiting Time: " << total_wt / n<<"\nAverage Turnaround Time: " << total_tat / n << endl;
    return 0;
}