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
	GetDistance_wrapper gd;
	ArgMin_wrapper am;
	NewMean_wrapper nm;
	ColorPixel_wrapper cp;
#else
	GetDistance gd;
	ArgMin am;
	NewMean nm;
	ColorPixel cp;
#endif
	sc_clock clk;
	sc_signal<bool> rst;
#ifndef NATIVE_SYSTEMC
	cynw_p2p< sc_dt::sc_uint<24> >rgbTG;
	cynw_p2p< sc_dt::sc_uint<24> >rgbTN;
	cynw_p2p< sc_dt::sc_biguint<192> >meanTG;
	cynw_p2p< sc_dt::sc_biguint<192> >meanNT;
	cynw_p2p< sc_dt::sc_biguint<192> >meanTC;
	cynw_p2p< sc_dt::sc_uint<24> >result;
	cynw_p2p< sc_dt::sc_biguint<144> >distance;
	cynw_p2p< sc_dt::sc_uint<3> >indexAT;
	cynw_p2p< sc_dt::sc_uint<3> >indexTN;
	cynw_p2p< sc_dt::sc_uint<3> >indexTC;
#else
	sc_fifo< sc_dt::sc_uint<24> >rgbTG;
	sc_fifo< sc_dt::sc_uint<24> >rgbTN;
	sc_fifo< sc_dt::sc_biguint<192> >meanTG;
	sc_fifo< sc_dt::sc_biguint<192> >meanNT;
	sc_fifo< sc_dt::sc_biguint<192> >meanTC;
	sc_fifo< sc_dt::sc_uint<24> >result;
	sc_fifo< sc_dt::sc_biguint<144> > distance;
	sc_fifo< sc_dt::sc_uint<3> > indexAT;
	sc_fifo< sc_dt::sc_uint<3> > indexTN;
	sc_fifo< sc_dt::sc_uint<3> > indexTC;
#endif

	std::string _output_bmp;
};
#endif
