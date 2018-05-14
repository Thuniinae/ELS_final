#include <cmath>
#ifndef NATIVE_SYSTEMC
#include "stratus_hls.h"
#endif

#include "SobelFilter.h"

SobelFilter::SobelFilter( sc_module_name n ): sc_module( n )
{
#ifndef NATIVE_SYSTEMC
	HLS_FLATTEN_ARRAY(rSpace);
	HLS_FLATTEN_ARRAY(gSpace);
	HLS_FLATTEN_ARRAY(bSpace);
	HLS_FLATTEN_ARRAY(val);
#endif
	SC_THREAD( do_filter );
	sensitive << i_clk.pos();
	dont_initialize();
	reset_signal_is(i_rst, false);
        
	i_rgb.clk_rst(i_clk, i_rst);
  o_result.clk_rst(i_clk, i_rst);
}

SobelFilter::~SobelFilter() {}

// sobel mask
const int mask[MASK_N][MASK_X][MASK_Y] = {{{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}},

                                    {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}}};

int color_to_int(int r, int g, int b) { return (r + g + b) / 3; }

void SobelFilter::do_filter() {
	{
#ifndef NATIVE_SYSTEMC
		HLS_DEFINE_PROTOCOL("main_reset");
#endif
		i_rgb.reset();
		o_result.reset();
		wait();
	}
	while (true) {
		for (unsigned int v = 0; v<MASK_Y; ++v) {
			for (unsigned int u = 0; u<MASK_X; ++u) {
				sc_uint<24> rgb = i_rgb.get();
				rSpace[u][v] = rgb.range(0, 7);
				gSpace[u][v] = rgb.range(8, 15);
				bSpace[u][v] = rgb.range(16, 23);
				wait();
			}
		}
		for (unsigned int i = 0; i<MASK_N; ++i) {
			val[i] = 0;
			wait();
		}
		for (unsigned int v = 0; v<MASK_Y; ++v) {
			for (unsigned int u = 0; u<MASK_X; ++u) {
				for (unsigned int i = 0; i != MASK_N; ++i) {
					val[i] += color_to_int(rSpace[u][v], gSpace[u][v], bSpace[u][v]) * mask[i][u][v];
					wait();
				}
			}
		}
		float total = 0;
		for (unsigned int i = 0; i != MASK_N; ++i) {
			total += val[i] * val[i];
			wait();
		}
		int result = (int)(std::sqrt(total));
		o_result.put(result);
	}
}
