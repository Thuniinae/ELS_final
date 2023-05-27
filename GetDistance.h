#ifndef GET_DISTANCE_H_
#define GET_DISTANCE_H_
#include <systemc>
#include <cmath>
#include "param.h"
using namespace sc_core;
using namespace sc_dt;
using namespace std;

#ifndef NATIVE_SYSTEMC
#include <cynw_p2p.h>
#endif

class GetDistance: public sc_module
{
public:
	sc_in_clk i_clk;
	sc_in < bool >  i_rst;
#ifndef NATIVE_SYSTEMC
	cynw_p2p< sc_dt::sc_uint<24> >::in i_rgb;
	cynw_p2p< sc_dt::sc_biguint<192> >::in i_mean;
	cynw_p2p< sc_dt::sc_biguint<128> >::out o_result;
#else
	sc_fifo_in< sc_dt::sc_uint<24> > i_rgb;
	sc_fifo_in< sc_dt::sc_biguint<192> > i_mean;
	sc_fifo_out< sc_dt::sc_biguint<128> > o_result;
#endif

	SC_HAS_PROCESS( GetDistance );
	GetDistance( sc_module_name n );
private:
	void do_calculation();
	sc_uint<24> read(void);
	sc_biguint<192> read_mean(void); // K-means
	void write(sc_biguint<128> distance); // K-distances
};
#endif
