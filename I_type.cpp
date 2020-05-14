#include <iostream>
#include <cmath>

#include "pipeline.h"
#include "parse_code.h"
#include "register.h"
#include "I_type.h"

using namespace std;

extern int internal_reg[32];
extern unsigned int pc;

Reg 
I_type_operation (Reg Info)
{
	Reg Result;

	int opcode = Info.op_code;
	int rs	   = Info.rs;
	int rt     = Info.rt;

	short imm = Info.immediate;

	int rs_val = Info.rs_val;
	int rt_val = Info.rt_val;

	switch (opcode)
	{
		case 8:
			Result = ADDI (rt, rs, imm, rs_val);
			break;
		case 12:
			Result = ANDI (rt, rs, imm, rs_val);
			break;
		case 13:
			Result = ORI (rt, rs, imm, rs_val);
			break;
		case 10:
			Result = SLTI (rt, rs, imm, rs_val);
			break;
		case 11:
			Result = SLTIU (rt, rs, imm, rs_val);
			break;
		case 35:
			Result = LW (rt, rs, imm, rs_val);
			break;
		case 43:
			Result = SW (rt, rs, imm, rs_val);
			break;
		case 33:
			Result = LH (rt, rs, imm, rs_val);
			break;
		case 37:
			Result = LHU (rt, rs, imm, rs_val);
			break;
		case 41:
			Result = SH (rt, rs, imm, rs_val);
			break;
		case 32:
			Result = LB (rt, rs, imm, rs_val);
			break;
		case 36:
			Result = LBU (rt, rs, imm, rs_val);
			break;
		case 40:
			Result = SB (rt, rs, imm, rs_val);
			break;
		case 15:
			Result = LUI (rt, imm);
			break;
		case 4:
			Result = BEQ (rs, rt, imm, rs_val, rt_val);
			pc = (unsigned int)Result.result;
			break;
		case 5:
			Result = BNE (rs, rt, imm, rs_val, rt_val);
			pc = (unsigned int)Result.result;
			break;
	}

	Result.type = I_TYPE;
	return Result;
}

Reg 
ADDI (int rt, int rs, short imm, int rs_val)
{
	Reg temp;
	temp.rs 	   = rs;
	temp.rt 	   = rt;
	temp.immediate = imm;
	temp.result = rs_val + (int)imm;

	return temp;
}

Reg 
ANDI (int rt, int rs, short imm, int rs_val)
{
	Reg temp;
	temp.rs		   = rs;
	temp.rt 	   = rt;
	temp.immediate = imm;

	unsigned int temp_num = (unsigned int)imm & 0x0000FFFF;
	temp.result = rs_val & temp_num;;

	return temp;
}

Reg 
ORI (int rt, int rs, short imm, int rs_val)
{
	Reg temp;
	temp.rs		   = rs;
	temp.rt 	   = rt;
	temp.immediate = imm;

	unsigned int temp_num = (unsigned int)imm & 0x0000FFFF;
	temp.result = rs_val | temp_num;

	return temp;
}

Reg 
SLTI (int rt, int rs, short imm, int rs_val)
{
	Reg temp;
	temp.rs		   = rs;
	temp.rt 	   = rt;
	temp.immediate = imm;

	if (rs_val < (int)imm)
		temp.result = 1;
	else
		temp.result = 0;

	return temp;
}

Reg 
SLTIU (int rt, int rs, short imm, int rs_val)
{
	Reg temp;
	temp.rs		   = rs;
	temp.rt 	   = rt;
	temp.immediate = imm;

	if ((unsigned int)rs_val < (unsigned int)imm)
		temp.result = 1;
	else
		temp.result = 0;

	return temp;
}

Reg 
LW (int rt, int rs, short imm, int rs_val)
{
	Reg temp;
	temp.rs		   = rs;
	temp.rt 	   = rt;
	temp.immediate = imm;

	temp.result   = rs_val + imm;
	temp.need_MEM = true;
	temp.RW       = READ;
	temp.BHW      = WORD;
	return temp;
}

Reg 
SW (int rt, int rs, short imm, int rs_val)
{
	Reg temp;
	temp.rs		   = rs;
	temp.rt 	   = rt;
	temp.immediate = imm;

	temp.result   = rs_val + imm;
	temp.need_MEM = true;
	temp.RW       = WRITE;
	temp.BHW      = WORD;
	return temp;
}

Reg 
LH (int rt, int rs, short imm, int rs_val)
{
	Reg temp;
	temp.rs		   = rs;
	temp.rt 	   = rt;
	temp.immediate = imm;

	temp.result   = rs_val+ imm;
	temp.need_MEM = true;
	temp.RW       = READ;
	temp.BHW      = HALF;
	temp.unsign	  = false;
	return temp;
}

Reg 
LHU (int rt, int rs, short imm, int rs_val)
{
	Reg temp;
	temp.rs		   = rs;
	temp.rt 	   = rt;
	temp.immediate = imm;

	temp.result   = rs_val + imm;
	temp.need_MEM = true;
	temp.RW       = READ;
	temp.BHW      = HALF;
	return temp;
}

Reg 
SH (int rt, int rs, short imm, int rs_val)
{
	Reg temp;
	temp.rs		   = rs;
	temp.rt 	   = rt;
	temp.immediate = imm;

	temp.result   = rs_val + imm;
	temp.need_MEM = true;
	temp.RW       = WRITE;
	temp.BHW      = HALF;
	return temp;
}

Reg 
LB (int rt, int rs, short imm, int rs_val)
{
	Reg temp;
	temp.rs		   = rs;
	temp.rt 	   = rt;
	temp.immediate = imm;

	temp.result   = rs_val + imm;
	temp.need_MEM = true;
	temp.RW       = READ;
	temp.BHW      = BYTE;
	temp.unsign   = false;
	return temp;
}

Reg 
LBU (int rt, int rs, short imm, int rs_val)
{
	Reg temp;
	temp.rs		   = rs;
	temp.rt 	   = rt;
	temp.immediate = imm;
	
	temp.result   = rs_val + imm;
	temp.need_MEM = true;
	temp.RW       = READ;
	temp.BHW      = BYTE;
	return temp;
}

Reg 
SB (int rt, int rs, short imm, int rs_val)
{
	Reg temp;
	temp.rs		   = rs;
	temp.rt 	   = rt;
	temp.immediate = imm;

	temp.result   = rs_val + imm;
	temp.need_MEM = true;
	temp.RW       = WRITE;
	temp.BHW      = BYTE;
	return temp;
}

Reg 
LUI (int rt, short imm)
{
	Reg temp;
	temp.rt		   = rt;
	temp.immediate = imm;

	temp.result   = imm * pow(2, 16);
	return temp;
}

Reg 
BEQ (int rs, int rt, short imm, int rs_val, int rt_val)
{
	Reg temp;
	temp.rs 	   = rs;
	temp.rt 	   = rt;
	temp.immediate = imm;
	temp.branch    = true;

	if (rs_val == rt_val)
		temp.result = pc + (int)imm;
	else
		temp.result = pc;

	return temp;
}

Reg 
BNE (int rs, int rt, short imm, int rs_val, int rt_val)
{
	Reg temp;
	temp.rs 	   = rs;
	temp.rt 	   = rt; 
	temp.immediate = imm;
	temp.branch    = true;

	if (rs_val != rt_val)
		temp.result = pc + (int)imm;
	else
		temp.result = pc;

	return temp;
}

