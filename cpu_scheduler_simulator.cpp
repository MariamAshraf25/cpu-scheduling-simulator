/*
Project Name: CPU Scheduling Algorithms Simulator
Author:       Mariam Ashraf
Major:        Computer Engineering
Date:         April 2025
Description:  A C++ CLI tool that simulates core OS CPU scheduling algorithms (FCFS, Non-Preemptive/Preemptive SJF, 
              Priority Scheduling, and Round Robin). It calculates waiting times and dynamically generates 
              text-based Gantt Charts for visual execution tracking.
 */
#include <bits/stdc++.h>
using namespace std;

struct Process {
    int id, arrival, burst, priority;
    int remaining, completion, start = -1, waiting, turnaround;
};

struct GanttBlock {
    int pid, start, end;
};

bool compareArrival(Process a, Process b) {
    return a.arrival < b.arrival;
}

void displayGanttChart(const vector<GanttBlock> &gantt) {
    cout << "\nGantt Chart:\n";
    for (const auto &block : gantt)
        cout << "| P" << block.pid << " ";
    cout << "|\n";

    for (const auto &block : gantt)
        cout << block.start << "    ";
    cout << gantt.back().end << "\n";
}

float FCFS(vector<Process> processes, float &total_waiting) {
    sort(processes.begin(), processes.end(), compareArrival);
    int time = 0;
    total_waiting = 0;
    vector<GanttBlock> gantt;

    for (auto &p : processes) {
        if (time < p.arrival)
            time = p.arrival;
        p.start = time;
        p.waiting = time - p.arrival;
        p.turnaround = p.waiting + p.burst;
        time += p.burst;
        total_waiting += p.waiting;
        gantt.push_back({p.id, p.start, time});
    }

    displayGanttChart(gantt);
    return total_waiting / processes.size();
}

float SJF_NonPreemptive(vector<Process> processes, float &total_waiting) {
    int time = 0, completed = 0;
    int n = processes.size();
    total_waiting = 0;
    vector<bool> done(n, false);
    vector<GanttBlock> gantt;

    while (completed < n) {
        int idx = -1, minBurst = INT_MAX;
        for (int i = 0; i < n; ++i) {
            if (!done[i] && processes[i].arrival <= time && processes[i].burst < minBurst) {
                minBurst = processes[i].burst;
                idx = i;
            }
        }
        if (idx != -1) {
            processes[idx].start = time;
            processes[idx].waiting = time - processes[idx].arrival;
            processes[idx].turnaround = processes[idx].waiting + processes[idx].burst;
            time += processes[idx].burst;
            done[idx] = true;
            total_waiting += processes[idx].waiting;
            completed++;
            gantt.push_back({processes[idx].id, processes[idx].start, time});
        } else {
            time++;
        }
    }

    displayGanttChart(gantt);
    return total_waiting / n;
}

float SJF_Preemptive(vector<Process> processes, float &total_waiting) {
    int time = 0, completed = 0;
    int n = processes.size();
    total_waiting = 0;
    vector<int> remaining(n);
    vector<GanttBlock> gantt;
    int prev = -1;

    for (int i = 0; i < n; i++) remaining[i] = processes[i].burst;

    while (completed < n) {
        int idx = -1, minRemaining = INT_MAX;
        for (int i = 0; i < n; i++) {
            if (processes[i].arrival <= time && remaining[i] > 0 && remaining[i] < minRemaining) {
                minRemaining = remaining[i];
                idx = i;
            }
        }

        if (idx != -1) {
            if (remaining[idx] == processes[idx].burst)
                processes[idx].start = time;

            if (prev != idx)
                gantt.push_back({processes[idx].id, time, time + 1});
            else
                gantt.back().end++;

            remaining[idx]--;
            if (remaining[idx] == 0) {
                processes[idx].completion = time + 1;
                processes[idx].turnaround = processes[idx].completion - processes[idx].arrival;
                processes[idx].waiting = processes[idx].turnaround - processes[idx].burst;
                total_waiting += processes[idx].waiting;
                completed++;
            }
            prev = idx;
        } else {
            prev = -1;
        }
        time++;
    }

    displayGanttChart(gantt);
    return total_waiting / n;
}

float Priority_Scheduling(vector<Process> processes, bool preemptive, float &total_waiting) {
    int n = processes.size(), time = 0, completed = 0;
    total_waiting = 0;
    vector<int> remaining(n);
    vector<bool> done(n, false);
    vector<GanttBlock> gantt;
    int prev = -1;

    for (int i = 0; i < n; i++) remaining[i] = processes[i].burst;

    while (completed < n) {
        int idx = -1, highPriority = INT_MAX;
        for (int i = 0; i < n; i++) {
            if (processes[i].arrival <= time && !done[i] && processes[i].priority < highPriority && remaining[i] > 0) {
                highPriority = processes[i].priority;
                idx = i;
            }
        }

        if (idx != -1) {
            if (preemptive) {
                if (remaining[idx] == processes[idx].burst)
                    processes[idx].start = time;

                if (prev != idx)
                    gantt.push_back({processes[idx].id, time, time + 1});
                else
                    gantt.back().end++;

                remaining[idx]--;
                if (remaining[idx] == 0) {
                    processes[idx].completion = time + 1;
                    processes[idx].turnaround = processes[idx].completion - processes[idx].arrival;
                    processes[idx].waiting = processes[idx].turnaround - processes[idx].burst;
                    total_waiting += processes[idx].waiting;
                    done[idx] = true;
                    completed++;
                }
                prev = idx;
                time++;
            } else {
                processes[idx].start = time;
                processes[idx].waiting = time - processes[idx].arrival;
                processes[idx].turnaround = processes[idx].waiting + processes[idx].burst;
                time += processes[idx].burst;
                total_waiting += processes[idx].waiting;
                done[idx] = true;
                completed++;
                gantt.push_back({processes[idx].id, processes[idx].start, time});
            }
        } else {
            prev = -1;
            time++;
        }
    }

    displayGanttChart(gantt);
    return total_waiting / n;
}

float RoundRobin(vector<Process> processes, int quantum, float &total_waiting) {
    int n = processes.size(), time = 0, completed = 0;
    total_waiting = 0;
    vector<int> remaining(n);
    queue<int> q;
    vector<bool> inQueue(n, false);
    vector<GanttBlock> gantt;

    for (int i = 0; i < n; i++) remaining[i] = processes[i].burst;

    sort(processes.begin(), processes.end(), compareArrival);
    q.push(0);
    inQueue[0] = true;

    while (!q.empty()) {
        int idx = q.front(); q.pop();
        inQueue[idx] = false;

        if (processes[idx].start == -1)
            processes[idx].start = max(time, processes[idx].arrival);

        int exec = min(quantum, remaining[idx]);
        int start_time = max(time, processes[idx].arrival);
        int end_time = start_time + exec;
        gantt.push_back({processes[idx].id, start_time, end_time});

        time = end_time;
        remaining[idx] -= exec;

        for (int i = 0; i < n; i++) {
            if (i != idx && processes[i].arrival <= time && !inQueue[i] && remaining[i] > 0)
                q.push(i), inQueue[i] = true;
        }

        if (remaining[idx] > 0) {
            q.push(idx);
            inQueue[idx] = true;
        } else {
            processes[idx].completion = time;
            processes[idx].turnaround = processes[idx].completion - processes[idx].arrival;
            processes[idx].waiting = processes[idx].turnaround - processes[idx].burst;
            total_waiting += processes[idx].waiting;
            completed++;
        }
    }

    displayGanttChart(gantt);
    return total_waiting / n;
}

int main() {
    vector<Process> processes;
    int type, quantum;
    char repeat;

    cout << "Enter number of processes: ";
    int n;
    cin >> n;

    for (int i = 0; i < n; i++) {
        Process p;
        p.id = i + 1;
        cout << "\nProcess " << i + 1 << " Arrival Time: ";
        cin >> p.arrival;
        cout << "Burst Time: ";
        cin >> p.burst;
        cout << "Priority: ";
        cin >> p.priority;
        processes.push_back(p);
    }

    do {
        cout << "\nSelect Scheduling Algorithm:\n"
             << "1. FCFS\n2. SJF Non-Preemptive\n3. SJF Preemptive\n"
             << "4. Priority Non-Preemptive\n5. Priority Preemptive\n"
             << "6. Round Robin\n"
             << "Enter your choice: ";
        cin >> type;

        float avg_waiting = 0, total_waiting = 0;

        switch (type) {
            case 1: avg_waiting = FCFS(processes, total_waiting); break;
            case 2: avg_waiting = SJF_NonPreemptive(processes, total_waiting); break;
            case 3: avg_waiting = SJF_Preemptive(processes, total_waiting); break;
            case 4: avg_waiting = Priority_Scheduling(processes, false, total_waiting); break;
            case 5: avg_waiting = Priority_Scheduling(processes, true, total_waiting); break;
            case 6:
                cout << "Enter Time Quantum: ";
                cin >> quantum;
                avg_waiting = RoundRobin(processes, quantum, total_waiting);
                break;
            default:
                cout << "Invalid choice.\n";
        }

        cout << "\nTotal Waiting Time: " << total_waiting << endl;
        cout << "Average Waiting Time: " << avg_waiting << endl;

        cout << "\nDo you want to try another algorithm? (y/n): ";
        cin >> repeat;

    } while (repeat == 'y' || repeat == 'Y');

    return 0;
}
