#include <cmath>
#ifndef NATIVE_SYSTEMC
#include "stratus_hls.h"
#endif

#include "SobelFilter.h"


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
	{
#ifndef NATIVE_SYSTEMC
		HLS_DEFINE_PROTOCOL("main_reset");
		i_rgb.reset();
		o_result.reset();
#endif
		wait();
	}

	int height, width;

	width = read();
	height = read();
	printf("get image size: %d, %d\n", width, height);
	

	// read initial k-means
	for (int i = 0; i < K; i++){
		mean[i] = read();
	}

	for (int j = 0; j < 30; j++) { // iterate for new means 30 times
		for (int i = 0; i < K; i++){ 
			mean_total[i] = 0; // initialize
		}
		for (int x = 0; x < width; x = x + 16) {		// for all 16x16 sampled pixels
      		for (int y = 0; y < height; y = y + 16) {	//
				sc_uint<24> rgb;
				int distance[K];
				int min_index;
				rgb = read();
				
				for (int i = 0; i < K; i++) // calculate distance to K-means
					distance[i] = cal_distance(rgb, mean[i]);

				// cluster the sample pixel, increment average of new mean it belongs to
				min_index = arg_min(distance, K);
				for (int i = 0; i < 3; i++) { // for R, G, B
					mean_total[min_index].range((i<<4) + 15, i<<4) = 
						mean_total[min_index].range((i<<4) + 15, i<<4)
						 + (rgb.range((i<<3) + 7, i<<3));
				}
			}
		}
		// calculate replace original mean with the new one
		for (int i = 0; i < K; i++)
			for (int k = 0; k < 3; k++) 
				mean[k].range((k<<3) + 7, k<<3) 
				= mean_total[k].range((k<<4) + 15, (k<<4)) >> 8; //divide by 256
	}

	// assume K-means has converge
	for (int x = 0; x < width; x++) {		// for all pixels in the image
		for (int y = 0; y < height; y++) {  //
			sc_uint<24> rgb;
			int distance[K];
			int min_index;
			rgb = read();
			
			for (int i = 0; i < K; i++) // calculate distance to K-means
				distance[i] = cal_distance(rgb, mean[i]);

			// cluster the pixel, output the color of the closest mean
			min_index = arg_min(distance, K);
			write(mean[min_index]);
		}
	}
}

sc_uint<24> SobelFilter::read() {
	sc_uint<24> rgb;
	#ifndef NATIVE_SYSTEMC
	{
		HLS_DEFINE_PROTOCOL("input");
		rgb = i_rgb.get();
		wait();
	}
	#else
	rgb = i_rgb.read();
	#endif
	//std::cout <<"read:" <<(int) rgb.range(7, 0) << std::endl;
	return rgb;
}

// calculate distance to mean
int SobelFilter::cal_distance(sc_uint<24> from_rgb, sc_uint<24> to_rgb) {
	// distance of root mean suqre
	int result;

	result = 0;
	// sum the difference square
	for (int i = 0; i < 3; i++) {
		result = result +  (from_rgb.range((i << 3) + 7, i << 3) 
			- to_rgb.range((i << 3) + 7, i << 3))*(from_rgb.range((i << 3) + 7, i << 3) 
			- to_rgb.range((i << 3) + 7, i << 3));
	}
	return result;
}

sc_uint<8> SobelFilter::arg_min(int distance[], int n){
	int min;
	sc_uint<8> index;

	min = distance[0];
	index = 0;
	for (int i = 1; i < n; i++) {
		if (distance[i] < min) {
			min = distance[i];
			index = i;
		}
	}
	return index;
}

void SobelFilter::write(sc_uint<24> rgb){
	#ifndef NATIVE_SYSTEMC
	{
		HLS_DEFINE_PROTOCOL("output");
		o_result.put(rgb);
		wait();
	}
	#else
	o_result.write(rgb);
	#endif
}

