#pragma once
#include <stdbool.h>
#include "kvfs.h"

#define MAX_COMMAND_LENGTH 64

bool check_for_beep_cmd();

void set_beep_cmd(bool a1);

typedef struct Command{
	char keyword[MAX_COMMAND_LENGTH];
	int paramcount;
	int func_id;
} Command;

int try_parse_command(); //return spec values

void register_commands();
