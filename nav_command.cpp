#include "nav_command.h"
#include "commands.h"



void nav_shell_command(vector<string> inputList, map<pid_t, Process> &processTable, string inputString) {


    if (inputList[0] == "exit") {
        // run exit
        resource_usage(processTable);
        exit_shell(processTable);
    }
    else if (inputList[0] == "jobs" && inputList[1].size() > 0 && inputList.size() > 1) {
        // run jobs
        all_jobs_in_out(processTable, inputList);
        resource_usage(processTable);
    }
    else if (inputList[0] == "jobs") {
        // run jobs
        all_jobs(processTable, inputList);
        resource_usage(processTable);
    }
    else if (inputList[0] == "kill") {
        // run kill
        kill_process(processTable, inputList);
    }
    else if (inputList[0] == "resume") {
        // run resume
        resume_process(processTable, inputList);
    }
    else if (inputList[0] == "sleep") {
        // run sleep
        sleep(inputList);
    }
    else if (inputList[0] == "suspend") {
        // run suspend
        suspend_process(processTable, inputList);
    }
    else if (inputList[0] == "wait") {
        // run wait
        wait_process(processTable, inputList);
    }

    else if (inputList[0] != "exit" && 
    inputList[0] != "jobs" &&
    inputList[0] != "kill" &&
    inputList[0] != "resume" &&
    inputList[0] != "sleep" &&
    inputList[0] != "suspend" &&
    inputList[0] != "wait" && 
    !inputList[0].empty()) {
        // add process

        add_process(processTable, inputList, inputString);

        
    }
}