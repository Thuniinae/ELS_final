#ifndef SOBEL_H_
#define SOBEL_H_
#include <systemc>

#define MASK_N 2
#define MASK_X 3
#define MASK_Y 3
#define WHITE  255
#define BLACK  0

int sobel(double threshold, unsigned int width, unsigned int height, unsigned int byte_per_pixel, char * image_s, char * image_t);

using namespace sc_core;
class Sobel: public sc_module
{
	public:
		SC_HAS_PROCESS( Sobel );
		Sobel( sc_module_name n );
	private:
};
#endif
