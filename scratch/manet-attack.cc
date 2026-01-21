#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/mobility-module.h"
#include "ns3/wifi-module.h"
#include "ns3/internet-module.h"
#include "ns3/aodv-module.h"
#include "ns3/applications-module.h"

using namespace ns3;
NS_LOG_COMPONENT_DEFINE ("ManetAttack");

uint32_t g_txPackets = 0;
uint32_t g_rxPackets = 0;
uint32_t g_dropPackets=0;

const uint32_t DROP_THRESHOLD = 50;

//adding blackhole attack
bool BlackholeDrop (
    Ptr<NetDevice> device,
    Ptr<const Packet> packet,
    uint16_t protocol,
    const Address &src,
    const Address &dst,
    NetDevice::PacketType packetType)
{
    static uint32_t dropCount = 0;
    static bool detected = false;

    const uint32_t DETECTION_THRESHOLD = 50;

    // Detection logic
    if (!detected)
    {
        dropCount++;

        if (dropCount >= DETECTION_THRESHOLD)
        {
            detected = true;
            NS_LOG_UNCOND ("[DETECTION] Blackhole node detected!");
            NS_LOG_UNCOND ("[PREVENTION] Activating prevention mechanism");
        }

        // Attack phase: drop packets
        return true;
    }

    // Prevention phase: allow packets
    return false;
}





           


double simTime = 10.0;   // simulation time in seconds



void RxPacketCounter (Ptr<const Packet> packet, const Address &from)
{
   g_rxPackets++;
}
void TxPacketCounter (Ptr<const Packet> packet)
{
    g_txPackets++;
}

int
main (int argc, char *argv[])
{LogComponentEnable ("ManetAttack", LOG_LEVEL_INFO);

uint32_t numNodes = 10;
uint32_t srcNode = 0;
uint32_t destNode = numNodes - 1;
uint16_t port = 9;
      // we will change to 100 later
  uint32_t maliciousNodeId = 5;

  NodeContainer nodes;
  nodes.Create (numNodes);

  // =====================
  // WiFi (Adhoc)
  // =====================
  WifiHelper wifi;
  wifi.SetStandard (WIFI_STANDARD_80211b);


  YansWifiChannelHelper channel = YansWifiChannelHelper::Default ();
  YansWifiPhyHelper phy;
  phy.SetChannel (channel.Create ());

  WifiMacHelper mac;
  mac.SetType ("ns3::AdhocWifiMac");

  NetDeviceContainer devices = wifi.Install (phy, mac, nodes);

  // =====================
  // Mobility
  // =====================
MobilityHelper mobility;
mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
mobility.Install (nodes);

for (uint32_t i = 0; i < numNodes; ++i)
{
    Ptr<MobilityModel> mob = nodes.Get (i)->GetObject<MobilityModel> ();
    mob->SetPosition (Vector (i * 50.0, 0.0, 0.0));
}
  

  // =====================
  // Internet + AODV
  // =====================
  AodvHelper aodv;
  InternetStackHelper internet;
  internet.SetRoutingHelper (aodv);
  internet.Install (nodes);

  // =====================
  // IP addressing
  // =====================
  Ipv4AddressHelper ipv4;
  ipv4.SetBase ("10.0.0.0", "255.255.255.0");
  Ipv4InterfaceContainer interfaces = ipv4.Assign (devices);

  // =====================
  // BLACKHOLE ATTACK
  // =====================
// ================================
// BLACKHOLE ATTACK
// ================================
Ptr<NetDevice> maliciousDevice = devices.Get (maliciousNodeId);

maliciousDevice->SetPromiscReceiveCallback (
    MakeCallback (&BlackholeDrop));

NS_LOG_UNCOND ("Malicious node ID = " << maliciousNodeId);
  

  

  // =====================
  // Simple traffic
  // ===================== 
OnOffHelper onoff ("ns3::UdpSocketFactory",
                   InetSocketAddress (interfaces.GetAddress (destNode), port));

ApplicationContainer sourceApps = onoff.Install (nodes.Get (srcNode));
sourceApps.Start (Seconds (1.0));
sourceApps.Stop (Seconds (simTime));
sourceApps.Get (0)->TraceConnectWithoutContext (
    "Tx", MakeCallback (&TxPacketCounter));


  PacketSinkHelper sink ("ns3::UdpSocketFactory",
                         InetSocketAddress (Ipv4Address::GetAny (), port));

  ApplicationContainer sinkApps = sink.Install (nodes.Get (destNode));
sinkApps.Start (Seconds (0.0));
sinkApps.Stop (Seconds (simTime));
Ptr<PacketSink> sink1 = DynamicCast<PacketSink> (sinkApps.Get (0));
sink1->TraceConnectWithoutContext ("Rx", MakeCallback (&RxPacketCounter));







  // =====================
  Simulator::Stop (Seconds (20.0));
  Simulator::Run ();



double pdr = 0.0;
if (g_txPackets > 0)
{
    pdr = (double) g_rxPackets / g_txPackets;
}

NS_LOG_UNCOND ("========= FINAL RESULTS =========");
NS_LOG_UNCOND ("Tx Packets = " << g_txPackets);
NS_LOG_UNCOND ("Rx Packets = " << g_rxPackets);
NS_LOG_UNCOND ("PDR = " << pdr);






Simulator::Destroy ();


  return 0;
}







