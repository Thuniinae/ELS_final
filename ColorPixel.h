#ifndef COLOR_PIXEL_H
#define COLOR_PIXEL_H_
#include <systemc>
#include <cmath>
#include "param.h"
using namespace sc_core;
using namespace sc_dt;
using namespace std;

#ifndef NATIVE_SYSTEMC
#include <cynw_p2p.h>
#endif

class ColorPixel: public sc_module
{
public:
	sc_in_clk i_clk;
	sc_in < bool >  i_rst;
#ifndef NATIVE_SYSTEMC
	cynw_p2p< sc_dt::sc_uint<3> >::in i_index;
	cynw_p2p< sc_dt::sc_biguint<192> >::in i_mean;
	cynw_p2p< sc_dt::sc_uint<24> >::out o_result;
#else
	sc_fifo_in< sc_dt::sc_uint<3> > i_index;
	sc_fifo_in< sc_dt::sc_biguint<192> > i_mean;
	sc_fifo_out< sc_dt::sc_uint<24> > o_result;
#endif

	SC_HAS_PROCESS( ColorPixel );
	ColorPixel( sc_module_name n );
private:
	void do_calculation();
    sc_biguint<192> read_mean();
    sc_uint<3> read_index();
    void write(sc_uint<24> rgb);
};
#endif
