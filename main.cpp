#include <iostream>
#include <string>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <vector>
#include <iomanip>
#include <map>

#include "register.h"
#include "pipeline.h"
#include "parse_code.h"

using namespace std;

extern int 	   		cpu_reg[32];
extern int			internal_reg[32];
extern int	   		mode;
extern unsigned int pc;
extern int			bubble;

extern vector<string> text;
extern map<int, int>  memory;

void 		   init_inst   (string file_name);
void 		   init_reg    (string file_name);
void		   init_memory ();

extern int 			MAX_cycle;
extern int 			now_cycle;

int 
main (int argc, char* argv[])
{
	// Get cycle from argument.
	MAX_cycle = atoi(argv[2]);

	// initialize instruction.
	init_inst(argv[1]);
	init_memory();

	// Initialize register.
	if (argc == 5)
		init_reg(argv[4]);
	else
	{
		for (int i = 0; i < 32; i++)
		{
			cpu_reg[i]  = 0;
			internal_reg[i] = 0;
		}
	}

	// Initialize program counter.
	pc 	   = 0;
	bubble = 0;

	// Initialize mode.
	mode = atoi(argv[3]);

	for (int i = 1; i <= MAX_cycle; i++)
	{
		// Do pipeline operation.
		WB();
		Reg MEMWB = MEM();
		Reg EXMEM = EX();
		Reg IDEX  = ID();
		Reg IFID  = IF();

		// Store middle register information.
		store_pipeline(IFID, IDEX, EXMEM, MEMWB);

		// Print output.
		print_status (i);
	}

	return 0;
}


// initialize register from given file.
void 
init_reg (string file_name)
{
	fstream file;
	file.open(file_name.c_str());

	int i = 0;

	while (!file.eof())
	{
		string temp;
		file >> temp;

		stringstream hexval(temp);
		hexval >> hex >> cpu_reg[i];
		hexval >> hex >> internal_reg[i];

		i++;
	}

	file.close();
	return;
}


// initialize text section from given file.
void 
init_inst (string file_name)
{
	fstream file;
	file.open(file_name.c_str());

	while (!file.eof())
	{
		string temp;
		file >> temp;
		text.push_back(temp);
	}

	file.close();
	return;
}

void 
init_memory ()
{
	for (int i = 0; i < (int)text.size(); i++)
	{
		unsigned int loc 	  = i * 4;
		unsigned int hex_inst = get_hex_inst (text[i]);
		unsigned int mask     = 0x000000FF;

		for (int j = 0; j < 4; j++)
		{
			memory[loc + j] = (hex_inst & mask) >> (8*j);
			mask = mask << 8;
		}
	}
}

