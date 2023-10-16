# SimpleShell and SimpleScheduler

## Introduction
SimpleShell and SimpleScheduler are programs designed to implement a basic process scheduling system in C. The SimpleShell is used to interact with the scheduler and submit programs for execution. The SimpleScheduler manages the execution of submitted programs on limited CPU resources.

## Usage

### SimpleShell
1. Run the SimpleShell to interact with the scheduler.
2. Provide the total number of CPU resources (NCPU) and the time slice in milliseconds (TSLICE) as command-line input parameters.
3. Use the following commands in SimpleShell

    - `submit .a.out` Submit a program for execution.
    - `history` Display the command history.

### SimpleScheduler
1. The SimpleScheduler is automatically started by SimpleShell.
2. It manages a ready queue of processes and schedules their execution on available CPU resources.
3. Processes in the ready queue are executed in a round-robin fashion, and the time slice is defined by TSLICE

Contribution Breakdown

Nishant:
     
Made changes to the shell to make it compatible with the scheduler .
Added process management.
Enhanced error handling.

Harshil:

Created the initial version of the scheduler.
Assisted in overall code structure and debugging.
