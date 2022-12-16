#include <iostream>
#include <algorithm> 
#include <cstdlib>
#include <iomanip>
#include <string.h> 
#include <queue>

using namespace std;

struct JobList {
    int pid;
    int arrivalTime;
    int burstTime;
    int served;
    int completeTime;
    int turnTime;
    int waitTime;
    int respondTime;
};


int main() {
    
    int j, quantum, index;
    struct JobList p[100];
    float aveTurnTime, aveWaitTime, aveRespondTime, cpuUtilisation, throughPut;
    int totalTurnTime = 0, totalWaitTime = 0, totalRespondTime = 0, totalIdleTime = 0, isComplete[100];
    memset(isComplete, 0, sizeof(isComplete));
    
    cout << "Enter the number of processes: ";
    cin >> j;
    cout << "Enter the desired time quantum: ";
    cin >> quantum;
    cout << endl; 
    
    for (int i = 0; i < j; i++) {
        cout << "Enter the AT of process " << i + 1 << ": ";
        cin >> p[i].arrivalTime;
        cout << "Enter the BT of process " << i + 1 << ": ";
        cin >> p[i].burstTime;
        isComplete[i] = p[i].burstTime;
        p[i].pid = i + 1;
        cout << endl;
    }
        
        queue<int> q;
        int current_time = 0;
        q.push(0);
        int completed = 0;
        int m[100];
        memset(m, 0, sizeof(m));
        m[0] = 1;
        
        while (completed != j) {
            index = q.front();
            q.pop();
            
            if (isComplete[index] == p[index].burstTime) {
                p[index].served = max(current_time, p[index].arrivalTime);
                totalIdleTime += p[index].served - current_time;
                current_time = p[index].served;
            }
            
            if (isComplete[index] - quantum > 0) {
                isComplete[index] -= quantum;
                current_time += quantum;
            }
            else {
                current_time += isComplete[index];
                isComplete[index] = 0;
                completed++;
                
                p[index].completeTime = current_time;
                p[index].turnTime = p[index].completeTime - p[index].arrivalTime;
                p[index].waitTime = p[index].turnTime - p[index].burstTime;
                p[index].respondTime = p[index].served - p[index].arrivalTime;
                
                totalTurnTime += p[index].turnTime;
                totalWaitTime += p[index].waitTime;
                totalRespondTime += p[index].respondTime;
            }
            
            for (int i = 1; i < j; i++) {
                if (isComplete[i] > 0 && p[i].arrivalTime <= current_time && m[i] == 0) {
                    q.push(i);
                    m[i] = 1;
                }
            }
            if (isComplete[index] > 0) {
                q.push(index);
            }
            
            if (q.empty()) {
                for (int i = 1; i < j; i++) {
                    if (isComplete[i] > 0) {
                        q.push(i);
                        m[i] = 1;
                        break;
                    }
                }
            }
        }
        aveTurnTime = (float)totalTurnTime / j;
        aveWaitTime = (float)totalWaitTime / j;
        aveRespondTime = (float)totalRespondTime / j;
        cpuUtilisation = ((p[j - 1].completeTime - totalIdleTime) / (float)p[j - 1].completeTime) * 100;
        throughPut = float(j) / (p[j - 1].completeTime - p[0].arrivalTime);
        
        cout << "#P\t" << "AT\t" << "BT\t" << "ST\t" << "CT\t" << "TT\t" << "WT\t" << "RT\t" << "\n" << endl;
        
        for (int i = 0; i < j; i++) {
            cout << p[i].pid << "\t" << p[i].arrivalTime << "\t" << p[i].burstTime << "\t" << p[i].served << "\t" << p[i].completeTime << "\t" << p[i].turnTime << "\t" << p[i].waitTime << "\t" << p[i].respondTime << "\t" << "\n" << endl;
        }
            
            cout << "Here are the average values: " << endl;
            cout << "Turnaround time: " << aveTurnTime << endl;
            cout << "Waiting time: " << aveWaitTime << endl;
            cout << "Response time: " << aveRespondTime << endl;
            cout << "CPU utilization: " << cpuUtilisation << "%" << endl;
            cout << "Throughput: " << throughPut << " process/unit time" << endl;
            
}

