#include <cmath>
#ifndef NATIVE_SYSTEMC
#include "stratus_hls.h"
#endif

#include "SobelFilter.h"

unsigned char Median(unsigned char *A, unsigned int n){

    for (unsigned int i = 0; i <= n / 2; i++){
		unsigned char temp;
    	unsigned int index;
		{
#ifndef NATIVE_SYSTEMC
			HLS_CONSTRAIN_LATENCY(0, 2, "latM0");
#endif
        temp = A[i];
        index = i;
		}
        for (unsigned int j = i + 1; j < n; j++){
#ifndef NATIVE_SYSTEMC
			HLS_CONSTRAIN_LATENCY(0, 3, "latM1");
#endif
            if (temp < A[j]){
                temp = A[j];
                index = j;
            }
        }
		{
#ifndef NATIVE_SYSTEMC
			HLS_CONSTRAIN_LATENCY(0, 2, "latM2");
#endif
        A[index] = A[i];
        A[i] = temp;
		}
    }
    return A[n / 2];
}

unsigned char Mean(unsigned char *A){
	int result;
	result = 0;
	for (int i = 0; i < 9; i++) { // weighted mean
		if (i == 4) result += A[i] * 2;	
		else result += A[i];
	}
	return result / 10;
}

void SobelFilter::do_median(int u, int v){
	sc_dt::sc_uint<24> rgb;
	// fill buffer
	if (u == 0) {
		for (int i = 0; i < 2; i++) {
			for (int j = 0; j <3; j++) {
				#ifndef NATIVE_SYSTEMC
				{
					HLS_DEFINE_PROTOCOL("input");
					rgb = i_rgb.get();
					wait();
				}
				#else
				rgb = i_rgb.read();
				#endif
				md_win[0][j * 3 + i] = rgb.range(7, 0); 
				md_win[1][j * 3 + i] = rgb.range(15, 8); 
				md_win[2][j * 3 + i] = rgb.range(23, 16); 
			}
		}
	}
	// fill new column
	for (int i = 2; i < 3; i++) {
		for (int j = 0; j <3; j++) {
			#ifndef NATIVE_SYSTEMC
			{
				HLS_DEFINE_PROTOCOL("input");
				rgb = i_rgb.get();
				wait();
			}
			#else
			rgb = i_rgb.read();
			#endif
			md_win[0][j * 3 + i] = rgb.range(7, 0); 
			md_win[1][j * 3 + i] = rgb.range(15, 8); 
			md_win[2][j * 3 + i] = rgb.range(23, 16); 
		}
	}
	// process
	mn_win[0][v * 3 + u] = Median(md_win[0], 9);
	mn_win[1][v * 3 + u] = Median(md_win[1], 9);
	mn_win[2][v * 3 + u] = Median(md_win[2], 9);
	// shift buffer
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j <3; j++) {
			md_win[0][j * 3 + i] = md_win[0][j * 3 + i + 1]; 
			md_win[1][j * 3 + i] = md_win[1][j * 3 + i + 1]; 
			md_win[2][j * 3 + i] = md_win[2][j * 3 + i + 1]; 
		}
	}
}

SobelFilter::SobelFilter( sc_module_name n ): sc_module( n )
{
#ifndef NATIVE_SYSTEMC
	HLS_FLATTEN_ARRAY(mean);
#endif
	SC_THREAD( do_filter );
	sensitive << i_clk.pos();
	dont_initialize();
	reset_signal_is(i_rst, false);
        
#ifndef NATIVE_SYSTEMC
	i_rgb.clk_rst(i_clk, i_rst);
  o_result.clk_rst(i_clk, i_rst);
#endif
}

SobelFilter::~SobelFilter() {}

void SobelFilter::do_filter() {
	sc_dt::sc_uint<24> rgb; // store input from FIFO
	{
#ifndef NATIVE_SYSTEMC
		HLS_DEFINE_PROTOCOL("main_reset");
		i_rgb.reset();
		o_result.reset();
#endif
		wait();
	}
	int width, height;
	#ifndef NATIVE_SYSTEMC
	{
		HLS_DEFINE_PROTOCOL("input");
		width = i_rgb.get();
		height = i_rgb.get();
		wait();
	}
	#else
	width = i_rgb.read();
	height = i_rgb.read();
	#endif
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			if (x == 0) {
				// fill buffer
				for (int u = 0; u < 2; u++) {
					for (int v = 0; v < 3; v++) {
						do_median(u, v);
					}
				}
			}
			// fill new column
			for (int u = 2; u < 3; u++) {
				for (int v = 0; v < 3; v++) {
					do_median(u, v);
				}
			}
			// process
			rgb.range(7, 0) = Mean(mn_win[0]);
			rgb.range(15, 8) = Mean(mn_win[1]);
			rgb.range(23, 16) = Mean(mn_win[2]);
			#ifndef NATIVE_SYSTEMC
			{
				HLS_DEFINE_PROTOCOL("output");
				o_result.put(rgb);
				wait();
			}
			#else
			o_result.write(rgb);
			#endif
			// shift window
			for (int u = 0; u < 2; u++) {
				for (int v = 0; v < 3; v++) {
					mn_win[0][v * 3 + u] = mn_win[0][v * 3 + u + 1];
					mn_win[1][v * 3 + u] = mn_win[1][v * 3 + u + 1];
					mn_win[2][v * 3 + u] = mn_win[2][v * 3 + u + 1];
				}
			}
			//std::cout << "(w, h) = " << x << " " << y << std::endl;
		}	
	}
}
