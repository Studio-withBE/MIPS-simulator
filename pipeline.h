#ifndef PIPELINE_H
#define PIPELINE_H

#include "register.h"

Reg IF ();
Reg ID ();
Reg EX ();
Reg MEM ();
void WB ();
void print_status (int cycle);

#endif
