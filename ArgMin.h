#ifndef ARG_MIN_H_
#define ARG_MIN_H_
#include <systemc>
#include <cmath>
#include "param.h"
using namespace sc_core;
using namespace sc_dt;
using namespace std;

#ifndef NATIVE_SYSTEMC
#include <cynw_p2p.h>
#endif

class ArgMin: public sc_module
{
public:
	sc_in_clk i_clk;
	sc_in < bool >  i_rst;
#ifndef NATIVE_SYSTEMC
	cynw_p2p< sc_dt::sc_biguint<128> >::in i_distance;
	cynw_p2p< sc_dt::sc_uint<3> >::out o_result;
#else
	sc_fifo_in< sc_dt::sc_biguint<128> > i_distance;
	sc_fifo_out< sc_dt::sc_uint<3> > o_result;
#endif

	SC_HAS_PROCESS( ArgMin );
	ArgMin( sc_module_name n );
private:
	void do_calculation();
    sc_biguint<128> read();
    void write(sc_uint<3> index);
};
#endif
