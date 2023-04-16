#include <iostream>
#include <string>
#include <vector>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <sys/resource.h>
#include <bits/stdc++.h>
#include <map>

#include "commands.h"
#include "nav_command.h"

using namespace std;

int LINE_LENGTH = 100;
int MAX_ARGS = 7;
int MAX_LENGTH = 20;
int MAX_PT_ENTRIES = 32;


map<pid_t, Process> processTable = map<pid_t, Process>();


int main() {

    // create long for loop instead of while so that the lab machines
    // dont explode
    for (int i = 0; i < 200; i++) {
        
        // https://man7.org/linux/man-pages/man7/signal.7.html
        // https://stackoverflow.com/questions/33508997/waitpid-wnohang-wuntraced-how-do-i-use-these
        // https://stackoverflow.com/questions/4053838/keyboard-signal-handling-adding-parameters-to-callback-handler-function-ubuntu
        signal(SIGCHLD, [](int signum) {
            pid_t pid = waitpid((pid_t)(-1), 0, WNOHANG);
            processTable.erase(pid);
        });




        char input[MAX_LENGTH];
        cout << "\n\nshell379: ";
        // https://cplusplus.com/reference/istream/istream/getline/
        cin.getline(input, LINE_LENGTH);

        string inputString = input;
        vector<string> inputList;
        string strToAdd;

        // https://stackoverflow.com/questions/14265581/parse-split-a-string-in-c-using-string-delimiter-standard-c
        string delimiter = " ";
        size_t last = 0;
        size_t next = 0;
        while ((next = inputString.find(delimiter, last)) != string::npos) {
            strToAdd = inputString.substr(last, next-last);
            last = next + 1;


            // init char with space for each strToAdd
            char* charString = new char[strToAdd.size()+1];
            strcpy(charString, strToAdd.c_str());
            inputList.push_back(charString);
        }
        // add the last strToAdd, since it isnt captured within loop
        strToAdd = inputString.substr(last);
        char* charString = new char[strToAdd.size()+1];
        strcpy(charString, strToAdd.c_str());
        inputList.push_back(charString);

        if (inputList.size() > MAX_ARGS) {
            cout << "too many arguments" << endl;
            break;
        }
        if (processTable.size() > MAX_PT_ENTRIES) {
            cout << "too many process table entries" << endl;
            break;
        }

        nav_shell_command(inputList, processTable, inputString);

    }

    cout << "thanks for using my shell!" << endl;
    return 0;
}
