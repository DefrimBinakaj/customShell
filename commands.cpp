#include "commands.h"
#include "nav_command.h"

// process w/ attributes -------------------------------------
Process::Process(pid_t pid, string command, string status) {
    this->pid = pid;
    this->command = command;
    this->status = status;
}


// convert string to char function -------------------------------------
// https://stackoverflow.com/questions/42493101/how-to-convert-a-vectorstring-to-a-vectorchar
vector<char *>convStrToChar(vector<string> StringVector) {
    vector<char *> CharVector = vector<char *>();

    for (const auto &str : StringVector) {
        char *charStr = new char[str.size() + 1];
        strcpy(charStr, str.c_str());
        CharVector.push_back(charStr);
    }

    return CharVector;
}



// https://stackoverflow.com/questions/15451287/what-does-iterator-second-mean
// https://stackoverflow.com/questions/26281979/c-loop-through-map
// exit function -------------------------------------
void exit_shell(map<pid_t, Process> &processTable) {

    // exit must kill all, therefore must resume / wait process before killing
    for (auto &it: processTable) {
        int pid = it.second.pid;
        kill(pid, SIGCONT);
        kill(pid, SIGTERM);
        waitpid(pid, nullptr, 0);
        kill(pid, SIGKILL);
    }
    processTable.clear();

    exit(0);
}





// print all jobs function -------------------------------------
void all_jobs(map<pid_t, Process> &processTable, vector<string> inputList) {

    
    printf("\nRunning processes: \n");    
    
    int processNumber = 0;


    vector<char *> tokens = convStrToChar(inputList);


    for (auto &it: processTable) {     

        if (processNumber == 0) {
            printf("#     PID S SEC    COMMAND\n");
        }


        // retrieve time of child w/ lab 3, regarding popen, fscanf, buffer
        char buffer[100];
        int processTime;        
        FILE *fp;
        sprintf(buffer, "ps -p %d -o times=", it.second.pid);
        fp = popen(buffer, "r");
        fscanf(fp, "%d", &processTime);
        pclose(fp);

        // https://stackoverflow.com/questions/5590381/easiest-way-to-convert-int-to-string-in-c
        string dispNum = to_string(processNumber) + ":";
        printf("%s %d %s %d %s\n",
        dispNum.c_str(), it.second.pid, it.second.status.c_str(), processTime, it.second.command.c_str());

        processNumber = processNumber + 1;

    }
    

    printf("Processes =      %ld active\n", processTable.size());
    printf("Completed processes: \n");
    
}





// print all_jobs_in_out function -------------------------------------
void all_jobs_in_out(map<pid_t, Process> &processTable, vector<string> inputList) {
    
    int processNumber = 0;


    vector<char *> tokens = convStrToChar(inputList);


    for (int i = 0; i < tokens.size(); i++) {

        if (tokens[i][0] == '<') {
            tokens[i] = tokens[i] + 1;
            freopen(tokens[i], "r", stdin);
        
        } else if (tokens[i][strlen(tokens[i]) - 1] == '<') {
            tokens[i][strlen(tokens[i])-1] = '\0';
            freopen(tokens[i], "r", stdin);
        
        } else if (tokens[i][0] == '>') {
            tokens[i] = tokens[i] + 1;
            freopen(tokens[i], "w", stdout);
        
        } else if (tokens[i][strlen(tokens[i]) - 1] == '>') {
            tokens[i][strlen(tokens[i])-1] = '\0';
            freopen(tokens[i], "w", stdout);
        }

    }

    printf("\nRunning processes: \n");    
    for (auto &it: processTable) {     

        if (processNumber == 0) {
            printf("#     PID S SEC    COMMAND\n");
        }


        char buffer[100];
        int processTime;
        FILE *fp;
        sprintf(buffer, "ps -p %d -o times=", it.second.pid);
        fp = popen(buffer, "r");
        fscanf(fp, "%d", &processTime);
        pclose(fp);


        string dispNum = to_string(processNumber) + ":";
        
        printf("%s %d %s %d %s\n",
        dispNum.c_str(), it.second.pid, it.second.status.c_str(), processTime, it.second.command.c_str());

        processNumber = processNumber + 1;

    }
    printf("Processes =      %ld active\n", processTable.size());
    printf("Completed processes: \n");
    fclose(stdin);
    
    
}





// kill function -------------------------------------
void kill_process(map<pid_t, Process> &processTable, vector<string> inputList) {

    try {
        pid_t pid = atoi(inputList[1].c_str());;
        // must resume / wait process before killing
        // https://www.geeksforgeeks.org/mapat-c-stl/
        if (&processTable.at(pid) != nullptr){
            kill(pid, SIGCONT);
            kill(pid, SIGTERM);
            waitpid(pid, nullptr, 0);
            kill(pid, SIGKILL);
            processTable.erase(pid);
        }
    }
    catch (std::out_of_range) {
        cout << "invalid kill call \n";
    }

}


// resume function -------------------------------------
void resume_process(map<pid_t, Process> &processTable, vector<string> inputList) {

    try {
        pid_t pid = atoi(inputList[1].c_str());;

        if (&processTable.at(pid) != nullptr) {
            kill(pid, SIGCONT);
            processTable.find(pid)->second.status = "R";
        }
    }
    catch (std::out_of_range) {
        cout << "invalid resume call \n";
    }

}


// sleep function -------------------------------------
void sleep(vector<string> inputList) {
    
    try {
        if (inputList.size() > 1) {
            // https://stackoverflow.com/questions/338273/why-does-printf-not-print-anything-before-sleep
            printf("sleeping...\n");
            fflush(stdout);
            // https://stackoverflow.com/questions/7663709/how-can-i-convert-a-stdstring-to-int
            int sleepTime = atoi(inputList[1].c_str());
            sleep(sleepTime);
        }
    }
    catch (std::out_of_range) {
        cout << "invalid sleep call \n";
    }

}




// suspend function -------------------------------------
void suspend_process(map<pid_t, Process> &processTable, vector<string> inputList) {
    
    // https://www.delftstack.com/howto/cpp/throw-out-of-range-exception-cpp/
    try {
        pid_t pid = atoi(inputList[1].c_str());
        if (&processTable.at(pid) != nullptr) {
            kill(pid, SIGSTOP);
            processTable.find(pid)->second.status = "S";
        }
    }
    catch (std::out_of_range) {
        cout << "invalid suspend call \n";
    }
    
}


// wait function -------------------------------------
void wait_process(map<pid_t, Process> &processTable, vector<string> inputList) {
    
    try {
        pid_t pid = atoi(inputList[1].c_str());;
        if (&processTable.at(pid) != nullptr){
            // wait for process
            if (waitpid(pid, nullptr, 0) > 0) {
                processTable.erase(pid);
            }
        }
    }
    catch (std::out_of_range) {
        cout << "invalid wait call \n";
    }
    
}







// add function -------------------------------------
// using lab 2, regarding fork
void add_process(map<pid_t, Process> &processTable, vector<string> inputList, string inputString) {
    
    pid_t pid = fork();

    vector<char *> tokens = convStrToChar(inputList);

    
    // fork error
    if (pid < 0) {
        perror("Failed to fork child");
        exit(1);
    }
    
    // child
    else if (pid == 0) {

        char* argv[tokens.size() + 1];
        for (int i = 0; i <= tokens.size(); i++) {
            argv[i] = tokens[i];
        }
        argv[tokens.size()] = nullptr;
        
        for (int i = 0; i < tokens.size(); i++) {

            if (tokens[i][0] == '<') {
                tokens[i] = tokens[i] + 1;
                // https://cplusplus.com/reference/cstdio/freopen/
                // https://stackoverflow.com/questions/10150468/how-to-redirect-cin-and-cout-to-files
                freopen(tokens[i], "r", stdin);
            
            } else if (tokens[i][strlen(tokens[i]) - 1] == '<') {
                // https://cboard.cprogramming.com/cplusplus-programming/71560-removing-last-char-char*.html
                tokens[i][strlen(tokens[i])-1] = '\0';
                freopen(tokens[i], "r", stdin);
            
            } else if (tokens[i][0] == '>') {
                tokens[i] = tokens[i] + 1;
                freopen(tokens[i], "w", stdout);
            
            } else if (tokens[i][strlen(tokens[i]) - 1] == '>') {
                tokens[i][strlen(tokens[i])-1] = '\0';
                freopen(tokens[i], "w", stdout);
            }

        }

        if (execvp(argv[0], argv) < 0) {
            fprintf(stderr, "exec error!\n%s\n", tokens[0]);
            exit(1);
        }
    }

    // parent
    else {
        // https://stackoverflow.com/questions/14275291/how-can-i-get-a-field-from-the-last-element-of-a-vector-in-c#:~:text=If%20you%20want%20to%20access,reference%20(and%20not%20iterator).
        string lastElem = tokens.back();


        if (lastElem[0] == '&') {
            Process madeProcess = Process(pid, inputString, "R");
            processTable.insert({pid, madeProcess});
        }
        else {
            Process madeProcess = Process(pid, inputString, "R");
            processTable.insert({pid, madeProcess});
            waitpid(pid, nullptr, 0);
            processTable.erase(pid);
        }

        

    }
}






// resource usage function -------------------------------------
// https://pubs.opengroup.org/onlinepubs/009696699/functions/getrusage.html
// https://stackoverflow.com/questions/11062804/measuring-the-runtime-of-a-c-code
// https://man7.org/linux/man-pages/man2/getrusage.2.html
void resource_usage(map<pid_t, Process> &processTable) {
    struct rusage usage;

    getrusage(RUSAGE_CHILDREN, &usage);

    int UserUsageSeconds = usage.ru_utime.tv_sec;
    int SystemUsageSeconds = usage.ru_stime.tv_sec;
    
    printf("User time =      %d seconds\n", UserUsageSeconds);
    printf("Sys  time =      %d seconds\n", SystemUsageSeconds);
}
