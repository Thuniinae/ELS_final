#include "System.h"
System::System( sc_module_name n, string input_bmp, string output_bmp ): sc_module( n ), 
	tb("tb"), gd("gd"), am("am"), nm("nm"), cp("cp"),
	clk("clk", CLOCK_PERIOD, SC_NS), rst("rst"), _output_bmp(output_bmp)
{
	tb.i_clk(clk);	// clock and reset signal for all modules
	tb.o_rst(rst);	//
	gd.i_clk(clk);	//
	gd.i_rst(rst);	//
	am.i_clk(clk);	//
	am.i_rst(rst);	//
	nm.i_clk(clk);	//
	nm.i_rst(rst);	//
	cp.i_clk(clk);	//
	cp.i_rst(rst);	//
	
	tb.o_rgbTG(rgbTG);
	tb.o_meanTG(meanTG);
	tb.o_indexTN(indexTN);
	tb.o_rgbTN(rgbTN);
	tb.o_indexTC(indexTC);
	tb.o_meanTC(meanTC);
	tb.i_indexAT(indexAT);
	tb.i_meanNT(meanNT);
	tb.i_result(result);

	gd.i_rgb(rgbTG);
	gd.i_mean(meanTG);
	gd.o_result(distance);

	am.i_distance(distance);
	am.o_result(indexAT);

	nm.i_index(indexTN);
	nm.i_rgb(rgbTN);
	nm.o_result(meanNT);

	cp.i_index(indexTC);
	cp.i_mean(meanTC);
	cp.o_result(result);

  	tb.read_bmp(input_bmp);
}

System::~System() {
  	tb.write_bmp(_output_bmp);
}
