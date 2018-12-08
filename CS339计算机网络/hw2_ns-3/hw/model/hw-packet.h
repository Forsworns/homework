#ifndef HW_PACKET_H
#define HW_PACKET_H

#include <iostream>
#include "ns3/header.h"
#include "ns3/ipv4-address.h"
#include "ns3/nstime.h"

namespace ns3
{
namespace hw
{
class HwHeader : public Header
{
  public:
    HwHeader(Ipv4Address dst = Ipv4Address(), uint32_t hopcount = 0, uint32_t dstSeqNo = 0);
    virtual ~HwHeader();

    static TypeId GetTypeId(void);
    virtual TypeId GetInstanceTypeId(void) const;
    virtual uint32_t GetSerializedSize() const;
    virtual void Serialize(Buffer::Iterator start) const;
    virtual uint32_t Deserialize(Buffer::Iterator start);
    virtual void Print(std::ostream &os) const;

    // set and get function for privates
    void SetDst(Ipv4Address destination)
    {
        m_dst = destination;
    }
    Ipv4Address GetDst() const
    {
        return m_dst;
    }

    void SetHopCount(uint32_t hopCount)
    {
        m_hopCount = hopCount;
    }
    uint32_t GetHopCount() const
    {
        return m_hopCount;
    }

    void SetDstSeqno(uint32_t sequenceNumber)
    {
        m_dstSeqNo = sequenceNumber;
    }
    uint32_t GetDstSeqno() const
    {
        return m_dstSeqNo;
    }

  private:
    Ipv4Address m_dst; // destination address
    uint32_t m_hopCount;    // hop counts ecummulated
    uint32_t m_dstSeqNo; // destination sequence number(priority/performance,etc)
};
static inline std::ostream &operator<<(std::ostream &os, const HwHeader &packet)
{
    packet.Print(os);
    return os;
}
} // namespace hw
} // namespace ns3

#endif