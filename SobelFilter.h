#ifndef SOBEL_FILTER_H_
#define SOBEL_FILTER_H_
#include <systemc>
using namespace sc_core;

#include <cynw_p2p.h>

#include "filter_def.h"

class SobelFilter: public sc_module
{
public:
	sc_in_clk i_clk;
	sc_in < bool >  i_rst;
	cynw_p2p< sc_uint<24> >::in i_rgb;
	cynw_p2p< sc_uint<32> >::out o_result;

	SC_HAS_PROCESS( SobelFilter );
	SobelFilter( sc_module_name n );
	~SobelFilter();
private:
	void do_filter();
	unsigned char rSpace[MASK_X][MASK_Y];
	unsigned char gSpace[MASK_X][MASK_Y];
	unsigned char bSpace[MASK_X][MASK_Y];
  int val[MASK_N];
};
#endif
