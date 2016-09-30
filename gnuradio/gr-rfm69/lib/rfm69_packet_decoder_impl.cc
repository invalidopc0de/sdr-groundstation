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

#include <stdexcept>
#include <stdlib.h>
#include <gnuradio/io_signature.h>
#include "rfm69_packet_decoder_impl.h"

using namespace std;
const char* nibble_table = "0123456789ABCDEF";

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
            buffer_i(0),
            verbose(false),
            d_msg_out(pmt::mp("pdu_out"))
            {
                message_port_register_out(d_msg_out);

                // Reset Buffer
                buffer_reset();

            }

        /*
         * Our virtual destructor.
         */
        rfm69_packet_decoder_impl::~rfm69_packet_decoder_impl()
        {
        }




        void rfm69_packet_decoder_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
        {
            /* <+forecast+> e.g. ninput_items_required[0] = noutput_items */
            ninput_items_required[0] = noutput_items;
        }

        int rfm69_packet_decoder_impl::general_work (int noutput_items,
                gr_vector_int &ninput_items,
                gr_vector_const_void_star &input_items,
                gr_vector_void_star &output_items)
        {
            const unsigned char *in = (const unsigned char *) input_items[0];
            std::vector<tag_t> tags;
            // TODO: Make sure this is the right tag
            get_tags_in_window(tags, 0, 0, noutput_items);

            for(int i=0;i<noutput_items;i++){

                // we are currently processing a message
                if(is_msg){
                    // add new bit
                    buffer_append(in[i]);
                    // end of message ? 
                    if ((buffer_i != 0 && bit_index == 8 && buffer_i == buffer_expected_len - 1) ||
                            buffer_i == (BUF_MAX_SIZE)-1){
                        is_msg = false; // stop
                        buffer_flush();
                    }
                }else{
                    // did we find the beginning of a new message ?
                    int check_tag = 0;
                    int tag_num = tags.size();

                    while(check_tag < tag_num && tags[check_tag].offset < i){
                        check_tag += 1;
                    }
                    if (check_tag != tag_num){
                        i = tags[check_tag].offset;
                        // reset buffer 
                        buffer_reset();
                        // add the first bit
                        buffer_append(in[i]);
                        // swith processing mode
                        is_msg = true;
                    } else {
                        i = noutput_items; // no more tags, so we're done
                    }
                }
            }
            // Tell runtime system how many input items we consumed on
            // each input stream.
            consume_each (noutput_items);

            // Tell runtime system how many output items we produced.
            return noutput_items;
        }

        int rfm69_packet_decoder_impl::buffer_append(unsigned char byte)
        {

            // need a new byte in buffer[] ?
            if(bit_index == 8){
                bit_index=0;
                buffer_i++;
            }

            // is the first byte complete ? If yes, it contains the length of the message
            // header len and CRC must be added
            if(buffer_i == 1 && bit_index == 0){
                buffer_expected_len =     (int)(buffer[0]);
                buffer_expected_len += 1;  // header is 1 byte (len)
            }

            // add new bit to the end of the buffer
            if(bit_index==0){
                buffer[buffer_i] = (byte & 0x1);
            }else{
                buffer[buffer_i] = (buffer[buffer_i] << 1) | (byte & 0x1);
            }
            // inc bit index
            bit_index++;

            return 1;
        }

        /****************************************************************
          BUFFER FLUSH
         ****************************************************************/
        // print/send out current buffer
        int rfm69_packet_decoder_impl::buffer_flush(){
            pmt::pmt_t header = pmt::make_dict();
            pmt::pmt_t msg;


            if(verbose){
                fprintf(stdout, "[verbose] Pkt: ");
                for(int j=0;j<buffer_i+1;j++){
                    fprintf(stdout, "%02x ", (unsigned char)buffer[j]);
                }
            }


            if(verbose){
                fprintf(stdout, "\n");
            }


            if(!d_drop_header){
                header = pmt::dict_add(header, pmt::intern("len"), pmt::from_long(buffer_expected_len));
            }
            msg = pmt::cons(header, pmt::init_u8vector(buffer_expected_len - 1, buffer));
            message_port_pub(d_msg_out, msg);

            return 1;
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

