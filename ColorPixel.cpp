#include"ColorPixel.h"

ColorPixel::ColorPixel( sc_module_name n ): sc_module( n )
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

void ColorPixel::do_calculation(){
	{
        #ifndef NATIVE_SYSTEMC
		HLS_DEFINE_PROTOCOL("main_reset");
		i_distance.reset();
		o_result.reset();
        #endif
		wait();
	}
    while (true){
		sc_uint<3> index;
		sc_biguint<192> mean;
		index = read_index();
		mean = read_mean();
		/*cout << "index: " << index << endl;
		cout << "mean:" << endl;
		for (int i = 0; i < 8; i++){
			for (int j =0; j < 3; j++) { // for R, G, B
				cout << mean.range((i * 24) + (j<<3) + 7, (i * 24) + (j<<3)) << ", ";
			}
			cout << endl;
		}
		cout << "result: ";
		for (int j =0; j < 3; j++) { // for R, G, B
			cout << mean.range((index * 24) + (j<<3) + 7, (index * 24) + (j<<3)) << ", ";
		}
		cout << endl;*/
		write((sc_uint<24>)mean.range(index * 24 + 23, index * 24));
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