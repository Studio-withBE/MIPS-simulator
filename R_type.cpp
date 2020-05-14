#include <iostream>
#include <cmath>

#include "parse_code.h"
#include "pipeline.h"
#include "register.h"
#include "R_type.h"

using namespace std;

extern unsigned int pc;

Reg 
R_type_operation (Reg Info)
{
	// Get register information.
	int fn = Info.funct;

	int rd = Info.rd;
	int rs = Info.rs;
	int rt = Info.rt;
	int sa = Info.sa;

	int rs_val = Info.rs_val;
	int rt_val = Info.rt_val;

	Reg Result;

	// Switch via function code and execute function.
	switch (fn)
	{
		case 32:
			Result = ADD (rd, rs, rt, rs_val, rt_val);
			break;
		case 34:
			Result = SUB (rd, rs, rt, rs_val, rt_val);
			break;
		case 36:
			Result = AND (rd, rs, rt, rs_val, rt_val);
			break;
		case 37:
			Result = OR (rd, rs, rt, rs_val, rt_val);
			break;
		case 39:
			Result = NOR (rd, rs, rt, rs_val, rt_val);
			break;
		case 42:
			Result = SLT (rd, rs, rt, rs_val, rt_val);
			break;
		case 43:
			Result = SLTU (rd, rs, rt, rs_val, rt_val);
			break;
		case 0:
			Result = SLL (rd, rt, rt_val, sa);
			break;
		case 2:
			Result = SRL (rd, rt, rt_val, sa);
			break;
		case 8:
			Result = JR (rs, rs_val);
			break;
	}

	Result.type = R_TYPE;
	return Result;
}


Reg 
ADD (int rd, int rs, int rt, int rs_val, int rt_val)
{
	Reg temp;
	temp.result = rs_val + rt_val;
	temp.rd 	= rd;
	temp.rs		= rs;
	temp.rt		= rt;

	return temp;
}

Reg 
SUB (int rd, int rs, int rt, int rs_val, int rt_val)
{
	Reg temp;
	temp.result = rs_val - rt_val;
	temp.rd		= rd;
	temp.rs		= rs;
	temp.rt		= rt;

	return temp;
}

Reg 
AND (int rd, int rs, int rt, int rs_val, int rt_val)
{
	Reg temp;
	temp.result = ((unsigned int)rs_val & (unsigned int)rt_val);
	temp.rd		= rd;
	temp.rs		= rs;
	temp.rt		= rt;

	return temp;
}

Reg 
OR (int rd, int rs, int rt, int rs_val, int rt_val)
{
	Reg temp;
	temp.result = ((unsigned int)rs_val | (unsigned int)rt_val);
	temp.rd		= rd;
	temp.rs		= rs;
	temp.rt		= rt;

	return temp;
}

Reg 
NOR (int rd, int rs, int rt, int rs_val, int rt_val)
{
	Reg temp;
	temp.result = ~((unsigned int)rs_val | (unsigned int)rt_val);
	temp.rd		= rd;
	temp.rs		= rs;
	temp.rt		= rt;

	return temp;
}

Reg 
SLT (int rd, int rs, int rt, int rs_val, int rt_val)
{
	Reg temp;

	if (rs_val < rt_val)
		temp.result = 1;
	else
		temp.result = 0;

	temp.rd = rd;
	temp.rs = rs;
	temp.rt = rt;

	return temp;
}

Reg 
SLTU (int rd, int rs, int rt, int rs_val, int rt_val)
{
	Reg temp;

	if ((unsigned int)rs_val < (unsigned int)rt_val)
		temp.result = 1;
	else
		temp.result = 0;

	temp.rd = rd;
	temp.rs = rs;
	temp.rt = rt;

	return temp;
}

Reg 
SLL (int rd, int rt, int rt_val, int sa)
{
	Reg temp;
	temp.result = ((unsigned int)rt_val << (unsigned int)sa);
	temp.rd 	= rd;
	temp.rt 	= rt;
	temp.sa 	= sa;

	return temp;
}

Reg 
SRL (int rd, int rt, int rt_val, int sa)
{
	Reg temp;
	temp.result = (unsigned int)rt_val >> (unsigned int)sa;
	temp.rd		= rd;
	temp.rt		= rt;
	temp.sa		= sa;

	return temp;
}

Reg 
JR (int rs, int rs_val)
{
	Reg temp;
	temp.rs 	= rs;
	temp.result = rs_val / 4;

	pc = temp.result;
	return temp;
}

