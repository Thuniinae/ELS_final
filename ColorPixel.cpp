#include"ColorPixel.h"

ColorPixel::ColorPixel( sc_module_name n ): sc_module( n )
{
	SC_THREAD( do_calculation );
	sensitive << i_clk.pos();
	dont_initialize();
	reset_signal_is(i_rst, false);
        
#ifndef NATIVE_SYSTEMC
	i_index.clk_rst(i_clk, i_rst);
	i_mean.clk_rst(i_clk, i_rst);
    o_result.clk_rst(i_clk, i_rst);
#endif
}

void ColorPixel::do_calculation(){
	{
        #ifndef NATIVE_SYSTEMC
		HLS_DEFINE_PROTOCOL("main_reset");
		i_index.reset();
		i_mean.reset();
		o_result.reset();
        #endif
		wait();
	}
    while (true){
		sc_uint<3> index;
		sc_biguint<192> mean;
		index = read_index();
		mean = read_mean();
		switch(index) {
			case 0:
				write((sc_uint<24>)mean.range(23, 0));
				break;
			case 1:
				write((sc_uint<24>)mean.range(47, 24));
				break;
			case 2:
				write((sc_uint<24>)mean.range(71, 48));
				break;
			case 3:
				write((sc_uint<24>)mean.range(95, 72));
				break;
			case 4:
				write((sc_uint<24>)mean.range(119, 96));
				break;
			case 5:
				write((sc_uint<24>)mean.range(143, 120));
				break;
			case 6:
				write((sc_uint<24>)mean.range(167, 144));
				break;
			default: // 7
				write((sc_uint<24>)mean.range(191, 168));
				break;
		}
	}
}

sc_biguint<192> ColorPixel::read_mean() {
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

sc_uint<3> ColorPixel::read_index() {
	sc_uint<3> i;
	#ifndef NATIVE_SYSTEMC
	{
		HLS_DEFINE_PROTOCOL("input");
		i = i_index.get();
		wait();
	}
	#else
		i = i_index.read();
	#endif
	return i;
}

void ColorPixel::write(sc_uint<24> rgb){
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