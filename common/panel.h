//******************************************************************************
// Copyright 2015 Cadence Design Systems, Inc.
// All Rights Reserved.
//
//******************************************************************************

#ifndef _PANEL_H_
#define _PANEL_H_

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <iostream>
#include <string>

using namespace std;

class panel
{
 public:
    char *        panelName;
    Display*      display;
    Window        panelWin;

    unsigned int  screen_num;
    unsigned int  panel_w;
    unsigned int  panel_h;
//    unsigned int  panel_x;
//    unsigned int  panel_y;
    int  panel_x;
    int  panel_y;
    unsigned int  border_w;
    unsigned long pixel_w;
    unsigned long pixel_b;

    GC            gc;
    XImage        *ximg;
    XEvent        panel_event;

    XSetWindowAttributes panelAtt;


    panel(
          char * dispName = "panel"
          ,char * dis_con_name=NULL
    )
    :panelName(dispName)
    {
        openPanelDisp(dis_con_name);
    }

    void panelInit(unsigned int p_w=100, unsigned int p_h=100
          ,unsigned int p_x=0, unsigned int p_y=0
          ,unsigned int b_w=3
    )
    {
         panel_w = p_w;
         panel_h = p_h;
         panel_x = p_x;
         panel_y = p_y;
        border_w = b_w;

        createPanelDisp();
        initPanelDisp();

//        cout<< "Width = " << panel_w << "\tHeight = " << panel_h << endl;
//        cout<< "X = " << panel_x << "\tY = " << panel_y << endl;
    }


    void openPanelDisp(char * dname)
    {
        // display = XOpenDisplay(char* dname);
        display = XOpenDisplay(dname);
        if(display==NULL)
        {
            cerr << "Can not connect to X-server" << endl;
        }
        screen_num = DefaultScreen(display);
    }

    void createPanelDisp()
    {
        pixel_w = WhitePixel(display, screen_num);
        pixel_b = BlackPixel(display, screen_num);

//        cout << "position x; " << panel_x << "\ty: " << panel_y << endl;
//        cout << "dimension w; " << panel_w << "\th: " << panel_h << endl;
//        cout << "border b: " << border_w << endl;

        panelWin = XCreateSimpleWindow(display,
                       DefaultRootWindow(display),
                       panel_x, panel_y,
                       panel_w, panel_h,
                       border_w, pixel_w, pixel_b
                    );

        XStoreName(display, panelWin, panelName);
        map();
    }

    void initPanelDisp()
    {
//        gc = XCreateGC(display, drawable, valuemask, &values); 
        gc = XCreateGC(display, panelWin, 0, 0);
        if(gc < 0)
        {
            cerr << "XCreateGC: Error" << endl;
        }
    }

    void initImg()
    {
        char *ImgData;
        ximg = XCreateImage(display, DefaultVisual(display, screen_num), DefaultDepth(display, screen_num), ZPixmap, 0, 0, panel_w, panel_h, 32, 0);

        int c_class = DefaultVisual(display, screen_num)->c_class;
        unsigned int depth = panel_w*panel_h;

        switch(c_class)
        {
            case TrueColor:
                switch(ximg->bits_per_pixel)
                {
                    case 32:
                        depth *= 4;
                        break;

                    case 24:
                        depth *= 3;
                        break;

                    case 16:
                    case 15:
                        depth *= 2;
                        break;

                }
                break;
            default:
                break;
        }

        ImgData = ximg->data = (char *)malloc(sizeof(char)*depth);
        for(unsigned int indx = 0; indx < depth; indx++)
        {
//            *ImgData++ = BlackPixel(display,screen_number);
            *ImgData++ = WhitePixel(display,screen_num);
//            *ImgData++ = BlackPixel(display,screen_num);
        }
        disp();
    }

    ~panel()
    {

        XDestroyWindow(display, panelWin);
        XCloseDisplay(display);
    }
    
    void map()
    {
        panelAtt.backing_store = WhenMapped;
        XChangeWindowAttributes(display, panelWin, CWBackingStore, &panelAtt);
        XMapWindow(display, panelWin);
        XFlush(display);
        XSelectInput(display, panelWin, ExposureMask|ButtonPressMask|EnterWindowMask);
    }

    void flush()
    {
        XFlush(display);
        XSelectInput(display, panelWin, ExposureMask|ButtonPressMask);
    }

    void disp()
    {
        XPutImage(display, panelWin, gc, ximg, 0, 0, 0, 0, panel_w, panel_h);
        flush();
    }
    
    void refresh()
    {
        XNextEvent(display, &panel_event);

        switch(panel_event.type)
        {
            case Expose:
                if(panel_event.xexpose.count>0)
                break;

                disp();
                break;
            case ButtonPress:
                disp();

                break;
        }
    }

    void update_row(unsigned long row, char * t_buff, unsigned int img_depth)
    {
        unsigned long buf_indx=0;
        unsigned long tmp_pix_val;
        for (unsigned long pix_indx=0; pix_indx < panel_w; pix_indx++)
        {
            buf_indx = pix_indx*img_depth;
            tmp_pix_val = t_buff[buf_indx+2];
            tmp_pix_val = (tmp_pix_val << 8) + t_buff[buf_indx+1];
            tmp_pix_val = (tmp_pix_val << 8) + t_buff[buf_indx];
            XPutPixel(ximg, pix_indx, row, tmp_pix_val); 
        }
        disp();
    }

    void update_row(unsigned long row, unsigned long * t_buff)
    {
        unsigned long tmp_pix_val;
        for (unsigned long pix_indx=0; pix_indx < panel_w; pix_indx++)
        {
            tmp_pix_val = t_buff[pix_indx];
            XPutPixel(ximg, pix_indx, row, tmp_pix_val); 
        }
        disp();
    }

    void update_pixel(unsigned long x, unsigned long y, unsigned long pixel)
    {
        XPutPixel(ximg, x, y, pixel);
        disp();
    }

    void move_window(unsigned int x_m, unsigned int y_m)
    {
        XMoveWindow(display, panelWin, x_m, y_m);

    }
    
}; // end_of_panel_class

#endif //end_of_PANEL_H_


