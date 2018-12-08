#include "hw-packet-queue.h"
#include <algorithm>
#include <functional>
#include "ns3/ipv4-route.h"
#include "ns3/socket.h"
#include "ns3/log.h"

// 每个节点维护一个包的队列，作缓存和丢弃

namespace ns3
{

NS_LOG_COMPONENT_DEFINE("HwPacketQueue");

namespace hw
{
uint32_t
PacketQueue::GetSize()
{
    Purge();
    return m_queue.size();
}

bool PacketQueue::Enqueue(QueueEntry &entry)
{
    NS_LOG_FUNCTION("Enqueing packet destined for" << entry.GetIpv4Header().GetDestination());
    Purge();
    uint32_t numPacketswithdst;
    for (std::vector<QueueEntry>::const_iterator i = m_queue.begin(); i != m_queue.end(); ++i)
    {
        if ((i->GetPacket()->GetUid() == entry.GetPacket()->GetUid()) && (i->GetIpv4Header().GetDestination() == entry.GetIpv4Header().GetDestination()))
        {
            return false;
        }
    }
    numPacketswithdst = GetCountForPacketsWithDst(entry.GetIpv4Header().GetDestination());
    NS_LOG_DEBUG("Number of packets with this destination: " << numPacketswithdst);
    if (numPacketswithdst >= m_maxLenPerDst || m_queue.size() >= m_maxLen)
    {
        NS_LOG_DEBUG("Max packets reached for this destination. Not queuing any further packets");
        return false;
    }
    else
    {
        // NS_LOG_DEBUG("Packet size while enqueing "<<entry.GetPacket()->GetSize());
        entry.SetExpireTime(m_queueTimeout);
        m_queue.push_back(entry);
        return true;
    }
}

void PacketQueue::DropPacketWithDst(Ipv4Address dst)
{
    NS_LOG_FUNCTION("Dropping packet to " << dst);
    Purge();
    for (std::vector<QueueEntry>::iterator i = m_queue.begin(); i != m_queue.end(); ++i)
    {
        if (IsEqual(*i, dst))
        {
            Drop(*i, "DropPacketWithDst ");
        }
    }
    m_queue.erase(std::remove_if(m_queue.begin(), m_queue.end(),
                                 std::bind2nd(std::ptr_fun(PacketQueue::IsEqual), dst)),
                  m_queue.end());
}

bool PacketQueue::Dequeue(Ipv4Address dst, QueueEntry &entry)
{
    NS_LOG_FUNCTION("Dequeueing packet destined for" << dst);
    Purge();
    for (std::vector<QueueEntry>::iterator i = m_queue.begin(); i != m_queue.end(); ++i)
    {
        if (i->GetIpv4Header().GetDestination() == dst)
        {
            entry = *i;
            m_queue.erase(i);
            return true;
        }
    }
    return false;
}

bool PacketQueue::Find(Ipv4Address dst)
{
    for (std::vector<QueueEntry>::const_iterator i = m_queue.begin(); i != m_queue.end(); ++i)
    {
        if (i->GetIpv4Header().GetDestination() == dst)
        {
            NS_LOG_DEBUG("Find");
            return true;
        }
    }
    return false;
}

uint32_t
PacketQueue::GetCountForPacketsWithDst(Ipv4Address dst)
{
    uint32_t count = 0;
    for (std::vector<QueueEntry>::const_iterator i = m_queue.begin(); i != m_queue.end(); ++i)
    {
        if (i->GetIpv4Header().GetDestination() == dst)
        {
            count++;
        }
    }
    return count;
}

struct IsExpired
{
    bool
    operator()(QueueEntry const &e) const
    {
        // NS_LOG_DEBUG("Expire time for packet in req queue: "<<e.GetExpireTime ());
        return (e.GetExpireTime() < Seconds(0));
    }
};

void PacketQueue::Purge()
{
    // NS_LOG_DEBUG("Purging Queue");
    IsExpired pred;
    for (std::vector<QueueEntry>::iterator i = m_queue.begin(); i != m_queue.end(); ++i)
    {
        if (pred(*i))
        {
            NS_LOG_DEBUG("Dropping outdated Packets");
            Drop(*i, "Drop outdated packet ");
        }
    }
    m_queue.erase(std::remove_if(m_queue.begin(), m_queue.end(), pred),
                  m_queue.end());
}

void PacketQueue::Drop(QueueEntry en, std::string reason)
{
    NS_LOG_LOGIC(reason << en.GetPacket()->GetUid() << " " << en.GetIpv4Header().GetDestination());
    // en.GetErrorCallback () (en.GetPacket (), en.GetIpv4Header (),
    //   Socket::ERROR_NOROUTETOHOST);
    return;
}

} // namespace hw
} // namespace ns3