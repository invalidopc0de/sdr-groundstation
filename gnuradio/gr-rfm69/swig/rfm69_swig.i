/* -*- c++ -*- */

#define RFM69_API

%include "gnuradio.i"			// the common stuff

//load generated python docstrings
%include "rfm69_swig_doc.i"

%{
#include "rfm69/rfm69_packet_decoder.h"
%}
GR_SWIG_BLOCK_MAGIC2(rfm69, rfm69_packet_decoder);

%include "rfm69/rfm69_packet_decoder.h"
GR_SWIG_BLOCK_MAGIC2(rfm69, rfm69_packet_decoder);
