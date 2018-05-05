#ifndef TESTBENCH_H_
#define TESTBENCH_H_

#include <string>
using namespace std;

#include "bmp.h"

class Testbench
{
	public:
		Testbench();
		~Testbench();

		void read_bmp(string infile_name);
		void write_bmp(string outfile_name);
    
	private:
		char *bitmap;
    bmp_data bmp;
    bmp_data bmp_out_c;
};
#endif
