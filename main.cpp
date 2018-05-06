#include <iostream>
#include <string>
using namespace std;

// Wall Clock Time Measurement
#include <sys/time.h>

#include "Testbench.h"
#include "Filter.h"

// TIMEVAL STRUCT IS Defined ctime
// use start_time and end_time variables to capture
// start of simulation and end of simulation
struct timeval start_time, end_time;

// int main(int argc, char *argv[])
int sc_main(int argc, char **argv) {
  if ((argc < 3) || (argc > 4)) {
    cout << "No arguments for the executable : " << argv[0] << endl;
    cout << "Usage : >" << argv[0] << " in_image_file_name out_image_file_name"
         << endl;
    return 0;
  }
  cout << "Starting Simulation" << endl;

  Testbench tb("tb");
	Filter filter("filter");
	sc_clock clk( "clk", 1, SC_NS );
	sc_fifo<unsigned char> r;
	sc_fifo<unsigned char> g;
	sc_fifo<unsigned char> b;
	sc_fifo<int> result;
	tb.i_clk(clk);
	filter.i_clk(clk);
	tb.o_r(r);
	tb.o_g(g);
	tb.o_b(b);
	tb.i_result(result);
	filter.i_r(r);
	filter.i_g(g);
	filter.i_b(b);
	filter.o_result(result);

  tb.read_bmp(argv[1]);
	sc_start();
	std::cout<< sc_core::sc_time_stamp() << std::endl;
  tb.write_bmp(argv[2]);

  return 0;
}
