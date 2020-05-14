#include <iostream>
#include <iomanip>
#include <list>

#include "register.h"

using namespace std;

list<Reg> IF_ID_REGISTER;
list<Reg> ID_EX_REGISTER;
list<Reg> EX_MEM_REGISTER;
list<Reg> MEM_WB_REGISTER;

int 	  cpu_reg[32];
int		  internal_reg[32];

// Store pipeline status from middle register.
void 
store_pipeline (Reg IFID, Reg IDEX, Reg EXMEM, Reg MEMWB)
{
	IF_ID_REGISTER.push_back(IFID);
	ID_EX_REGISTER.push_back(IDEX);
	EX_MEM_REGISTER.push_back(EXMEM);
	MEM_WB_REGISTER.push_back(MEMWB);
}
