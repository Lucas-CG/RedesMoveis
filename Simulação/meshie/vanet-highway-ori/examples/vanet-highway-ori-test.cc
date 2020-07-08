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
#include "ns3/HighwayOri.h"
#include "ns3/ControllerOri.h"
#include "ns3/network-module.h"
#include "ns3/packet-metadata.h"


NS_LOG_COMPONENT_DEFINE ("HADI");

using namespace ns3;
using namespace std;

float simTime = 5000; 
double setDelta = 0.1;
int highwayLength = 2000;
int numOfLanes = 3;
int laneWidth = 5;
int medianGap = 5;
double apInitDist = 1990;
int obInitDist = 300;
bool changeLane = true;
bool autoInject = true;
int injectionMixValue = 100;
int injectionGap = 200;
float truckVelocity = 0;
float sedanVelocity = 13;
bool setTwoDirectional = false;

int main (int argc, char *argv[])
{
	
  // process command-line args
  CommandLine cmd;
  cmd.AddValue("time", "simulation time", simTime);
  cmd.AddValue("deltaT", "Delta T", setDelta);
  cmd.AddValue("hlength", "Highway length", highwayLength);
  cmd.AddValue("numOfLanes", "Number of lanes", numOfLanes);
  cmd.AddValue("laneWidth", "Set Lane Width", laneWidth);
  cmd.AddValue("medianGap", "Set median gap", medianGap);
  cmd.AddValue("apInitDist", "Initial position of the RSU", apInitDist);
  cmd.AddValue("apInitDist", "Initial position of the Obstacle", obInitDist);
  cmd.AddValue("changeLane", "Set Change Lane", changeLane);
  cmd.AddValue("setTwoDirectional", "Set a highway with 2 ways (east and west)", setTwoDirectional);
  cmd.AddValue("autoInject", "Set Auto Inject", autoInject);
  cmd.AddValue("injectMixValue", "Set the ratio of car to truck", injectionMixValue);
  cmd.AddValue("injectionGap", "Minimum distance between two vehicles", injectionGap);
  cmd.AddValue("sedanVelocity", "Set max sedan velocity", sedanVelocity);
  cmd.AddValue("truckVelocity", "Set max truck velocity", truckVelocity);
  cmd.Parse(argc, argv);
   
    Ptr<Highway> highway = CreateObject<Highway > ();
                                             
    highway->SetSimTime(simTime);
    highway->SetDeltaT(setDelta);
    highway->SetHighwayLength(highwayLength);
    highway->SetNumberOfLanes(numOfLanes);
    highway->SetLaneWidth(laneWidth);
    highway->SetMedianGap(medianGap);
    highway->SetApInitDist(apInitDist);
    highway->SetObInitDist(obInitDist);
    highway->SetChangeLane(changeLane);
    highway->SetTwoDirectional(setTwoDirectional);
    highway->SetAutoInject(autoInject);
    highway->SetInjectionMixValue(injectionMixValue);
    highway->SetInjectionGap(injectionGap);
    highway->SetSedanVelocity(sedanVelocity);
    highway->SetTruckVelocity(truckVelocity);   
    
    Ptr<Controller> controller = CreateObject<Controller > (highway);
  
    
    if(highway->GetApInitDist() < 0)
    {
      cout << "Invalid initial distance" << endl;
      exit(1);
    }
    
    /// Bind the Highway/Vehicle events to the event handlers. Controller's will catch them.  
    highway->SetInitVehicleCallback(MakeCallback(&Controller::InitVehicle, controller));    
    highway->SetReceiveDataCallback(MakeCallback(&Controller::ReceiveData, controller));

    ns3::PacketMetadata::Enable();
        
    Simulator::Schedule(Seconds(0.0), &Highway::Start, highway);    
    Simulator::Schedule(Seconds(simTime), &Highway::Stop, highway);    
    Simulator::Schedule(Seconds(0.0), &Controller::Start, controller);
    Simulator::Schedule(Seconds(simTime-0.1), &Controller::Stop, controller);      
    Simulator::Stop(Seconds(simTime));
    Simulator::Run();
    Simulator::Destroy();
  return 0;
  
}
