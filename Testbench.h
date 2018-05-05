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

		unsigned int width() {
			return bmp.get_width();
		}

		unsigned int height() {
			return bmp.get_height();
		}

		unsigned int byte_per_pixel() {
			return bmp.get_bytes_per_pixel();
		}

		unsigned int size() {
			return _size;
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
		unsigned int _size;
};
#endif
