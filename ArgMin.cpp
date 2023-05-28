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
		sc_uint<16> t1, t2, t3, t4;
		sc_uint<3> i1, i2, i3, i4;

		distance = read();
		{	// parallel find minimum
			#ifndef NATIVE_SYSTEMC
				HLS_CONSTRAIN_LATENCY(0, HLS_ACHIEVABLE, "AG_loop");
			#endif
			// stage 1
			if (distance.range(17, 0) <= distance.range(35, 18)){
				t1 = distance.range(17, 0);
				i1 = 0;
			}
			else{
				t1 = distance.range(35, 18);
				i1 = 1;
			}
			if (distance.range(53, 36) <= distance.range(71, 54)){
				t2 = distance.range(53, 36);
				i2 = 2;
			}
			else {
				t2 = distance.range(71, 54);
				i2 = 3;
			}
			if (distance.range(89, 72) <= distance.range(107,90)){
				t3 = distance.range(89, 72);
				i3 = 4;
			}
			else {
				t3 = distance.range(107, 90);
				i3 = 5;
			}
			if (distance.range(125, 108) <= distance.range(143,126)){
				t4 = distance.range(125, 108);
				i4 = 6;
			}
			else {
				t4 = distance.range(143,126);
				i4 = 7;
			}
			// stage 2
			if (t1 <= t2){
				t2 = t1;
				i2 = i1;
			}
			if (t3 <= t4){
				t4 = t3;
				i4 = i3;
			}
			// stage 3
			if (t2 <= t4)
				i4 = i2;
		}
		write(i4);
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