/* -*- c++ -*- */
/* 
 * Copyright 2016 Mark Saunders.
 * 
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 * 
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "rfm69_packet_decoder_impl.h"

namespace gr {
  namespace rfm69 {

    rfm69_packet_decoder::sptr
    rfm69_packet_decoder::make(bool drop_header)
    {
      return gnuradio::get_initial_sptr
        (new rfm69_packet_decoder_impl(drop_header));
    }

    /*
     * The private constructor
     */
    rfm69_packet_decoder_impl::rfm69_packet_decoder_impl(bool drop_header)
      : gr::block("rfm69_packet_decoder",
              gr::io_signature::make(1, 1, sizeof(char)),
              gr::io_signature::make(0, 0, 0)),

              d_drop_header(drop_header),

              is_msg(false),
              buffer_expected_len(0),
              bit_index(0),
              buffer_i(0)
    {
      message_port_register_out(pmt::mp("pdu_out"));

      // Reset Buffer
      buffer_reset();
    }

    /*
     * Our virtual destructor.
     */
    rfm69_packet_decoder_impl::~rfm69_packet_decoder_impl()
    {
    }

    void
    rfm69_packet_decoder_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
      /* <+forecast+> e.g. ninput_items_required[0] = noutput_items */
      ninput_items_required[0] = noutput_items;
    }

    int
    rfm69_packet_decoder_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
      const unsigned char *in = (const unsigned char *) input_items[0];
      unsigned char *out = (unsigned char *) output_items[0];

      // Do <+signal processing+>
      // Tell runtime system how many input items we consumed on
      // each input stream.
      consume_each (noutput_items);

      // Tell runtime system how many output items we produced.
      return noutput_items;
    }

    /****************************************************************
    BUFFER RESET
    ****************************************************************/
    int rfm69_packet_decoder_impl::buffer_reset(){
      memset(buffer,0,BUF_MAX_SIZE);
      bit_index=0;
      buffer_i=0;
      return 1;
    }


  } /* namespace rfm69 */
} /* namespace gr */

