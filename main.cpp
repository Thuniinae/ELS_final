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
    bmp.read_file_header(tb_infile);
    bmp.read_info_header(tb_infile);
		cout << "bmp.get_width() == " << bmp.get_width() << endl;
		cout << "bmp.get_bytes_per_pixel() == " << bmp.get_bytes_per_pixel() << endl;
		cout << "bmp.get_width_bytes() == " << bmp.get_width_bytes() << endl;
		cout << "bmp.get_height() == " << bmp.get_height() << endl;
		char * bitmap = new char [bmp.get_width_bytes()*bmp.get_height()];
		for (unsigned int i = 0; i< bmp.get_height(); i++) {
			bmp.read_row(i, &bitmap[i*bmp.get_width_bytes()], tb_infile);
		}
		
    gettimeofday(&start_time, 0);
		// Processing
    gettimeofday(&end_time, 0);
		
		// Write BMP
    bmp_out_c.fhdt = bmp.fhdt;
    bmp_out_c.write_file_header(tb_outfile_c);
    bmp_out_c.ihdt = bmp.ihdt;
    bmp_out_c.write_info_header(tb_outfile_c);
		for (unsigned int i = 0; i< bmp_out_c.get_height(); i++) {
			bmp_out_c.write_row(i, &bitmap[i*bmp.get_width_bytes()], tb_outfile_c);
		}

		delete bitmap;

    int total_usecs = (end_time.tv_sec - start_time.tv_sec) * 1000000 +
                     (end_time.tv_usec - start_time.tv_usec);

    cout << "==============================================="<< endl;
    cout << "   Time Taken for simulation is: " << total_usecs << " uS" << endl; 
    cout << "==============================================="<< endl;
    return 0;
}

