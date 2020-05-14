#ifndef R_TYPE_H
#define R_TYPE_H

#include "register.h"

Reg R_type_operation (Reg Info);

Reg ADD  (int rd, int rs, int rt, int rs_val, int rt_val);
Reg SUB  (int rd, int rs, int rt, int rs_val, int rt_val);
Reg AND  (int rd, int rs, int rt, int rs_val, int rt_val);
Reg OR   (int rd, int rs, int rt, int rs_val, int rt_val);
Reg NOR  (int rd, int rs, int rt, int rs_val, int rt_val);
Reg SLT  (int rd, int rs, int rt, int rs_val, int rt_val);
Reg SLTU (int rd, int rs, int rt, int rs_val, int rt_val);
Reg SLL  (int rd, int rt, int sa, int rt_val);
Reg SRL  (int rd, int rt, int sa, int rt_val);
Reg JR   (int rs, int rs_val);

#endif
