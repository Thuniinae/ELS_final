//******************************************************************************
// Copyright 2015 Cadence Design Systems, Inc.
// All Rights Reserved.
//
//******************************************************************************

#ifndef _PIXEL_STRUCT_H_INCLUDED
#define _PIXEL_STRUCT_H_INCLUDED

#define IBITS 8
#define MAX_PIX_VAL 255
#define MIN_PIX_VAL 0

class pixel
{
    public:

    sc_uint<IBITS> r, g, b;

    pixel():r(0), g(0), b(0)
    {
    }

    pixel(sc_uint<IBITS> ri, sc_uint<IBITS> gi, sc_uint<IBITS> bi):r(ri),g(gi),b(bi)
    {
    }

    pixel(unsigned int ri, unsigned int gi, unsigned int bi):r(ri),g(gi),b(bi)
    {
    }

    pixel(const pixel& oi):r(oi.r),g(oi.g),b(oi.b)
    {
    }

    void set(sc_uint<IBITS> ri, sc_uint<IBITS> gi, sc_uint<IBITS> bi)
    {
        r = ri;
        g = gi;
        b = bi;
    }

    void set(const unsigned int ri, const unsigned int gi, const unsigned int bi)
    {
        r = ri;
        g = gi;
        b = bi;
    }

    void set(const pixel& px)
    {
        r = px.r;
        g = px.g;
        b = px.b;
    }

    void negate()
    {
        r = (MAX_PIX_VAL-r);
        g = (MAX_PIX_VAL-g);
        b = (MAX_PIX_VAL-b);
    }

    void operator = (const pixel& px)
    {
        r = px.r;
        g = px.g;
        b = px.b;
    }

    bool operator == (const pixel& px)
    {
        return( (r == px.r) && (g == px.g) && (b == px.b) );
    }

    bool operator != (const pixel& px)
    {
        return( (r != px.r) || (g != px.g) || (b != px.b) );
    }
};

inline void cynw_interpret(const pixel &in, sc_uint<24> &out)
{
    out = (in.r, in.g, in.b);
}

inline void cynw_interpret(const sc_uint<24> &in, pixel &out)
{
    out.r = in.range(23, 16);
    out.g = in.range(15, 8);
    out.b = in.range(7, 0);
}

inline void sc_trace (sc_trace_file *tf, const pixel& px, const std::string& name)
{
    if(tf)
    {
        tf->trace( px.r, name+std::string(".b") );
        tf->trace( px.g, name+std::string(".b") );
        tf->trace( px.b, name+std::string(".b") );
    }
}

inline ostream & operator << (ostream& os, const pixel& px)
{
#ifndef STRATUS
    os << "(" << px.r.to_string().c_str() << ","
       << px.g.to_string().c_str() << ","
       << px.b.to_string().c_str() << ")";
#endif
    return os;
}

#endif 
