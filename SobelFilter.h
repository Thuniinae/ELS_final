#ifndef SOBEL_FILTER_H_
#define SOBEL_FILTER_H_
#include <systemc>
#include <cmath>
#include "param.h"
using namespace sc_core;
using namespace sc_dt;
using namespace std;

#ifndef NATIVE_SYSTEMC
#include <cynw_p2p.h>
#endif

class SobelFilter: public sc_module
{
public:
	sc_in_clk i_clk;
	sc_in < bool >  i_rst;
#ifndef NATIVE_SYSTEMC
	cynw_p2p< sc_dt::sc_uint<24> >::in i_rgb;
	cynw_p2p< sc_dt::sc_uint<24> >::out o_result;
#else
	sc_fifo_in< sc_dt::sc_uint<24> > i_rgb;
	sc_fifo_out< sc_dt::sc_uint<24> > o_result;
#endif

	SC_HAS_PROCESS( SobelFilter );
	SobelFilter( sc_module_name n );
	~SobelFilter();
private:
	sc_uint<24> mean[K];
	sc_uint<48> mean_total[K];
	void do_filter();
	sc_uint<24> read();
	void write(sc_uint<24> rgb);
	int cal_distance(sc_uint<24> from_rgb, sc_uint<24> to_rgb);
	sc_uint<8> arg_min(int distance[], int n);
};
#endif
