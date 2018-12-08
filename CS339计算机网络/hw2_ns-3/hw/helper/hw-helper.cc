#include "hw-helper.h"
#include "ns3/hw-routing-protocol.h"
#include "ns3/node-list.h"
#include "ns3/names.h"
#include "ns3/ptr.h"
#include "ns3/ipv4-list-routing.h"

namespace ns3
{
HwHelper::HwHelper():Ipv4RoutingHelper()
{
    m_agentFactory.SetTypeId("ns3::hw::RoutingProtocol");
}

HwHelper *HwHelper::Copy(void) const
{
    return new HwHelper(*this);
}

// main part of this helper, bind Hw routing to each protocol
Ptr<Ipv4RoutingProtocol> HwHelper::Create(Ptr<Node> node) const
{
    Ptr<hw::RoutingProtocol> agent = m_agentFactory.Create<hw::RoutingProtocol>();
    node->AggregateObject(agent);
    return agent;
}

void HwHelper::Set(std::string name, const AttributeValue &value)
{
    m_agentFactory.Set(name, value);
}
} // namespace ns3