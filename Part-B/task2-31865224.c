/*
 * Monash University
 * FIT2100 - Operating Systems
 * In-semester Assignment Part B
 * Task 2 : Preemptive Scheduling
 * RR, Round Robin scheduling with time quantum of 2
 * Created by rhyme on 23/9/22.
 *
 * Reads in process file. Stores data. Creates PCBs as processes arrive. Outputs arrival, running and exit times
 *      to the user. Schedules each process for a time quantum of 2 seconds as they arrive and then moves on to the
 *      next. Services waiting processes at the end of each round. Calculates wait time and turnaround time for each
 *      process and then whether it's met its deadline. Write results to text file
 *
 * Created by rhyme on 23/9/22.
 * Last modified date 7/10/22
 *
 * References:
 *      1. Wikimedia Foundation. (2022, August 3). Round-robin scheduling.
 *          Wikipedia. Retrieved October 5, 2022,
 *          from https://en.wikipedia.org/wiki/Round-robin_scheduling
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <wchar.h>
#include <unistd.h>

#define maxProcesses 100
char process_names[maxProcesses][11];  /// Holds all process names
int arrival_times[maxProcesses];    /// Holds all process arrival times
int service_times[maxProcesses];    /// Holds all process service times
int deadlines[maxProcesses];    /// Holds all process deadlines

int turnaround_times[maxProcesses];     /// Holds all process turnaround times
int wait_times[maxProcesses];   /// Holds all process wait times
int deadlines_met[maxProcesses];    /// Holds all process deadlines that are met
int totalProcesses = 0;     /// Counts total number of processes arrived in the system

typedef enum {
    /*
     * Special enumerated data type for process state.
     */
    READY, RUNNING, EXIT   /// Different states a Process could be in
} process_state_t;

typedef struct {
    /*
     * C data structure used as a process control block.
     * The scheduler should create one instance per running process in the system.
     * Times are measured in units of seconds
     */
    char process_name[11];  /// A string that identifies the process by name.
    int entryTime;  /// The time the process entered the system.
    int serviceTime;    /// The total CPU time required by
    int remainingTime;  /// Remaining service time until completion
    int deadline;    /// Target deadline to meet
    int waitTime;  /// Total time process waits after arrival before it starts running
    process_state_t state;  ///  current process state (e.g. READY)
} pcb_t;

void store_processes(char* line, int processNumber){
    /*
     * Reads in each process data line from text file and stores name, arrival, service and deadlines in array
     *
     * :arguments:
     *      line: Pointer to start of line we are reading
     *      processNumber: Process/line number we are reading, used to store data
     * :return: void
     */
    char process_name[11];  /// Character array to store process name
    char arrival_time_string[11];   /// Character array to store arrival name
    char service_time_string[11];   /// Character array to store service name
    char deadline_string[11];   /// Character array to store deadline name
    int data = 1, i = 0, j = 0, k = 0, l = 0;  /// variables to handle iteration of each array

    while (*line) {  /// Continue till end of current line
        switch (data)  ///  Switch till all data types have been handled
        {
            case 1: /// Switch case to handle process name
                if ((!isspace(*line))){  /// Check each char for next char
                    process_name[i] = *line;   /// Store process name by char
                    i++;  /// Iterate by char
                } else if(isspace(*line)){  /// Check each char for whitespace
                    strcpy(process_names[processNumber], process_name);     /// Store process name in array
                    data++;
                } break;

            case 2: /// Switch case to handle arrival time
                if ((!isspace(*line))){  /// Check each char for next char
                    arrival_time_string[j] = *line;  /// Store arrival time by char
                    j++;  // Iterate by char
                } else if(isspace(*line)){  // Check each char for whitespace
                    arrival_times[processNumber] = atoi(arrival_time_string);   /// Store arrival time in array
                    data++;
                } break;

            case 3: /// Switch case to handle service time
                if ((!isspace(*line))){  /// Check each char for next char
                    service_time_string[k] = *line;   /// Store service time by char
                    k++;  /// Iterate by char
                } else if(isspace(*line)){  /// Check each char for whitespace
                    service_times[processNumber] = atoi(service_time_string);   /// Store service time in array
                    data++;
                } break;

            case 4:  /// Switch case to handle deadline
                if ((!isspace(*line))){  /// Check each char for next char
                    deadline_string[l] = *line;  /// Store deadline by char
                    l++;  /// Iterate by char
                } else if(isspace(*line)){  /// Check each char for whitespace
                    deadlines[processNumber] = atoi(deadline_string);           /// Store deadline in array
                    data++;
                } break;
        }
        line++;   /// Increment line pointer
    }
}

void read_processes(char *process_file) {
    /*
     * Opens process file, reads in each process line by line and stores data in respective arrays
     *
     * :arguments:
     *      process_file: Pointer to file name, contains process data
     * :return: void
     */
    FILE * fptr;  /// File pointer to process file
    char * line = NULL; /// char pointer for each line of file
    size_t len = 0;    ///  Length of file

    /// Attempt to open file in read mode, else exit as failure
    fptr = fopen(process_file, "r");
    if (fptr == NULL)
        exit(EXIT_FAILURE);

    /// Read in file line by line
    while (getline(&line, &len, fptr) != -1) {
        store_processes(line, totalProcesses);   /// Read each line and store data in each array
        totalProcesses++;   /// Increment process count
    }

    /// Free resources and close file stream
    fclose(fptr);
    if (line) {free(line);}
}

void schedule(){
    /*
    * Schedules all processes based on Round Robin with a quantum time of 2
    *
    * :arguments: None
    * :return: void
    */
    const int timeQuantum = 2;   /// Time quantum for Robin Rounds
    pcb_t ready_queue[100];    /// Array of PCBs as they arrive in the system
    int currentRunningProcess = 0;/// Keep track of process which is currently running in the system
    int timeCounter = 0;   /// Counts time in seconds
    int pcbCounter = 0;    /// Counts Process block being serviced
    /// Keep iterating Robin Rounds till no processes are needed to be serviced
    while(true){
        bool complete = true;
        /// Iterate all processes one at a time
        for (int i=0; i<totalProcesses-1; i++){
            /// If process still needs to be serviced
            if (pcbCounter < totalProcesses-1){  /// Until PCBs have been created for all processes in system
                printf("Time% 5d:  %s has entered the system.\n", arrival_times[i], process_names[i]);
                pcb_t newly_arrived;  /// Initialize PCB with values
                strcpy(newly_arrived.process_name, process_names[pcbCounter]);
                newly_arrived.entryTime = arrival_times[pcbCounter];
                newly_arrived.serviceTime = service_times[pcbCounter];
                newly_arrived.remainingTime = service_times[pcbCounter];
                newly_arrived.deadline = deadlines[pcbCounter];
                newly_arrived.state = READY;
                newly_arrived.waitTime = 0;
                ready_queue[pcbCounter] = newly_arrived;   /// Add PCB to queue
                pcbCounter++;
            }
            /// This means there are processes yet to be serviced.
            if (ready_queue[i].remainingTime > 0 && ready_queue[i].state != EXIT){
                complete = false;  /// Do not exit as processes remain to be serviced
                ready_queue[i].state = RUNNING;   /// Set process to be serviced state
                printf("Time% 5d:  %s is in the running state.\n", timeCounter, ready_queue[i].process_name);

                if (ready_queue[i].remainingTime > timeQuantum){ /// Remaining time more than time quantum
                    ///  Service process by time quantum
                    timeCounter += timeQuantum;   /// Pass time
                    ready_queue[i].remainingTime -= timeQuantum;   /// Reduce remaining time
                } else { /// Remaining time less than time quantum
                    ///  Service process by time quantum
                    timeCounter += ready_queue[i].remainingTime;   /// Pass time
                    ready_queue[i].waitTime = timeCounter - ready_queue[i].serviceTime - ready_queue[i].entryTime;   /// Calculate wait time
                    ready_queue[i].remainingTime = 0;  /// End process

                    /// Add times to array before delete PCB
                    wait_times[i] = ready_queue[i].waitTime;
                    turnaround_times[i] = ready_queue[i].serviceTime + ready_queue[i].waitTime;  /// Turnaround = Service + Wait
                    if (turnaround_times[i] <= ready_queue[currentRunningProcess].deadline) {
                        deadlines_met[i] = 1;   /// Turnaround <= Deadline
                    } else {
                        deadlines_met[i] = 0;   /// Turnaround > Deadline
                    }
                    /// Kill Process, set state to EXIT
                    printf("Time% 5d:  %s has finished execution.\n", timeCounter,ready_queue[i].process_name);
                    ready_queue[i].state = EXIT;
                }
                sleep(1);
            }
        }
        if (complete){  /// Exit now as all processes have been serviced
            break;
        }
    }
}

void write_processes(){
    /*
    * Takes each data array and writes process data to results file
    *
    * :arguments: None
    * :return: void
    */
    char * path = "results-2.txt";
    /// Declare file pointer
    FILE *fptr;
    char whitespace = ' ';
    /// open the file from path in write mode
    fptr = fopen(path, "wb"); //"ab+");
    /// Iterate over each process in system and write stats to results file
    for (int i=0; i < totalProcesses-1; i++){
        /// Each each_process of the results holds one process only
        char * process_name = process_names[i];  /// Store pointer to current process name
        fwrite(process_name, 1, strlen(process_name), fptr);   /// Write Process name to current line
        putc(whitespace, fptr);  /// Put whitespace

        char wait_time[20];     /// Buffer to store wait time
        sprintf(wait_time,"%d",wait_times[i]);      /// Convert integer wait time to string
        fwrite(wait_time, 1, strlen(wait_time), fptr);  /// Write wait time to current line
        putc(whitespace, fptr);  /// Put whitespace

        char turnaround_time[20];      /// Buffer to store turnaround time
        sprintf(turnaround_time,"%d",turnaround_times[i]);      /// Convert integer turnaround time to string
        fwrite(turnaround_time, 1, strlen(turnaround_time), fptr); /// Write turnaround time to current line
        putc(whitespace, fptr);  /// Put whitespace

        char deadline[20];/// Buffer to store deadline time
        sprintf(deadline,"%d",deadlines_met[i]);/// Convert integer deadline time to string
        fwrite(deadline, 1, strlen(deadline), fptr);/// Write deadline time to current line
        putc('\n', fptr); /// Put newline at end of line
    }
    /// close the file
    fclose(fptr);
}

int main(int argc, char *argv[]) {
    /*
     * Main function called upon start. Uses command line arguments as file path if provided.
     * Runs Round Robin scheduling to handle processes and outputs results to text file
     *
     * :arguments:
     *      argc: Total number of command line arguments passed
     *      *argv[]: Pointer to array storing all command line arguments passed
     * :return: 0 as program exits
     */
    char* path = "processes.txt";
    /// Set file path passed as argument
    if (argc == 2){
        if (argv[1][0] == '/'){
            path = argv[1];
        }
    }
    /// Read in processes from text file
    read_processes(path);
    /// Schedule processes and print updates
    schedule();
    /// output text to file
    write_processes();
    return 0;
}
