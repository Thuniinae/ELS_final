#include"GetDistance.h"

GetDistance::GetDistance( sc_module_name n ): sc_module( n )
{
	SC_THREAD( do_calculation );
	sensitive << i_clk.pos();
	dont_initialize();
	reset_signal_is(i_rst, false);
        
#ifndef NATIVE_SYSTEMC
	i_rgb.clk_rst(i_clk, i_rst);
	i_mean.clk_rst(i_clk, i_rst);
    o_result.clk_rst(i_clk, i_rst);
#endif
}

void GetDistance::do_calculation(){
	{
        #ifndef NATIVE_SYSTEMC
		HLS_DEFINE_PROTOCOL("main_reset");
		i_rgb.reset();
		i_mean.reset();
		o_result.reset();
        #endif
		wait();
	}
	while (true){
		sc_uint<24> rgb;
		sc_biguint<192> mean;
		sc_biguint<144> result;

		rgb = read();
		mean = read_mean();
		// can be unrolled
		for (int j = 0; j < K; j++) {
			#ifndef NATIVE_SYSTEMC
				HLS_UNROLL_LOOP(ON,"GD_loop");
			#endif
			// sum the difference square of RGB
			result.range((j * 18) + 17, j * 18)
				= (rgb.range(7, 0) - mean.range((j * 24) + 7, (j * 24)) )
				* (rgb.range(7, 0) - mean.range((j * 24) + 7, (j * 24)) )
				+ (rgb.range(15, 8) - mean.range((j * 24) + 15, (j * 24) + 8) )
				* (rgb.range(15, 8) - mean.range((j * 24) + 15, (j * 24) + 8) )
				+ (rgb.range(23, 16) - mean.range((j * 24) + 23, (j * 24) + 16) )
				* (rgb.range(23, 16) - mean.range((j * 24) + 23, (j * 24) + 16) );
		}
		write(result);
	}
}

sc_uint<24> GetDistance::read(void){
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
	return rgb;
}

sc_biguint<192> GetDistance::read_mean(void){
	sc_biguint<192> mean;
    #ifndef NATIVE_SYSTEMC
	{
		HLS_DEFINE_PROTOCOL("input");
		mean = i_mean.get();
		wait();
	}
	#else
	    mean = i_mean.read();
	#endif
	return mean;
}

void GetDistance::write(sc_biguint<144> distance){ // K-distances
    #ifndef NATIVE_SYSTEMC
	{
		HLS_DEFINE_PROTOCOL("output");
		o_result.put(distance);
		wait();
	}
	#else
	    o_result.write(distance);
	#endif
}
