#pragma once

#define MSG_DELIM ": "
#define TAB_PAD "  "

//Labels - global
#define OS_NAME "SCHizOS"
#define BOOT_MSG "BOOT"
#define IO_ERR "I/O"
#define FAT_ERR "FAT"
#define DISK_ERR "DISK"
#define TASK_ERR "Processes"
#define CMD_ERR "CMD"


//Errors
#define ERR_BAD_PRINTF_FORMAT "BAD printf format!\r\n"
#define ERR_READ_BOOT "Reading boot sector failed!\r\n"
#define ERR_FAT_MEM_OUT "Not enough memory to read FAT! Required %lu bytes, available %u bytes.\r\n"
#define ERR_FAT_READ_FAIL "Reading in FAT failed!\r\n"
#define ERR_ROOT_DIR_READ "Read root directory failed!\r\n"
#define ERR_NO_AVAIL_HANDLE "No available file handle!\r\n"
#define ERR_INIT_ERROR "Read error in initialization!\r\n"
#define ERR_READ "Read error!\r\n"
#define ERR_NO_VALID_DIR "%s is not a valid directory!\r\n"
#define ERR_INVALID "%s is not found or invalid!\r\n"
#define ERR_INIT "Init error\r\n"
#define ERR_CMD_NOT_FOUND "No valid command found\r\n"
#define ERR_CMD_FILE_NOT_FOUND_IN_DIR "Specified file not found in directory!\r\n"

#define ERR_DRIVE_PARAMS "Getting drive parameters failed!\r\n"
#define ERR_READ_SECTORS "Error occured while trying to read sectors.\r\n"
#define ERR_NO_PROCC "No process to kill!\r\n"

#define ERR_CMD_INVALID "No command with that id found!\r\n"

//Infos
#define INFO_STAGE2_INI "Stage 2 Loaded.\nLoading disk...\r\n"
#define INFO_FAT_INI "Initializing FAT...\r\n"
#define INFO_KERNEL_LOAD "Loading Kernel...\r\n"
#define INFO_KERNEL_EXEC "Executing Kernel...\r\n"

//Helps
#define HELP_MAN1 "---=Help=---\n"
#define HELP_MAN2 "This command will guide through this tiny, sufferable adventure of yours.\n"
#define HELP_MAN3 "Common command usage: <command> <arg1> ... <argN> (arguments are a must if they are needed)\n"
#define HELP_MAN4 "To get help with commands: helpcmds\n"
#define HELP_MAN5 "To get help with utility information: helputils\n"
#define HELP_MAN6 "Enjoy your stay!...\n"

#define HELP2_MAN1 "---=Commands Help <page 1>=---\n"
#define HELP2_MAN2 "help/guide - Displays the main help text.\n"
#define HELP2_MAN3 "helpcommands - \n"
#define HELP2_MAN4 "helputils - \n"
#define HELP2_MAN5 "logout/shutdown/lo - shuts the OS down.\n"
#define HELP2_MAN6 "cls/clear - clears the screen.\n"
#define HELP2_MAN7 "lang <lang_id> - Changes keyboard language (EN is 0, DE is 1, HU is 2, FR is 3)\n"
#define HELP2_MAN8 "motd - displays the message of the day.\n"
#define HELP2_MAN9 "freemode/cmdmode - enables moving on the screen in all directions with the cursor.\n"
#define HELP2_MAN10 "random/rand/rnd - Gives you a random generated number.\n"
#define HELP2_MAN11 "hunglish - Generates you a new hungarian word.\n"
#define HELP2_MAN12 "dir/ls - Lists the files and directories in the current directory.\n"
#define HELP2_MAN13 "cat <file> - Prints a file content from a loaded file.\n"
#define HELP2_MAN14 "cd <dir> - Enters in a directory.\n"
#define HELP2_MAN15 "pwd - Prints the working directory.\n"
#define HELP2_MAN16 "echo <message> - Prints a message.\n"


#define HELP3_MAN1 "---=Misc. Help=---\n"
#define HELP3_MAN2 "SCHizOS os a monolithic kernel based, very basic OS.\n"
#define HELP3_MAN3 "It contains some more advanced implementations like random,\n"
#define HELP3_MAN4 "tasks, processes, keyboard language changing, commands and some\n"
#define HELP3_MAN5 "other useful features.\n"
#define HELP3_MAN6 "I am open to suggestions to improvements.\n"

#define MSG_LOGOUT "Logging out and shutting down...\n"
#define MSG_SETLANG "Changing keyboard language to...\n"
#define LANG_EN "English\n"
#define LANG_DE "German\n"
#define LANG_HU "Hungarian\n"
#define LANG_XU "French\n"
