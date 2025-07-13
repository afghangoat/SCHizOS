#pragma once
#include <stdint.h>
#include <stdbool.h>

bool islower(char chr);
char toupper(char chr);

bool isalnum(char chr);	//Checks whether a character is alphanumeric

bool isalpha(char chr);	//Checks whether a character is a letter

bool isblank(char chr);	//Checks whether a character is a space or tab

bool iscntrl(char chr);	//Checks whether a character is a control character

bool isdigit(char chr);	//Checks whether a character is a decimal digit

bool isgraph(char chr);	//Checks whether a character has a graphical representation

bool isprint(char chr);	//Checks whether a character is a printable character

bool ispunct(char chr);	//Checks whether a character is a punctuation character

bool isspace(char chr);	//Checks whether a character is a whitespace character

bool isupper(char chr);	//Checks whether a character is an uppercase letter

bool isxdigit(char chr);	//Checks whether a character is a hexadecimal digit

char tolower(char chr);	//Returns a lowercase version of a character
