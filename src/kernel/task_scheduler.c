#include "task_scheduler.h"
#include <stdio.h>
#include <stdlang.h>

#define PROCESSES_MAX 32

typedef bool (*Proc_func)(void);

typedef struct{
	int pid_t;
	Proc_func p_func; //if returned false, then kill process
	bool persistent;
	int data;
} Process;

int jprocess=0;
Process global_processes[PROCESSES_MAX];

bool persistent_prc(){
	return true; //Default behaviour
}

void create_process(int pid,int data){
	for(int i=0;i<jprocess;i++){
		
		if(global_processes[i].pid_t==-1){
			
			global_processes[i].pid_t=pid;
			global_processes[i].data=data;
			global_processes[i].p_func=persistent_prc; //currently only this is supported
			global_processes[i].persistent=true;
			return;
		}
	}
	
	global_processes[jprocess].pid_t=pid;
	global_processes[jprocess].data=data;
	global_processes[jprocess].p_func=persistent_prc; //currently only this is supported
	global_processes[jprocess].persistent=true;
	
	jprocess++;
}

bool kill_process(int pid){
	bool killed=false;
	for(int i=0;i<jprocess;i++){
		if(global_processes[i].pid_t==pid){
			killed=true;
			global_processes[i].pid_t=-1; //count as killed
			break;
		}
		
	}
	if(killed==false){
		printf(TASK_ERR MSG_DELIM ERR_NO_PROCC);
	}
	return killed;
}

void kill_persistent(){
	for(int i=0;i<jprocess;i++){
		if(global_processes[i].persistent==true&&global_processes[i].pid_t!=-1){
			global_processes[i].pid_t=-1;
		}
	}
}

int check_process(int pid){
	//Look for process data, -1 will be returned if not found
	for(int i=0;i<jprocess;i++){
		if(global_processes[i].pid_t==pid){
			return global_processes[i].data;
		}
		
	}
	return -1;
}

void init_task_scheduler(){
	for(int i=0;i<PROCESSES_MAX;i++){
		global_processes[i].pid_t=-1; //clear
	}
}
