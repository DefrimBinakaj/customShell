// https://stackoverflow.com/questions/15891781/how-to-call-on-a-function-found-on-another-file
#ifndef NAV_COMMAND_H
#define NAV_COMMAND_H

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
#include <map>

#include "commands.h"

using namespace std;

void nav_shell_command(vector<string> inputList, map<pid_t, Process> &processTable, string inputString);

#endif