#ifndef INCLUDED_WIFI_OFDM_SYMBOL_MAPPER_BVC_H
#define INCLUDED_WIFI_OFDM_SYMBOL_MAPPER_BVC_H

#include <wifi_ofdm/api.h>
#include <gnuradio/tagged_stream_block.h>

namespace gr {
  namespace wifi_ofdm {

    /*!
     * \brief <+description of block+>
     * \ingroup wifi_ofdm
     *
     */
    class WIFI_OFDM_API symbol_mapper_bvc : virtual public gr::tagged_stream_block
    {
     public:
      typedef boost::shared_ptr<symbol_mapper_bvc> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of wifi_ofdm::symbol_mapper_bvc.
       *
       * To avoid accidental use of raw pointers, wifi_ofdm::symbol_mapper_bvc's
       * constructor is in a private implementation
       * class. wifi_ofdm::symbol_mapper_bvc::make is the public interface for
       * creating new instances.
       */
      static sptr make(const std::string& tagname);
    };

  } // namespace wifi_ofdm
} // namespace gr

#endif /* INCLUDED_WIFI_OFDM_SYMBOL_MAPPER_BVC_H */

