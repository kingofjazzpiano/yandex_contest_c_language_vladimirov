#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>
#include <iso646.h>
#include <stdbool.h>


bool starts_with(const char *str, const char *prefix);
bool encoder(const char *line);


int main(){
	size_t LINE_BUFFER_SIZE = (size_t)100;
	char *line = (char *)malloc(sizeof(char) * LINE_BUFFER_SIZE);
	if (NULL == line){
		fprintf(stderr, "Not enough memory!\n");
		abort();
	}
	while (fgets(line, (int)LINE_BUFFER_SIZE, stdin)){
		if (false == encoder((const char *)line)){
			return 0;
		}
	}
}


bool starts_with(const char *str, const char *prefix){
	// Checks if str contains a prefix.
	if (NULL == str or NULL == prefix){
		fprintf(stderr, "Null pointer error!\n");
		abort();
	}
	for (size_t i = 0; prefix[i] != '\0'; i++){
		if (str[i] != prefix[i]){
			return false;
		}
	}
	return true;
}


bool encoder(const char *line){
	if (NULL == line){
		return true;
	}

#define ASCII_TABLE_SIZE (size_t)128

	uint8_t mnemonics[ASCII_TABLE_SIZE];
	uint8_t registers[ASCII_TABLE_SIZE];

	mnemonics['I'] = 0x00;
	mnemonics['D'] = 0x80;
	mnemonics['B'] = 0x90;
	mnemonics['L'] = 0xA0;
	mnemonics['V'] = 0xB0;
	mnemonics['N'] = 0xC0;
	mnemonics['T'] = 0xC4;

	registers['A'] = 0b00;
	registers['B'] = 0b01;
	registers['C'] = 0b10;
	registers['D'] = 0b11;

	if (not (starts_with(line, "MOVI ") or starts_with(line, "ADD ")
			or starts_with(line, "SUB ") or starts_with(line, "MUL ")
			or starts_with(line, "DIV ") or starts_with(line, "IN ")
			or starts_with(line, "OUT "))){
		printf("ERROR");
		return false;
	}
	size_t i = 0;
	for ( ; line[i] != ' '; i++);
	uint8_t command = mnemonics[(size_t)line[i - 1]];
	i++;
	if (0x00 == command){  // MOVI immediate
		command = (uint8_t)atoi(line + i);
	}
	else if (0x80 == command){  // ADD R, R
		if (line[i] < 'A' or line[i] > 'D' or line[i + 3] < 'A' or line[i + 3] > 'D'){
			printf("ERROR");
			return false;
		}
		command |= (registers[(size_t)line[i]] << 2);  // first register
		command |= (registers[(size_t)line[i + 3]]);  // second register
	}
	else if (0x90 == command){  // SUB R, R
		if (line[i] < 'A' or line[i] > 'D' or line[i + 3] < 'A' or line[i + 3] > 'D'){
			printf("ERROR");
			return false;
		}
		command |= (registers[(size_t)line[i]] << 2);  // first register
		command |= (registers[(size_t)line[i + 3]]);  // second register
	}
	else if (0xA0 == command){  // MUL R, R
		if (line[i] < 'A' or line[i] > 'D' or line[i + 3] < 'A' or line[i + 3] > 'D'){
			printf("ERROR");
			return false;
		}
		command |= (registers[(size_t)line[i]] << 2);  // first register
		command |= (registers[(size_t)line[i + 3]]);  // second register
	}
	else if (0xB0 == command){  // DIV R, R
		if (line[i] < 'A' or line[i] > 'D' or line[i + 3] < 'A' or line[i + 3] > 'D'){
			printf("ERROR");
			return false;
		}
		command |= (registers[(size_t)line[i]] << 2);  // first register
		command |= (registers[(size_t)line[i + 3]]);  // second register			
	}
	else if (0xC0 == command){  // IN R
		if (line[i] < 'A' or line[i] > 'D'){
			printf("ERROR");
			return false;
		}
		command |= (registers[(size_t)line[i]]);
	}
	else if (0xC4 == command){  // OUT R
		if (line[i] < 'A' or line[i] > 'D'){
			printf("ERROR");
			return false;
		}
		command |= (registers[(size_t)line[i]]);
	}
	else {
		printf("ERROR");
		return false;
	}
	printf("0x%x ", command);
	return true;
}
