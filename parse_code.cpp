#include <iostream>
#include <string>
#include <sstream>

#include "parse_code.h"

using namespace std;

// Convert string instruction to hexadecimal integer
unsigned int 
get_hex_inst (string inst)
{
	unsigned int temp;
	stringstream ss;
	ss << hex << inst;
	ss >> temp;
	return temp;
}


// Get address from binary instruction.
unsigned int 
get_address (unsigned int hex_inst)
{
	const unsigned int addr_mask = 0x03FFFFFF;
	unsigned int next = (hex_inst & addr_mask);
	return next;
}


// Get op-code from binary instruction.
unsigned int 
get_opcode (unsigned int hex_inst)
{
	const unsigned int op_mask = 0xFC000000;
	unsigned int code = (hex_inst & op_mask) >> 26;
	return code;
}


// Get type of instruction from binary instruction.
int 
get_type (unsigned int hex_inst)
{
	unsigned int op_code = get_opcode(hex_inst);

	if (op_code == 0)
		return R_TYPE;

	else if (op_code == 2 || op_code == 3)
		return J_TYPE;

	return I_TYPE;
}


// Get immediate from binary instruction.
short 
get_immediate (unsigned int hex_inst)
{
	const unsigned int imask = 0x0000FFFF;
	short immediate = (short)(hex_inst & imask);
	return immediate;
}


// Get rs from binary instruction.
int 
get_rs (unsigned int hex_inst)
{
	const unsigned int mask = 0x03E00000;
	int rs = (hex_inst & mask) >> 21;
	return rs;
}


// Get rt from binary instruction.
int 
get_rt (unsigned int hex_inst)
{
	const unsigned int mask = 0x001F0000;
	int rt = (hex_inst & mask) >> 16;
	return rt;
}


// Get rd from binary instruction.
int 
get_rd (unsigned int hex_inst)
{
	const unsigned int mask = 0x0000F800;
	int rd = (hex_inst & mask) >> 11;
	return rd;
}


// Get sa from binary instruction.
int 
get_sa (unsigned int hex_inst)
{
	const unsigned int mask = 0x000007C0;
	int sa = (hex_inst & mask) >> 6;
	return sa;
}


// Get function from binary instruction.
int 
get_funct (unsigned int hex_inst)
{
	const unsigned int funct_mask = 0x0000003F;
	int funct = (hex_inst & funct_mask);
	return funct;
}
