#pragma once
#include <stdbool.h>

void create_process(int pid,int data);

bool kill_process(int pid);

void kill_persistent();

int check_process(int pid);

void init_task_scheduler();
