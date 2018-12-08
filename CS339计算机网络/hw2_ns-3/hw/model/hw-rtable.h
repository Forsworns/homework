#ifndef HW_RTABLE_H
#define HW_RTABLE_H

#include <cassert>
#include <map>
#include <sys/types.h>
#include "ns3/ipv4.h"
#include "ns3/ipv4-route.h"
#include "ns3/timer.h"
#include "ns3/net-device.h"
#include "ns3/output-stream-wrapper.h"

namespace ns3
{
namespace hw
{
enum RouteFlags
{
    VALID = 0,   // !< VALID
    INVALID = 1, // !< INVALID
};

class RoutingTableEntry
{
  public:
    RoutingTableEntry(Ptr<NetDevice> dev = 0, Ipv4Address dst = Ipv4Address(), uint32_t seqNo = 0,
                      Ipv4InterfaceAddress iface = Ipv4InterfaceAddress(), uint32_t hops = 0, Ipv4Address nextHop = Ipv4Address(),
                      Time lifetime = Simulator::Now(), Time SettlingTime = Simulator::Now(), bool changedEntries = false);

    ~RoutingTableEntry();

    // get set function for private parameter
    Ipv4Address GetDestination() const
    {
        return m_ipv4Route->GetDestination();
    }

    Ptr<Ipv4Route> GetRoute() const
    {
        return m_ipv4Route;
    }
    
    void SetRoute(Ptr<Ipv4Route> route)
    {
        m_ipv4Route = route;
    }

    void SetNextHop(Ipv4Address nextHop)
    {
        m_ipv4Route->SetGateway(nextHop);
    }

    Ipv4Address GetNextHop() const
    {
        return m_ipv4Route->GetGateway();
    }

    void SetOutputDevice(Ptr<NetDevice> device)
    {
        m_ipv4Route->SetOutputDevice(device);
    }

    Ptr<NetDevice> GetOutputDevice() const
    {
        return m_ipv4Route->GetOutputDevice();
    }

    Ipv4InterfaceAddress GetInterface() const
    {
        return m_iface;
    }

    void SetInterface(Ipv4InterfaceAddress iface)
    {
        m_iface = iface;
    }

    void SetSeqNo(uint32_t sequenceNumber)
    {
        m_seqNo = sequenceNumber;
    }

    uint32_t GetSeqNo() const
    {
        return m_seqNo;
    }

    void SetHop(uint32_t hopCount)
    {
        m_hops = hopCount;
    }

    uint32_t GetHop() const
    {
        return m_hops;
    }

    void SetLifeTime(Time lifeTime)
    {
        m_lifeTime = lifeTime;
    }

    Time GetLifeTime() const
    {
        return (Simulator::Now() - m_lifeTime);
    }

    void SetSettlingTime(Time settlingTime)
    {
        m_settlingTime = settlingTime;
    }

    Time GetSettlingTime() const
    {
        return (m_settlingTime);
    }

    void SetFlag(RouteFlags flag)
    {
        m_flag = flag;
    }

    RouteFlags GetFlag() const
    {
        return m_flag;
    }

    void SetEntriesChanged(bool entriesChanged)
    {
        m_entriesChanged = entriesChanged;
    }

    bool GetEntriesChanged() const
    {
        return m_entriesChanged;
    }

    bool operator==(Ipv4Address const destination) const
    {
        return (m_ipv4Route->GetDestination() == destination);
    }

    void Print(Ptr<OutputStreamWrapper> stream) const;

  private:
    // Fields
    uint32_t m_seqNo;
    uint32_t m_hops;
    Time m_lifeTime;
    Ptr<Ipv4Route> m_ipv4Route;
    Ipv4InterfaceAddress m_iface;
    RouteFlags m_flag;
    Time m_settlingTime;
    uint32_t m_entriesChanged;
};


class RoutingTable
{
  public:
    RoutingTable();
    bool AddRoute(RoutingTableEntry &r);
    bool DeleteRoute(Ipv4Address dst);
    bool LookupRoute(Ipv4Address dst, RoutingTableEntry &rt);
    bool LookupRoute(Ipv4Address id, RoutingTableEntry &rt, bool forRouteInput);
    bool Update(RoutingTableEntry &rt);
    void GetListOfDestinationWithNextHop(Ipv4Address nxtHp, std::map<Ipv4Address, RoutingTableEntry> &dstList);
    void GetListOfAllRoutes(std::map<Ipv4Address, RoutingTableEntry> &allRoutes);
    void DeleteAllRoutesFromInterface(Ipv4InterfaceAddress iface);
    void Clear()
    {
        m_ipv4AddressEntry.clear();
    }
    void Purge(std::map<Ipv4Address, RoutingTableEntry> &removedAddresses);
    void Print(Ptr<OutputStreamWrapper> stream) const;
    uint32_t RoutingTableSize();
    bool AddIpv4Event(Ipv4Address address, EventId id);
    bool DeleteIpv4Event(Ipv4Address address);
    bool AnyRunningEvent(Ipv4Address address);
    bool ForceDeleteIpv4Event(Ipv4Address address);
    EventId GetEventId(Ipv4Address address);

    Time Getholddowntime() const
    {
        return m_holddownTime;
    }
    void Setholddowntime(Time t)
    {
        m_holddownTime = t;
    }

  private:
    // Fields
    std::map<Ipv4Address, RoutingTableEntry> m_ipv4AddressEntry;
    std::map<Ipv4Address, EventId> m_ipv4Events;
    Time m_holddownTime;
};
} // namespace hw
} // namespace ns3

#endif