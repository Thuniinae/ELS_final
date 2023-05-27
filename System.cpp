#include "System.h"
System::System( sc_module_name n, string input_bmp, string output_bmp ): sc_module( n ), 
	tb("tb"), gd1("gd1"), gd2("gd2"), am1("am1"), am2("am2"), nm("nm"), cp("cp"),
	clk("clk", CLOCK_PERIOD, SC_NS), rst("rst"), _output_bmp(output_bmp)
{
	tb.i_clk(clk);	// clock and reset signal for all modules
	tb.o_rst(rst);	//
	gd1.i_clk(clk);	//
	gd1.i_rst(rst);	//
	am1.i_clk(clk);	//
	am1.i_rst(rst);	//
	nm.i_clk(clk);	//
	nm.i_rst(rst);	//
	gd2.i_clk(clk);	//
	gd2.i_rst(rst);	//
	am2.i_clk(clk);	//
	am2.i_rst(rst);	//
	cp.i_clk(clk);	//
	cp.i_rst(rst);	//
	
	tb.o_rgb1(rgb1);			// module connections
	tb.o_mean1(i_mean1);
	gd1.i_rgb(rgb1);
	gd1.i_mean(i_mean1);
	gd1.o_result(distance1);
	am1.i_distance(distance1);
	am1.o_result(index1);
	nm.i_index(index1);
	tb.o_rgb2(rgb2);
	nm.i_rgb(rgb2);
	nm.o_result(o_mean1);
	tb.i_mean1(o_mean1);
	tb.o_mean2(mean2);
	tb.o_rgb3(rgb3);
	gd2.i_mean(mean2);
	gd2.i_rgb(rgb3);
	gd2.o_result(distance2);
	am2.i_distance(distance2);
	am2.o_result(index2);
	cp.i_index(index2);
	tb.o_mean3(mean3);
	cp.i_mean(mean3);
	cp.o_result(result);
	tb.i_result(result);
  	tb.read_bmp(input_bmp);
}

System::~System() {
  	tb.write_bmp(_output_bmp);
}
