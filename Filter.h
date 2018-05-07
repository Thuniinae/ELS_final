#ifndef FILTER_H_
#define FILTER_H_
#include <systemc>
using namespace sc_core;

#define MASK_N 2
#define MASK_X 3
#define MASK_Y 3
#define WHITE 255
#define BLACK 0
#define THRESHOLD 90

class Filter: public sc_module
{
public:
	sc_in_clk i_clk;
	sc_fifo_in< unsigned char > i_r;
	sc_fifo_in< unsigned char > i_g;
	sc_fifo_in< unsigned char > i_b;
	sc_fifo_out< int > o_result;

	SC_HAS_PROCESS( Filter );
	Filter( sc_module_name n );
	~Filter();
private:
	void do_filter();
};
#endif
