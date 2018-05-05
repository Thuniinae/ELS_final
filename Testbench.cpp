#include "Testbench.h"
#include "bmp.h"

Testbench::Testbench() {
}

Testbench::~Testbench() {
}

void Testbench::read_bmp(string infile_name) {
	ifstream tb_infile;
	tb_infile.open(infile_name.c_str(), ios::binary);
	if(!tb_infile.is_open())
	{
		cout << "Input File Open Error" << endl;
	}
	bmp.read_file_header(tb_infile);
	bmp.read_info_header(tb_infile);
	cout << "bmp.get_width() == " << bmp.get_width() << endl;
	cout << "bmp.get_bytes_per_pixel() == " << bmp.get_bytes_per_pixel() << endl;
	cout << "bmp.get_width_bytes() == " << bmp.get_width_bytes() << endl;
	cout << "bmp.get_height() == " << bmp.get_height() << endl;
	bitmap = new char [bmp.get_width_bytes()*bmp.get_height()];
	for (unsigned int i = 0; i< bmp.get_height(); i++) {
		bmp.read_row(i, &bitmap[i*bmp.get_width_bytes()], tb_infile);
	}
}

void Testbench::write_bmp(string outfile_name) {
	ofstream tb_outfile_c;
	tb_outfile_c.open(outfile_name.c_str(), ios::binary);
	if(!tb_outfile_c.is_open())
	{
		cout << "Color Output File Open Error" << endl;
	}
	bmp_out_c.fhdt = bmp.fhdt;
	bmp_out_c.write_file_header(tb_outfile_c);
	bmp_out_c.ihdt = bmp.ihdt;
	bmp_out_c.write_info_header(tb_outfile_c);
	for (unsigned int i = 0; i< bmp_out_c.get_height(); i++) {
		bmp_out_c.write_row(i, &bitmap[i*bmp.get_width_bytes()], tb_outfile_c);
	}
}
