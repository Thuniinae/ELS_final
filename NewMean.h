#ifndef NEW_MEAN_H_
#define NEW_MEAN_H_
#include <systemc>
#include <cmath>
#include "param.h"
using namespace sc_core;
using namespace sc_dt;
using namespace std;

#ifndef NATIVE_SYSTEMC
#include <cynw_p2p.h>
#endif

class NewMean: public sc_module
{
public:
	sc_in_clk i_clk;
	sc_in < bool >  i_rst;
#ifndef NATIVE_SYSTEMC
	cynw_p2p< sc_dt::sc_uint<3> >::in i_index;
	cynw_p2p< sc_dt::sc_uint<24> >::in i_rgb;
	cynw_p2p< sc_dt::sc_biguint<192> >::out o_result;
#else
	sc_fifo_in< sc_dt::sc_uint<3> > i_index;
	sc_fifo_in< sc_dt::sc_uint<24> > i_rgb;
	sc_fifo_out< sc_dt::sc_biguint<192> > o_result;
#endif

	SC_HAS_PROCESS( NewMean );
	NewMean( sc_module_name n );
private:
	void do_calculation();
	sc_uint<10> mean_num[K];
	sc_uint<54> total_mean[K];
	sc_uint<3> read_index();
	sc_uint<24> read();
	void write(sc_biguint<192> mean);

};
#endif
