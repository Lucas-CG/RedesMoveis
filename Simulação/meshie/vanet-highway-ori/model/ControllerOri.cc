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

#include <iostream>
#include <sstream>
#include <iomanip>
#include "ns3/ControllerOri.h"

using namespace std;
using namespace ns3;

namespace ns3
{
	
  /* ------------------------------------------------------------------------------------------------------- */
	time_t start, end;
	
	void Controller::Start()
	{
		struct tm * timeinfo;
		time(&start);
		timeinfo = localtime(&start);
		cout << "Iniciando a simulacao: " << asctime(timeinfo);
	}
	
	/* ------------------------------------------------------------------------------------------------------- */
	
	void Controller::Stop()
	{
		this->ServerData();
	}
	
	
	/* ------------------------------------------------------------------------------------------------------- */
	
	Controller::Controller()
	{ 
		T=-1.0; 
		Plot=false;
		m_receivedPackets = 0;
	}
	
	/* ------------------------------------------------------------------------------------------------------- */
	
	Controller::Controller(Ptr<Highway> highway)
	{
		this->highway=highway;				
		raio = 5000;	
		m_receivedPackets = 0;	
	}
	
	/* ------------------------------------------------------------------------------------------------------- */

	void Controller::SetHighway(Ptr<Highway> highway)
	{
		this->highway=highway;
	}
	
	/* ------------------------------------------------------------------------------------------------------- */

	Ptr<Highway> Controller::GetHighway()
	{
		return this->highway;
	}
	
	
	/* ------------------------------------------------------------------------------------------------------- */
	
	void Controller::CalculaMetricas()
	{
		cout <<"Total de pacotes enviados: " <<highway->GetGeneratedPackets() <<endl;
		cout <<"Total de pacotes recebidos: " <<m_receivedPackets <<endl;
		cout <<"Percentual de entrega: " <<((m_receivedPackets*100)/highway->GetGeneratedPackets()) <<"%" <<endl;
		
		ofstream file;
		stringstream aux;
		file.open("/tmp/TxRate.log",ios::app);
		aux <<((m_receivedPackets*100)/highway->GetGeneratedPackets()) <<endl;
		file << aux.str().c_str();
		file.close();
	}
		
  /* ------------------------------------------------------------------------------------------------------- */
  
  void Controller::ServerData() 
	{
		stringstream dataString;
		ofstream dataFile;
		struct tm * timeinfo;
				
		// Gera a hora que terminou a simulação
		time(&end);
		timeinfo = localtime(&end);
		cout << "\nFim da Simulacao: " << asctime(timeinfo);	
		// Calcula o tempo de simulação
		int horas, minutos, segundos;	
		segundos = difftime(end, start);
		horas = segundos/3600;
		minutos = (segundos - horas*3600)/60;
		segundos = (segundos - horas*3600 - minutos*60);	
		cout << "Tempo de simulacao: " << setw(2) << horas << cout.fill('0') << ":" << setw(2) << minutos << cout.fill(0) 
		<<  ":" << setw(2) <<  segundos << cout.fill(0) << endl;
		
		char output[30];
		time_t rawtime;
		time(&rawtime);
		timeinfo = localtime(&rawtime);
		stringstream s;	
		strftime(output, 30, "%H:%M:%S.%Y-%m-%d", timeinfo);
		
		CalculaMetricas();
	}
	
  /* ------------------------------------------------------------------------------------------------------- */
  
  
  bool Controller::InitVehicle(Ptr<Highway> highway, int& VID)
  {
	
		Ptr<Obstacle> ap, obst;
		  
		int apDist = highway->GetApInitDist();
		int obDist = highway->GetObInitDist();
	  
		// Simula a posY do Ap no canteiro central
		int lane = highway->GetNumberOfLanes();
	  
		// Criação de Aps no canteiro central       
		ap = CreateObject<Obstacle>();
		ap->SetIsAp(true);     
		ap->SetVehicleId(VID++);
		ap->SetReceiveCallback(highway->GetReceiveDataCallback());
		ap->SetupWifi(highway->GetWifiHelper(), highway->GetYansWifiPhyHelper(), highway->GetNqosWaveMacHelper());
		ap->SetPosition(Vector(apDist, highway->GetYForLane(lane,1), 2));
		ap->SetLength(1);
		ap->SetWidth(1);
		ap->SetDirection(0);     
		highway->AddVehicle(ap);	
		
		// Criação de obstáculos (acidente)       
		obst = CreateObject<Obstacle>();
		obst->SetVehicleId(VID++);
		obst->SetReceiveCallback(highway->GetReceiveDataCallback());
		obst->SetupWifi(highway->GetWifiHelper(), highway->GetYansWifiPhyHelper(), highway->GetNqosWaveMacHelper());
		obst->SetPosition(Vector(obDist, highway->GetYForLane(lane,1), 0));
		obst->SetLength(1);
		obst->SetWidth(1);
		obst->SetDirection(1);     
		highway->AddVehicle(obst);	  
      
		return true;
  }
  
  /* ------------------------------------------------------------------------------------------------------- */
	
	void Controller::ReceiveData (Ptr<Vehicle> veh, Ptr<const Packet> pkt, Address sender)
	{
		std::string typeMsg1, pos1, time1, payload;
		int typeMsg2, pos2;
		double time2;
		stringstream payload2;
		
		uint8_t *buffer = new uint8_t[pkt->GetSize ()];
		pkt->CopyData(buffer, pkt->GetSize ());
		payload = std::string((char*)buffer);
				
		std::size_t found1 = payload.find_first_of("-");
		std::size_t found2 = payload.find_last_of("-");
		std::size_t found3 = payload.find_first_of("_");
		std::size_t found4 = payload.find_last_of("_");
		std::size_t found5 = payload.find_first_of("*");
		std::size_t found6 = payload.find_last_of("*");
		
		typeMsg1.assign (payload, (found1+1),(found2-found1));
		pos1.assign (payload, (found3+1),(found4-found3));
		time1.assign (payload, (found5+1),(found6-found5));
		
		typeMsg2 = atoi(typeMsg1.c_str());
		pos2 = atoi(pos1.c_str());
		time2 = atof(time1.c_str());	
		
		m_receivedPackets += 1;	
		
		if((typeMsg2 == 01) and (veh->GetPosition().x > pos2) and (veh->GetStopWarning() == false))
		{
			if(veh->GetIsAp() == true)
			{
				cout <<"MENSAGEM CHEGOU NO PONTO DE ACESSO!" <<endl;
				payload2 <<"-" <<02 <<"-" <<"_" << veh->GetPosition().x <<"_" <<"*" <<Simulator::Now().GetSeconds() <<"*";
				Ptr<Packet> packet = Create<Packet> ((uint8_t*) payload2.str().c_str(), 1500);
				veh->SendTo(veh->GetBroadcastAddress(), packet);				
				payload2.str("");
			}
			else
			{
				cout <<"Msg: " <<typeMsg2 <<" - IDVeh: " <<veh->GetVehicleId() <<" - posVeh: " <<veh->GetPosition().x <<" - PosRem: " <<pos2 <<endl;
				veh->SetSendWarning(true);
			}
			
		}
		else if ((typeMsg2 == 02) and (veh->GetPosition().x < pos2) and (veh->GetStopWarning() == false))
		{
			cout <<"Msg: " <<typeMsg2 <<" - IDVeh: " <<veh->GetVehicleId() <<" - posVeh: " <<veh->GetPosition().x <<" - PosRem: " <<pos2 <<endl;
			payload2 <<"-" <<02 <<"-" <<"_" << veh->GetPosition().x <<"_" <<"*" <<Simulator::Now().GetSeconds() <<"*";
			Ptr<Packet> packet = Create<Packet> ((uint8_t*) payload2.str().c_str(), 1500);
			veh->SendTo(veh->GetBroadcastAddress(), packet);				
			payload2.str("");
			
			veh->SetSendWarning(false);
			veh->SetStopWarning(true);
		}
	}
		
}
