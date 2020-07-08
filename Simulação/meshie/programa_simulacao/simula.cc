/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/csma-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/ipv4-global-routing-helper.h"
#include "ns3/error-model.h"

#include <string>

// Default Network Topology
//
//  n1   n2   n3   n4
//  |    |    |    |
//  ================
//    LAN 10.1.2.0


using namespace ns3;
using namespace std;

NS_LOG_COMPONENT_DEFINE ("Example");

int 
main (int argc, char *argv[])
{
  bool verbose = true;
  uint32_t nClients = 3;
  string app ("ECHO");

  CommandLine cmd;
  cmd.AddValue ("nClients", "Number of \"extra\" CSMA nodes/devices", nClients);
  cmd.AddValue ("verbose", "Tell the application to log if true", verbose);
  cmd.AddValue ("application", "Define the application to be used", app);

  cmd.Parse (argc,argv);
  
  cout << "Application " << app << endl;

  if (verbose)
    {
      	if (!app.compare ("ECHO")) {
		LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_INFO);
      		LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_INFO);
      	} else if (!app.compare ("UDP")) {
      		LogComponentEnable ("UdpClient", LOG_LEVEL_INFO);
      		LogComponentEnable ("UdpServer", LOG_LEVEL_INFO);
	}
    }

  nClients = nClients == 0 ? 1 : nClients;

  NodeContainer server;
  server.Create (1);

  NodeContainer clients;
  clients.Create (nClients);

  NodeContainer nodes;
  nodes.Add (server.Get (0));
  nodes.Add (clients);

  Ptr<RateErrorModel> em = CreateObject<RateErrorModel> ();
  em->SetAttribute ("ErrorRate", DoubleValue (0.0001));

  CsmaHelper csma;
  csma.SetChannelAttribute ("DataRate", StringValue ("100Mbps"));
  csma.SetChannelAttribute ("Delay", TimeValue (NanoSeconds (6560)));
  csma.SetDeviceAttribute ("ReceiveErrorModel", PointerValue (em));

  NetDeviceContainer csmaDevices;
  csmaDevices = csma.Install (nodes);

  InternetStackHelper stack;
  stack.Install (nodes);

  Ipv4AddressHelper address;

  address.SetBase ("10.1.2.0", "255.255.255.0");
  Ipv4InterfaceContainer csmaInterfaces;
  csmaInterfaces = address.Assign (csmaDevices);

  ApplicationContainer serverApps;
  if (!app.compare ("ECHO")) {
  UdpEchoServerHelper echoServer (9);
	serverApps = echoServer.Install (nodes.Get (0));

  } else if (!app.compare ("UDP")) {
  UdpServerHelper udpServer (9);
	serverApps = udpServer.Install (nodes.Get (0));
  }

  serverApps.Start (Seconds (1.0));
  serverApps.Stop (Seconds (10.0));

  ApplicationContainer clientApps;
  if (!app.compare ("ECHO")) {
  UdpEchoClientHelper echoClient (csmaInterfaces.GetAddress (0), 9);
  	echoClient.SetAttribute ("MaxPackets", UintegerValue (50));
  	echoClient.SetAttribute ("Interval", TimeValue (MilliSeconds (100)));
  	echoClient.SetAttribute ("PacketSize", UintegerValue (1024));

  	clientApps = echoClient.Install (nodes.Get (nClients));
  } else if (!app.compare ("UDP")) {
  UdpClientHelper udpClient (csmaInterfaces.GetAddress (0), 9);
  	udpClient.SetAttribute ("MaxPackets", UintegerValue (50));
 	udpClient.SetAttribute ("Interval", TimeValue (MilliSeconds (100)));
  	udpClient.SetAttribute ("PacketSize", UintegerValue (1024));
  
  	clientApps = udpClient.Install (clients);
  }

  clientApps.Start (Seconds (2.0));
  clientApps.Stop (Seconds (10.0));

  Simulator::Run ();
  Simulator::Destroy ();
  return 0;
}
