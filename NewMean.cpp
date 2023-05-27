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
		for (int i = 0; i < Sample(Height) * Sample(Width); i++) // for all sampled pixels
			total_mean[read_index()] += read();
		for (int i = 0; i < 8; i++)
			new_means.range((i<<3) * 3 + 15, (i<<3) * 3) = Sample(Sample(total_mean[i])); // divide 256 to get average
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