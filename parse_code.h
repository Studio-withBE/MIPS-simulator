#ifndef PARSE_CODE_H
#define PARSE_CODE_H

#include <string>

#define R_TYPE 0
#define I_TYPE 1
#define J_TYPE 2

#define READ 1
#define WRITE 2

using namespace std;

unsigned int get_hex_inst  (string inst);
unsigned int get_address   (unsigned int hex_inst);
unsigned int get_opcode    (unsigned int hex_inst);

short get_immediate (unsigned int hex_inst);

int get_type  (unsigned int hex_inst);
int get_rs	  (unsigned int hex_inst);
int get_rt	  (unsigned int hex_inst);
int get_rd    (unsigned int hex_inst);
int get_sa    (unsigned int hex_inst);
int get_funct (unsigned int hex_inst);

#endif
