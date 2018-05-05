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

		unsigned int size() {
			return _size;
		}

		char get_byte (unsigned int i) {
			return source_bitmap[i];
		}
		void put_byte (unsigned int i, char b) {
			target_bitmap[i] = b;
		}
    
	private:
		char *source_bitmap;
		char *target_bitmap;
    bmp_data bmp;
    bmp_data bmp_out_c;
		unsigned int _size;
};
#endif
