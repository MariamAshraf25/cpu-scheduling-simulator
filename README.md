# CPU Scheduling Algorithms Simulator (C++)

A comprehensive systems-programming utility written in C++11 that simulates and benchmarks core Operating System **CPU Scheduling Algorithms**. 

This simulator models how a processor allocates execution time to processes within the ready queue based on different scheduling paradigms, helping to analyze CPU efficiency, throughput, and process waiting times.

---

## Implemented Scheduling Strategies

The simulator provides detailed implementations for the following standard algorithms:

1. **FCFS (First-Come, First-Served):** Simplest non-preemptive algorithm scheduling processes strictly by arrival order.
2. **SJF (Shortest Job First - Non-Preemptive):** Minimizes average waiting time by executing the process with the shortest burst time first.
3. **SRTF (Shortest Remaining Time First - Preemptive SJF):** Interrupts the current process if a newly arrived process possesses a shorter remaining burst time.
4. **Priority Scheduling (Preemptive & Non-Preemptive):** Allocates the CPU based on assigned process priority ranks (lower numerical integer value represents higher priority).
5. **Round Robin (RR):** Cyclic preemptive scheduling that slices execution time into a strict **Time Quantum** using a FIFO structure (`std::queue`).

---

## Features & Visualization

* **Text-Based Gantt Chart Generation:** Automatically outputs an aligned, readable text-based Gantt Chart in the terminal, charting the exact timeline boundaries and execution sequence blocks.
* **Performance Benchmark:** Computes metrics including **Total Waiting Time** and **Average Waiting Time** for each execution run, enabling quick comparison across different strategies under identical concurrency profiles.

---

## Installation

git clone [https://github.com/MariamAshraf25/cpu-scheduling-simulator.git](https://github.com/MariamAshraf25/cpu-scheduling-simulator.git)

---

## Author
**Mariam Ashraf** | Computer Engineering Student | Faculty of Engineering - Capital University (Formerly Helwan)  
[LinkedIn Profile](https://www.linkedin.com/in/mariam-ashraf-84415b2b8)