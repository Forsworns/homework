
#ifndef INCLUDED_WIFI_OFDM_COARSE_CFO_FIXER_CC_H
#define INCLUDED_WIFI_OFDM_COARSE_CFO_FIXER_CC_H

#include <wifi_ofdm/api.h>
#include <gnuradio/block.h>

namespace gr {
  namespace wifi_ofdm {

    /*!
     * \brief <+description of block+>
     * \ingroup wifi_ofdm
     *
     */
    class WIFI_OFDM_API coarse_cfo_fixer_cc : virtual public gr::block
    {
     public:
      typedef boost::shared_ptr<coarse_cfo_fixer_cc> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of wifi_ofdm::coarse_cfo_fixer_cc.
       *
       * To avoid accidental use of raw pointers, wifi_ofdm::coarse_cfo_fixer_cc's
       * constructor is in a private implementation
       * class. wifi_ofdm::coarse_cfo_fixer_cc::make is the public interface for
       * creating new instances.
       */
      static sptr make(int delay);
    };

  } // namespace wifi_ofdm
} // namespace gr

#endif /* INCLUDED_WIFI_OFDM_COARSE_CFO_FIXER_CC_H */

