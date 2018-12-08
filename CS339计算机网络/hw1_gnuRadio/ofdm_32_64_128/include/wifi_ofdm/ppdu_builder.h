
#ifndef INCLUDED_WIFI_OFDM_PPDU_BUILDER_H
#define INCLUDED_WIFI_OFDM_PPDU_BUILDER_H

#include <wifi_ofdm/api.h>
#include <gnuradio/block.h>

namespace gr {
  namespace wifi_ofdm {

    /*!
     * \brief <+description+>
     *
     */
    class WIFI_OFDM_API ppdu_builder : virtual public block
    {
    public:
      typedef boost::shared_ptr<ppdu_builder> sptr;
      static sptr make(int seed,int rate,int FFTsize);
      virtual void update_rate(int rate)=0;
    };

  } // namespace wifi_ofdm
} // namespace gr

#endif /* INCLUDED_WIFI_OFDM_PPDU_BUILDER_H */

