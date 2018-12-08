#ifndef HW_PACKETQUEUE_H
#define HW_PACKETQUEUE_H

#include <vector>
#include "ns3/ipv4-routing-protocol.h"
#include "ns3/simulator.h"

namespace ns3
{
namespace hw
{
class QueueEntry
{
  public:
    typedef Ipv4RoutingProtocol::UnicastForwardCallback UnicastForwardCallback;
    typedef Ipv4RoutingProtocol::ErrorCallback ErrorCallback;
    QueueEntry(Ptr<const Packet> pa = 0, Ipv4Header const &h = Ipv4Header(),
               UnicastForwardCallback ucb = UnicastForwardCallback(),
               ErrorCallback ecb = ErrorCallback())
        : m_packet(pa),
          m_header(h),
          m_ucb(ucb),
          m_ecb(ecb),
          m_expire(Seconds(0))
    {
    }

    bool operator==(QueueEntry const &o) const
    {
        return ((m_packet == o.m_packet) && (m_header.GetDestination() == o.m_header.GetDestination()) && (m_expire == o.m_expire));
    }

    // set and get functions for privates
    UnicastForwardCallback GetUnicastForwardCallback() const
    {
        return m_ucb;
    }
    void SetUnicastForwardCallback(UnicastForwardCallback ucb)
    {
        m_ucb = ucb;
    }
    
    ErrorCallback GetErrorCallback() const
    {
        return m_ecb;
    }
    
    void SetErrorCallback(ErrorCallback ecb)
    {
        m_ecb = ecb;
    }
    
    Ptr<const Packet> GetPacket() const
    {
        return m_packet;
    }
    void SetPacket(Ptr<const Packet> p)
    {
        m_packet = p;
    }
    
    Ipv4Header GetIpv4Header() const
    {
        return m_header;
    }
    void SetIpv4Header(Ipv4Header h)
    {
        m_header = h;
    }

    void SetExpireTime(Time exp)
    {
        m_expire = exp + Simulator::Now();
    }
    Time GetExpireTime() const
    {
        return m_expire - Simulator::Now();
    }

  private:
    Ptr<const Packet> m_packet; // data packet
    Ipv4Header m_header;    // ip header
    UnicastForwardCallback m_ucb;   // unicast forword callback
    ErrorCallback m_ecb;    // error callback
    Time m_expire;  // expire time for queue entry
};

// when a route is not availabe, use this queue to buffer packets, 
// it's based on FIFO strategy
class PacketQueue
{
  public:
    PacketQueue()
    {
    }
    bool Enqueue(QueueEntry &entry);
    bool Dequeue(Ipv4Address dst, QueueEntry &entry);
    void DropPacketWithDst(Ipv4Address dst); // drop the packet with specific address
    bool Find(Ipv4Address dst);
    uint32_t GetCountForPacketsWithDst(Ipv4Address dst);
    uint32_t GetSize();

    // Fields
    uint32_t GetMaxQueueLen() const
    {
        return m_maxLen;
    }
    void SetMaxQueueLen(uint32_t len)
    {
        m_maxLen = len;
    }
    uint32_t GetMaxPacketsPerDst() const
    {
        return m_maxLenPerDst;
    }
    void SetMaxPacketsPerDst(uint32_t len)
    {
        m_maxLenPerDst = len;
    }
    Time GetQueueTimeout() const
    {
        return m_queueTimeout;
    }
    void SetQueueTimeout(Time t)
    {
        m_queueTimeout = t;
    }

  private:
    std::vector<QueueEntry> m_queue;
    void Purge();
    void Drop(QueueEntry en, std::string reason); // notify that a packet has been dropped out from the head of queue
    uint32_t m_maxLen; // the maximum number of packets that we allow a protocol to buffer  
    // according to TA,'s assignment this is one of the parameter fixed in fact 
    uint32_t m_maxLenPerDst; // the maximum number of packets that we allow per destination to buffer
    Time m_queueTimeout; // the maximum period that a routing protocol is allowed to buffer a packet for
    static bool IsEqual(QueueEntry en, const Ipv4Address dst)
    {
        return (en.GetIpv4Header().GetDestination() == dst);
    }
};

} // namespace hw
} // namespace ns3

#endif