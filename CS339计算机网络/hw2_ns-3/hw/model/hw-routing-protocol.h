#ifndef HW_ROUTING
#define HW_ROUTING

#include "hw-rtable.h"
#include "hw-packet-queue.h"
#include "hw-packet.h"

#include "ns3/node.h"
#include "ns3/random-variable-stream.h"
#include "ns3/ipv4-routing-protocol.h"
#include "ns3/ipv4-interface.h"
#include "ns3/ipv4-l3-protocol.h"
#include "ns3/output-stream-wrapper.h"

namespace ns3
{

namespace hw
{
class RoutingProtocol : public Ipv4RoutingProtocol
{
public:
  static TypeId GetTypeId();
  static const uint32_t HW_PORT;

  RoutingProtocol();
  virtual ~RoutingProtocol();
  virtual void DoDispose();

  // From Ipv4RoutingProtocol
  Ptr<Ipv4Route> RouteOutput(Ptr<Packet> p, const Ipv4Header &header, Ptr<NetDevice> oif, Socket::SocketErrno &sockerr);
  bool RouteInput(Ptr<const Packet> p, const Ipv4Header &header, Ptr<const NetDevice> idev, UnicastForwardCallback ucb,
                  MulticastForwardCallback mcb, LocalDeliverCallback lcb, ErrorCallback ecb);
  virtual void PrintRoutingTable(Ptr<OutputStreamWrapper> stream, Time::Unit unit = Time::S) const;
  virtual void NotifyInterfaceUp(uint32_t interface);
  virtual void NotifyInterfaceDown(uint32_t interface);
  virtual void NotifyAddAddress(uint32_t interface, Ipv4InterfaceAddress address);
  virtual void NotifyRemoveAddress(uint32_t interface, Ipv4InterfaceAddress address);
  virtual void SetIpv4(Ptr<Ipv4> ipv4);

  // Methods to handle protocol parameters
  void SetEnableBufferFlag(bool f);
  bool GetEnableBufferFlag() const;
  void SetWSTFlag(bool f);
  bool GetWSTFlag() const;
  void SetEnableRAFlag(bool f);
  bool GetEnableRAFlag() const;

  int64_t AssignStreams(int64_t stream);

private:
  // Protocol parameters.
  uint32_t Holdtimes;
  Time m_periodicUpdateInterval;
  Time m_settlingTime;
  Ipv4Address m_mainAddress;
  Ptr<Ipv4> m_ipv4;
  std::map<Ptr<Socket>, Ipv4InterfaceAddress> m_socketAddresses;
  Ptr<NetDevice> m_lo;
  RoutingTable m_routingTable;
  RoutingTable m_advRoutingTable;
  uint32_t m_maxQueueLen;
  uint32_t m_maxQueuedPacketsPerDst;
  Time m_maxQueueTime;
  PacketQueue m_queue;
  bool EnableBuffering;
  bool EnableWST;
  double m_weightedFactor;
  bool EnableRouteAggregation;
  Time m_routeAggregationTime;
  UnicastForwardCallback m_scb;
  ErrorCallback m_ecb;
  void
  Start();
  void
  DeferredRouteOutput(Ptr<const Packet> p, const Ipv4Header &header, UnicastForwardCallback ucb, ErrorCallback ecb);
  void
  LookForQueuedPackets(void);
  void
  SendPacketFromQueue(Ipv4Address dst, Ptr<Ipv4Route> route);
  Ptr<Socket>
  FindSocketWithInterfaceAddress(Ipv4InterfaceAddress iface) const;

  // Receive hw control packets
  void RecvHw(Ptr<Socket> socket);
  void Send(Ptr<Ipv4Route>, Ptr<const Packet>, const Ipv4Header &);
  Ptr<Ipv4Route> LoopbackRoute(const Ipv4Header &header, Ptr<NetDevice> oif) const;
  Time GetSettlingTime(Ipv4Address dst);
  void SendTriggeredUpdate();
  void SendPeriodicUpdate();
  void MergeTriggerPeriodicUpdates();
  void Drop(Ptr<const Packet>, const Ipv4Header &, Socket::SocketErrno);
  Timer m_periodicUpdateTimer;
  Timer m_triggeredExpireTimer;

  Ptr<UniformRandomVariable> m_uniformRandomVariable;
};

} // namespace hw

} // namespace ns3

#endif