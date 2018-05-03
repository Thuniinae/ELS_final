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

    ifstream tb_infile;
    ofstream tb_outfile_c;

    bmp_data bmp;
    bmp_data bmp_out_c;
    
		tb_infile.open(argv[1], ios::binary);
    if(!tb_infile.is_open())
    {
        cout << "Input File Open Error" << endl;
    }

    tb_outfile_c.open(argv[2], ios::binary);
    if(!tb_outfile_c.is_open())
    {
        cout << "Color Output File Open Error" << endl;
    }

		// Read BMP
    //$ead and copy FILE header
    bmp.read_file_header(tb_infile);
    bmp_out_c.fhdt = bmp.fhdt;
    bmp_out_c.write_file_header(tb_outfile_c);
		
    gettimeofday(&start_time, 0);
		// Processing

    gettimeofday(&end_time, 0);
		
		// Write BMP
    //$ead and copy INFO header
    bmp.read_info_header(tb_infile);
    bmp_out_c.ihdt = bmp.ihdt;
    bmp_out_c.write_info_header(tb_outfile_c);

    int total_usecs = (end_time.tv_sec - start_time.tv_sec) * 1000000 +
                     (end_time.tv_usec - start_time.tv_usec);

    cout << "==============================================="<< endl;
    cout << "   Time Taken for simulation is: " << total_usecs << " uS" << endl; 
    cout << "==============================================="<< endl;
    return 0;
}

