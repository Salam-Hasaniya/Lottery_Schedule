# Lottery_Schedule
This project is a Lottery Scheduling Algorithm in C, simulating a process scheduling system where each process is assigned a number of tickets. The scheduler randomly selects a ticket, and the corresponding process gets executed for a fixed time slice. Processes lose tickets over time and are removed once they finish execution.

## Function Overview

| Function                 | Purpose                                                 |
|--------------------------|---------------------------------------------------------|
| `createProcess`         | Creates a new process with a name, size, and ticket count |
| `createList`           | Initializes an empty linked list                         |
| `setTicketRanges`      | Assigns start and end ticket ranges to each process      |
| `resetTicketRanges`    | Resets tickets when they are exhausted                   |
| `addToList`           | Adds a process to the queue                              |
| `getRandomNumber`      | Generates a random number to select a ticket             |
| `findProcessByTicketRange` | Finds a process based on the drawn ticket number   |
| `removeProcessFromList` | Removes a finished process from the list               |
| `lottery`              | Runs the lottery scheduler until all processes finish   |

