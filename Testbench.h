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

		unsigned int get_width() {
			return bmp.get_width();
		}

		unsigned int get_height() {
			return bmp.get_height();
		}
		
		unsigned int get_width_bytes() {
			return bmp.get_width_bytes();
		}

		unsigned int get_bytes_per_pixel() {
			return bmp.get_bytes_per_pixel();
		}

		char* get_source_image () {
			return source_bitmap;
		}
		char* get_target_image () {
			return target_bitmap;
		}
    
	private:
		char *source_bitmap;
		char *target_bitmap;
    bmp_data bmp;
    bmp_data bmp_out_c;
};
#endif
