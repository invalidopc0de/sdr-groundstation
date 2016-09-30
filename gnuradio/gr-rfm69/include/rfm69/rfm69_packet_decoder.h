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


#ifndef INCLUDED_RFM69_RFM69_PACKET_DECODER_H
#define INCLUDED_RFM69_RFM69_PACKET_DECODER_H

#include <rfm69/api.h>
#include <gnuradio/block.h>

namespace gr {
  namespace rfm69 {

    /*!
     * \brief <+description of block+>
     * \ingroup rfm69
     *
     */
    class RFM69_API rfm69_packet_decoder : virtual public gr::block
    {
     public:
      typedef boost::shared_ptr<rfm69_packet_decoder> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of rfm69::rfm69_packet_decoder.
       *
       * To avoid accidental use of raw pointers, rfm69::rfm69_packet_decoder's
       * constructor is in a private implementation
       * class. rfm69::rfm69_packet_decoder::make is the public interface for
       * creating new instances.
       */
      static sptr make(bool drop_header);
    };

  } // namespace rfm69
} // namespace gr

#endif /* INCLUDED_RFM69_RFM69_PACKET_DECODER_H */

