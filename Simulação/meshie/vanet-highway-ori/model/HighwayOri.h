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

#ifndef CLASS_HIGHWAY_
#define CLASS_HIGHWAY_

#include "ns3/callback.h"
#include "ns3/ptr.h"
#include "ns3/object.h"
#include "ns3/random-variable-stream.h"
#include "ns3/vector.h"
#include "ns3/VehicleOri.h"
#include "ns3/ModelOri.h"
#include "ns3/LaneChangeOri.h"
#include <vector>
#include <string>
#include "ns3/packet-metadata.h"
#include "ns3/wifi-helper.h"
#include "ns3/ipv4-address-helper.h"


namespace ns3
{
  /**
  * \brief Highway is a place holder of the Vehicle (s) which manages each step of the Vehicle mobility.
  * 
  * A Highway has up to total 10 lists (queue of lanes), maximum 5 (lanes) for each direction. At each step (interval dt), Highway 
  * browse vehicles of each lane in order of their poistions. Highway moves each Vehicle or does the change of lane based on the
  * required information given by each vehicle and its adjacent vehicles following IDM Model and LaneChange rules. 
  * It is possible to add vehicles to the Highway manually, or we can set the Highway to do so automatically (AutoInjection).
  * Vehicle depoloyments can happen at InitHighway or through handling the InitVehicle event raised by Highway. Although, it is possible to 
  * add vehicles to the highway at anytime of simulation.
  * Also, Highway raises a ControlVehicle event at each passed interval dt to give possibility to access/control Vehicles of the Highway.
  * Vehicles can communicate wirelessly at anytime using the shared WifiChannel given by Highway, 
  * although they may want to use and apply other channels. It is up to user how to catch the Highway events and deal with them.
  */
  class Highway: public ns3::Object
  {
    private: 

      std::list<Ptr<Vehicle> > m_vehicles[5];    // list of vehicles in positive direction (+1) up to maximum 5 lanes.
      std::list<Ptr<Vehicle> > m_vehiclesOpp[5]; // list of vehicles in negative direction (-1) up to maximum 5 lanes.
      std::list<Ptr<Vehicle> > m_vehiclesAp;
      bool m_twoDirectioanl;                // true if it's a two directional roadway, false if one directional.
      int m_numberOfLanes;                  // number of lanes for each direction, maximum value is 5.
      double m_highwayLength;	            // the length of the highway.
      double m_laneWidth;                   // the width of each lane in the roadway.
      double m_medianGap;                   // the width of the median.
      double m_injectionSafetyGap;          // the entrance gap criteria in meter.
      double m_sedanTruckPerc;              // the percantage of sedans against trucks when autoinjection is used.
      Ptr<LaneChange> m_laneChangeSedan;    // a common IDM/MOBIL lanechange model for sedans.
      Ptr<LaneChange> m_laneChangeTruck;    // a common IDM/MOBIL lanechange model for trucks.
      Ptr<Model> m_sedan;                   // a common IDM/MOBIL driving model for sedans.
      Ptr<Model> m_truck;                   // a common IDM/MOBIL driving model for trucks.
      double m_dt;                          // the mobility step interval. (duraion between each step)
      int m_vehicleId;                      // auto increment vehicle id which will be assigned to vehicles.
      bool m_stopped;	                    // true, if the highway manager is stopped.
      bool m_autoInject;                    // true, if we desire that vehicles automatically being injected into the highway. 
      bool m_changeLaneSet;                 // true, if we desire the vehicles be able to change their lanes with IDM/MOBIL conditions.
      Ptr<Vehicle> m_tempVehicles[2];       // temp vehicles.
	  
	  YansWifiPhyHelper wifiPhy;
	  YansWifiChannelHelper wifiChannel;
	  NqosWaveMacHelper wifi80211pMac;
      Wifi80211pHelper wifi80211p;
	  
	  int m_apInitDist;
      int m_obInitDist;
      float m_sedanVelocity;
      float m_truckVelocity;
      double m_simTime;
      int m_generatedPackets;

      /// Initializes the Highway and raises the event InitVehicle.
      void InitHighway();
      /// Injects Vehicles based on given minimum gap and percentage p.
      void InjectVehicles(double minGap, int p);
      /// Translates the Vehicles to the new position.
      void TranslateVehicles();
	  /// Calculates the position and velocity of each vehicle for the passed step and the next step. 
      void TranslatePositionVelocity(std::list<Ptr<Vehicle> > vehicles[], double dt);
	  /// Calculates the acceleration of the vehicles in passed step and for the next step.
      void Accelerate(std::list<Ptr<Vehicle> > vehicles[], double dt);
	  /// Changes the vehicle lanes if possible.
      void ChangeLane(std::list<Ptr<Vehicle> > vehicles[]);
	  /// Changes the vehicle lanes from current lanes to the destination lane.
      void DoChangeLaneIfPossible(std::list<Ptr<Vehicle> > vehicles[], int curLane, int desLane);
	  /// Find the Vehicles on the Side of the current vehicle veh.
      void FindSideVehicles(std::list<Ptr<Vehicle> > vehicles[], Ptr<Vehicle> veh, int sideLane);
      /// Prints all vehicles in Highway.
      void PrintVehicles();

      /**
      * An event called at Highway initialization (InitHighway).
      * It gives the Highway and the current Vehicle Id value.
	  * This callback must point to the function which handles such event.
      */
      
      Callback<bool, Ptr<Highway>, int&> m_initVehicle;
            
      /// For Catching an event when a packet is received by any vehicles in the Highway.
      VehicleReceiveCallback m_receiveData;

    public: 

      /// Override TypeId.
      static TypeId GetTypeId (void);
      /**
	  * Setting the default values:
      * dt=1.0 , VehicleId=1, Number of Lanes=1, Highway Length=1000(m), Width of Lanes=5(m), Median Gap=5(m).
      * Injection Safety Gap=5(m), Percentage of Sedans in comparison to Trucks=80%, One Directional and Changing Lane set to False.
	  * Nqos Mac, Yans Phy, and 6-mb WIFI_PHY_STANDARD_80211a with Transmission range around 250-300(m).
	  */
      Highway();
      /// Destructor to clean up the lists.
      ~Highway();
      /**
      * Starts the highway.
      */
      void Start();
      /**
      * Stops the highway. Therefore no vehicles mobility after.
      */
      void Stop();
      /**
      * Creates a IDM model similar to a sedan.  
      */
      Ptr<Model> CreateSedanModel();
      /**
      * Creates a IDM model similar to a truck.
      */
      Ptr<Model> CreateTruckModel();
      /**
      * \returns the Number of Lanes in the Highway.
      */
      int GetNumberOfLanes();
      /**
      * \param value the Number of Lanes the highway can have for each direction [min=0, max=5].
      */
      void SetNumberOfLanes(int value);
      /**
      * \param value true if we desire a two-directional Highway, false if one-directional.      
      */
      void SetTwoDirectional(bool value);
      /**
      * \returns true if its a two-directional Highway, false if one-directional.
      */
      bool GetTwoDirectional();
      /**
      * \returns the Length of the Highway.
      */
      double GetHighwayLength();
      /**
      * \param value the Length of the Highway.
      */
      void SetHighwayLength(double value);
      /**
      * \returns the Width of the Lanes.
      */
      double GetLaneWidth();
      /**
      * \param value the Width of the Lanes.
      */
      void SetLaneWidth(double value);
      /**
      * \returns the Median Gap. (width of the median)
      */
      double GetMedianGap();
      /**
      * \param value the Median Gap. (width of the median)
      */
      void SetMedianGap(double value);
      /**
      * \returns the current Sedan Model in use.
      */
      Ptr<Model> GetSedanModel();
      /**
      * \param value the desired Sedan Model for use.
      */
      void SetSedanModel(Ptr<Model> value);
      /**
      * \returns the current Truck Model in use.
      */
      Ptr<Model> GetTruckModel();
      /**
      * \param value the desired Truck Model for use.
      */
      void SetTruckModel(Ptr<Model> value);
      /**
      * \returns the current Sedan LaneChange Model in use.
      */
      Ptr<LaneChange> GetSedanLaneChange();
      /**
      * \param value the desired Sedal LaneChange Model for use.
      */
      void SetSedanLaneChange(Ptr<LaneChange> value);
      /**
      * \returns the current Truck LaneChange Model in use.
      */
      Ptr<LaneChange> GetTruckLaneChange();
      /**
      * \param value the desired Truck LaneChange Model for use.
      */
      void SetTruckLaneChange(Ptr<LaneChange> value);
      /**
      * \returns the y (center) of the wanted lane and desired direction. 
      */
      double GetYForLane(int lane,int dir);
      /**
      * \returns true if auto-injection is on, otherwise false.
      */
      bool GetAutoInject();
      /**
      * \param value true will turn the auto-injection on, false will turn it off.
      */
      void SetAutoInject(bool value);
      /**
      * \returns the Injection Gap (meters) needed by the injection at entrance (InjectVehicles()) of the Highway.
      */
      double GetInjectionGap();
      /**
      * \param value the Injection Gap (meters) used in InjectVehicles() for entrance. 
      */
      void SetInjectionGap(double value);
      /**
      * \returns the Injection Mix Value, the percentage of sedans being created in contrast to trucks.
      */
      double GetInjectionMixValue();
      /**
      * \param value Injection Mix Value, the percentage of sedans being created in contrast to trucks.
      */
      void SetInjectionMixValue(double value);
      /**
      * \returns true if changing lanes in Highway is on, false if off.
      */
      bool GetChangeLane();
      /**
      * \param value true to turn the changing lanes on, false for off.
      */
      void SetChangeLane(bool value);
      /**
      * \returns the value of interval dt, the duration of each mobility step. A interval between each steps.
      */
      double GetDeltaT(void);
      /**
      * \param value the interval dt, the duration of each mobility step. A interval between each steps.
      */
      void SetDeltaT(double value);
      /**
      * it will add the vehicle in to the Highway based on the vehicle lane and direction to the appropriate Highway list.
      */
      void AddVehicle(Ptr<Vehicle> vehicle);
      /**
      * \returns the last (currently) value of auto-incremented Vehicle Id.
      */
      int GetLastVehicleId();
	  /**
	  * \returns the WifiHelper used by the highway.
	  */
      WifiHelper GetWifiHelper();
	  /**
	  * \returns the NqosWifiMacHelepr used by the Highway.
	  */
      NqosWaveMacHelper GetNqosWaveMacHelper();
	  /**
	  * \returns the YansWifiPhyHelper used by the Highway.
	  */
      YansWifiPhyHelper GetYansWifiPhyHelper();
	  /**
	  * \returns the shared WifiChannel used by/in/for the Highway.
	  */
      YansWifiChannelHelper GetWifiChannelHelper();
            
      /**
      * \returns the retrieved Vehicle at the specific Index from the list of highway vehicles.
      */
      Ptr<Vehicle> GetVehicle(std::list<Ptr<Vehicle> > v, int index);
      /**
      * \returns the Vehicle from the Highway given its VehicleId (vid).
      */
      Ptr<Vehicle> FindVehicle(int vid);
      /**
      * \returns the list of vehicles within the specific Range from a desired Vehicle in the Highway.
      */
      std::list<Ptr<Vehicle> > FindVehiclesInRange(Ptr<Vehicle> vehicle, double range);
      std::list<Ptr<Vehicle> > FindVehiclesInRange(Ptr<Vehicle> vehicle, double range, int dir);
      /**
      * \returns the list of vehicles for each Lane and Direction in a specific segment of the Highway from x1 to x2.
      */
      std::list<Ptr<Vehicle> > FindVehiclesInSegment(double x1, double x2, int lane, int dir);

      /// Returns the Highway Init Vehicle callback.
      Callback<bool, Ptr<Highway>, int&> GetInitVehicleCallback();
      /// Sets the Highway Init Vehicle callback.
      void SetInitVehicleCallback(Callback<bool, Ptr<Highway>, int&> initVehicle);
      
      void SetObInitDist(int initDist);
      int GetObInitDist(void);
      void SetSedanVelocity(float sedanVelocity);
      float GetSedanVelocity(void);
      void SetTruckVelocity(float truckVelocity);
      float GetTruckVelocity(void);
      void SetApInitDist(int initDist);
      int GetApInitDist(void);
      void SetSimTime(double simTime);
      double GetSimTime();
      Ptr<Vehicle> FindMacVeh(std::string macAddress);
      Ptr<YansWifiChannel> GetYansWifiChannel();
	  
	  /// Returns the Highway's Receive Data callback.
      VehicleReceiveCallback GetReceiveDataCallback();
      /// Sets the Highway's Receive Data callback.
      void SetReceiveDataCallback(VehicleReceiveCallback receiveData);
            
	  /**
      * Runs one mobility Step for the given highway.
	  * This function is called each interval dt to simulated the mobility through TranslateVehicles().
      */
      static void Step(Ptr<Highway> highway);
      
      int GetGeneratedPackets ();
  };
};
#endif
