#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct _process {
    char name;
    int pid;
    int size;
    int tickets;
    int originalTickets;
    int start; 
    int end; 
    struct _process* next;
} Process;

typedef struct _linkedList {
    Process* first;
    Process* last;
} List;

int pidSequence = 0;
int timeSlice = 1;
int totalTickets = 0;

Process* createProcess(char newName, int newSize, int newTickets) {
    Process* newProcess = (Process*)malloc(sizeof(Process));
    
    newProcess->name = newName;
    newProcess->pid = ++pidSequence;
    newProcess->size = newSize;
    newProcess->tickets = newTickets;
    newProcess->originalTickets = newTickets;
    newProcess->start = 0;
    newProcess->end = 0;
    newProcess->next = NULL;
    return newProcess;
}

List* createList() {
    List* newList = (List*)malloc(sizeof(List));
    newList->first = NULL;
    newList->last = NULL;
    return newList;
}

void setTicketRanges(List* l) {
    Process* currentProcess = l->first;
    int currentStart = 1;
    
    totalTickets = 0;
    while (currentProcess != NULL) {
        if (currentProcess->tickets > 0) {// Only assign a range if the process has tickets
            currentProcess->start = currentStart;
            currentProcess->end = currentStart + currentProcess->tickets - 1;
            currentStart = currentProcess->end + 1;
            totalTickets += currentProcess->tickets; 
        } else {// If tickets are zero, set the range to indicate no active tickets
            currentProcess->start = 0;
            currentProcess->end = 0;
        }
        currentProcess = currentProcess->next;
    }
}

void resetTicketRanges(List* l) {
    Process* currentProcess = l->first;
    while (currentProcess != NULL) {
        currentProcess->tickets = currentProcess->originalTickets;
        currentProcess = currentProcess->next;
    }
    setTicketRanges(l);
}

int allTicketsExhausted(List* l) {
    Process* currentProcess = l->first;
    while (currentProcess != NULL) {
        if (currentProcess->tickets > 0) {
            return 0;
        }
        currentProcess = currentProcess->next;
    }
    return 1;
}

int addToList(List* list, Process* process) {
    if (list == NULL) 
    return -1;
    
    if(list->first == NULL) {
        list->first = process;
        list->last = process;
    } else {
        list->last->next = process;
        list->last = process;
    }
    
    setTicketRanges(list);
    return 0;
}

int getRandomNumber(int min, int max) {
    return rand() % (max - min + 1) + min;
}

Process* findProcessByTicketRange(List* list, int randomTicket) {
    Process* tempProcess = list->first;
    while (tempProcess != NULL) {
        if (randomTicket >= tempProcess->start && randomTicket <= tempProcess->end) {
            return tempProcess;
        }
        tempProcess = tempProcess->next;
    }
    return NULL;
}

void removeProcessFromList (List* list, Process* process) {
    if (list->first == process) {
        list->first = process->next;
    } else {
        Process* tempProcess = list->first;
        while (tempProcess->next != process) {
            tempProcess = tempProcess->next;
        }
        tempProcess->next = process->next;
    }
    free(process);
    setTicketRanges(list);
}

int lottery(List* list) {
    if (list->first == NULL) {
        printf("No processes to schedule.\n");
        return 0;
    }   
    while(list->first != NULL) {
        if (allTicketsExhausted(list)) {
            printf("\nAll tickets exhausted. Resetting ticket counts...\n");
            resetTicketRanges(list);
        }
        
        int randomTicket = getRandomNumber(1, totalTickets);
        printf("\nRandom ticket drawn: %d\n", randomTicket);
        Process* runningProcess = findProcessByTicketRange(list, randomTicket);
        
        if (runningProcess == NULL) {
            printf("Error: No process found with ticket %d\n", randomTicket);
            return -1;
        }
        
        printf("Running Process %c (pid: %d) (tickets left: %d) size start = %d; size left = %d \n", 
        runningProcess->name, runningProcess->pid, runningProcess->tickets - 1,runningProcess->size, runningProcess->size - timeSlice);
        
        runningProcess->size -= timeSlice;
        runningProcess->tickets--;
        
        if (runningProcess->size <= 0) {
            printf("=== Process %c has finished. ===\n", runningProcess->name);
            removeProcessFromList(list, runningProcess);
        } else {
            setTicketRanges(list);
        }
        Process* tempProcess = list->first;
        while (tempProcess != NULL) {
            printf("\n%c [%d - %d]", tempProcess->name, tempProcess->start, tempProcess->end);
            tempProcess = tempProcess->next;
        }
    }
    return 0;
}

int main() {
    srand(time(NULL));
    List* list = createList();
    
    addToList(list, createProcess('A',10,15));
    addToList(list, createProcess('B',10,5));
    addToList(list, createProcess('C',10,5));
    
    printf("=== Starting the Lottery Scheduler ===\n");
    
    Process* tempProcess = list->first;
    while (tempProcess != NULL) {
        printf("\n%c [%d - %d]", tempProcess->name, tempProcess->start, tempProcess->end);
        tempProcess = tempProcess->next;
    }
    
    lottery(list);
    return 0;
}
