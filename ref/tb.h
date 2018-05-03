//******************************************************************************
// Copyright 2015 Cadence Design Systems, Inc.
// All Rights Reserved.
//
//******************************************************************************

#ifndef  _TB_INCLUDED_
#define  _TB_INCLUDED_

#include <systemc.h>
#include "stratus_hls.h"
#include "cynw_p2p.h"

#include "bmp.h"

#ifndef NO_DISPLAY
#include "panel.h"
#endif

#include <string>
#include "image_struct.h"

#include "im_sz.h"
#include "lb.h"

using namespace std;

SC_MODULE(tb)
{
public:

    // reset and clock are required ports
    sc_out<bool>     rst;
    sc_in_clk        clk;

    cynw_p2p<pixel, ioConfig >::base_in     tb_din;
    lb::out<ioConfig>                       tb_lineBuf_out;
    im_sz::out<ioConfig>                    tb_img_size;

    SC_CTOR(tb)
            :rst("rst")
            ,clk("clk")
            ,tb_din("tb_din")
            ,tb_lineBuf_out("tb_lineBuf_out")
            ,tb_img_size("tb_img_size")
#ifndef NO_DISPLAY
            ,in_panel("Input Image")
            ,out_c_panel("Output Image")
#endif
            ,nclocks(0)
            ,count_enable(false)
    {
        init_files();

#ifndef NO_DISPLAY
        init_panel();
#endif

        SC_CTHREAD(tb_source, clk.pos());
        SC_CTHREAD(tb_sink, clk.pos());
        SC_CTHREAD(counter, clk.pos());

        tb_img_size.clk_rst(clk, rst);
        tb_lineBuf_out.clk_rst(clk, rst);


    }//SC_CTOR(tb)

    ~tb();  //Destructor

    bmp_data bmp;
    bmp_data bmp_out_c;

#ifndef NO_DISPLAY
    panel in_panel;
    panel out_c_panel;
#endif

    // Member functions 
    void init_files();

#ifndef NO_DISPLAY
    void init_panel();
#endif

    void tb_source();     //Data Source Thread 
    void tb_sink();       //Data Sink Thread 
    void counter();

    //Class Members 
    ifstream tb_infile;
    ofstream tb_outfile_c;

    string if_base;
    string of_base;
    string of_base_c;

    sc_signal<unsigned long>    nclocks;
    sc_signal<bool>             count_enable;

}; //SC_MODULE(tb)

#endif //_TB_INCLUDED_

