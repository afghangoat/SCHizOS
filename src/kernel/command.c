//My own command com implementation
#include "command.h"
#include "stdio.h"
#include <string.h>
#include <math.h>
#include <stdlang.h>
#include <stdlib.h>
#include <arch/i686/io.h>
#include "task_scheduler.h"

#define SCREEN_WIDTH 80

int global_count_id=0;
bool countto(){
	if(global_count_id <= 0){
        return false; // stop the process
    }
    global_count_id--;
}

int cmd_needs_beep=0;

bool check_for_beep_cmd(){
	if(cmd_needs_beep==0){
		return false;
	}
	return true;
}
void set_beep_cmd(bool a1){
	if(a1==true){
		cmd_needs_beep=1;
	} else {
		cmd_needs_beep=0;
	}
	
}

char cur_command[SCREEN_WIDTH]=" ";

#define MAX_COMMAND_COUNT 64

#define MAX_PARAM_LEN 30

int command_count=0;
Command global_commands[MAX_COMMAND_COUNT];

int global_param_count=0;
char global_param_storage[10][MAX_PARAM_LEN];
void clear_global_param_storage(){
	for(int i=0;i<10;i++){
		for(int j=0;j<MAX_PARAM_LEN;j++){
			global_param_storage[i][j]='\0';
		}
	}
}
/*typedef struct Command{
	char keyword[MAX_COMMAND_LENGTH];
	int paramcount;
	int func_id;
} Command;*/

#define COMMAND_ID_HELP 1
#define COMMAND_ID_HELPCMD 2
#define COMMAND_ID_HELPUTILS 3
#define COMMAND_ID_LOGOUT 4
#define COMMAND_ID_CLS 5
#define COMMAND_ID_CLANG 6
#define COMMAND_ID_MOTD 7
#define COMMAND_ID_MFREE 8
#define COMMAND_ID_MCMD 9
#define COMMAND_ID_RAND 10
#define COMMAND_ID_HUNGLISH 11
#define COMMAND_ID_DIR 12
#define COMMAND_ID_CAT 13
#define COMMAND_ID_CLANG2 14
#define COMMAND_ID_COUNTTO 15

//TODO set keyboard value task which get executed by timer
int exec_command(int cmd_id){
	int retval=1;
	cmd_needs_beep=1;
	switch(cmd_id){
		case COMMAND_ID_HELP:
			set_global_color(0x30);
			printf(HELP_MAN1);
			printf(HELP_MAN2);
			printf(HELP_MAN3);
			printf(HELP_MAN4);
			printf(HELP_MAN5);
			printf(HELP_MAN6);
			set_global_color(0x0);
			break;
		case COMMAND_ID_HELPCMD:
			set_global_color(0x30);
			printf(HELP2_MAN1);
			printf(HELP2_MAN2);
			printf(HELP2_MAN3);
			printf(HELP2_MAN4);
			printf(HELP2_MAN5);
			printf(HELP2_MAN6);
			printf(HELP2_MAN7);
			printf(HELP2_MAN8);
			printf(HELP2_MAN9);
			printf(HELP2_MAN10);
			printf(HELP2_MAN11);
			printf(HELP2_MAN12);
			printf(HELP2_MAN13);
			set_global_color(0x0);
			break;
		
		case COMMAND_ID_HELPUTILS:
			set_global_color(0x30);
			printf(HELP3_MAN1);
			printf(HELP3_MAN2);
			printf(HELP3_MAN3);
			printf(HELP3_MAN4);
			printf(HELP3_MAN5);
			printf(HELP3_MAN6);
			set_global_color(0x0);
			break;
		
		case COMMAND_ID_LOGOUT:
			set_global_color(0x46);
			printf(MSG_LOGOUT);
			set_global_color(0x0);
			i686_panic();
			break;
		
		case COMMAND_ID_CLS:
			clrscr();
			break;
		
		case COMMAND_ID_CLANG:
			printf("\nQQQQ");
			/**/
			break;
			
		case COMMAND_ID_MOTD:
			printf("\n");
			display_motd();
			break;
			
		case COMMAND_ID_MFREE:
			
			enable_free_mode(true);
			retval=2;
			break;
		case COMMAND_ID_MCMD:
		
			enable_free_mode(false);
			retval=3;
			break;
			
		case COMMAND_ID_RAND:
			int t_rand=seeded_rand();
			printf("\n %d",t_rand);
			break;
		
		case COMMAND_ID_HUNGLISH:
			printf("\n");
			gen_hunglish();
			break;
		case COMMAND_ID_DIR:
			printf("\n");
			printRoot();
			break;
		case COMMAND_ID_CAT:
			printf("\n");
			char* filename=global_param_storage[0];
			char correct[FAT_NAME_LENGTH];
			int k=0;
			for(int i=0;i<20;i++){
				if(filename[i]=='\0'){
					break;
				}
				if(filename[i]!='.'){
					correct[k]=filename[i];
					k++;
				}
			}
			
			//printf("%s",filename);
			
			char* temp = getFileContents(correct);
			if(temp==NULL){
				printf("Error reading file!: %s",filename);
			}
			uint32_t i=0;
			while(temp[i]!='\0'){
				char c =temp[i];
				if (c == '\0'){
					break;
				}
				putc(c);
				i++;
			}
			break;
		
		case COMMAND_ID_CLANG2:
			printf("\n");
			int lang_id=(int)(global_param_storage[0][0]-48);
			if(lang_id>3){
				lang_id=0;
			}
			
			//printf("XX%dXX",global_param_storage[0][0]);
			
			create_process(1,lang_id,persistent_prc,true);
			set_global_color(0x47);
			printf(MSG_SETLANG);
			//EN,DE,HU,XU
			
			switch(lang_id){
				case 0:
					printf(LANG_EN);
					break;
				case 1:
					printf(LANG_DE);
					break;
				case 2:
					printf(LANG_HU);
					break;
				case 3:
					printf(LANG_XU);
					break;
			}
			set_global_color(0x0);
			break;
		case COMMAND_ID_COUNTTO:
			printf("\n");
			
			int count_id=(int)(global_param_storage[0][0]-48)+(int)(global_param_storage[0][0]-48)*10+(int)(global_param_storage[0][0]-48)*100;
			global_count_id=count_id;
			int pid=5;
			create_process(1,pid,countto,true);
			break;
		
		default:
			//
			printf("\n" CMD_ERR MSG_DELIM ERR_CMD_INVALID);
			break;
	}
	return retval;
}
bool parse_command(int cmd_idx,int len){
	//cur_command
	//global_commands
	int argc=global_commands[cmd_idx].paramcount;
	
	bool ok=true;
	int i;
	for(i = 0; i < len; i++){
		if(cur_command[i] == ' '){
			i++;
			break;
		}
		if(global_commands[cmd_idx].keyword[i] != cur_command[i] || 
		   cur_command[i] == '\0' || 
		   global_commands[cmd_idx].keyword[i] == '\0'){
			return false;
		}
	}
	
	if(argc != 0){
		clear_global_param_storage();
		int filled_params = 0;
		int gparam_helper = 0;

		while(cur_command[i] != '\0' && filled_params < 10){
			if(cur_command[i] == ' '){
				if(gparam_helper > 0){ // only advance if something was added
					global_param_storage[filled_params][gparam_helper] = '\0';
					filled_params++;
					gparam_helper = 0;
				}
				// skip consecutive spaces
				i++;
				continue;
			}

			if(gparam_helper < MAX_PARAM_LEN - 1){
				global_param_storage[filled_params][gparam_helper] = cur_command[i];
				gparam_helper++;
			}
			i++;
		}

		// terminate last parameter
		if(gparam_helper > 0){
			global_param_storage[filled_params][gparam_helper] = '\0';
			filled_params++;
		}

		if(filled_params == argc){
			ok = true;
			global_param_count = argc;
		} else {
			ok = false;
			global_param_count = -1;
		}
	}
	return ok;
}
void clear_command(){
	for(int i=0;i<SCREEN_WIDTH;i++){
		cur_command[i]='\0';
	}
}

int try_parse_command(){
	
	//reset current command
	clear_command();
	
	int startx=-1;
	int starty=-1;
	getcursor(&startx,&starty);
	if(startx==-1||starty==-1){
		err_inv_command:
		printf(IO_ERR MSG_DELIM ERR_CMD_NOT_FOUND);
		return -1;
	}
	for(int i=0;i<min(startx,MAX_COMMAND_LENGTH);i++){
		cur_command[i]=getchr(i,starty);
	}
	
	int retval=1;
	//try to interpret command
	for(int i=0;i<command_count;i++){
		if(parse_command(i,startx)==true){
			retval=exec_command(global_commands[i].func_id);
			break;
		}
	}
	return retval; //success, 2 is freemode change, 3 is cmd mode change
}

void register_command(char* cmd_name,int paramcount,int func_id){
	strcpy(global_commands[command_count].keyword,cmd_name);
	global_commands[command_count].paramcount=paramcount;
	global_commands[command_count].func_id=func_id;
	
	command_count++;
}

void register_commands(){
	cmd_needs_beep=0;
	
	global_param_count=0;
	command_count=0;
	
	register_command("help",0,COMMAND_ID_HELP);
	register_command("guide",0,COMMAND_ID_HELP);
	
	register_command("helpcmds",0,COMMAND_ID_HELPCMD);
	register_command("helputils",0,COMMAND_ID_HELPUTILS);
	
	register_command("logout",0,COMMAND_ID_LOGOUT);
	register_command("shutdown",0,COMMAND_ID_LOGOUT); //Easily define aliases
	register_command("lo",0,COMMAND_ID_LOGOUT);
	
	//cls
	register_command("cls",0,COMMAND_ID_CLS);
	register_command("clear",0,COMMAND_ID_CLS);
	
	//changelang
	//register_command("changelang",1,COMMAND_ID_CLANG2);
	
	//motd
	register_command("motd",0,COMMAND_ID_MOTD);
	
	//freemode
	register_command("freemode",0,COMMAND_ID_MFREE);
	
	//cmdmode, command mode
	register_command("cmdmode",0,COMMAND_ID_MCMD);
	
	//rand/rnd
	register_command("rand",0,COMMAND_ID_RAND);
	register_command("rnd",0,COMMAND_ID_RAND);
	register_command("random",0,COMMAND_ID_RAND);
	
	//hunglish
	register_command("hunglish",0,COMMAND_ID_HUNGLISH);
	
	//Dir
	register_command("dir",0,COMMAND_ID_DIR);
	register_command("ls",0,COMMAND_ID_DIR);
	
	//Cat
	register_command("cat",1,COMMAND_ID_CAT);
	
	register_command("lang",1,COMMAND_ID_CLANG2);
	
	register_command("countto",1,COMMAND_ID_COUNTTO);
}
