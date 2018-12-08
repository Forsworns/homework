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

#ifndef INCLUDED_WIFI_OFDM_SYMBOL_PARSER_VC_IMPL_H
#define INCLUDED_WIFI_OFDM_SYMBOL_PARSER_VC_IMPL_H

#include <wifi_ofdm/symbol_parser_vc.h>

namespace gr {
  namespace wifi_ofdm {

    static const gr_complex d_pilot_128[8] = {
      gr_complex(1,0),gr_complex(1,0),gr_complex(1,0),gr_complex(-1,0),gr_complex(1,0),gr_complex(1,0),gr_complex(1,0),gr_complex(-1,0)
    };
    static const gr_complex d_long_128[128] = {
      gr_complex(0,0),gr_complex(0,0),gr_complex(0,0),gr_complex(0,0),gr_complex(0,0),gr_complex(0,0),gr_complex(1,0),gr_complex(1,0),gr_complex(-1,0),gr_complex(-1,0),
      gr_complex(1,0),gr_complex(1,0),gr_complex(-1,0),gr_complex(1,0),gr_complex(-1,0),gr_complex(1,0),gr_complex(1,0),gr_complex(1,0),gr_complex(1,0),gr_complex(1,0),
      gr_complex(1,0),gr_complex(-1,0),gr_complex(-1,0),gr_complex(1,0),gr_complex(1,0),gr_complex(-1,0),gr_complex(1,0),gr_complex(-1,0),gr_complex(1,0),gr_complex(1,0),
      gr_complex(1,0),gr_complex(1,0),gr_complex(0,0),gr_complex(1,0),gr_complex(-1,0),gr_complex(-1,0),gr_complex(1,0),gr_complex(1,0),gr_complex(-1,0),gr_complex(1,0),
      gr_complex(-1,0),gr_complex(1,0),gr_complex(-1,0),gr_complex(-1,0),gr_complex(-1,0),gr_complex(-1,0),gr_complex(-1,0),gr_complex(1,0),gr_complex(1,0),gr_complex(-1,0),
      gr_complex(-1,0),gr_complex(1,0),gr_complex(-1,0),gr_complex(1,0),gr_complex(-1,0),gr_complex(1,0),gr_complex(1,0),gr_complex(1,0),gr_complex(1,0),gr_complex(0,0),
      gr_complex(0,0),gr_complex(0,0),gr_complex(0,0),gr_complex(0,0),gr_complex(0,0),gr_complex(0,0),gr_complex(0,0),gr_complex(0,0),gr_complex(0,0),gr_complex(0,0),gr_complex(1,0),gr_complex(1,0),gr_complex(-1,0),gr_complex(-1,0),
      gr_complex(1,0),gr_complex(1,0),gr_complex(-1,0),gr_complex(1,0),gr_complex(-1,0),gr_complex(1,0),gr_complex(1,0),gr_complex(1,0),gr_complex(1,0),gr_complex(1,0),
      gr_complex(1,0),gr_complex(-1,0),gr_complex(-1,0),gr_complex(1,0),gr_complex(1,0),gr_complex(-1,0),gr_complex(1,0),gr_complex(-1,0),gr_complex(1,0),gr_complex(1,0),
      gr_complex(1,0),gr_complex(1,0),gr_complex(0,0),gr_complex(1,0),gr_complex(-1,0),gr_complex(-1,0),gr_complex(1,0),gr_complex(1,0),gr_complex(-1,0),gr_complex(1,0),
      gr_complex(-1,0),gr_complex(1,0),gr_complex(-1,0),gr_complex(-1,0),gr_complex(-1,0),gr_complex(-1,0),gr_complex(-1,0),gr_complex(1,0),gr_complex(1,0),gr_complex(-1,0),
      gr_complex(-1,0),gr_complex(1,0),gr_complex(-1,0),gr_complex(1,0),gr_complex(-1,0),gr_complex(1,0),gr_complex(1,0),gr_complex(1,0),gr_complex(1,0),gr_complex(0,0),
      gr_complex(0,0),gr_complex(0,0),gr_complex(0,0),gr_complex(0,0)
    };
    static const float d_pilot_sign[127] = {
      1,1,1,1, -1,-1,-1,1, -1,-1,-1,-1, 1,1,-1,1, -1,-1,1,1, -1,1,1,-1, 1,1,1,1, 1,1,-1,1,
      1,1,-1,1, 1,-1,-1,1, 1,1,-1,1, -1,-1,-1,1, -1,1,-1,-1, 1,-1,-1,1, 1,1,1,1, -1,-1,1,1,
      -1,-1,1,-1, 1,-1,1,1, -1,-1,-1,1, 1,-1,-1,-1, -1,1,-1,-1, 1,-1,1,1, 1,1,-1,1, -1,1,-1,1,
      -1,-1,-1,-1, -1,1,-1,1, 1,-1,1,-1, 1,1,1,-1, -1,1,-1,-1, -1,1,1,1, -1,-1,-1,-1, -1,-1,-1
    };
    static const int d_subcarrier_type_128[128] = {
      0,0,0,0,0,0,1,1,1,1,1,-1,1,1,1,1,1,1,1,1,1,1,1,1,1,-1,1,1,1,1,1,1,
      0,1,1,1,1,1,1,-1,1,1,1,1,1,1,1,1,1,1,1,1,1,-1,1,1,1,1,1,0,0,0,0,0,
      0,0,0,0,0,0,1,1,1,1,1,-1,1,1,1,1,1,1,1,1,1,1,1,1,1,-1,1,1,1,1,1,1,
      0,1,1,1,1,1,1,-1,1,1,1,1,1,1,1,1,1,1,1,1,1,-1,1,1,1,1,1,0,0,0,0,0
    };
    
    static const int d_desubcarr_idx_128[128] = {
      64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119,120,121,122,123,124,125,126,127,
      0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63
    };

    static const gr_complex d_pilot_64[4] = {
      gr_complex(1,0),gr_complex(1,0),gr_complex(1,0),gr_complex(-1,0)
    };
    static const gr_complex d_long_64[64] = {
      gr_complex(0,0),gr_complex(0,0),gr_complex(0,0),gr_complex(0,0),gr_complex(0,0),gr_complex(0,0),gr_complex(1,0),gr_complex(1,0),gr_complex(-1,0),gr_complex(-1,0),
      gr_complex(1,0),gr_complex(1,0),gr_complex(-1,0),gr_complex(1,0),gr_complex(-1,0),gr_complex(1,0),gr_complex(1,0),gr_complex(1,0),gr_complex(1,0),gr_complex(1,0),
      gr_complex(1,0),gr_complex(-1,0),gr_complex(-1,0),gr_complex(1,0),gr_complex(1,0),gr_complex(-1,0),gr_complex(1,0),gr_complex(-1,0),gr_complex(1,0),gr_complex(1,0),
      gr_complex(1,0),gr_complex(1,0),gr_complex(0,0),gr_complex(1,0),gr_complex(-1,0),gr_complex(-1,0),gr_complex(1,0),gr_complex(1,0),gr_complex(-1,0),gr_complex(1,0),
      gr_complex(-1,0),gr_complex(1,0),gr_complex(-1,0),gr_complex(-1,0),gr_complex(-1,0),gr_complex(-1,0),gr_complex(-1,0),gr_complex(1,0),gr_complex(1,0),gr_complex(-1,0),
      gr_complex(-1,0),gr_complex(1,0),gr_complex(-1,0),gr_complex(1,0),gr_complex(-1,0),gr_complex(1,0),gr_complex(1,0),gr_complex(1,0),gr_complex(1,0),gr_complex(0,0),
      gr_complex(0,0),gr_complex(0,0),gr_complex(0,0),gr_complex(0,0)
    };
    static const int d_subcarrier_type_64[64] = {
      0,0,0,0,0,0,1,1,1,1,1,-1,1,1,1,1,1,1,1,1,1,1,1,1,1,-1,1,1,1,1,1,1,
      0,1,1,1,1,1,1,-1,1,1,1,1,1,1,1,1,1,1,1,1,1,-1,1,1,1,1,1,0,0,0,0,0
    };
    
    static const int d_desubcarr_idx_64[64] = {
      32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,
      0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31
    };

    static const gr_complex d_pilot_32[2] = {
      gr_complex(1,0),gr_complex(1,0)
    };
    static const gr_complex d_long_32[32] = {
      gr_complex(0,0),gr_complex(0,0),gr_complex(0,0),gr_complex(0,0),gr_complex(0,0),gr_complex(0,0),gr_complex(1,0),gr_complex(1,0),gr_complex(-1,0),gr_complex(-1,0),
      gr_complex(1,0),gr_complex(1,0),gr_complex(-1,0),gr_complex(1,0),gr_complex(-1,0),gr_complex(1,0),gr_complex(1,0),gr_complex(1,0),gr_complex(1,0),gr_complex(1,0),
      gr_complex(1,0),gr_complex(-1,0),gr_complex(-1,0),gr_complex(1,0),gr_complex(1,0),gr_complex(-1,0),gr_complex(1,0),gr_complex(-1,0),gr_complex(1,0),gr_complex(1,0),
      gr_complex(1,0),gr_complex(1,0)
    };
    static const int d_subcarrier_type_32[32] = {
      0,0,0,0,0,0,1,1,1,1,1,-1,1,1,1,1,1,1,1,1,1,1,1,1,1,-1,1,1,1,1,1,1
    };
    
    static const int d_desubcarr_idx_32[32] = {
      16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,
      0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15
    };

    class symbol_parser_vc_impl : public symbol_parser_vc
    {
     private:
      // Nothing to declare in this block.
      gr_complex * d_channel_est;
      gr_complex * d_buf;
      int d_pilot_idx;
      const pmt::pmt_t d_bname;

     public:
      symbol_parser_vc_impl(int FFTsize);
      ~symbol_parser_vc_impl();

      void symbol_eq(gr_complex* out, const gr_complex* in, int pilot_idx);
      void channel_estimation(const gr_complex* in);

      // Where all the action really happens
      void forecast (int noutput_items, gr_vector_int &ninput_items_required);

      int general_work(int noutput_items,
           gr_vector_int &ninput_items,
           gr_vector_const_void_star &input_items,
           gr_vector_void_star &output_items);
    };

  } // namespace wifi_ofdm
} // namespace gr

#endif /* INCLUDED_WIFI_OFDM_SYMBOL_PARSER_VC_IMPL_H */

