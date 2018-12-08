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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
//CS337 
#include <gnuradio/io_signature.h>
#include "symbol_sync_cvc_impl.h"
#include <volk/volk.h>
#include <gnuradio/expj.h>
#include <gnuradio/math.h>

namespace gr {
  namespace wifi_ofdm {
    #define d_debug 0
    #define dout d_debug && std::cout
    static int d_nfft = 64;                                                     // 子载波数目，也即是fft的矩阵规模
    static int d_nsps = 80;                                                     // d_ncp+d_nfft
    static int d_ncp = 16;                                                      // 循环前缀CP长度取四分之一比较合适
    static const float d_thres = 0.9;
    static gr_complex d_dumPhase = gr_complex(1,0);
    symbol_sync_cvc::sptr
    symbol_sync_cvc::make(int FFTsize)
    {
	  d_nfft = FFTsize;
      return gnuradio::get_initial_sptr
        (new symbol_sync_cvc_impl(FFTsize));
    }

    /*
     * The private constructor
     */
    symbol_sync_cvc_impl::symbol_sync_cvc_impl(int FFTsize)
      : gr::block("symbol_sync_cvc",
              gr::io_signature::make(1, 1, sizeof(gr_complex)),
              gr::io_signature::make(1, 1, sizeof(gr_complex)*d_nfft)),
              d_bname(pmt::intern(alias()))
    {
      d_state =0;
      d_symbol_cnt =0;
      d_conj_long = (gr_complex*) volk_malloc(sizeof(gr_complex)*d_nfft,volk_get_alignment());
	  if (FFTsize == 64) {
		  d_nsps = 80;
		  d_ncp = 16;
		  volk_32fc_conjugate_32fc(d_conj_long, d_long_pre_64, d_nfft);
		  volk_32fc_x2_conjugate_dot_prod_32fc(&d_long_eng, d_conj_long, d_conj_long, d_nfft);
		  set_tag_propagation_policy(TPP_DONT);
	  }
	  else if(FFTsize==32) {
		  d_nsps = 40;
		  d_ncp = 8;
		  volk_32fc_conjugate_32fc(d_conj_long, d_long_pre_32, d_nfft);
		  volk_32fc_x2_conjugate_dot_prod_32fc(&d_long_eng, d_conj_long, d_conj_long, d_nfft);
		  set_tag_propagation_policy(TPP_DONT);
	  }
	  else{
		  d_nsps = 160;
		  d_ncp = 32;
		  volk_32fc_conjugate_32fc(d_conj_long, d_long_pre_128, d_nfft);
		  volk_32fc_x2_conjugate_dot_prod_32fc(&d_long_eng, d_conj_long, d_conj_long, d_nfft);
		  set_tag_propagation_policy(TPP_DONT);
	  }
    }

    /*
     * Our virtual destructor.
     */
    symbol_sync_cvc_impl::~symbol_sync_cvc_impl()
    {
      volk_free(d_conj_long);
    }

    void
    symbol_sync_cvc_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
      /* <+forecast+> e.g. ninput_items_required[0] = noutput_items */
      ninput_items_required[0] = noutput_items * d_nfft + 2 * d_nfft;
    }

    int
    symbol_sync_cvc_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
      const gr_complex *in = (const gr_complex *) input_items[0];
      gr_complex *out = (gr_complex *) output_items[0];
      gr_complex eng_fir,eng_sec;
      gr_complex tmp_auto;
      int nout=0, ncon=0, nin;
      float first_cross, second_cross, data_cross, fine_cfo;
      
      if(ninput_items[0]<d_nfft*2 || noutput_items==0){
        consume_each(0);
        return 0;
      }

      if(d_state==0){
        // searching for long preamble               use the cfo calculated in last module
        nin = ninput_items[0] - d_nsps;
        while(ncon<nin &&  (nout < noutput_items * d_nfft) ){
          volk_32fc_x2_dot_prod_32fc(&d_first_long,&in[ncon],d_conj_long,d_nfft);
          volk_32fc_x2_dot_prod_32fc(&d_second_long,&in[ncon+d_nfft],d_conj_long,d_nfft);
          volk_32fc_x2_conjugate_dot_prod_32fc(&eng_fir,&in[ncon],&in[ncon],d_nfft);                                    //result input taps point_num
          volk_32fc_x2_conjugate_dot_prod_32fc(&eng_sec,&in[ncon+d_nfft],&in[ncon+d_nfft],d_nfft);
          first_cross = std::abs(d_first_long)/(std::sqrt(std::abs(eng_fir*d_long_eng))+1e-12);
          second_cross = std::abs(d_second_long)/(std::sqrt(std::abs(eng_sec*d_long_eng))+1e-12);
          if( first_cross > d_thres && second_cross > d_thres){
          // sync of long preamble
          // fine-tune the first one for it contains no smoothed symbols
          // only use the first 32 samples
            volk_32fc_x2_conjugate_dot_prod_32fc(&tmp_auto, &in[ncon+d_nfft], &in[ncon], d_nfft/2);
            fine_cfo = std::arg(tmp_auto)/(float)d_nfft;
            // change state
            d_state =1;
            d_symbol_cnt =0;
            add_item_tag(0,nitems_written(0)+nout/d_nfft,pmt::intern("long_pre"),pmt::PMT_T,d_bname);
            add_item_tag(0,nitems_written(0)+nout/d_nfft,pmt::intern("symbol_idx"),pmt::from_long(d_symbol_cnt++),d_bname);
            // FIXME: find a more stable way to fine tune CFO
            volk_32fc_s32fc_x2_rotator_32fc(&out[nout],&in[ncon],gr_expj(-fine_cfo),&d_dumPhase,d_nfft);
            //std::memcpy(out,&in[ncon],sizeof(gr_complex)*d_nfft);
            nout += d_nfft;
            ncon += d_nfft*2;
            dout <<"DEBUG--symbol_sync: first cross="<<first_cross<<" second_cross="<<second_cross
            <<" fine_est="<<fine_cfo<<std::endl;
            //break;
            consume_each(ncon);
            return nout/d_nfft;
          }else{
            ncon++;
          }
        }
      }else{
        // synced to ofdm symbols
        nin = ninput_items[0] - 2 * d_nfft;
        while(ncon<nin && (nout < noutput_items * d_nfft) ){
          volk_32fc_x2_conjugate_dot_prod_32fc(&tmp_auto, &in[ncon+d_nfft], &in[ncon], d_ncp);
          volk_32fc_x2_conjugate_dot_prod_32fc(&eng_fir,&in[ncon],&in[ncon],d_ncp);
          volk_32fc_x2_conjugate_dot_prod_32fc(&eng_sec,&in[ncon+d_nfft],&in[ncon+d_nfft],d_ncp);
          data_cross = std::abs(tmp_auto)/(std::sqrt(std::abs(eng_fir*eng_sec))+1e-12);
          if(data_cross > d_thres){
            // still sync
            fine_cfo = std::arg(tmp_auto)/(float)d_nfft;
            dout<<"DEBUG--symbol_sync: data_cross="<<data_cross<<" ,fine_cfo="<<fine_cfo
              <<" ,symbol_idx="<<d_symbol_cnt<<std::endl;
            add_item_tag(0,nitems_written(0)+nout/d_nfft,pmt::intern("symbol_idx"),pmt::from_long(d_symbol_cnt++),d_bname);
            // FIXME: find a more stable way to fine tune CFO
            volk_32fc_s32fc_x2_rotator_32fc(&out[nout],&in[ncon+16],gr_expj(-fine_cfo),&d_dumPhase,d_nfft);
            //std::memcpy(&out[nout],&in[ncon+16],sizeof(gr_complex)*d_nfft);

            nout += d_nfft;
            ncon += d_nsps;
            
          }else{
            // non sync anymore
            d_symbol_cnt =0;
            d_state = 0;
            consume_each(ncon+1);
            return nout/d_nfft;
          }
        }
      }
      consume_each (ncon);
      return nout/d_nfft;
    }

  } /* namespace wifi_ofdm */
} /* namespace gr */

