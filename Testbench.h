#ifndef TESTBENCH_H_
#define TESTBENCH_H_

#include <string>
using namespace std;

class Testbench
{
	public:
		Testbench();
		~Testbench();

		int read_bmp(string infile_name);
		int write_bmp(string outfile_name);

		unsigned int get_width() {
			return width;
		}

		unsigned int get_height() {
			return height;
		}
		
		unsigned int get_width_bytes() {
			return width_bytes;
		}

		unsigned int get_bytes_per_pixel() {
			return bytes_per_pixel;
		}

		unsigned char* get_source_image () {
			return source_bitmap;
		}
		unsigned char* get_target_image () {
			return target_bitmap;
		}
    
	private:
		unsigned int rgb_raw_data_offset;
		unsigned int width;
		unsigned int height;
		unsigned int width_bytes;
		unsigned char  bits_per_pixel;
		unsigned short bytes_per_pixel;
		unsigned char *source_bitmap;
		unsigned char *target_bitmap;
};
#endif
