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

// CFO means carrier frequency offset, here roughly fix the 

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "coarse_cfo_fixer_cc_impl.h"
#include <volk/volk.h>
#include <gnuradio/math.h>
#include <gnuradio/expj.h>

namespace gr {
  namespace wifi_ofdm {
    #define TWO_PI 2 * M_PI
    #define d_debug 0
    #define dout d_debug && std::cout
    static const int d_cap = 8192*2;
    static const float d_threshold = 0.9;
    static const int d_escape = 320;
    inline void phase_wrap(float& phase){       //
      while(phase>=TWO_PI)
        phase -= TWO_PI;
      while(phase<=-TWO_PI)
        phase += TWO_PI;
    }
    coarse_cfo_fixer_cc::sptr
    coarse_cfo_fixer_cc::make(int delay)
    {
      return gnuradio::get_initial_sptr
        (new coarse_cfo_fixer_cc_impl(delay));
    }

    /*
     * The private constructor
     */
    coarse_cfo_fixer_cc_impl::coarse_cfo_fixer_cc_impl(int delay)
      : gr::block("coarse_cfo_fixer_cc",
              gr::io_signature::make2(2, 2, sizeof(gr_complex),sizeof(gr_complex)),
              gr::io_signature::make(1, 1, sizeof(gr_complex))),
              d_bname(pmt::intern(alias())),
              d_delay(delay)
    {
      set_tag_propagation_policy(TPP_DONT);

      set_max_noutput_items(d_cap-delay);
      set_history(delay);
      d_inXdelay = (gr_complex*) volk_malloc(sizeof(gr_complex)*d_cap,volk_get_alignment());
      d_inSqu = (gr_complex*) volk_malloc(sizeof(gr_complex)*d_cap,volk_get_alignment());
      d_delaySqu = (gr_complex*) volk_malloc(sizeof(gr_complex)*d_cap,volk_get_alignment());
      d_state = 0;
      d_nCnt =0;
      d_phase = 0;
    }

    /*
     * Our virtual destructor.
     */
    coarse_cfo_fixer_cc_impl::~coarse_cfo_fixer_cc_impl()
    {
      volk_free(d_inXdelay);
      volk_free(d_inSqu);
      volk_free(d_delaySqu);
    }

    void
    coarse_cfo_fixer_cc_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
      /* <+forecast+> e.g. ninput_items_required[0] = noutput_items */
      for(int i =0;i<ninput_items_required.size();++i)
        ninput_items_required[i] = noutput_items + history();
    }

    int
    coarse_cfo_fixer_cc_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
      const gr_complex *in = (const gr_complex *) input_items[0];
      const gr_complex *delay= (const gr_complex *) input_items[1];
      gr_complex *out = (gr_complex *) output_items[0];
      int nout = std::max(std::min(noutput_items,std::min(ninput_items[0],ninput_items[1])-(int)history()+1),0);
      int nin = nout+d_delay-1;
      float tmp_auto;
      if(nout==0){
        consume_each(0);
        return 0;
      }
      volk_32fc_x2_multiply_32fc(d_inXdelay,delay,in,nin);
      volk_32fc_x2_multiply_conjugate_32fc(d_inSqu,in,in,nin);
      volk_32fc_x2_multiply_conjugate_32fc(d_delaySqu,delay,delay,nin);
      gr_complex nom(0,0), inpwr(0,0), delpwr(0,0);
      for(int i=0;i<d_delay-1;++i){
        nom += d_inXdelay[i];
        inpwr += d_inSqu[i];
        delpwr += d_delaySqu[i];
      }
      int ngen =0 ;
      while(ngen<nout){
        nom += d_inXdelay[d_delay-1+ngen];
        inpwr += d_inSqu[d_delay-1+ngen];
        delpwr += d_delaySqu[d_delay-1+ngen];
        out[ngen] = in[ngen] * gr_expj(-d_phase); // conj(delay) * in --> cfo    this function request a x, return a complex cosx+sinxi
        d_phase += d_coarse_cfo;
        phase_wrap(d_phase);
        tmp_auto = std::abs(nom)/(std::sqrt(std::abs(inpwr*delpwr))+1e-12);
        // main loop
        if(d_state==1){
          d_nCnt--;
          if(d_nCnt == 0){
            d_state = 0;
          }
        }else{
          // state ==0
          if(tmp_auto>d_threshold && tmp_auto<=1.0f){
            d_nCnt++;
            if(d_nCnt == d_delay){
              d_state = 1;
              dout<<"DEBUG-CoarseCfo: auto val--nom="<<std::abs(nom)<<",inpwr="<<std::abs(inpwr)<<" ,delpwr="<<std::abs(delpwr)<<std::endl;
              d_nCnt = d_escape;
              d_phase = 0;
              d_coarse_cfo = std::arg(nom)/(float)d_delay;
              add_item_tag(0,nitems_written(0)+ngen,pmt::intern("cfo_est"),pmt::from_float(d_coarse_cfo),d_bname);
              dout<<"DEBUG-CoarseCfo:auto_calc="<<tmp_auto<<" ,cfo_est="<<d_coarse_cfo<<" ,added at:"<<nitems_written(0)+ngen<<std::endl;
            }
          }else{
            d_nCnt =0;
          }
        }
        nom -= d_inXdelay[ngen];
        inpwr -= d_inSqu[ngen];
        delpwr -= d_delaySqu[ngen];
        ngen++;
      }
      
      consume_each (nout);
      return nout;
    }

  } /* namespace wifi_ofdm */
} /* namespace gr */

