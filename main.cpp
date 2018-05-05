#include <iostream>
#include <string>
using namespace std;

// Wall Clock Time Measurement
#include <sys/time.h>

#include "bmp.h"
#include "Testbench.h"
#include "Sobel.h"


// TIMEVAL STRUCT IS Defined ctime
// use start_time and end_time variables to capture 
// start of simulation and end of simulation
struct timeval start_time, end_time;

//int main(int argc, char *argv[])
int sc_main(int argc, char **argv)
{
    if((argc < 3)||(argc > 4))
    {
      cout << "No arguments for the executable : "<< argv[0] << endl;
      cout << "Usage : >" << argv[0] << " in_image_file_name out_image_file_name" << endl ;
      return 0;
    }
    cout << "Starting Simulation" << endl;

		Testbench tb;
		tb.read_bmp(argv[1]);

    gettimeofday(&start_time, 0);
		// Processing
		for (unsigned int i = 0; i< tb.get_height(); i++) {
			for (unsigned int j = 0; j< tb.get_width_bytes(); j++) {
				unsigned int n = i*tb.get_width_bytes()+j;
				tb.get_target_image()[n] = tb.get_source_image()[n];
			}
		}
    gettimeofday(&end_time, 0);
		
		tb.write_bmp(argv[2]);

    int total_usecs = (end_time.tv_sec - start_time.tv_sec) * 1000000 +
                     (end_time.tv_usec - start_time.tv_usec);

    cout << "==============================================="<< endl;
    cout << "   Time Taken for simulation is: " << total_usecs << " uS" << endl; 
    cout << "==============================================="<< endl;
    return 0;
}

