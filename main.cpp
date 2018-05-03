#include <iostream>
#include <string>

// Wall Clock Time Measurement
#include <sys/time.h>

#include "bmp.h"

using namespace std;

// TIMEVAL STRUCT IS Defined ctime
// use start_time and end_time variables to capture 
// start of simulation and end of simulation
struct timeval start_time, end_time;

int main(int argc, char *argv[])
{
    if((argc < 3)||(argc > 4))
    {
      cout << "No arguments for the executable : "<< argv[0] << endl;
      cout << "Usage : >" << argv[0] << " in_image_file_name out_image_file_name" << endl ;
      return 0;
    }
    cout << "Starting Simulation" << endl;

    bmp_data bmp;
    bmp_data bmp_out_c;
		// Read BMP
		
    gettimeofday(&start_time, 0);
		// Processing

    gettimeofday(&end_time, 0);
		
		// Write BMP


    int total_usecs = (end_time.tv_sec - start_time.tv_sec) * 1000000 +
                     (end_time.tv_usec - start_time.tv_usec);

    cout << "==============================================="<< endl;
    cout << "   Time Taken for simulation is: " << total_usecs << " uS" << endl; 
    cout << "==============================================="<< endl;
    return 0;
}

