// help to manage the creation of HW routing instance and insert it on nodes as a sublayer between transport and IP layers
#ifndef HW_HELPER_H
#define HW_HELPER_H

#include "ns3/object-factory.h"
#include "ns3/node.h"
#include "ns3/node-container.h"
#include "ns3/ipv4-routing-helper.h"

namespace ns3
{
class HwHelper : public Ipv4RoutingHelper
{
  public:
    HwHelper();
    HwHelper *Copy(void) const;
    virtual Ptr<Ipv4RoutingProtocol> Create(Ptr<Node> node) const;
    void Set(std::string name, const AttributeValue &value);
  private:
    ObjectFactory m_agentFactory;
};
} // namespace ns3

#endif