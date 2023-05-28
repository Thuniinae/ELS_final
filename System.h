#ifndef SYSTEM_H_
#define SYSTEM_H_
#include <systemc>
using namespace sc_core;

#include "Testbench.h"
#ifndef NATIVE_SYSTEMC
#include "GetDistance_wrap.h"
#include "ArgMin_wrap.h"
#include "NewMean_wrap.h"
#include "ColorPixel_wrap.h"
#else
#include "GetDistance.h"
#include "ArgMin.h"
#include "NewMean.h"
#include "ColorPixel.h"
#endif

class System: public sc_module
{
public:
	SC_HAS_PROCESS( System );
	System( sc_module_name n, std::string input_bmp, std::string output_bmp );
	~System();
private:
  Testbench tb;
#ifndef NATIVE_SYSTEMC
	GetDistance_wrapper gd1, gd2;
	ArgMin_wrapper am1, am2;
	NewMean_wrapper nm;
	ColorPixel_wrapper cp;
#else
	GetDistance gd1, gd2;
	ArgMin am1, am2;
	NewMean nm;
	ColorPixel cp;
#endif
	sc_clock clk;
	sc_signal<bool> rst;
#ifndef NATIVE_SYSTEMC
	cynw_p2p< sc_dt::sc_uint<24>  rgb1;
	cynw_p2p< sc_dt::sc_uint<24> >rgb2;
	cynw_p2p< sc_dt::sc_uint<24> >rgb3;
	cynw_p2p< sc_dt::sc_uint<24> >rgb4;
	cynw_p2p< sc_dt::sc_biguint<192> >i_mean1;
	cynw_p2p< sc_dt::sc_biguint<192> >mean2;
	cynw_p2p< sc_dt::sc_biguint<192> >o_mean1;
	cynw_p2p< sc_dt::sc_uint<24> >result;
	cynw_p2p< sc_dt::sc_biguint<144> >distance1;
	cynw_p2p< sc_dt::sc_biguint<144> >distance2;
	cynw_p2p< sc_dt::sc_uint<3> >index1;
	cynw_p2p< sc_dt::sc_uint<3> >index2;
#else
	sc_fifo< sc_dt::sc_uint<24> >rgb1;
	sc_fifo< sc_dt::sc_uint<24> >rgb2;
	sc_fifo< sc_dt::sc_uint<24> >rgb3;
	sc_fifo< sc_dt::sc_uint<24> >rgb4;
	sc_fifo< sc_dt::sc_biguint<192> >i_mean1;
	sc_fifo< sc_dt::sc_biguint<192> >mean2;
	sc_fifo< sc_dt::sc_biguint<192> >o_mean1;
	sc_fifo< sc_dt::sc_uint<24> >result;
	sc_fifo< sc_dt::sc_biguint<144> > distance1;
	sc_fifo< sc_dt::sc_biguint<144> > distance2;
	sc_fifo< sc_dt::sc_uint<3> > index1;
	sc_fifo< sc_dt::sc_uint<3> > index2;
	sc_fifo< sc_dt::sc_biguint<192> > mean3;
#endif

	std::string _output_bmp;
};
#endif
