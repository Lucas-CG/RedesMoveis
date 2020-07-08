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

#ifndef CLASS_CONTROLLER_
#define CLASS_CONTROLLER_

#include "ns3/HighwayOri.h"
#include "ns3/ObstacleOri.h"

using namespace ns3;
using namespace std;

namespace ns3
{

  /**
  * \brief Controller is the main class to manage the events (callbacks), traces, rules, and etc.
  *
  * Controller can be assumed as an application which is tied with the highway and vehicles.
  * we implement the VANETs simulations here, design and form the basic of each experiments. 
  */
  class Controller : public Object
  {
    private:
		double T;
		double raio;		
		int m_receivedPackets;
      
    public:   
    
		Ptr<Highway> highway;    
		/// Constructor.
		Controller();
		/// Constructor.
		Controller(Ptr<Highway> highway);
		/// to broadcast a warning by vehicle veh. 
		void BroadcastBeacons(Ptr<Obstacle> ap);
		/// event handler for InitVehicle callbacks.
		bool InitVehicle(Ptr<Highway> highway,  int& VID);
		/// sets the highway bound to this controller.
		void SetHighway(Ptr<Highway> highway);
		/// returns the highway bound to this controller.
		Ptr<Highway> GetHighway();
		/// a flag for plotting vehicles.
		bool Plot;
		void ServerData();
		void Start();
		void Stop();
		void ReceiveData (Ptr<Vehicle> veh, Ptr<const Packet> pkt, Address sender);
		void CalculaMetricas();
  };
}
#endif
