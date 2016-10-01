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

#ifndef INCLUDED_RFM69_RFM69_PACKET_DECODER_IMPL_H
#define INCLUDED_RFM69_RFM69_PACKET_DECODER_IMPL_H

#include <rfm69/rfm69_packet_decoder.h>
#include <pmt/pmt.h>

#define BUF_MAX_SIZE	2048 // bytes
#define MSG_MAX_SIZE    255 // From the spec

using namespace std;

namespace gr {
  namespace rfm69 {

    class rfm69_packet_decoder_impl : public rfm69_packet_decoder
    {
     private:
      bool d_drop_header; // block arg  
      bool verbose;
      pmt::pmt_t d_msg_out;


      bool is_msg;
      unsigned char buffer[BUF_MAX_SIZE];	// store message
	  unsigned char buffer_expected_len; 		// message length according to message header, in bits
      int bit_index;				// bit index, within a byte. byte_index would have been better ?
      int buffer_i; // index of buffer[]

      struct timeval time_init;
      struct timeval time_sync_found;
      bool set_sync_word( const std::string &sync_word);
      char pack_bytes (const unsigned char* unpacked_bytes, int len, char current);
      // manage buffer
      int buffer_flush();
      int buffer_append(unsigned char byte);
      int buffer_reset();

     public:
      rfm69_packet_decoder_impl(bool drop_header);
      ~rfm69_packet_decoder_impl();

      // Where all the action really happens
      void forecast (int noutput_items, gr_vector_int &ninput_items_required);

      int general_work(int noutput_items,
           gr_vector_int &ninput_items,
           gr_vector_const_void_star &input_items,
           gr_vector_void_star &output_items);

    };

  } // namespace rfm69
} // namespace gr

#endif /* INCLUDED_RFM69_RFM69_PACKET_DECODER_IMPL_H */

