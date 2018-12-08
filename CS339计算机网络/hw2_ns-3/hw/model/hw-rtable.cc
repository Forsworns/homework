#include "hw-rtable.h"
#include "ns3/simulator.h"
#include <iomanip>
#include "ns3/log.h"

// 每个路由节点维护一个table，其中的entry为目标地址和路径的pair

namespace ns3
{

NS_LOG_COMPONENT_DEFINE("DsdvRoutingTable");

namespace hw
{
RoutingTableEntry::RoutingTableEntry(Ptr<NetDevice> dev,
                                     Ipv4Address dst,
                                     uint32_t seqNo,
                                     Ipv4InterfaceAddress iface,
                                     uint32_t hops,
                                     Ipv4Address nextHop,
                                     Time lifetime,
                                     Time SettlingTime,
                                     bool areChanged)
    : m_seqNo(seqNo),
      m_hops(hops),
      m_lifeTime(lifetime),
      m_iface(iface),
      m_flag(VALID),
      m_settlingTime(SettlingTime),
      m_entriesChanged(areChanged)
{
    m_ipv4Route = Create<Ipv4Route>();
    m_ipv4Route->SetDestination(dst);
    m_ipv4Route->SetGateway(nextHop);
    m_ipv4Route->SetSource(m_iface.GetLocal());
    m_ipv4Route->SetOutputDevice(dev);
}
RoutingTableEntry::~RoutingTableEntry()
{
}
RoutingTable::RoutingTable()
{
}

// look up the route for given address and pass the entry to the second parameter
bool RoutingTable::LookupRoute(Ipv4Address id,
                               RoutingTableEntry &rt)
{
    if (m_ipv4AddressEntry.empty())
    {
        return false;
    }
    std::map<Ipv4Address, RoutingTableEntry>::const_iterator i = m_ipv4AddressEntry.find(id);
    if (i == m_ipv4AddressEntry.end())
    {
        return false;
    }
    rt = i->second;
    return true;
}

// 废弃?
bool RoutingTable::LookupRoute(Ipv4Address id,
                               RoutingTableEntry &rt,
                               bool forRouteInput)
{
    if (m_ipv4AddressEntry.empty())
    {
        return false;
    }
    std::map<Ipv4Address, RoutingTableEntry>::const_iterator i = m_ipv4AddressEntry.find(id);
    if (i == m_ipv4AddressEntry.end())
    {
        return false;
    }
    if (forRouteInput == true && id == i->second.GetInterface().GetBroadcast())//输入为广播地址则取消
    {
        return false;
    }
    rt = i->second;
    return true;
}

bool RoutingTable::DeleteRoute(Ipv4Address dst)
{
    if (m_ipv4AddressEntry.erase(dst) != 0)
    {
        // NS_LOG_DEBUG("Route erased");
        return true;
    }
    return false;
}

uint32_t
RoutingTable::RoutingTableSize()
{
    return m_ipv4AddressEntry.size();
}

bool RoutingTable::AddRoute(RoutingTableEntry &rt)
{
    std::pair<std::map<Ipv4Address, RoutingTableEntry>::iterator, bool> result = m_ipv4AddressEntry.insert(std::make_pair(
        rt.GetDestination(), rt));
    return result.second;
}

bool RoutingTable::Update(RoutingTableEntry &rt)
{
    std::map<Ipv4Address, RoutingTableEntry>::iterator i = m_ipv4AddressEntry.find(rt.GetDestination());
    if (i == m_ipv4AddressEntry.end())
    {
        return false;
    }
    i->second = rt;
    return true;
}
// delete the route on selected interface 
void RoutingTable::DeleteAllRoutesFromInterface(Ipv4InterfaceAddress iface)
{
    if (m_ipv4AddressEntry.empty())
    {
        return;
    }
    for (std::map<Ipv4Address, RoutingTableEntry>::iterator i = m_ipv4AddressEntry.begin(); i != m_ipv4AddressEntry.end();)
    {
        if (i->second.GetInterface() == iface)
        {
            std::map<Ipv4Address, RoutingTableEntry>::iterator tmp = i;
            ++i;
            m_ipv4AddressEntry.erase(tmp);
        }
        else
        {
            ++i;
        }
    }
}

void RoutingTable::GetListOfAllRoutes(std::map<Ipv4Address, RoutingTableEntry> &allRoutes)
{
    for (std::map<Ipv4Address, RoutingTableEntry>::iterator i = m_ipv4AddressEntry.begin(); i != m_ipv4AddressEntry.end(); ++i)
    {
        if (i->second.GetDestination() != Ipv4Address("127.0.0.1") && i->second.GetFlag() == VALID)
        {
            allRoutes.insert(
                std::make_pair(i->first, i->second));
        }
    }
}

// look up which destination can reach via the given nextHop Address
void RoutingTable::GetListOfDestinationWithNextHop(Ipv4Address nextHop,
                                                   std::map<Ipv4Address, RoutingTableEntry> &unreachable)
{
    unreachable.clear();
    for (std::map<Ipv4Address, RoutingTableEntry>::const_iterator i = m_ipv4AddressEntry.begin(); i != m_ipv4AddressEntry.end(); ++i)
    {
        if (i->second.GetNextHop() == nextHop)
        {
            unreachable.insert(std::make_pair(i->first, i->second));
        }
    }
}

void RoutingTableEntry::Print(Ptr<OutputStreamWrapper> stream) const
{
    *stream->GetStream() << std::setiosflags(std::ios::fixed) << m_ipv4Route->GetDestination() << "\t\t" << m_ipv4Route->GetGateway() << "\t\t"
                         << m_iface.GetLocal() << "\t\t" << std::setiosflags(std::ios::left)
                         << std::setw(10) << m_hops << "\t" << std::setw(10) << m_seqNo << "\t"
                         << std::setprecision(3) << (Simulator::Now() - m_lifeTime).GetSeconds()
                         << "s\t\t" << m_settlingTime.GetSeconds() << "s\n";
}

// delte all outdated entries if lifetime is expired
void RoutingTable::Purge(std::map<Ipv4Address, RoutingTableEntry> &removedAddresses)
{
    if (m_ipv4AddressEntry.empty())
    {
        return;
    }
    for (std::map<Ipv4Address, RoutingTableEntry>::iterator i = m_ipv4AddressEntry.begin(); i != m_ipv4AddressEntry.end();)
    {
        std::map<Ipv4Address, RoutingTableEntry>::iterator itmp = i;
        if (i->second.GetLifeTime() > m_holddownTime && (i->second.GetHop() > 0))
        {
            for (std::map<Ipv4Address, RoutingTableEntry>::iterator j = m_ipv4AddressEntry.begin(); j != m_ipv4AddressEntry.end();)
            {
                if ((j->second.GetNextHop() == i->second.GetDestination()) && (i->second.GetHop() != j->second.GetHop()))
                {
                    std::map<Ipv4Address, RoutingTableEntry>::iterator jtmp = j;
                    removedAddresses.insert(std::make_pair(j->first, j->second));
                    ++j;
                    m_ipv4AddressEntry.erase(jtmp);
                }
                else
                {
                    ++j;
                }
            }
            removedAddresses.insert(std::make_pair(i->first, i->second));
            ++i;
            m_ipv4AddressEntry.erase(itmp);
        }
        /*          else if (i->second.GetLifeTime() > m_holddownTime)
          {
          ++i;
          itmp->second.SetFlag(INVALID);
          }*/
        else
        {
            ++i;
        }
    }
    return;
}

void RoutingTable::Print(Ptr<OutputStreamWrapper> stream) const
{
    *stream->GetStream() << "\nDSDV Routing table\n"
                         << "Destination\t\tGateway\t\tInterface\t\tHopCount\t\tSeqNum\t\tLifeTime\t\tSettlingTime\n";
    for (std::map<Ipv4Address, RoutingTableEntry>::const_iterator i = m_ipv4AddressEntry.begin(); i != m_ipv4AddressEntry.end(); ++i)
    {
        i->second.Print(stream);
    }
    *stream->GetStream() << "\n";
}

bool RoutingTable::AddIpv4Event(Ipv4Address address,
                                EventId id)
{
    std::pair<std::map<Ipv4Address, EventId>::iterator, bool> result = m_ipv4Events.insert(std::make_pair(address, id));
    return result.second;
}

bool RoutingTable::AnyRunningEvent(Ipv4Address address)
{
    EventId event;
    std::map<Ipv4Address, EventId>::const_iterator i = m_ipv4Events.find(address);
    if (m_ipv4Events.empty())
    {
        return false;
    }
    if (i == m_ipv4Events.end())
    {
        return false;
    }
    event = i->second;
    if (event.IsRunning())
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool RoutingTable::ForceDeleteIpv4Event(Ipv4Address address)
{
    EventId event;
    std::map<Ipv4Address, EventId>::const_iterator i = m_ipv4Events.find(address);
    if (m_ipv4Events.empty() || i == m_ipv4Events.end())
    {
        return false;
    }
    event = i->second;
    Simulator::Cancel(event);
    m_ipv4Events.erase(address);
    return true;
}

bool RoutingTable::DeleteIpv4Event(Ipv4Address address)
{
    EventId event;
    std::map<Ipv4Address, EventId>::const_iterator i = m_ipv4Events.find(address);
    if (m_ipv4Events.empty() || i == m_ipv4Events.end())
    {
        return false;
    }
    event = i->second;
    if (event.IsRunning())
    {
        return false;
    }
    if (event.IsExpired())
    {
        event.Cancel();
        m_ipv4Events.erase(address);
        return true;
    }
    else
    {
        m_ipv4Events.erase(address);
        return true;
    }
}

EventId
RoutingTable::GetEventId(Ipv4Address address)
{
    std::map<Ipv4Address, EventId>::const_iterator i = m_ipv4Events.find(address);
    if (m_ipv4Events.empty() || i == m_ipv4Events.end())
    {
        return EventId();
    }
    else
    {
        return i->second;
    }
}
} // namespace hw
}