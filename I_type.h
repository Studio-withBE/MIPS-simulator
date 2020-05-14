#ifndef I_TYPE_H
#define I_TYPE_H

#include "register.h"

Reg I_type_operation (Reg Info);

Reg ADDI  (int rt, int rs, short imm, int rs_val);
Reg ANDI  (int rt, int rs, short imm, int rs_val);
Reg ORI   (int rt, int rs, short imm, int rs_val);
Reg SLTI  (int rt, int rs, short imm, int rs_val);
Reg SLTIU (int rt, int rs, short imm, int rs_val);
Reg LW	  (int rt, int rs, short imm, int rs_val);
Reg SW    (int rt, int rs, short imm, int rs_val);
Reg LH    (int rt, int rs, short imm, int rs_val);
Reg LHU   (int rt, int rs, short imm, int rs_val);
Reg SH	  (int rt, int rs, short imm, int rs_val);
Reg LB    (int rt, int rs, short imm, int rs_val);
Reg LBU   (int rt, int rs, short imm, int rs_val);
Reg SB    (int rt, int rs, short imm, int rs_val);
Reg BEQ   (int rs, int rt, short imm, int rs_val, int rt_val);
Reg BNE   (int rs, int rt, short imm, int rs_val, int rt_val);
Reg LUI   (int rt, short imm);

#endif
