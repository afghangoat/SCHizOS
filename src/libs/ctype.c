#include "ctype.h"

bool islower(char chr){
    return chr >= 'a' && chr <= 'z';
}

bool isupper(char chr){
	return chr >= 'A' && chr <= 'Z';
}	//Checks whether a character is an uppercase letter

char toupper(char chr){
	if(islower(chr)==true){
		return chr-32;
	} else {
		return chr;
	}
}

bool isalnum(char chr){
	return (isupper(chr)||islower(chr)||(chr>='0'&&chr<='9'));
}	//Checks whether a character is alphanumeric

bool isalpha(char chr){
	return isupper(chr)||islower(chr);
}	//Checks whether a character is a letter

bool isblank(char chr){
	if(chr=='\t'||chr==' '){
		return true;
	}
	return false;
}	//Checks whether a character is a space or tab

bool iscntrl(char chr){
	return chr<=16;
}	//Checks whether a character is a control character
bool isdigit(char chr){
	return chr>='0'&&chr<='9';
}	//Checks whether a character is a decimal digit
bool isgraph(char chr){
	if(chr=='\n'||chr=='\b'||chr==' '){
		return false;
	}
	
	return true;
}	//Checks whether a character has a graphical representation

bool isprint(char chr){
	if(isgraph(chr)==true||iscntrl(chr)==true){
		return false;
	}
	return true;
}	//Checks whether a character is a printable character

bool ispunct(char chr){
	//period, comma, apostrophe, quotation, question, exclamation, brackets, braces, parenthesis, dash, hyphen, ellipsis, colon, semicolon, add back ||chr=='…' if you want to suck
	if(chr=='.'||chr==','||chr=='\''||chr=='\"'||chr=='!'||chr==';'||chr==':'||chr=='('||chr==')'||chr=='['||chr==']'||chr=='{'||chr=='}'||chr=='?'){
		return true;
	}
	return false;
}	//Checks whether a character is a punctuation character

bool isspace(char chr){
	if(chr==' '){
		return true;
	}
	return false;
}	//Checks whether a character is a whitespace character

bool isxdigit(char chr){
	if(isdigit(chr)==true||toupper(chr)=='A'||toupper(chr)=='B'||toupper(chr)=='C'||toupper(chr)=='D'||toupper(chr)=='E'||toupper(chr)=='F'){
		return true;
	}
	return false;
}	//Checks whether a character is a hexadecimal digit

char tolower(char chr){
	if(isupper(chr)==true){
		return chr+32;
	} else {
		return chr;
	}
}	//Returns a lowercase version of a character
