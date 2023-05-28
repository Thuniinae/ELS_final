#include"ArgMin.h"

ArgMin::ArgMin( sc_module_name n ): sc_module( n )
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

void ArgMin::do_calculation(){
	{
        #ifndef NATIVE_SYSTEMC
		HLS_DEFINE_PROTOCOL("main_reset");
		i_distance.reset();
		o_result.reset();
        #endif
		wait();
	}
    while (true){
		sc_biguint<144> distance;
		sc_uint<16> min;
		sc_uint<3> index;

		distance = read();
		min = distance.range(17, 0);
		index = 0;
		// can be more efficient
		for (int i = 1; i < 8; i++) {
			if (distance.range((i * 18) + 17, (i * 18)) < min) {
				min = distance.range((i * 18) + 17, (i * 18));
				index = i;
			}
		}
		write(index);
	}
}

sc_biguint<144> ArgMin::read() {
	sc_biguint<144> d;
	#ifndef NATIVE_SYSTEMC
	{
		HLS_DEFINE_PROTOCOL("input");
		d = i_distance.get();
		wait();
	}
	#else
		d = i_distance.read();
	#endif
	return d;
}

void ArgMin::write(sc_uint<3> index){
	#ifndef NATIVE_SYSTEMC
	{
		HLS_DEFINE_PROTOCOL("output");
		o_result.put(index);
		wait();
	}
	#else
		o_result.write(index);
	#endif
}