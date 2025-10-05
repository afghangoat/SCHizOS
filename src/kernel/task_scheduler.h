#pragma once
#include <stdbool.h>


typedef bool (*Proc_func)(void);

//void create_process(int pid,int data);
void create_process(int pid,int data,Proc_func func, bool persistent);

bool persistent_prc();

bool kill_process(int pid);

void kill_persistent();

int check_process(int pid);

void init_task_scheduler();

void run_scheduler_loop();
