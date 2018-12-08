
#ifndef INCLUDED_WIFI_OFDM_HEADER_PREFIXER_H
#define INCLUDED_WIFI_OFDM_HEADER_PREFIXER_H

#include <wifi_ofdm/api.h>
#include <gnuradio/block.h>

namespace gr {
  namespace wifi_ofdm {

    /*!
     * \brief <+description+>
     *
     */
    class WIFI_OFDM_API header_prefixer : virtual public block
    {
    public:
      typedef boost::shared_ptr<header_prefixer> sptr;
      static sptr make(int FFTsize);
    };

  } // namespace wifi_ofdm
} // namespace gr

#endif /* INCLUDED_WIFI_OFDM_HEADER_PREFIXER_H */

