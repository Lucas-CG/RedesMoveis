/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2005-2009 Old Dominion University [ARBABI]
 *
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
 *
 * Author: Hadi Arbabi <marbabi@cs.odu.edu>
 */

/*
	This the starting point of the simulation and experiments.
	The main function will parse the input and parameter settings.
	Creates a highway and set the highway parameters. then bind the events (callbacks)
	to the created controller and designed handlers. Sets the highway start and end time,
	and eventually runs the simulation which is basically running a highway with a controller.
	You can add your functions to controller to create various scenarios. 
*/

#include <fstream>
#include <iostream>
#include <iomanip>
#include "ns3/core-module.h"
#include "ns3/mobility-module.h"
#include "ns3/wifi-module.h"
#include "Highway.h"
#include "Controller.h"
#include "ns3/network-module.h"
#include "ns3/packet-metadata.h"

#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/csma-module.h"
#include "ns3/internet-module.h"

NS_LOG_COMPONENT_DEFINE ("HADI");

using namespace ns3;
using namespace std;

static void Start(Ptr<Highway> highway)
{
  highway->Start();
}

static void Stop(Ptr<Highway> highway)
{
  highway->Stop();
}

int main (int argc, char *argv[])
{
  float simTime=100.0;
  bool plot=false;

  // process command-line args
  CommandLine cmd;
  cmd.AddValue ("time", "simulation time", simTime);
  cmd.AddValue ("plot", "generate output fot gnuplot", plot);  
  cmd.Parse(argc, argv);

  Ptr<Highway> highway=CreateObject<Highway>();
  Ptr<Controller> controller=CreateObject<Controller>();

  controller->SetHighway(highway);
  controller->Plot=plot;
  
  highway->SetHighwayLength(1000);
  highway->SetLaneWidth(5);
  highway->SetNumberOfLanes(2);
  highway->SetChangeLane(true);
  highway->SetTwoDirectional(true);
  highway->SetMedianGap(5);
  highway->SetInjectionGap(10);
  highway->SetInjectionMixValue(80);
  highway->SetAutoInject(false);
  highway->SetDeltaT(0.1);

  // Change the transmission range of wifi shared in the Highway.
  //highway->GetYansWifiPhyHelper().SetTxPowerStart(21.5); // 250-300 meter transmission range 
  //highway->GetYansWifiPhyHelper().SetTxPowerStart(21.5);   // 250-300 meter transmission range 
  
  // Bind the Highway/Vehicle events to the event handlers. Controller's will catch them.  
  highway->SetControlVehicleCallback(MakeCallback(&Controller::ControlVehicle,controller));
  highway->SetInitVehicleCallback(MakeCallback(&Controller::InitVehicle,controller));
  highway->SetReceiveDataCallback(MakeCallback(&Controller::ReceiveData,controller));
  
  //ns3::PacketMetadata::Enable();
  //Config::SetDefault ("ns3::WifiRemoteStationManager::FragmentationThreshold", StringValue ("2200"));
  //Config::SetDefault ("ns3::WifiRemoteStationManager::RtsCtsThreshold", StringValue ("2200"));
  
  Simulator::Schedule(Seconds(0.0), &Start, highway);
  Simulator::Schedule(Seconds(simTime), &Stop, highway);
  Simulator::Stop(Seconds(simTime));
  Simulator::Run();
  Simulator::Destroy();

  return 0;
}
