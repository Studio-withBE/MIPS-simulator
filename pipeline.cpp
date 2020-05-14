#include <iostream>
#include <string>
#include <utility>
#include <iomanip>
#include <vector>
#include <list>
#include <map>

#include "parse_code.h"
#include "register.h"
#include "R_type.h"
#include "I_type.h"

using namespace std;

extern int 	     cpu_reg[32];
extern int		 internal_reg[32];
int				 print_reg[32];

extern list<Reg> IF_ID_REGISTER;
extern list<Reg> ID_EX_REGISTER;
extern list<Reg> EX_MEM_REGISTER;
extern list<Reg> MEM_WB_REGISTER;

int				 MAX_cycle;
int			     mode;
int				 bubble;
bool			 hazard;
unsigned int     pc;
unsigned int	 prev_pc;
unsigned int     pc_print;
map<int, char>	 memory;
vector<string>   text;

string			 inst_print;
int				 loc_print;
int				 data_print;
int				 byte_print;
char			 memory_print;

// Print status of PC, instruction, registers and memory I/O.
void 
print_status (int cycle)
{
	cout << "Cycle " << dec << cycle << endl;
	cout << "PC: ";
	cout << setfill('0') << setw(4) << hex << pc_print * 4 << endl;
	cout << "Instruction: " << inst_print << endl;
	cout << "Register: " << endl;

	for (int i = 0; i < 32; i++)
	{
		cout << "[" << dec << i << "] ";
		cout << setfill('0') << setw(8) << hex << uppercase << print_reg[i] << endl;
	}

	cout << "Memory I/O: ";

	if (memory_print == 'R' || memory_print == 'W')
	{
		cout << memory_print << " " << byte_print << " ";
		cout << setfill('0') << setw(4) << hex << uppercase << loc_print << " ";
		cout << setfill('0') << setw(byte_print * 2) << hex << uppercase << data_print;
		memory_print = 'N';
	}

	cout << endl;
}

// Copy current cpu register status for printing.
void 
copy_for_print ()
{
	for (int i = 0; i < 32; i++)
		print_reg[i] = cpu_reg[i];
}

// Instrunction Fetch
Reg 
IF ()
{
	if (pc >= text.size())
	{
		Reg temp;
		temp.stall = true;
		return temp;
	}

	// To insert bubble between branch instruction and next.
	if (bubble > 0)
	{
		pc_print = prev_pc;
		inst_print = text[prev_pc];
		for (int i = 0; i < 8; i++)
			inst_print[i] = toupper(inst_print[i]);

		// Disenable bubble.
		bubble -= 1;
		Reg temp;
		temp.stall = true;
		return temp;
	}

	string 		 instruction = text[pc];
	unsigned int hex_inst    = get_hex_inst(instruction);

	// Handle Nop.
	if (hex_inst == 0)
	{
		Reg temp;
		temp.stall = true;
		pc += 1;
		return temp;
	}

	// Print current program counter and instruction.
	pc_print = pc;

	inst_print = instruction;
	for (int i = 0; i < 8; i++)
		inst_print[i] = toupper(inst_print[i]);

	// Increase program counter and save previous counter.
	prev_pc = pc;
	pc += 1;

	// Return IF/ID register.
	Reg temp;
	temp.hex_inst = hex_inst;
	return temp;
}


// Instruction Decode
Reg 
ID ()
{
	// Copy registers for printing.
	copy_for_print();

	if (IF_ID_REGISTER.empty())
	{
		Reg temp;
		temp.stall = true;
		return temp;
	}

	// Get register information from IF/ID register.
	Reg IF_reg = IF_ID_REGISTER.front();

	if (IF_reg.stall == true)
	{
		IF_ID_REGISTER.pop_front();
		return IF_reg;
	}

	unsigned int hex_inst = IF_reg.hex_inst;

	Reg temp;
	temp.hex_inst = hex_inst;
	temp.type = get_type (hex_inst);

	// Instruction type checking
	if (temp.type == J_TYPE)
	{
		// In case of J-type, get address from instuction
		// Then, modify program counter.
		temp.op_code = get_opcode (hex_inst);
		
		switch (temp.op_code)
		{
			// j instruction.
			case 2:
				pc = get_address (hex_inst);
				temp.stall = true;
				break;
			// jal instuction.
			case 3:
				temp.result = pc * 4;
				if (mode == 1)
					internal_reg[31] = temp.result;
				else
					cpu_reg[31] = temp.result;
				temp.need_MEM = false;
				pc = get_address (hex_inst);
				bubble += 1;
				break;
		}
	}

	// In other cases, parse the instuction. (not J-type)
	else if (temp.type == R_TYPE)
	{
		temp.funct = get_funct (hex_inst);
		temp.rs	   = get_rs	   (hex_inst);
		temp.rt    = get_rt    (hex_inst);
		temp.rd    = get_rd    (hex_inst);
		temp.sa    = get_sa    (hex_inst);

		// To avoid control hazard from JR instruction.
		if (temp.funct == 8)
			bubble += 1;

		// Handle data hazard.
		if (mode == 1)
		{
			temp.rs_val = internal_reg[temp.rs];
			temp.rt_val = internal_reg[temp.rt];
			hazard = false;
		}
		else
		{
			temp.rs_val = cpu_reg[temp.rs];
			temp.rt_val = cpu_reg[temp.rt];
		}
	}

	else if (temp.type == I_TYPE)
	{
		temp.op_code = get_opcode (hex_inst);
		temp.rs		 = get_rs     (hex_inst);
		temp.rt      = get_rt     (hex_inst);

		temp.immediate = get_immediate (hex_inst);

		// Handle control hazard by load instruction.
		if (temp.op_code == 35 || temp.op_code == 33 || temp.op_code == 37 || temp.op_code == 32 || temp.op_code == 36)
			bubble++;

		// Handle data hazard.
		if (mode == 1)
		{
			temp.rs_val  = internal_reg[temp.rs];
			temp.rt_val  = internal_reg[temp.rt];
			hazard = false;
		}
		else
		{
			temp.rs_val = cpu_reg[temp.rs];
			temp.rt_val = cpu_reg[temp.rs];
		}

		// If the instruction is branch, add stall before next instruction.
		if (temp.op_code == 4 || temp.op_code == 5)
			bubble += 1;
	}

	// Remove current IF/ID register.
	IF_ID_REGISTER.pop_front();

	// Return ID/EX register.
	return temp;
}


// Execution stage
Reg 
EX ()
{
	if (ID_EX_REGISTER.empty())
	{
		Reg temp;
		temp.stall = true;
		return temp;
	}

	// Get ID/EX register information.
	Reg temp = ID_EX_REGISTER.front();

	if (temp.stall == true)
	{
		ID_EX_REGISTER.pop_front();
		return temp;
	}

	if (temp.type == J_TYPE)
	{
		ID_EX_REGISTER.pop_front();
		return temp;
	}

	Reg Result;

	// Data Hazard detection
	if (mode == 1 && temp.rd != 0)
	{
		if (temp.rd == ID_EX_REGISTER.front().rs || temp.rd == ID_EX_REGISTER.front().rt)
			hazard = true;
	}

	// Type checking and use ALU
	if (temp.type == R_TYPE)
	{
		Result = R_type_operation (temp);
		Result.type = R_TYPE;
		Result.need_MEM = false;

		if (mode == 1)
			internal_reg[Result.rd] = Result.result;
		else
			cpu_reg[Result.rd] = Result.result;
	}

	else if (temp.type == I_TYPE)
	{
		Result = I_type_operation(temp);
		Result.type = I_TYPE;

		if (mode == 1 && Result.need_MEM == false && Result.branch == false)
			internal_reg[Result.rt] = Result.result;
	}

	// Remove current ID/EX register.
	ID_EX_REGISTER.pop_front();

	// Return EX/MEM register.
	return Result;
}


// Memory access stage
Reg 
MEM ()
{
	if (EX_MEM_REGISTER.empty())
	{
		Reg temp;
		temp.stall = true;
		return temp;
	}

	// Get EX/MEM register.
	Reg temp = EX_MEM_REGISTER.front();

	// If current instruction doesn't want to use memory,
	// just return and goto WB stage.
	if (temp.need_MEM == false || temp.stall == true)
	{
		cout << endl;
		EX_MEM_REGISTER.pop_front();
		return temp;
	}

	// Data Hazard detection
	if (mode == 1 && temp.rd != 0)
	{
		if (temp.rd == ID_EX_REGISTER.front().rs || temp.rd == ID_EX_REGISTER.front().rt)
			hazard = true;
	}

	// Get location of memory from register.
	int loc = temp.result;

	// For printing memory I/O
	loc_print 	 = loc;
	byte_print 	 = temp.BHW;

	// Check whether instuction want to read or write.
	if (temp.RW == READ)
	{
		memory_print = 'R';
		unsigned int data = 0;
		unsigned int mask = 0x000000FF;

		for (int i = temp.BHW - 1; i >= 0; i--) {
			data += memory[loc + i] & mask;

			if (i != 0)
				data = data << 8;
		}

		// Sign extension for LB, LH instruction.
		if (temp.unsign == false)
		{
			unsigned int test = 0;

			if (temp.BHW == BYTE)
			{
				mask = 0x00000080;
				test = (data & mask) >> 7;
			}
			else if (temp.BHW == HALF)
			{
				mask = 0x00008000;
				test = (data & mask) >> 15;
			}

			if (test == 1)
			{
				if (temp.BHW == BYTE)
					data = (data | 0xFFFFFF00);
				else if (temp.BHW == HALF)
					data = (data | 0xFFFF0000);
			}
		}
		
		temp.result = data;

		// Handle data hazard.
		if (mode == 1)
			internal_reg[temp.rt] = temp.result;

		data_print = temp.result;
	}
	else if (temp.RW == WRITE)
	{
		memory_print = 'W';
		unsigned int mask = 0x000000FF;
		unsigned int data = 0;

		for (int i = 0; i < temp.BHW; i++)
		{
			// Handle data hazard by moding.
			if (mode == 1)
				data = internal_reg[temp.rt] & mask;
			else
				data = cpu_reg[temp.rt] & mask;

			data = data >> (8 * i);
			memory.insert(make_pair(loc + i, (char)data));
			mask = mask << 8;
		}

		data_print = internal_reg[temp.rt];
		temp.stall = true;
	}

	cout << endl;

	// Remove current EX/MEM register.
	EX_MEM_REGISTER.pop_front();
	return temp;
}

void 
WB ()
{
	if (MEM_WB_REGISTER.empty())
		return;

	// Get register information from MEM/WB register.
	Reg temp = MEM_WB_REGISTER.front();
	
	// If instuction require stall, just do nothing.
	if (temp.stall == true)
	{
		MEM_WB_REGISTER.pop_front();
		return;
	}

	// Instruction type checking.
	if (temp.type == R_TYPE)
	{
		//Write back result of ALU to destination register.
		if (temp.rd != -1)
			cpu_reg[temp.rd] = temp.result;
	}
	else if (temp.type == I_TYPE && temp.RW != WRITE && temp.branch != true)
	{
		// Write back result to target register.
		if (temp.rt != -1)
			cpu_reg[temp.rt] = temp.result;
	}
	else if (temp.type == J_TYPE && temp.stall == false)
	{
		// Only JAL instruction can be survived.
		cpu_reg[31] = temp.result;
	}

	// Remove MEM/
	MEM_WB_REGISTER.pop_front();
	return;
}
