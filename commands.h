#ifndef COMMANDS_H
#define COMMANDS_H

#include <iostream>
#include <string>
#include <vector>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <sys/resource.h>
#include <map>
#include <iomanip>
#include <fstream>



using namespace std;

class Process {
public: 
    pid_t pid;
    string command;
    string status;

    Process(pid_t pid, string command, string status);
};

map<pid_t, Process> *getProcessTable();

Process *getProcess(pid_t pid);

vector<char *>convStrToChar(vector<string> StringVector);



void exit_shell(map<pid_t, Process> &processTable);

void all_jobs(map<pid_t, Process> &processTable, vector<string> inputList);

void all_jobs_in_out(map<pid_t, Process> &processTable, vector<string> inputList);

void kill_process(map<pid_t, Process> &processTable, vector<string> inputList);

void resume_process(map<pid_t, Process> &processTable, vector<string> inputList);

void sleep(vector<string> inputList);

void suspend_process(map<pid_t, Process> &processTable, vector<string> inputList);

void wait_process(map<pid_t, Process> &processTable, vector<string> inputList);

void add_process(map<pid_t, Process> &processTable, vector<string> inputList, string inputString);

void resource_usage(map<pid_t, Process> &processTable);


#endif