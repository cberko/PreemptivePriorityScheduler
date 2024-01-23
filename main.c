//
// Created by Ceylanberk Tola on 8.01.2024.
//
#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_PROCESSES 1000

int currentTime = 0;
int checkFlag = 0;
typedef struct {
    char instruction[20];
    int executionTime;
} InstructionSet;
typedef struct {
    char processID[5]; //Process struct with its attributes
    int priority;
    int arrivalTime;
    char type[10];
    int index;
    int *executionTimes;
    int currentInstruction;
    int totalInstructions;
    int execQuant;
    int execTime;
    int waitingTime;
    int turnAroundTime;
    int burstTime;
    int completeTime;
    int lastEnterTime;
} Process;
Process *lasExecutedProcess;
InstructionSet instructions[] = { //Hardcoded instructions

  {"instr1", 90},
  {"instr2", 80},
  {"instr3", 70},
  {"instr4", 60},
  {"instr5", 50},
  {"instr6", 40},
  {"instr7", 30},
  {"instr8", 20},
  {"instr9", 30},
  {"instr10", 40},
  {"instr11", 50},
  {"instr12", 60},
  {"instr13", 70},
  {"instr14", 80},
  {"instr15", 90},
  {"instr16", 80},
  {"instr17", 70},
  {"instr18", 60},
  {"instr19", 50},
  {"instr20", 40},
  {"exit", 10}
};
//Set instructions time for each process
int process1[] = {90, 50, 90, 60, 70, 80, 40, 90, 60, 70, 80, 10};
int process2[] = {60, 80, 50, 40, 50, 40, 50, 40, 10};
int process3[] = {20, 30, 60, 50, 70, 80, 50, 20, 30, 10};
int process4[] = {30, 80, 50, 30, 80, 10};
int process5[] = {30, 80, 50, 30, 80, 80, 50, 30, 80, 50, 10};
int process6[] = {40, 30, 40, 50, 60, 50, 30, 40, 30, 40, 50, 60, 50, 30, 10};
int process7[] = {20, 90, 40, 50, 80, 10};
int process8[] = {80, 60, 70, 90, 10};
int process9[] = {50, 60, 30, 90, 30, 10};
int process10[] = {40, 70, 50, 50, 80, 50, 20, 70, 80, 10};



void assignInstructions(Process *p) {
  //It's a pretty basic method that assigns the instructions to the process
  if (strcmp(p->processID, "P1") == 0) {
    p->executionTimes = process1;
    p->totalInstructions = 12;
    p->currentInstruction = 0;
    p->execQuant = 0;
    p->execTime =790;
  }
  else if (strcmp(p->processID, "P2") == 0) {
    p->executionTimes = process2;
    p->totalInstructions = 9;
    p->currentInstruction = 0;
    p->execQuant = 0;
    p->execTime = 420;
  }
  else if(strcmp(p->processID, "P3") == 0) {
    p->executionTimes= process3;
    p->totalInstructions = 10;
    p->currentInstruction = 0;
    p->execQuant = 0;
    p->execTime = 420;
  }
  else if(strcmp(p->processID, "P4") == 0) {
    p->executionTimes = process4;
    p->totalInstructions = 6;
    p->currentInstruction = 0;
    p->execQuant = 0;
    p->execTime = 280;
  }
  else if(strcmp(p->processID, "P5") == 0) {
    p->executionTimes = process5;
    p->totalInstructions = 11;
    p->currentInstruction = 0;
    p->execQuant = 0;
    p->execTime = 570;
  }
  else if(strcmp(p->processID, "P6") == 0) {
    p->executionTimes = process6;
    p->totalInstructions = 15;
    p->currentInstruction = 0;
    p->execQuant = 0;
    p->execTime = 610;
  }
  else if(strcmp(p->processID, "P7") == 0) {
    p->executionTimes = process7;
    p->totalInstructions = 6;
    p->currentInstruction = 0;
    p->execQuant = 0;
    p->execTime = 290;
  
  }
  else if(strcmp(p->processID, "P8") == 0) {
    p->executionTimes = process8;
    p->totalInstructions = 5;
    p->currentInstruction = 0;
    p->execQuant = 0;
    p->execTime = 310;
   
  }
  else if(strcmp(p->processID, "P9") == 0) {
    p->executionTimes = process9;
    p->totalInstructions = 6;
    p->currentInstruction = 0;
    p->execQuant = 0;
    p->execTime = 270;
  }
  else if (strcmp(p->processID, "P10") == 0) {
    p->executionTimes = process10;
    p->totalInstructions = 10;
    p->currentInstruction = 0;
    p->execQuant = 0;
    p->execTime = 520;
  }
}
int numReadyProcesses = 0;
void addReadyProcess(Process *p, Process *ReadyQueue[]) {
  //adds the process to the ready queue
  ReadyQueue[numReadyProcesses] = p;
  numReadyProcesses++;
  //printf("Process %s is added to ready queue\n", p->processID);
  //printf("NUM READY PROCESSES: %d\n", numReadyProcesses);
  //printf("current time: %d\n", currentTime);
}
Process removeFromReadyQueue(Process p, Process *ReadyQueue[]) {
  //removes the process from the ready queue
  Process *dummy;
  for(int i = 0; i <ReadyQueue; i++) {
    if(strcmp(ReadyQueue[i]->processID, p.processID) == 0) {
      dummy = ReadyQueue[i];
      for(int j = i; j < numReadyProcesses; j++) {
        ReadyQueue[j] = ReadyQueue[j+1];
      }
      numReadyProcesses--;
      //printf("Process %s is removed from ready queue\n", dummy->processID);
      //printf("NUM READY PROCESSES: %d\n", numReadyProcesses);
      //printf("current time: %d\n", currentTime);
      return *dummy;
    }
  }
  //printf("Process not found\n");
  exit(1);
}

Process* selectNewProcess(Process *ReadyQueue[]) {
  //selects the process to execute
  Process *selectedPlat;
  Process *selectedOther;
  int platFlag = 0; //flag for platinum process
  int otherFlag = 0; //flag for other processes
  int minTime = 100000;
  Process *dummy;
  for(int i = 0; i < numReadyProcesses; i++) {
    if(ReadyQueue[i]->lastEnterTime < minTime) {
      minTime = ReadyQueue[i]->lastEnterTime;
    }
    if(strcmp(ReadyQueue[i]->type, "PLATINUM") == 0) {
      if (ReadyQueue[i]->arrivalTime <= currentTime) {
        if(platFlag == 0){
          selectedPlat = ReadyQueue[i];
          platFlag = 1;
        }
        else{
          if(ReadyQueue[i]->priority > selectedPlat->priority) {
            selectedPlat = ReadyQueue[i]; //if priority is higher, select that process
          }
          else if(ReadyQueue[i]->priority == selectedPlat->priority) {
              if(ReadyQueue[i]->arrivalTime < selectedPlat->arrivalTime) {
                selectedPlat = ReadyQueue[i]; //if arrival time is lower, select that process
              }
              else if(ReadyQueue[i]->arrivalTime == selectedPlat->arrivalTime) {
                if(strcmp(ReadyQueue[i]->processID, selectedPlat->processID) <0) {
                  selectedPlat = ReadyQueue[i]; //if process id is lower, select that process
                }
              }
          }
        }
        
      }
    }
    else {
      if (ReadyQueue[i]->lastEnterTime <= currentTime) {
        if (otherFlag == 0) {
          selectedOther = ReadyQueue[i];
          otherFlag = 1; //in order to not return null
        }
        if (ReadyQueue[i]->priority > selectedOther->priority) {
          //printf("selectedOther: %s\n with priority", selectedOther->processID, selectedOther->priority);
          selectedOther = ReadyQueue[i]; //if priority is higher, select that process
        }
        else if (ReadyQueue[i]->priority == selectedOther->priority) {
          if (ReadyQueue[i]->lastEnterTime < selectedOther->lastEnterTime) {
            selectedOther = ReadyQueue[i]; //if last enter time is lower, select that process
          }
          else if (ReadyQueue[i]->lastEnterTime == selectedOther->lastEnterTime) {
            if (strcmp(ReadyQueue[i]->processID, selectedOther->processID) < 0) {
              selectedOther = ReadyQueue[i];//if process id is lower, select that process
            }
          }
        }
      }
    }
  }
  if(platFlag == 1) {
    //If there is a platinum process, select that process
    dummy = selectedPlat;
    return dummy;
  }
  else if(otherFlag == 1) {
    //If there is no platinum process, select the other process
    dummy = selectedOther;
    return dummy;
  }
  else {
    //If there is no process to select at that time, select the process with the lowest last enter time
    currentTime = minTime;
    return selectNewProcess(ReadyQueue);
  }
}
bool checkPreemption(Process *p, Process **readyQueue) {
  //the function that checks if the process should be preempted
  for(int i = 0; i < numReadyProcesses; i++) {
    if(readyQueue[i]->lastEnterTime <= currentTime){
      if(strcmp(readyQueue[i]->type, "PLATINUM") == 0) {
        return true;
      }
      else if(readyQueue[i]->priority > p->priority) {
          return true;
        }
      }
    }
  return false;
}



void priority_update(Process *p) {
  //updates the type of the process if it is needed
  if(p->execQuant == 3 && strcmp(p->type, "SILVER") == 0) {
    strcpy(p->type, "GOLD");
    p->execQuant = 0;
    //printf("Process %s is upgraded to GOLD\n", p->processID);
  }
  else if(p->execQuant == 5 && strcmp(p->type, "GOLD") == 0) {
    strcpy(p->type, "PLATINUM");
    p->execQuant = 0;
    //printf("Process %s is upgraded to PLATINUM\n", p->processID);
  }
  else{
    return;
  }
}
void checkContextSwitch(Process p, Process c) {
  //checks if a context switch is needed
  if (strcmp(p.processID, c.processID) != 0) {
    //printf("Context switch is needed\n");
    currentTime += 10;
  }
  
}

void scheduler(int* arrivalTimes,Process* readyQueue[],Process* processes[],int numProcesses){
  //scheduler function to schedule the processes
  Process *temp= NULL;
  Process* completedProcesses[numProcesses];
  int completed = 0;
  while(completed < numProcesses) { //while there are still processes to execute
    Process *p = selectNewProcess(readyQueue);
    if(checkFlag == 0){ //This is for first context switch
      //printf("context switch\n");
      currentTime +=10;
      checkFlag =1;
    }
    if(temp != NULL)
      checkContextSwitch(*p, *temp);
    removeFromReadyQueue(*p, readyQueue);
    //printf("Process %s is running\n", p->processID);
    int quantum = (strcmp(p->type, "SILVER") == 0) ? 80 : 120;
    int timeSpent = 0;
    while(timeSpent < quantum && p->currentInstruction < p->totalInstructions) { //while the process is executing
      if (strcmp(p->type, "PLATINUM") == 0) {
        for(int i=p->currentInstruction; i<p->totalInstructions; i++){
          currentTime += p->executionTimes[i]; //if the process is platinum, execute all the instructions
          //printf("current timeza: %d\n", currentTime);
          p->currentInstruction++;
        }
        completedProcesses[completed] = p; //add it to the completed processes
        break;
      }
      else {
        timeSpent += p->executionTimes[p->currentInstruction];
        currentTime += p->executionTimes[p->currentInstruction];
        p->currentInstruction++;
      }
      if(checkPreemption(p, readyQueue)) //check preemption after execute instruction
        break;
    }
      if(p->currentInstruction >= p->totalInstructions) { //if the process is completed
        //printf("Process %s is finished\n", p->processID);
        //printf("Completion: %d\n", currentTime);
        p->completeTime = currentTime;
        p->turnAroundTime = p->completeTime - p->arrivalTime;
        //printf("Process %s Turnaround Time: %d\n", p->processID, p->turnAroundTime);
        
        p->waitingTime = p->turnAroundTime - p->execTime;
        //printf("Process %s Waiting Time: %d\n", p->processID, p->waitingTime);
        completedProcesses[completed] = p;
        completed++;
        temp = NULL;
        checkFlag = 0;
      }
      else { //if the process is not completed
        lasExecutedProcess = p;
        p->execQuant++;
        //printf("execQuant: %d\n", p->execQuant);
        p->lastEnterTime = currentTime;
        temp = p;
        priority_update(p);
        addReadyProcess(p, readyQueue);
      }
      
    }
  //Arithmetic operations to calculate the average turnaround and waiting times
  float avgTurnAroundTime = 0.0;
  float avgWaitingTime = 0.0;
  int totalTurnAroundTime = 0;
  int totalWaitingTime = 0;
  for(int i = 0; i < numProcesses; i++) {
    totalTurnAroundTime += completedProcesses[i]->turnAroundTime;
    //printf("Process %s Turnaround Time: %d\n", completedProcesses[i]->processID, completedProcesses[i]->turnAroundTime);
    totalWaitingTime+= completedProcesses[i]->waitingTime;
  }
  avgTurnAroundTime = (float)totalTurnAroundTime / numProcesses;
  avgWaitingTime = (float)totalWaitingTime / numProcesses;
  avgWaitingTime - (int)(avgWaitingTime) == 0 ? printf("%d\n",(int)avgWaitingTime) : printf("%.1f\n", avgWaitingTime);
  avgTurnAroundTime - (int)(avgTurnAroundTime) == 0 ? printf("%d\n",(int)avgTurnAroundTime) : printf("%.1f\n", avgTurnAroundTime);
  
  
}
int main() {
  FILE *file = fopen("definition.txt", "r");
  if (file == NULL) {
    perror("Unable to open the file");
    return 1;
  }
  int numProcesses = 0;
  Process processes[MAX_PROCESSES];
  while (numProcesses < MAX_PROCESSES && fscanf(file, "%4s %d %d %9s",
                                                processes[numProcesses].processID, &processes[numProcesses].priority,
                                                &processes[numProcesses].arrivalTime, processes[numProcesses].type) == 4) {
    processes[numProcesses].index = numProcesses;
    processes[numProcesses].lastEnterTime = processes[numProcesses].arrivalTime;
    numProcesses++;
  }//read the file and assign the processes to the process array
  Process **readyQueue = (Process **)malloc(numProcesses * sizeof(Process *));
  int *arrivalTimes = (int *)malloc(numProcesses * sizeof(int));
  if (!readyQueue || !arrivalTimes) {
    perror("Memory allocation failed");
    fclose(file);
    return 1;
  }
  for(int i = 0; i < numProcesses; i++) {
    assignInstructions(&processes[i]);
    arrivalTimes[i] = processes[i].arrivalTime;
    addReadyProcess(&processes[i], readyQueue);
    //Initialize the ready queue and arrival times
  }
//  for(int i=0; i<=sizeof(arrivalTimes)/sizeof(arrivalTimes[0]); i++){
//    //printf("%d\n", arrivalTimes[i]);
//    //printf("%s\n", readyQueue[i]->processID);
//    //printf("%d\n", readyQueue[i]->arrivalTime);
//    //printf("%s\n", readyQueue[i]->type);
//    //printf("%d\n", readyQueue[i]->priority);
//  }
  scheduler(arrivalTimes, readyQueue, processes, numProcesses);
  free(readyQueue);
  free(arrivalTimes);
  fclose(file);
  return 0;
}

