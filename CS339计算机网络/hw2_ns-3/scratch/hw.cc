// ./waf --run "scratch/hw --verbose=true"
// ./waf --run <program> --command-template="gdb --args %s <args>"
#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <utility>
#include <map>
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/applications-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/internet-module.h"
#include "ns3/flow-monitor-helper.h"
#include "ns3/ipv4-flow-classifier.h"
#include "ns3/ipv4-global-routing-helper.h"
#include "ns3/aodv-module.h"
#include "ns3/olsr-module.h"
#include "ns3/dsdv-module.h"
#include "ns3/dsr-module.h"
// #include "ns3/hw-module.h"
// #include "ns3/stats-module.h" // to use gnuplot directly

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("NetworkHW");
// some macro
/*
// these microes are only for pre stage, couldn't be used in the for loop while compiling
#define CONNECT(text1, text2, number1, number2) text1##number1##text2##number2
#define NTRC(n1, n2) NodeContainer CONNECT(nt, r, n1, n2) = NodeContainer(terminals.Get(n1), routers.Get(n2));
#define NRRC(n1, n2) NodeContainer CONNECT(nr, r, n1, n2) = NodeContainer(routers.Get(n1), routers.Get(n2));
#define DTRC(n1, n2) NetDeviceContainer CONNECT(dt, r, n1, n2) = pointToPointHelper.Install(CONNECT(nt, r, n1, n2));
#define DRRC(n1, n2) NetDeviceContainer CONNECT(dr, r, n1, n2) = pointToPointHelper.Install(CONNECT(nr, r, n1, n2));
#define ITRC(address, n1, n2)          \
    ipv4.SetBase(address, subnetMask); \
    Ipv4InterfaceContainer CONNECT(it, r, n1, n2) = ipv4.Assign(CONNECT(dt, r, n1, n2));
#define IRRC(address, n1, n2)          \
    ipv4.SetBase(address, subnetMask); \
    Ipv4InterfaceContainer CONNECT(ir, r, n1, n2) = ipv4.Assign(CONNECT(dr, r, n1, n2));
*/

// some constant
const int routerNum = 30;
const int terminalNum = 50;
const float eachTestTime = 0.1f;
const int udpPacketSize = 210;
const std::string udpPacketInterval = "3.75ms"; // should calculate the packetRate = packetSize / packetInterval
const std::string udpPacketDataRate = "448kb/s";
const std::string p2pLinkDataRate = "10Mbps";
const std::string p2pLinkDelay = "2ms";
const std::string routerThroughput = "3kpps";
const uint32_t routerMemoryLength = 2000000;
const char subnetMask[] = "255.255.255.0";
const double errorRate = 0.00001;
const uint16_t port = 9; // Discard port (RFC 863)

// some file name
const std::string fileHeader = "output/hw_";
const std::string queueFile = "queue.data";
const std::string errorModelFile = "errorModel.data";
const std::string latencyFile = "latency.data";
const std::string throughputFile = "throughput.data";
const std::string routerTableFile = "routerTable.txt";
const std::string onoffApplicationFile = "onoffApplication.txt";
const std::string terminalsAddressFile = "terminalAddress.txt";
const std::string routersAddressFile = "routerAddress.txt";
const std::string trFile = "networkTrace.tr";

typedef std::pair<int, int> nodePair;
// topology
int trList[50] = {1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 8, 8, 8, 9, 9, 9, 10, 11, 11, 12, 13, 14, 14, 15, 16, 16, 16, 17,
                  17, 17, 18, 19, 20, 21, 21, 21, 21, 22, 24, 25, 26, 27, 28, 29, 29, 29, 30, 30, 30};
int rrList1[] = {1, 1, 1, 2, 3, 4, 5, 5, 6, 7, 7, 8, 9, 9, 10, 10, 11, 11, 11, 12, 13, 14, 14, 16, 17, 17, 18, 18,
                 19, 19, 20, 20, 21, 22, 22, 23, 23, 24, 24, 25, 26, 26, 27, 27, 28, 29};
int rrList2[] = {2, 4, 5, 3, 4, 6, 6, 9, 7, 8, 13, 10, 11, 12, 13, 16, 12, 14, 22, 13, 20, 15, 17, 20, 18, 19, 19,
                 23, 20, 23, 21, 26, 29, 23, 25, 24, 25, 25, 28, 27, 28, 29, 28, 30, 29, 30};
enum RoutingType
{
    GLOBAL,
    AODV,
    HW
};

class RoutingExperiment
{
public:
    RoutingExperiment(int protocol,bool verbose);
    void Run(int totalTestTime);
    void RxDrop(Ptr<const Packet> p);
    inline std::string PrintReceivedPacket(Ptr<Socket> socket, Address senderAddress);
private:
    Ptr<Socket> SetupPacketReceive(Ipv4Address addr, Ptr<Node> node);
    void ReceivePacket(Ptr<Socket> socket);
    void CheckThroughput();
    void CheckLatency(FlowMonitorHelper* fmhelper, Ptr<FlowMonitor> monitor);
    void CheckQueueLength(NodeContainer &routers);
    
    uint32_t m_bytesTotal;
    uint32_t m_packetsReceived;

    std::string m_protocolName;
    RoutingType m_protocol;

    bool verbose;   // to control the output

    // some struct
    std::map<int, std::vector<int> > RT;                                  // one-to-group map
    std::set<nodePair> RR;                                               // one-to-one map
    std::map<int, std::vector<std::pair<NodeContainer, int> > > rtNodeMap; // one-to-group map
    std::map<nodePair, NodeContainer> rrNodeMap;                      // one-to-one map
    std::map<int, std::vector<std::pair<NetDeviceContainer, int> > > rtDeviceMap;
    std::map<nodePair, NetDeviceContainer> rrDeviceMap;
    std::map<int, Ipv4InterfaceContainer> rtIpMap;
    std::map<int, Ipv4Address> terminalAddressMap;
    std::map<int, std::vector<Ipv4Address> > routerAddressMap;
};

int main(int argc, char *argv[])
{
    bool verbose = true;
    float totalTestTime = 120;
    int protocol = 1;
    Time::SetResolution(Time::NS);

    CommandLine cmd;
    cmd.Usage("Note: run script with '--PrintHelp' to check the argument");
    cmd.AddValue("time", "The Length of Testing Time", totalTestTime);
    cmd.AddValue("verbose", "Enable The Log Message", verbose);
    cmd.AddValue("protocol", "0=GLOBAL;1=AODV;2=HW", (int&)protocol);
    cmd.Parse(argc, argv);
    
    RoutingExperiment experiment(protocol,verbose);
    
    std::ofstream fout(fileHeader+throughputFile);
    fout << "Simulatation Second,"
        << "ReceiveRate,"
        << "packetsReceived"
        << std::endl;
    fout.close();

    fout.open(fileHeader+queueFile);
    fout << "Simulatation Second,"
        << "queue length"
        << std::endl;
    fout.close();

    fout.open(fileHeader+latencyFile);
    fout << "Simulatation Second,"
        << "average latency"
        << std::endl;
    fout.close();

    fout.open(fileHeader+errorModelFile);
    fout << "Simulatation Second,"
        << "congestion window"
        << std::endl;
    fout.close();

    experiment.Run(totalTestTime);
}

RoutingExperiment::RoutingExperiment(int pro,bool v)
    : m_bytesTotal(0),
      m_packetsReceived(0),
      m_protocol(RoutingType(pro)),
      verbose(v)
{
}

void RoutingExperiment::Run(int totalTestTime)
{
    std::ofstream fout;
    for (int i = 0; i < routerNum; ++i)
    {
        RT.emplace(i, std::vector<int>());
        rtNodeMap.emplace(i, std::vector<std::pair<NodeContainer, int> >());
        rtDeviceMap.emplace(i, std::vector<std::pair<NetDeviceContainer, int> >());
        routerAddressMap.emplace(i, std::vector<Ipv4Address>());
    }
    for (int i = 0; i < terminalNum; ++i)
    {
        RT.at(trList[i] - 1).emplace_back(i);
    }

    for (int i = 0; i < int(sizeof(rrList1) / sizeof(int)); ++i)
    {
        RR.emplace(nodePair(rrList1[i] - 1, rrList2[i] - 1));
    }

    if (verbose)
    {
        LogComponentEnable("NetworkHW", LOG_LEVEL_INFO);
    }

    Config::SetDefault("ns3::OnOffApplication::PacketSize", UintegerValue(udpPacketSize));
    Config::SetDefault("ns3::OnOffApplication::DataRate", StringValue(udpPacketDataRate));

    NS_LOG_INFO("Create nodes");
    NodeContainer terminals;
    terminals.Create(terminalNum);
    NodeContainer routers;
    routers.Create(routerNum);
    NS_LOG_INFO("Create containers between node and router");
    for (auto &x : RT)
    {
        for (auto &vec : x.second)
        {
            NodeContainer tempNode = NodeContainer(routers.Get(x.first), terminals.Get(vec));
            rtNodeMap.at(x.first).emplace_back(std::make_pair(tempNode, vec));
        }
    }

    NS_LOG_INFO("Create containers between node and router");
    for (auto &x : RR)
    {
        NodeContainer tempNode = NodeContainer(routers.Get(x.first), routers.Get(x.second));
        rrNodeMap.emplace(x, tempNode);
    }

    NS_LOG_INFO("Install protocol stack");
    AodvHelper aodvHelper;
    OlsrHelper olsrHelper;
    DsdvHelper dsdvHelper;
    DsrHelper dsrHelper;
    DsrMainHelper dsrMainHelper;
    Ipv4GlobalRoutingHelper globalHelper;
    //HwHelper hwHelper;
    Ipv4ListRoutingHelper ipv4ListRoutingHelper;
    InternetStackHelper stack;

    OutputStreamWrapper routerTableOs(fileHeader+routerTableFile, std::ios_base::app);
    Ptr<OutputStreamWrapper> routerTableOsPtr(&routerTableOs);
    switch (m_protocol)
    {
    case GLOBAL: // only 50 and 25 receive?
    {
        m_protocolName = "GLOBAL";
        //globalHelper.PrintRoutingTableAllEvery(Time(2), routerTableOsPtr);
        globalHelper.PrintRoutingTableAllAt (Time(2), routerTableOsPtr);
        break;
    }
    case AODV: // better
    {
        m_protocolName = "aodv";
        ipv4ListRoutingHelper.Add(aodvHelper, 100);
        //aodvHelper.PrintRoutingTableAllEvery(Time(2), routerTableOsPtr);
        aodvHelper.PrintRoutingTableAllAt (Time(2), routerTableOsPtr);
        break;
    }  
    /*case HW:
    //{
        m_protocolName = "HW";
        ipv4ListRoutingHelper.Add(hwHelper, 100);
        //hwHelper.PrintRoutingTableAllEvery(Time(2), routerTableOsPtr);
        hwHelper.PrintRoutingTableAllAt (Seconds (4), routerTableOsPtr);
    }*/
    default:
    {
        NS_FATAL_ERROR("No such protocol:" << m_protocol);
        break;
    }
    }
    if (m_protocol == AODV || m_protocol == HW)
    {
        stack.SetRoutingHelper(ipv4ListRoutingHelper);
        stack.Install(terminals);
        stack.Install(routers);
    }
    else if (m_protocol == GLOBAL)
    {
        stack.Install(terminals);
        stack.Install(routers);
    }

    NS_LOG_INFO("Create p2p channel");
    PointToPointHelper pointToPointHelper;
    pointToPointHelper.SetDeviceAttribute("DataRate", StringValue(p2pLinkDataRate));
    pointToPointHelper.SetChannelAttribute("Delay", StringValue(p2pLinkDelay));

    NS_LOG_INFO("Create error model");
    Ptr<RateErrorModel> em = CreateObject<RateErrorModel>();
    em->SetAttribute("ErrorRate",DoubleValue(errorRate));

    NS_LOG_INFO("Install channels between node and router");
    NS_LOG_INFO("Assign ip address between node and router");
    NS_LOG_INFO("Install channels between routers");
    NS_LOG_INFO("Assign ip address between router and router");
    // every *.odd is always for router self
    // the r in x.x.r.x means the r th district with router r
    Ipv4AddressHelper ipv4;
    std::string addr1 = "10.12.", addr2 = ".0";
    for (auto &rt : rtNodeMap)
    {
        std::string tempAddr = addr1 + std::to_string(rt.first+1) + addr2;
        ipv4.SetBase((char *)tempAddr.data(), subnetMask);
        for (auto &vec : rt.second)
        {
            NetDeviceContainer tempDevice = pointToPointHelper.Install(vec.first);
            tempDevice.Get(1)->SetAttribute("ReceiveErrorModel",PointerValue(em));
            tempDevice.Get(1)->TraceConnectWithoutContext ("PhyRxDrop", MakeCallback (&RoutingExperiment::RxDrop,this));
            rtDeviceMap.at(rt.first).emplace_back(std::make_pair(tempDevice, vec.second));
            Ipv4InterfaceContainer tempIp = ipv4.Assign(tempDevice);
            rtIpMap.emplace(rt.first, tempIp);
            terminalAddressMap.emplace(vec.second, tempIp.GetAddress(1));
            routerAddressMap.at(rt.first).emplace_back(tempIp.GetAddress(0));
        }
        for (auto &rr : rrNodeMap)
        {
            if(rr.first.first==rt.first)
            {
                NetDeviceContainer tempDevice = pointToPointHelper.Install(rr.second);
                rrDeviceMap.emplace(rr.first, tempDevice);
                Ipv4InterfaceContainer tempIp = ipv4.Assign(tempDevice);
                rtIpMap.emplace(rr.first.first, tempIp);
                routerAddressMap.at(rr.first.first).emplace_back(tempIp.GetAddress(0));
                routerAddressMap.at(rr.first.second).emplace_back(tempIp.GetAddress(1));
            }   
        }
    }

    
    NS_LOG_INFO("Network topology has been built!");
    if(m_protocol == GLOBAL){
        Ipv4GlobalRoutingHelper::PopulateRoutingTables ();
    }

    if (verbose)
    {
        fout.open(fileHeader+terminalsAddressFile);
        for (auto &x : terminalAddressMap)
        {
            // ip of terminals
            int addrForPrint = x.second.Get();
            fout << "terminals " << x.first + 1 << "  " 
                << (addrForPrint >> 24) % 256 << "." 
                << (addrForPrint >> 16) % 256 << "." 
                << (addrForPrint >> 8) % 256 << "." 
                << addrForPrint % 256 << std::endl;
        }  
        fout.close();
        fout.open(fileHeader+routersAddressFile);
        for (auto &x : routerAddressMap)
        {
            for (auto &vec : x.second)
            {
                // ip of routers
                int addrForPrint = vec.Get();
                fout << "router " << x.first + 1 << "  " 
                    << (addrForPrint >> 24) % 256 << "." 
                    << (addrForPrint >> 16) % 256 << "." 
                    << (addrForPrint >> 8) % 256 << "." 
                    << addrForPrint % 256 << std::endl;
            }
        }
        fout.close();
    }

    NS_LOG_INFO("Create the applicaton");
    // create the OnOff application to send UDP packets
    ApplicationContainer apps;
    AddressValue remoteAddress;
    OnOffHelper onOff("ns3::UdpSocketFactory", Address(InetSocketAddress(Ipv4Address::GetAny(), port)));
    onOff.SetConstantRate(DataRate(udpPacketDataRate), udpPacketSize);
    onOff.SetAttribute("OnTime", StringValue("ns3::ConstantRandomVariable[Constant=1]")); // without offtime
    onOff.SetAttribute("OffTime", StringValue("ns3::ConstantRandomVariable[Constant=0]"));

    // start the simulation
    // create sink for each terminal node
    for (int i = 0; i < terminalNum; ++i)
    {
        // GetId() => 0-49
        Ptr<Socket> sink = SetupPacketReceive(terminalAddressMap[i], terminals.Get(i));
    }

    //create onff apps
    fout.open(fileHeader+onoffApplicationFile); 
    Ptr<UniformRandomVariable> randomEngine = CreateObject<UniformRandomVariable>();
    for (float i = 1.0f; i < totalTestTime; i += eachTestTime)
    {
        int randomTestNum = floor(randomEngine->GetValue(1,26));
        for(int j=0;j<randomTestNum;++j)
        {
            int remoteIndex = floor(randomEngine->GetValue(0,terminalNum));
            int localIndex = floor(randomEngine->GetValue(0,terminalNum));
            if(remoteIndex==localIndex){
                remoteIndex = (remoteIndex+1)%terminalNum;
            }
            remoteAddress = AddressValue(InetSocketAddress(terminalAddressMap.at(remoteIndex),port));  // remote node
            onOff.SetAttribute("Remote", remoteAddress);
            
            apps = onOff.Install(terminals.Get(localIndex));   
            apps.Start(Seconds(i));
            apps.Stop(Seconds(i + eachTestTime));                                 
            fout<<"time "<< i << " node " << localIndex << " "
                <<terminalAddressMap.at(localIndex)<<" to node "<<remoteIndex << " "
                <<terminalAddressMap.at(remoteIndex)<<std::endl;
        }
    }
    fout.close();

    AsciiTraceHelper ascii;
    pointToPointHelper.EnableAsciiAll(ascii.CreateFileStream(fileHeader+trFile));

    FlowMonitorHelper flowmonHelper;
    Ptr<FlowMonitor> monitor = flowmonHelper.InstallAll();

    NS_LOG_INFO("start simulation");

    if (verbose)
    {
        flowmonHelper.SerializeToXmlFile("simple-global-routing.flowmon", false, false);
    }

    CheckThroughput();
    CheckLatency(&flowmonHelper, monitor);
    CheckQueueLength(routers);
    
    Simulator::Stop(Seconds(totalTestTime));
    Simulator::Run();
    NS_LOG_INFO("Done");
    Simulator::Destroy();
}

void RoutingExperiment::RxDrop(Ptr<const Packet> p)
{
    std::ofstream fout(fileHeader+errorModelFile,std::ios::app);
    fout << "RxDrop at" << Simulator::Now ().GetSeconds () << std::endl;
    fout.close();
}


inline std::string RoutingExperiment::PrintReceivedPacket(Ptr<Socket> socket, Address senderAddress)
{
    std::ostringstream oss;
    oss << Simulator::Now().GetSeconds() << " node " << socket->GetNode()->GetId() + 1;
    if (InetSocketAddress::IsMatchingType(senderAddress))
    {
        InetSocketAddress addr = InetSocketAddress::ConvertFrom(senderAddress);
        int senderId = 0;
        for(auto x : terminalAddressMap)
        {
            if(x.second==addr.GetIpv4())
            {
                senderId = x.first;
                break;
            }
        }
        oss << " received one packet from " << addr.GetIpv4() <<" node " << senderId + 1;
    }
    else
    {
        oss << " received one packet!";
    }

    return oss.str();
}

// calculate the throughput
void RoutingExperiment::ReceivePacket(Ptr<Socket> socket)
{
    Ptr<Packet> packet;
    Address senderAddress;
    while (packet = socket->RecvFrom(senderAddress))
    {
        m_bytesTotal += packet->GetSize();
        m_packetsReceived += 1;
        if(verbose)
        {
            NS_LOG_UNCOND(PrintReceivedPacket(socket, senderAddress));
        }
    }
}

Ptr<Socket> RoutingExperiment::SetupPacketReceive(Ipv4Address ipv4Address, Ptr<Node> node)
{
    TypeId tid = TypeId::LookupByName("ns3::UdpSocketFactory");
    Ptr<Socket> sink = Socket::CreateSocket(node, tid);
    InetSocketAddress localAddress = InetSocketAddress(ipv4Address, port);
    sink->Bind(localAddress);
    sink->SetRecvCallback(MakeCallback(&RoutingExperiment::ReceivePacket,this));
    return sink;
}

void RoutingExperiment::CheckThroughput()
{
    NS_LOG_INFO("Set the throughtput checker");
    uint32_t bitPerSecond = m_bytesTotal * 8.0;
    m_bytesTotal = 0;
    std::ofstream fout(fileHeader+throughputFile, std::ios::app);
    fout << (Simulator::Now()).GetSeconds() << " "
        << bitPerSecond << " "
        << m_packetsReceived
        << std::endl;
    fout.close();
    m_packetsReceived = 0;
    Simulator::Schedule(Seconds(1.0), &RoutingExperiment::CheckThroughput,this);
}

void RoutingExperiment::CheckQueueLength(NodeContainer &routers){
    NS_LOG_INFO("Set the queue checker");
    uint32_t queueMaximum = 0;
    for(int i = 0; i<routerNum; ++i)
    {
        Ptr<Node> tempNode = routers.Get(i);
        uint32_t queueTemp = 0;
        for(uint16_t j=1;j<tempNode->GetNDevices();++j)
        {
            queueTemp += tempNode->GetDevice(j)->GetObject<PointToPointNetDevice>()->GetQueue()->GetNBytes();
        } 
        queueMaximum = queueMaximum > queueTemp ? queueMaximum : queueTemp;
    }
    std::ofstream fout(fileHeader+queueFile, std::ios::app);
    fout << (Simulator::Now()).GetSeconds() << " "
        << queueMaximum
        << std::endl;
    fout.close();
    Simulator::Schedule(Seconds(0.01),&RoutingExperiment::CheckQueueLength,this,routers);
}

void RoutingExperiment::CheckLatency(FlowMonitorHelper* fmhelper, Ptr<FlowMonitor> monitor)
{
    NS_LOG_INFO("Set the latency checker");
    double latency = 0;
    monitor->CheckForLostPackets ();
    std::map<FlowId, FlowMonitor::FlowStats> flowStats = monitor->GetFlowStats ();
    Ptr<Ipv4FlowClassifier> classifier = DynamicCast<Ipv4FlowClassifier> (fmhelper->GetClassifier ());
    for (std::map<FlowId, FlowMonitor::FlowStats>::const_iterator i = flowStats.begin (); i != flowStats.end (); ++i)
    {
        Ipv4FlowClassifier::FiveTuple t = classifier->FindFlow (i->first);
        // UDP_PROT_NUMBER = 17
        if (17 == unsigned(t.protocol))
        {
            latency += i->second.delaySum.GetSeconds ();
        }
        else
        {
            std::cout << "This is not UDP traffic" << std::endl;
        }
    }
    std::ofstream fout(fileHeader+latencyFile, std::ios::app);
    fout << (Simulator::Now()).GetSeconds() << " "
        << latency
        << std::endl;
    fout.close();
    Simulator::Schedule(Seconds(1.0),&RoutingExperiment::CheckLatency,this, fmhelper, monitor);
}