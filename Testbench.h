#ifndef TESTBENCH_H_
#define TESTBENCH_H_

#include <string>
using namespace std;

#include <systemc>
using namespace sc_core;
using namespace sc_dt;

#ifndef NATIVE_SYSTEMC
#include <cynw_p2p.h>
#endif

#define K 8
#define WHITE 255
#define BLACK 0
#define THRESHOLD 90

// use a queue to hold the time that each sample was sent
// for latency calculations
#include <queue>
static std::queue<sc_time> time_queue;

class Testbench : public sc_module {
public:
	sc_in_clk i_clk;
	sc_out < bool >  o_rst;
#ifndef NATIVE_SYSTEMC
	cynw_p2p< sc_dt::sc_uint<24> >::base_out o_rgbTG;
	cynw_p2p< sc_dt::sc_uint<24> >::base_out o_rgbTN;
	cynw_p2p< sc_dt::sc_biguint<192> >::base_out o_meanTG;
	cynw_p2p< sc_dt::sc_biguint<192> >::base_out o_meanTC;
	cynw_p2p< sc_dt::sc_uint<3> >::base_out o_indexTN;
	cynw_p2p< sc_dt::sc_uint<3> >::base_out o_indexTC;
	cynw_p2p< sc_dt::sc_uint<3> >::base_in i_indexAT;
	cynw_p2p< sc_dt::sc_biguint<192> >::base_in i_meanNT;
	cynw_p2p< sc_dt::sc_uint<24> >::base_in i_result;
#else
	sc_fifo_out< sc_dt::sc_uint<24> > o_rgbTG;
	sc_fifo_out< sc_dt::sc_uint<24> > o_rgbTN;
	sc_fifo_out< sc_dt::sc_biguint<192> > o_meanTG;
	sc_fifo_out< sc_dt::sc_biguint<192> > o_meanTC;
	sc_fifo_out< sc_dt::sc_uint<3> > o_indexTN;
	sc_fifo_out< sc_dt::sc_uint<3> > o_indexTC;
	sc_fifo_in< sc_dt::sc_uint<3> > i_indexAT;
	sc_fifo_in< sc_dt::sc_biguint<192> > i_meanNT;
	sc_fifo_in< sc_dt::sc_uint<24> > i_result;
#endif

  SC_HAS_PROCESS(Testbench);

  Testbench(sc_module_name n);
  ~Testbench();

  int read_bmp(string infile_name);
  int write_bmp(string outfile_name);

  unsigned int get_width() { return width; }

  unsigned int get_height() { return height; }

  unsigned int get_width_bytes() { return width_bytes; }

  unsigned int get_bytes_per_pixel() { return bytes_per_pixel; }

  unsigned char *get_source_image() { return source_bitmap; }
  unsigned char *get_target_image() { return target_bitmap; }

private:
  unsigned int input_rgb_raw_data_offset;
  const unsigned int output_rgb_raw_data_offset;
  unsigned int width;
  unsigned int height;
  unsigned int width_bytes;
  unsigned char bits_per_pixel;
  unsigned short bytes_per_pixel;
  unsigned char *source_bitmap;
  unsigned char *target_bitmap;
	
	unsigned int n_txn;
	sc_time max_txn_time;
	sc_time min_txn_time;
	sc_time total_txn_time;
	sc_time total_start_time;
	sc_time total_run_time;


  int clock_cycle( sc_time time );
  sc_uint<24> pixel(unsigned int i, unsigned int j);
  void write(sc_uint<24> rgb, int channel);
  void write_mean(sc_biguint<192> mean, int channel);
  sc_biguint<192> read_mean();
  sc_uint<3> read_index();
  void write_index(sc_uint<3> index, int channel);
  void feed_rgb();
	void fetch_result();
  bool converge (sc_biguint<192> mean1, sc_biguint<192> mean2, sc_uint<8> threshold);
};
#endif
