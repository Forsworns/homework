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

#include <gnuradio/io_signature.h>
#include "symbol_parser_vc_impl.h"
#include <volk/volk.h>
#include <gnuradio/expj.h>
#include <gnuradio/math.h>

namespace gr
{
namespace wifi_ofdm
{
#define TWO_PI M_PI * 2.0f
inline float phase_wrap(float phase)
{
  while (phase >= TWO_PI)
    phase -= TWO_PI;
  while (phase <= -TWO_PI)
    phase += TWO_PI;
  return phase;
}
//64 point FFT sampling
//48 are data subcarriers and 4 are pilot subcarriers and 12 are null subcarriers
//in d_subcarrier_type[], data marked as 1, pilot marked as -1, null marked as 0
static int d_nfft = 64;
static int d_ndata = 48;
static const pmt::pmt_t d_preTag = pmt::intern("long_pre"); //intern() return the value of the key in hash table
symbol_parser_vc::sptr                                      //sptr:shared_ptr ����ָ��
symbol_parser_vc::make(int FFTsize)                         //constructor calls private constructor, returns sptr
{
  d_nfft = FFTsize;
  d_ndata = FFTsize / 4 * 3;
  return gnuradio::get_initial_sptr(new symbol_parser_vc_impl(FFTsize));
}

/*
     * The private constructor
     */
symbol_parser_vc_impl::symbol_parser_vc_impl(int FFTsize)
    : gr::block("symbol_parser_vc",
                gr::io_signature::make(1, 1, sizeof(gr_complex) * d_nfft), //create an io_signature for input/output port
                gr::io_signature::make(1, 1, sizeof(gr_complex) * d_ndata)),
      d_bname(pmt::intern(alias())) //alias()����ģ�������intern()��Ϊpmt_t����
{
  d_channel_est = (gr_complex *)volk_malloc(sizeof(gr_complex) * d_nfft, volk_get_alignment()); //allocate space for d_nfft complex numbers
  d_buf = (gr_complex *)volk_malloc(sizeof(gr_complex) * d_nfft, volk_get_alignment());
  d_pilot_idx = 0;
  set_tag_propagation_policy(TPP_DONT);
}

/*
     * Our virtual destructor.
     */
symbol_parser_vc_impl::~symbol_parser_vc_impl()
{
  volk_free(d_channel_est);
  volk_free(d_buf);
}

//equalizer
void symbol_parser_vc_impl::symbol_eq(gr_complex *out, const gr_complex *in, int pilot_idx)
{
  int nout = 0;
  if (d_nfft == 64)
  {
    // result��Y(equalized) = Y * (1/C)
    for (int i = 0; i < d_nfft; i++)
    {
      d_buf[d_desubcarr_idx_64[i]] = in[i] * d_channel_est[i];
    }
    // A single OFDM symbol contains 52 subcarriers; 48 are data subcarriers and 4 are pilot subcarriers(-21, -7, 7, 21)
    // pilot subcarriers index: 11, 25, 39, 53 (original + 32)
    // estimate carrier phase
    gr_complex scalar = d_buf[11] * d_pilot_64[0];
    scalar += d_buf[25] * d_pilot_64[1];
    scalar += d_buf[39] * d_pilot_64[2];
    scalar += d_buf[53] * d_pilot_64[3];
    float carrier_phase = (d_pilot_sign[pilot_idx] == 1) ? std::arg(scalar) : std::arg(scalar) + M_PI;
    carrier_phase = phase_wrap(carrier_phase);
    // FIXME:estimate attennuation?
    for (int i = 0; i < d_nfft; ++i)
    {
      if (d_subcarrier_type_64[i] == 1)
      {
        out[nout++] = gr_expj(-carrier_phase) * d_buf[i];
      }
    }
  }
  else if (d_nfft == 32)
  {
    // result��Y(equalized) = Y * (1/C)
    for (int i = 0; i < d_nfft; i++)
    {
      d_buf[d_desubcarr_idx_32[i]] = in[i] * d_channel_est[i];
    }
    // A single OFDM symbol contains 52 subcarriers; 48 are data subcarriers and 4 are pilot subcarriers(-21, -7, 7, 21)
    // pilot subcarriers index: 11, 25, 39, 53 (original + 32)
    // estimate carrier phase
    gr_complex scalar = d_buf[11] * d_pilot_32[0];
    scalar += d_buf[25] * d_pilot_32[1];
    float carrier_phase = (d_pilot_sign[pilot_idx] == 1) ? std::arg(scalar) : std::arg(scalar) + M_PI;
    carrier_phase = phase_wrap(carrier_phase);
    // FIXME:estimate attennuation?
    for (int i = 0; i < d_nfft; ++i)
    {
      if (d_subcarrier_type_32[i] == 1)
      {
        out[nout++] = gr_expj(-carrier_phase) * d_buf[i];
      }
    }
  }
  else
  {
    // result��Y(equalized) = Y * (1/C)
    for (int i = 0; i < d_nfft; i++)
    {
      d_buf[d_desubcarr_idx_128[i]] = in[i] * d_channel_est[i];
    }
    // A single OFDM symbol contains 52 subcarriers; 48 are data subcarriers and 4 are pilot subcarriers(-21, -7, 7, 21)
    // pilot subcarriers index: 11, 25, 39, 53 (original + 32)
    // estimate carrier phase
    gr_complex scalar = d_buf[11] * d_pilot_128[0];
    scalar += d_buf[25] * d_pilot_128[1];
    scalar += d_buf[39] * d_pilot_128[2];
    scalar += d_buf[53] * d_pilot_128[3];
    scalar += d_buf[75] * d_pilot_128[4];
    scalar += d_buf[89] * d_pilot_128[5];
    scalar += d_buf[103] * d_pilot_128[6];
    scalar += d_buf[117] * d_pilot_128[7];
    float carrier_phase = (d_pilot_sign[pilot_idx] == 1) ? std::arg(scalar) : std::arg(scalar) + M_PI;
    carrier_phase = phase_wrap(carrier_phase);
    // FIXME:estimate attennuation?
    for (int i = 0; i < d_nfft; ++i)
    {
      if (d_subcarrier_type_128[i] == 1)
      {
        out[nout++] = gr_expj(-carrier_phase) * d_buf[i];
      }
    }
  }
}

void symbol_parser_vc_impl::channel_estimation(const gr_complex *in)
{
  gr_complex noise_pwr_est = gr_complex(1e-16, 0);
  gr_complex *mmse_tmp;
  if (d_nfft == 64)
  {
    mmse_tmp = new gr_complex[64];
    for (int i = 0; i < d_nfft; ++i)
    {
      if (d_subcarrier_type_64[d_desubcarr_idx_64[i]] == 0)
      {
        noise_pwr_est += std::norm(in[i]); //null subcarriers : estimate noise; noise_pwr_est:����ģ��ƽ����
      }
      else
      {
        // including pilot and data subcarriers
        // ��ʹ��zero-forcing : d_channel_est[i] = d_long[d_desubcarr_idx[i]]/in[i];
        // ����ʹ��MMSE : 1/C(d_channel_est) = (|H_k|^2 + sigma_noise^2)/H*_k
        mmse_tmp[i] = std::conj(d_long_64[d_desubcarr_idx_64[i]]) * in[i]; //conj: complex conjugate�����
      }
      noise_pwr_est /= gr_complex(12.0, 0); //average noise of 12 null subcarriers
      // mmse normalization
      for (int i = 0; i < d_nfft; ++i)
      {
        if (d_subcarrier_type_64[d_desubcarr_idx_64[i]] != 0)
        { //not null subcarriers
          d_channel_est[i] = (std::norm(in[i]) + noise_pwr_est) / mmse_tmp[i];
        }
      }
    }
  }
  else if (d_nfft == 32)
  {
    mmse_tmp = new gr_complex[32];
    for (int i = 0; i < d_nfft; ++i)
    {
      if (d_subcarrier_type_32[d_desubcarr_idx_32[i]] == 0)
      {
        noise_pwr_est += std::norm(in[i]); //null subcarriers : estimate noise; noise_pwr_est:����ģ��ƽ����
      }
      else
      {
        // including pilot and data subcarriers
        // ��ʹ��zero-forcing : d_channel_est[i] = d_long[d_desubcarr_idx[i]]/in[i];
        // ����ʹ��MMSE : 1/C(d_channel_est) = (|H_k|^2 + sigma_noise^2)/H*_k
        mmse_tmp[i] = std::conj(d_long_32[d_desubcarr_idx_32[i]]) * in[i]; //conj: complex conjugate�����
      }
      noise_pwr_est /= gr_complex(6.0, 0);
      // mmse normalization
      for (int i = 0; i < d_nfft; ++i)
      {
        if (d_subcarrier_type_32[d_desubcarr_idx_32[i]] != 0)
        { //not null subcarriers
          d_channel_est[i] = (std::norm(in[i]) + noise_pwr_est) / mmse_tmp[i];
        }
      }
    }
  }
  else
  {
    mmse_tmp = new gr_complex[128];
    for (int i = 0; i < d_nfft; ++i)
    {
      if (d_subcarrier_type_128[d_desubcarr_idx_128[i]] == 0)
      {
        noise_pwr_est += std::norm(in[i]); //null subcarriers : estimate noise; noise_pwr_est:����ģ��ƽ����
      }
      else
      {
        // including pilot and data subcarriers
        // ��ʹ��zero-forcing : d_channel_est[i] = d_long[d_desubcarr_idx[i]]/in[i];
        // ����ʹ��MMSE : 1/C(d_channel_est) = (|H_k|^2 + sigma_noise^2)/H*_k
        mmse_tmp[i] = std::conj(d_long_128[d_desubcarr_idx_128[i]]) * in[i]; //conj: complex conjugate�����
      }
      noise_pwr_est /= gr_complex(24.0, 0);
      // mmse normalization
      for (int i = 0; i < d_nfft; ++i)
      {
        if (d_subcarrier_type_128[d_desubcarr_idx_128[i]] != 0)
        { //not null subcarriers
          d_channel_est[i] = (std::norm(in[i]) + noise_pwr_est) / mmse_tmp[i];
        }
      }
    }
  }
}

void symbol_parser_vc_impl::forecast(int noutput_items, gr_vector_int &ninput_items_required)
{
  /* <+forecast+> e.g. ninput_items_required[0] = noutput_items */
  ninput_items_required[0] = noutput_items * d_ndata / d_nfft; //how many input items
}

int symbol_parser_vc_impl::general_work(int noutput_items,
                                        gr_vector_int &ninput_items,
                                        gr_vector_const_void_star &input_items,
                                        gr_vector_void_star &output_items)
{
  const gr_complex *in = (const gr_complex *)input_items[0];
  gr_complex *out = (gr_complex *)output_items[0];
  int nin = std::min(ninput_items[0], noutput_items); //number of input items
  int nout = 0;                                       //current output index(output item length = d_ndata = 48)
  int ncon = 0;                                       //current consuming input index(input item length = d_nfft = 64)
  if (nin == 0)
  {
    consume_each(0);
    return 0;
  }

  std::vector<tag_t> tags;
  get_tags_in_range(tags, 0, nitems_read(0), nitems_read(0) + nin, d_preTag);
  if (!tags.empty())
  {
    if (tags[0].offset == nitems_read(0))
    {
      // found a long preamble, that is, the beginning of an ofdm symbol
      channel_estimation(&in[0]);
      d_pilot_idx = 0;
      add_item_tag(0, nitems_written(0) + nout / d_ndata, pmt::intern("hdr"), pmt::PMT_T, d_bname);
      ncon++;
    }
    else
    {
      // truncate to the end of current ofdm symbol
      nin = (int)(tags[0].offset - nitems_read(0));
    }
  }
  for (; ncon < nin; ++ncon)
  {
    // channel gain & carrier phase
    symbol_eq(&out[nout], &in[ncon * d_nfft], d_pilot_idx); //run equalizer
    add_item_tag(0, nitems_written(0) + nout / d_ndata, pmt::intern("symbol_idx"), pmt::from_long(d_pilot_idx), d_bname);
    d_pilot_idx++;
    d_pilot_idx %= 127;
    nout += d_ndata;
  }
  // Tell runtime system how many input items we consumed on
  // each input stream.
  consume_each(ncon);

  // Tell runtime system how many output items we produced.
  return nout / d_ndata;
}

} /* namespace wifi_ofdm */
} /* namespace gr */
