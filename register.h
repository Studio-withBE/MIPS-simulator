#ifndef REGISTER_H
#define REGISTER_H

#define BYTE 1
#define HALF 2
#define WORD 4

// Register class.
class Reg
{
	public:

		bool stall;
		bool need_MEM;
		bool branch;
		bool unsign;

		unsigned int hex_inst;
		unsigned int address;

		int type;
		int funct;
		int op_code;

		int rs;
		int rt;
		int rd;
		int sa;

		short immediate;

		int result;
		int RW;
		int BHW;

		int rs_val;
		int rt_val;
		int rd_val;

		// Constructor of this class.
		// Initialize all value for register class.
		Reg()
		{
			hex_inst = 0;
			address  = 0;
			type     = 0;
			funct    = 0;
			op_code  = 0;
			result   = 0;

			rs = -1;
			rt = -1;
			rd = -1;
			sa = -1;
			RW = 0;

			BHW		  = 0;
			immediate = 0;
			stall     = false;
			need_MEM  = false;
			branch	  = false;
			unsign    = true;

			rs_val = 0;
			rt_val = 0;
			rd_val = 0;
		}
};

void store_pipeline(Reg IFID, Reg IDEX, Reg EXMEM, Reg MEMWB);

#endif
