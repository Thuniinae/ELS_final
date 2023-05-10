#include <cmath>
#ifndef NATIVE_SYSTEMC
#include "stratus_hls.h"
#endif

#include "SobelFilter.h"
void selectionSort(unsigned char* A, int n){

    int j;            
    unsigned char t;  // temporary variable for swapping

    for (int i = 0; i < n - 1; i++){  // iterate from 0 to n-1
        j = i;
        for (int k = i + 1; k < n; k++){
#ifndef NATIVE_SYSTEMC
			HLS_CONSTRAIN_LATENCY(0, 2, "lats0");
#endif
            if (A[k] < A[j]) j = k;  // store current smallest element index in j
        }
		{
#ifndef NATIVE_SYSTEMC
			HLS_CONSTRAIN_LATENCY(0, 4, "lats1");
#endif
        t = A[i]; A[i] = A[j]; A[j] = t;  // swap A[i] and A[j]
		}
    }
} 

SobelFilter::SobelFilter( sc_module_name n ): sc_module( n )
{
#ifndef NATIVE_SYSTEMC
	HLS_FLATTEN_ARRAY(val);
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

// sobel mask
const int mask[MASK_N][MASK_X][MASK_Y] = {{{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}},

                                    {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}}};

void SobelFilter::do_filter() {
	unsigned char mdWin[3][MASK_X * MASK_Y];  // median filter window of size MASK_X x MASK_Y
	int mean[3];  // mean filter result of R/G/B
	unsigned char buffer[3][BUF_X][BUF_Y];  //buffer of R/G/B
	sc_dt::sc_uint<24> rgb; // store input from FIFO
	{
#ifndef NATIVE_SYSTEMC
		HLS_DEFINE_PROTOCOL("main_reset");
		i_rgb.reset();
		o_result.reset();
#endif
		wait();
	}
	while (true) {
		
		for (int i = 0; i < 3; i++){
#ifndef NATIVE_SYSTEMC
			HLS_CONSTRAIN_LATENCY(0, 1, "lat01");
#endif
      		mean[i] = 0;
    	}
		for (int i = 0; i < MASK_Y; i++){  // iterate all pixel in filter window
			// fill buffer
			for (int x = 0; x < BUF_X; x++){
				for (int y = 0; y < BUF_Y; y++){
#ifndef NATIVE_SYSTEMC
					{
						HLS_DEFINE_PROTOCOL("input");
						rgb = i_rgb.get();
						wait();
					}
#else
					rgb = i_rgb.read();
#endif	
					{
#ifndef NATIVE_SYSTEMC
						HLS_CONSTRAIN_LATENCY(0, 3, "lat02");
#endif
						buffer[0][x][y] = rgb.range(7,0);
						buffer[1][x][y] = rgb.range(15,8);
						buffer[2][x][y] = rgb.range(23,16);
					}
				}
			}
			for (int j = 0; j < MASK_X; j++){
				// read buffer and fill it into median window
				for (int y = 0; y < BUF_Y; y++){  // 0, 1, 2
					for (int x = 0; x < BUF_X; x++){// 0, 1
#ifndef NATIVE_SYSTEMC
						HLS_CONSTRAIN_LATENCY(0, 4, "lat03");
#endif
						mdWin[0][y * MASK_Y + x] = buffer[0][x][y];
						mdWin[1][y * MASK_Y + x] = buffer[1][x][y];
						mdWin[2][y * MASK_Y + x] = buffer[2][x][y];
					}
				}
				// shift buffer
				for (int y = 0; y < BUF_Y; y++){  // 0, 1, 2
					for (int x = 0; x < BUF_X - 1; x++){  // 0
#ifndef NATIVE_SYSTEMC
						HLS_CONSTRAIN_LATENCY(0, 6, "lat04");
#endif
						buffer[0][x][y] = buffer[0][x + 1][y];
						buffer[1][x][y] = buffer[1][x + 1][y];
						buffer[2][x][y] = buffer[2][x + 1][y];
					}
				}
				// write buffer
				for (int y = 0; y < MASK_Y; y++){  // 0, 1, 2
					for (int x = BUF_X; x < MASK_X; x++){  // 2
#ifndef NATIVE_SYSTEMC
						{
							HLS_DEFINE_PROTOCOL("input");
							rgb = i_rgb.get();
							wait();
						}
#else
						rgb = i_rgb.read();
#endif	
						{
#ifndef NATIVE_SYSTEMC
					HLS_CONSTRAIN_LATENCY(0, 3, "lat05");
#endif
							buffer[0][x][y] = rgb.range(7,0);
							buffer[1][x][y] = rgb.range(15,8);
							buffer[2][x][y] = rgb.range(23,16);
						}
					}
				}
				// read new pixels from last column of buffer
				for (int y = 0; y < MASK_Y; y++){  // 0, 1, 2
					for (int x = BUF_X; x < MASK_X; x++){  // 2
#ifndef NATIVE_SYSTEMC
						HLS_CONSTRAIN_LATENCY(0, 4, "lat06");
#endif
						mdWin[0][y * MASK_Y + x] = buffer[0][x - 1][y];
						mdWin[1][y * MASK_Y + x] = buffer[1][x - 1][y];
						mdWin[2][y * MASK_Y + x] = buffer[2][x - 1][y];
					}
				}
				for (int k = 0; k < 3; k++){  // iterate R/G/B
					selectionSort(mdWin[k], MASK_X * MASK_Y);  // median filter
					{
#ifndef NATIVE_SYSTEMC
					HLS_CONSTRAIN_LATENCY(0, 5, "lat07");
#endif
					if (i == 1 && j==1)  // weighted mean filter
					{
						mean[k] += mdWin[k][int(MASK_X * MASK_Y / 2)]*2 / 10;  // centered pixel
					}else{
						mean[k] += mdWin[k][int(MASK_X * MASK_Y / 2)] / 10;  // other pixels
					}
					}
				}
			}
		}
			// ouput median of R/G/B
			{
#ifndef NATIVE_SYSTEMC
					HLS_CONSTRAIN_LATENCY(0, 3, "lat08");
#endif
				rgb.range(7,0) = mean[0];
				rgb.range(15,8) = mean[1];
				rgb.range(23,16) = mean[2];
			}
#ifndef NATIVE_SYSTEMC
			{
				HLS_DEFINE_PROTOCOL("output");
				o_result.put(rgb);
				wait();
			}
#else
				o_result.write(rgb);
				wait(10); //emulate module delay
#endif
	}
}
