/* -*- c++ -*- */
/* 
 * Copyright 2018 <+YOU OR YOUR COMPANY+>.
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


#ifndef INCLUDED_WIFI_OFDM_BLOCK_DECODER_VC_H
#define INCLUDED_WIFI_OFDM_BLOCK_DECODER_VC_H

#include <wifi_ofdm/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
  namespace wifi_ofdm {

    /*!
     * \brief <+description of block+>
     * \ingroup wifi_ofdm
     *
     */
    class WIFI_OFDM_API block_decoder_vc : virtual public gr::sync_block
    {
     public:
      typedef boost::shared_ptr<block_decoder_vc> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of wifi_ofdm::block_decoder_vc.
       *
       * To avoid accidental use of raw pointers, wifi_ofdm::block_decoder_vc's
       * constructor is in a private implementation
       * class. wifi_ofdm::block_decoder_vc::make is the public interface for
       * creating new instances.
       */
      static sptr make(int FFTsize);
    };

  } // namespace wifi_ofdm
} // namespace gr

#endif /* INCLUDED_WIFI_OFDM_BLOCK_DECODER_VC_H */

