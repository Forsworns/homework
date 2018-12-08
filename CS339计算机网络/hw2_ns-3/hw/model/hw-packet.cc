#include "hw-packet.h"
#include "ns3/address-utils.h"
#include "ns3/packet.h"

namespace ns3
{
namespace hw
{

NS_OBJECT_ENSURE_REGISTERED(HwHeader);

// construct a header with destination IP, the count of hop and destination sequence number 
HwHeader::HwHeader(Ipv4Address dst, uint32_t hopCount, uint32_t dstSeqNo)
    : m_dst(dst),
      m_hopCount(hopCount),
      m_dstSeqNo(dstSeqNo)
{
}

HwHeader::~HwHeader()
{
}

// build a typeId to return to the get funciton
TypeId HwHeader::GetTypeId(void)
{
    static TypeId tid = TypeId("ns3::hw::HwHeader")
                            .SetParent<Header>()
                            .SetGroupName("Hw")
                            .AddConstructor<HwHeader>();
    return tid;
}

TypeId HwHeader::GetInstanceTypeId() const
{
    return GetTypeId();
}

uint32_t HwHeader::GetSerializedSize() const
{
    return 12;
}

// i is the header should be written, serialize the header for future use in Packet::AddHeader
void HwHeader::Serialize(Buffer::Iterator i) const
{
    WriteTo(i, m_dst);
    i.WriteHtonU32(m_hopCount);
    i.WriteHtonU32(m_dstSeqNo);
}

// generate the information from a serialized header struct to rebuild a struct
uint32_t HwHeader::Deserialize(Buffer::Iterator start)
{
    Buffer::Iterator i = start;

    // header (dst-hopCount-seqNo)
    ReadFrom(i, m_dst);
    m_hopCount = i.ReadNtohU32();
    m_dstSeqNo = i.ReadNtohU32();

    uint32_t dist = i.GetDistanceFrom(start);
    NS_ASSERT(dist == GetSerializedSize());
    return dist;
}

void HwHeader::Print(std::ostream &os) const
{
    os << "DestinationIpv4: " << m_dst
       << " Hopcount: " << m_hopCount
       << " SequenceNumber: " << m_dstSeqNo;
}
} // namespace dsdv
} // namespace ns3