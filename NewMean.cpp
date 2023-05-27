#include "NewMean.h"

NewMean::NewMean( sc_module_name n ): sc_module( n )
{
	SC_THREAD( do_calculation );
	sensitive << i_clk.pos();
	dont_initialize();
	reset_signal_is(i_rst, false);
        
#ifndef NATIVE_SYSTEMC
	i_distance.clk_rst(i_clk, i_rst);
    o_result.clk_rst(i_clk, i_rst);
#endif
}

void NewMean::do_calculation(){
	{
        #ifndef NATIVE_SYSTEMC
		HLS_DEFINE_PROTOCOL("main_reset");
		i_distance.reset();
		o_result.reset();
        #endif
		wait();
	}
	while (true) {
		sc_biguint<192> new_means;
		for (int i = 0; i < K; i++) // initialize
			total_mean[i] = 0;
		for (int i = 0; i < Sample(Height) * Sample(Width); i++){ // for all sampled pixels
			sc_uint<24> rgb;
			sc_uint<3> index;
			rgb = read();
			index = read_index();
			for (int j =0; j < 3; j++) { // for R, G, B
				total_mean[index].range((j<<4) + 15, j<<3)
					= total_mean[index].range((j<<4) + 15, j<<3) 
					+ rgb.range((j<<3) + 7, j<<3) ;
			}
		}
		for (int i = 0; i < 8; i++){
			for (int j =0; j < 3; j++) { // for R, G, B
				new_means.range((i * 24) + (j<<3) + 7, (i * 24) + (j<<3))
				=  Sample(Sample(total_mean[i].range((j<<4) + 15, j<<3)));
			}
		}
		write(new_means);
	}
}

sc_uint<24> NewMean::read() {
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

sc_uint<3> NewMean::read_index() {
	sc_uint<8> index;
	#ifndef NATIVE_SYSTEMC
	{
		HLS_DEFINE_PROTOCOL("input");
		index = i_index.get();
		wait();
	}
	#else
		index = i_index.read();
	#endif
	return index;
}

void NewMean::write(sc_biguint<192> mean){
	#ifndef NATIVE_SYSTEMC
	{
		HLS_DEFINE_PROTOCOL("output");
		o_result.put(mean);
		wait();
	}
	#else
		o_result.write(mean);
	#endif
}