//******************************************************************************
// Copyright 2015 Cadence Design Systems, Inc.
// All Rights Reserved.
//
//******************************************************************************

#include "tb.h"
#define NO_OF_WAITS 2

tb::~tb()  //Destructor
{
    tb_infile.close();
    tb_outfile_c.close();
} //End of Destructor

void tb::init_files()
{
    of_base = getenv("BDW_SIM_CONFIG_DIR");
    of_base_c = of_base + "/" + esc_argv(2);
    if_base = esc_argv(1);

    cout << "Input File Name: " << if_base << endl;
    cout << "Output File Name: " << of_base_c << endl;

    tb_infile.open(if_base.c_str(), ios::binary);
    if(!tb_infile.is_open())
    {
        cout << basename() << ": Input File Open Error" << endl;
    }

    tb_outfile_c.open(of_base_c.c_str(), ios::binary);
    if(!tb_outfile_c.is_open())
    {
        cout << basename() << ": Color Output File Open Error" << endl;
    }

    //$ead and copy FILE header
    bmp.read_file_header(tb_infile);
    bmp_out_c.fhdt = bmp.fhdt;
    bmp_out_c.write_file_header(tb_outfile_c);

    //$ead and copy INFO header
    bmp.read_info_header(tb_infile);
    bmp_out_c.ihdt = bmp.ihdt;
    bmp_out_c.write_info_header(tb_outfile_c);

}

#ifndef NO_DISPLAY
void tb::init_panel()
{
    unsigned int x_in = 100; 
    unsigned int y_in = 100; 
    cout << "Height = " << bmp.get_height() << " Width = " << bmp.get_width() << endl;
    in_panel.panelInit(bmp.get_width(), bmp.get_height(), x_in, y_in);
    in_panel.move_window(x_in, y_in);

    unsigned int x_out = x_in + bmp.get_width() + 5*in_panel.border_w; 
    unsigned int y_out = y_in; 

    out_c_panel.panelInit(bmp_out_c.get_width(), bmp_out_c.get_height(), x_out, y_out);
    out_c_panel.move_window(x_out, y_out);
    in_panel.initImg();
    out_c_panel.initImg();
}
#endif

void tb::tb_source()
{

    //Declare and define internal variables 
    unsigned long in_row_count=0;
    char tmp_buff[bmp.get_width_bytes()];
    unsigned long in_pix_indx=0;

    //Initialize output ports
    pixel p_tmp;;
    tb_img_size.reset();
    tb_lineBuf_out.reset();
    rst = 0;
    wait(5);
    rst = 1;
    while(true)
    {
        sc_uint<10> xsize = sc_uint<10>(bmp.get_width());
        sc_uint<10> ysize = sc_uint<10>(bmp.get_height());
        tb_img_size.trig(xsize, ysize);

        cout << "Size Passed " << endl;
        tb_lineBuf_out.set_size(ysize, xsize);
        tb_lineBuf_out.start_tx();
        while (!tb_lineBuf_out.y_done())
        {
            bmp.read_row(in_row_count, tmp_buff, tb_infile);
            unsigned long in_panel_buff[bmp.get_width()];
            unsigned int in_p_count = 0;
            unsigned int in_pix_count = 0;
            while(!tb_lineBuf_out.x_done())
            {
                in_pix_indx = ((bmp.get_width()-in_pix_count)*(bmp.get_bytes_per_pixel()))-1;
                p_tmp.r = tmp_buff[in_pix_indx]&0xFF;
                p_tmp.g = tmp_buff[in_pix_indx-1]&0xFF;
                p_tmp.b = tmp_buff[in_pix_indx-2]&0xFF;

                tb_lineBuf_out.put(p_tmp);

                count_enable.write(true);
                unsigned long tb_out = (tmp_buff[in_pix_indx]&0xFF);
                tb_out = (tb_out << 8) + (tmp_buff[in_pix_indx-1]&0xFF);
                tb_out = (tb_out << 8) + (tmp_buff[in_pix_indx-2]&0xFF);
                in_panel_buff[in_pix_count] = tb_out;
                in_p_count = in_pix_count;
                in_pix_count++;
            }
            tb_lineBuf_out.next_y();
#ifndef NO_DISPLAY
            in_panel.update_row(in_row_count, in_panel_buff);
#endif
            in_row_count++;
        }
        tb_lineBuf_out.end_tx();
        while(1)
        {
            wait();
        }
    }
}

void tb::tb_sink()
{
    bool first_count = true;
    unsigned long out_pix_count=0;
    unsigned long out_pix_indx=0;
    unsigned long out_row_count=0;
    char tmp_buff1_c[bmp_out_c.get_width_bytes()];
    unsigned long out_panel_buff_c[bmp_out_c.get_width()];
    wait();

    while(true)
    { 
        out_pix_indx = ((bmp_out_c.get_width()-out_pix_count)*(bmp_out_c.get_bytes_per_pixel()))-1;
        pixel tmp_pix = tb_din.get();
        if(first_count == true)
        {
            cout << " --------------------------------------------- " << endl;
            cout << " The Latency for the first pixel " << nclocks.read() << " cycles" << endl << endl;
            first_count = false;
        }
        tmp_buff1_c[out_pix_indx]   = tmp_pix.r&0xFF;
        tmp_buff1_c[out_pix_indx-1] = tmp_pix.g&0xFF;
        tmp_buff1_c[out_pix_indx-2] = tmp_pix.b&0xFF;
            
        out_panel_buff_c[out_pix_count]= (tmp_pix.r<<16)+(tmp_pix.g<<8)+(tmp_pix.b);
        out_pix_count++;

        if(out_pix_count == bmp_out_c.get_width())
        {
            bmp_out_c.write_row(out_row_count, tmp_buff1_c, tb_outfile_c);
            tb_outfile_c.flush();
#ifndef NO_DISPLAY
            out_c_panel.update_row(out_row_count, out_panel_buff_c);
#endif
            out_pix_count = 0;
            out_row_count++;
        }
        if(out_row_count == bmp_out_c.get_height())
        {
			esc_log_latency("filter", nclocks.read(), "# of clocks for the whole frame");

            cout << " Number of clocks for the whole frame: " << nclocks.read() << " cycles" << endl;
            cout << " Throughput: " << nclocks.read()/(bmp_out_c.get_width()*bmp_out_c.get_height()) << " cycles/pixel" << endl;
            cout << " --------------------------------------------- " << endl;
#if defined(NO_DISPLAY) || defined(NO_CONT_DISP)
			
            esc_stop();
#else
            while(1)
            {
                in_panel.refresh();
                out_c_panel.refresh();
            }
#endif
        }
    }
}

void tb::counter()
{
    nclocks = 0;
    while (1) 
    { 
        wait();
        if(count_enable.read() == true)
        {
            nclocks.write( nclocks.read()+1 );
        }
    }
}

