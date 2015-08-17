/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2010,2011,2012,2013 TELEMATICS LAB, Politecnico di Bari
 *
 * This file is part of LTE-Sim
 *
 * LTE-Sim is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 3 as
 * published by the Free Software Foundation;
 *
 * LTE-Sim is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with LTE-Sim; if not, see <http://www.gnu.org/licenses/>.
 *
 * Author: Giuseppe Piro <g.piro@poliba.it>
 */
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <sstream>
#include <stdexcept>
#include <vector>
#include <string>
#include <string.h>
#include <map>
#include <cmath>
#include <algorithm>
#include "dl-my-algo-packet-scheduler2.h"
#include "../mac-entity.h"
#include "../../packet/Packet.h"
#include "../../packet/packet-burst.h"
#include "../../../device/NetworkNode.h"
#include "../../../flows/radio-bearer.h"
#include "../../../protocolStack/rrc/rrc-entity.h"
#include "../../../flows/application/Application.h"
#include "../../../device/ENodeB.h"
#include "../../../protocolStack/mac/AMCModule.h"
#include "../../../phy/lte-phy.h"
#include "../../../core/spectrum/bandwidth-manager.h"
#include "../../../core/idealMessages/ideal-control-messages.h"
#include "../../../utility/eesm-effective-sinr.h"
#include "../../../flows/MacQueue.h"
#include "../../../componentManagers/NetworkManager.h"
#include "../../../device/UserEquipment.h"
#include "../../../flows/total_flow.h"
#include "../../../flows/QoS/QoSForMY_algo2.h"
#define my_algo
//#define round_robin
//#define Max_cqi
//#define SCHEDULER_DEBUG

DL_my_algo_PacketScheduler2::DL_my_algo_PacketScheduler2()
{
  SetMacEntity (0);
  CreateFlowsToSchedule ();

  m_roundRobinId = 0;
}

DL_my_algo_PacketScheduler2::~DL_my_algo_PacketScheduler2()
{
  Destroy ();
}
double
DL_my_algo_PacketScheduler2::ComputeSchedulingMetric (RadioBearer *bearer, double spectralEfficiency, int subChannel)
{
  //double metric;
  //metric = spectralEfficiency * 180000;
    double metric = (spectralEfficiency * 180000.)/
	    	    bearer->GetAverageTransmissionRate();
					  
    //std::cout << "spectralEfficiency : " << spectralEfficiency << std::endl;
    //std::cout << "subChannel : " << subChannel << std::endl;
    //int nbOfRBs = GetMacEntity ()->GetDevice ()->GetPhy ()->GetBandwidthManager ()->GetUlSubChannels ().size ();
    //std::cout << "nbOfRBs= " << nbOfRBs << std::endl ;
        

  //metric = spectralEfficiency * 180000; 
  return metric;
}
double
DL_my_algo_PacketScheduler2::ComputeSchedulingMetric1 (RadioBearer *bearer, double spectralEfficiency, int subChannel,double demand , double total,double credit_rate,double avg_credit)
{
  //double metric;
  //metric = spectralEfficiency * 180000;
    double metric = (spectralEfficiency * 180000.)/
	    	    bearer->GetAverageTransmissionRate();
    					  
  return metric*(2-credit_rate);
}
double
DL_my_algo_PacketScheduler2::mlwdf_ComputeSchedulingMetric (RadioBearer *bearer, double spectralEfficiency,int subChannel,double credit_rate,double avg_credit ,int demand){
     
     //double metric = (((4.0/7)*(max-avg)+(3.0/7)*(max-min))* 180000.);
       /*
   * For the M-LWDF scheduler the metric is computed
   * as follows:
   *
   * metric = -log(dropProbability)/targetDelay *
   *  			* HOL * availableRate/averageRate
   */
  double demand_ratio=0;
  double transbit=(spectralEfficiency * 180000.)/1000;          //convert to kbps
  demand_ratio=demand/transbit;
  if(transbit > 300){
        if(demand > 300 ){     
           demand_ratio=1;
        }else{
              demand_ratio=demand/transbit;  
        }
  }
  if(transbit < 300 && transbit > 150){
        if(demand > 300){
           demand_ratio=0.8;
        }else if(demand < 300 && demand > 150){     
           if(transbit > demand){
                demand_ratio=demand/transbit;
           }else{
                demand_ratio=1;     
           }
        }else{
           demand_ratio=demand/transbit;
        }
  }
  if(transbit < 150 ){
        if(demand < 150){
                demand_ratio=1;
        }else{
                demand_ratio=0.8;
        }
  }
  if(transbit == 0)
        demand_ratio=0;

  double metric;
  
        if ((bearer->GetApplication ()->GetApplicationType () == Application::APPLICATION_TYPE_INFINITE_BUFFER)
	         ||
        	  (bearer->GetApplication ()->GetApplicationType () == Application::APPLICATION_TYPE_CBR))
          {
	          metric = (spectralEfficiency * 180000.)
	        			/
        	    	    bearer->GetAverageTransmissionRate();
          }
         else
           {
        
           QoSForMY_algo2 *qos = (QoSForMY_algo2*) bearer->GetQoSParameters ();

           double a = (-log10 (qos->GetDropProbability())) / qos->GetMaxDelay ();
           double HOL = bearer->GetHeadOfLinePacketDelay ();

          if(bearer->GetApplication ()->GetApplicationType () == Application::APPLICATION_TYPE_TRACE_BASED){
                        metric =  (a * HOL)
	        		 *
	        		 (spectralEfficiency * 180000.);
          }
          if(bearer->GetApplication ()->GetApplicationType () == Application::APPLICATION_TYPE_VOIP){
                        metric =  (a * HOL)
	        		 *
	        		 (spectralEfficiency * 180000.);
          }
                
          }
          if(avg_credit > 1.5){
                metric = metric*(2-credit_rate)*(2-credit_rate);
          }else{
                metric = metric*(2-credit_rate);
          }
        
  //cout << "RB= " << subChannel << " "<<bearer->GetApplication ()->GetApplicationType ()<<"\t,demand = " << demand << "\t,transbit = " << transbit << "\t,demand_ratio = " << std::left << setw(12) <<demand_ratio << "\t,metric = " << std::left << setw(12) << metric << "\ttotal metric = " << std::left << setw(12) << metric*demand_ratio << ",delay = " << std::left << setw(12) << bearer->GetHeadOfLinePacketDelay () << ",credit = "<< 2-credit_rate << endl;
  return metric*demand_ratio;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------
//
//
//
//
//-----------------------------------------------------------        my algo2        ----------------------------------------------------------------

void
DL_my_algo_PacketScheduler2::RBsAllocation ()
{
#ifdef SCHEDULER_DEBUG
	std::cout << " ---- DL_my_algo_PacketScheduler2::my_algo";
#endif

  FlowsToSchedule* flows = GetFlowsToSchedule ();
  
  int nbOfRBs = GetMacEntity ()->GetDevice ()->GetPhy ()->GetBandwidthManager ()->GetDlSubChannels ().size ();

  //create a matrix of flow metrics
  double metrics[nbOfRBs][flows->size ()];
  /*
  for (int i = 0; i < nbOfRBs; i++)
    {
	  for (int j = 0; j < flows->size (); j++)
	    {
		  metrics[i][j] = ComputeSchedulingMetric (flows->at (j)->GetBearer (),
				                                   flows->at (j)->GetSpectralEfficiency ().at (i),
	    		                                   i);
	    }
    }*/

#ifdef SCHEDULER_DEBUG
  std::cout << ", available RBs " << nbOfRBs << ", flows " << flows->size () << std::endl;
  /*for (int ii = 0; ii < flows->size (); ii++)
    {
	  std::cout << "\t metrics for flow "
			  << flows->at (ii)->GetBearer ()->GetApplication ()->GetApplicationID () << ":";
	  for (int jj = 0; jj < nbOfRBs; jj++)
	    {
		  std::cout << " " << metrics[jj][ii];
	    }
	  std::cout << std::endl;
    }*/
#endif


  AMCModule *amc = GetMacEntity ()->GetAmcModule ();
  double l_dAllocatedRBCounter = 0;

  int l_iNumberOfUsers = ((ENodeB*)this->GetMacEntity()->GetDevice())->GetNbOfUserEquipmentRecords();

  bool * l_bFlowScheduled = new bool[flows->size ()];
  double * cqi = new double[flows->size ()];
  int l_iScheduledFlows = 0;
  std::vector<double> * l_bFlowScheduledSINR = new std::vector<double>[flows->size ()];
  
  int radius=NetworkManager::Init()->GetCellByID (0)->GetRadius()*1000;              //get radius
  std::vector<UserEquipment*>* users = NetworkManager::Init()->GetUserEquipmentContainer();
  int nbUE=users->size();
  int nbCell = NetworkManager::Init()->GetNbCell();
  int nbFlows = total_flow::Init()->get_total_flows();
  const double guarantee_MB=total_flow::Init()->get_data_rate();        
  const double guarantee_bit = (guarantee_MB*1000*8.0)/100 ;            //bit per TTI
  static std::vector<bool> IsCell_Center(nbUE*nbFlows , false);
  static std::vector<double> credit(nbUE,0.0);
  vector<bool> flag(nbUE*nbFlows, false);
  
  for (int i=0;i<nbUE;i++)
  {
        //---------------get ue radius----------
        double x=users->at(i)->GetMobilityModel ()->GetAbsolutePosition()->GetCoordinateX();    //ue x,y position
        double y=users->at(i)->GetMobilityModel ()->GetAbsolutePosition()->GetCoordinateY();
        
        double x1=users->at(i)->GetCell () ->GetCellCenterPosition ()->GetCoordinateX ();       //enb x,y position
        double y1=users->at(i)->GetCell () ->GetCellCenterPosition ()->GetCoordinateY ();
                             
        double ue_radius=sqrt( (x-x1)*(x-x1) + (y-y1)*(y-y1) );          
        //Enb ID = users->at(i)->GetCell ()->GetIdCell()
        //UEid = users->at(i)->GetIDNetworkNode()
                        
        if(ue_radius < radius*2.0/3){
                for(int k=i*nbFlows;k<(i+1)*nbFlows;k++){                             
                        IsCell_Center[k]=true;
                }
        }else{
                for(int k=i*nbFlows;k<(i+1)*nbFlows;k++){                           
                        IsCell_Center[k]=false;
                }
        }           
  }

  //RBs allocation
  std::vector<vector<double> >  my_metrics(nbOfRBs, vector<double>(flows->size ()));
  
  int max_edge_RB=0;
        double edge_demand_bit=0;
        double center_demand_bit=0;
        double total_demand_bit=0;
        for(int k=0;k<flows->size ();k++)
        {
                if(IsCell_Center[flows->at (k)->GetBearer ()->GetApplication ()->GetApplicationID ()]==false){
                      edge_demand_bit+=flows->at (k)->GetDataToTransmit(); 
                }else{
                        center_demand_bit+=flows->at (k)->GetDataToTransmit();
                }
        }
        if( ( edge_demand_bit/(edge_demand_bit + center_demand_bit) ) * nbOfRBs <  1.0/5.0*nbOfRBs){
                max_edge_RB=( edge_demand_bit/( edge_demand_bit + center_demand_bit) ) * nbOfRBs;
        } else{
                max_edge_RB=1.0/5.0*nbOfRBs;
        }
        total_demand_bit = edge_demand_bit + center_demand_bit;
        
   vector<double> copy_credit(nbUE , 0.0 );
   copy_credit.assign(credit.begin(), credit.end()); 
   double min= *min_element(copy_credit.begin(), copy_credit.end());
   for(int i=0;i<nbUE;i++){
        copy_credit[i]-=min;
   }

   double max= *max_element(copy_credit.begin(), copy_credit.end());
   if(max==0){
        max=1;
   }
   double sum_credit=0;
   for(int i=0;i<nbUE;i++){
        //cout << "copy_credit[" << i << "] = " << copy_credit[i]/max << endl;
        sum_credit+=(2-copy_credit[i]/max);
   }

  for (int s = 0; s < nbOfRBs; s++)
  {
      for (int j = 0; j < flows->size (); j++)
      {            
	        my_metrics[s][j] = mlwdf_ComputeSchedulingMetric (flows->at (j)->GetBearer (),
	                                   flows->at (j)->GetSpectralEfficiency ().at (s),
		                                   s,copy_credit[flows->at (j)->GetBearer ()->GetApplication ()->GetApplicationID ()/nbFlows]/max ,
                                                      sum_credit/nbUE ,flows->at (j)->GetDataToTransmit() );
      }
      
      for (int j = 0; j < flows->size (); j++)
      {
                metrics[s][j] = ComputeSchedulingMetric1 (flows->at (j)->GetBearer (),
                                           flows->at (j)->GetSpectralEfficiency ().at (s),
   		                                   s,flows->at (j)->GetDataToTransmit(),
		                                   total_demand_bit,copy_credit[flows->at (j)->GetBearer ()->GetApplication ()->GetApplicationID ()/nbFlows]/max ,
                                                      sum_credit/nbUE);
      }
	    
      if (l_iScheduledFlows == flows->size ())
          break;

      double targetMetric = 0;
      bool RBIsAllocated = false;
      FlowToSchedule* scheduledFlow;
      int l_iScheduledFlowIndex = 0;
      
      //std::cout << "avg_cqi = " << avg_cqi << std::endl;

      static int last=0;
      RBIsAllocated = false;
      if(s<max_edge_RB)
      {
            targetMetric=-1;
            for(int k=last;k<last+flows->size ();k++)
            {
                int iter=k;
                if(iter>=flows->size ()){
                    iter%=flows->size ();    
                }            
                if(metrics[s][iter] > targetMetric && !l_bFlowScheduled[iter] && flows->at (iter)->GetDataToTransmit() >0 
                        && IsCell_Center[flows->at (iter)->GetBearer ()->GetApplication ()->GetApplicationID ()]==false)
                {
                        targetMetric=metrics[s][iter];
                        RBIsAllocated = true;
                        scheduledFlow = flows->at (iter);
                        l_iScheduledFlowIndex = iter;
                }
            }
            if(!RBIsAllocated)
            {
                targetMetric=-1;
                for(int k=last;k<last+flows->size ();k++)
                {
                        int iter=k;
                        if(iter>=flows->size ()){
                                iter%=flows->size ();    
                        }
                        if(my_metrics[s][iter] > targetMetric && !l_bFlowScheduled[iter] && flows->at (iter)->GetDataToTransmit() >0 )
                        {
                                targetMetric=my_metrics[s][iter];
                                RBIsAllocated = true;
                                scheduledFlow = flows->at (iter);
                                l_iScheduledFlowIndex = iter;
                        }
                }        
            }
      }else
      {
           targetMetric=-1;
           l_iScheduledFlowIndex=0;
           for (int k = last; k < last+flows->size (); k++)
           {    
               int iter=k;
               if(iter>=flows->size ()){
                        iter%=flows->size ();    
               }
               if (my_metrics[s][iter] > targetMetric && !l_bFlowScheduled[iter] && flows->at (iter)->GetDataToTransmit() >0 )
               {
                        targetMetric = my_metrics[s][iter];
                        RBIsAllocated = true;
                        scheduledFlow = flows->at (iter);
                        l_iScheduledFlowIndex = iter;               
                        //std::cout << "\tschedule to flow : " << flows->at (k)->GetBearer ()->GetApplication ()->GetApplicationID () << std::endl;  
               }                   
           }     
      }
      last=l_iScheduledFlowIndex+1;

        
      if (RBIsAllocated)
        {
          l_dAllocatedRBCounter++;

          scheduledFlow->GetListOfAllocatedRBs()->push_back (s); // the s RB has been allocated to that flow!

#ifdef SCHEDULER_DEBUG
         /* std::cout << "\t *** RB " << s << " assigned to the "
                  " flow " << scheduledFlow->GetBearer ()->GetApplication ()->GetApplicationID ()
                  << std::endl;*/
            std::cout << "\t *** RB " << s << " assigned to the "
                  " flow " << scheduledFlow->GetBearer ()->GetApplication ()->GetApplicationID ()
                  << "  cqi   " << scheduledFlow->GetCqiFeedbacks ().at (s)
                  << "  cell_center  " << IsCell_Center[scheduledFlow->GetBearer ()->GetApplication ()->GetApplicationID ()]
                  << std::endl;
#endif
          double sinr = amc->GetSinrFromCQI (scheduledFlow->GetCqiFeedbacks ().at (s));
          l_bFlowScheduledSINR[l_iScheduledFlowIndex].push_back(sinr);

          double effectiveSinr = GetEesmEffectiveSinr (l_bFlowScheduledSINR[l_iScheduledFlowIndex]);
          int mcs = amc->GetMCSFromCQI (amc->GetCQIFromSinr (effectiveSinr));
          int transportBlockSize = amc->GetTBSizeFromMCS (mcs, scheduledFlow->GetListOfAllocatedRBs ()->size ());
          if (transportBlockSize >= scheduledFlow->GetDataToTransmit() * 8)
          {
              l_bFlowScheduled[l_iScheduledFlowIndex] = true;
              l_iScheduledFlows++;
          }
        }
    }

  delete [] cqi;
  delete [] l_bFlowScheduled;
  delete [] l_bFlowScheduledSINR;


  //Finalize the allocation
  PdcchMapIdealControlMessage *pdcchMsg = new PdcchMapIdealControlMessage ();

  for (FlowsToSchedule::iterator it = flows->begin (); it != flows->end (); it++)
    {
      FlowToSchedule *flow = (*it);
      if (flow->GetListOfAllocatedRBs ()->size () > 0)
        {
          //this flow has been scheduled
          std::vector<double> estimatedSinrValues;
          for (int rb = 0; rb < flow->GetListOfAllocatedRBs ()->size (); rb++ )

            {
              double sinr = amc->GetSinrFromCQI (
                      flow->GetCqiFeedbacks ().at (flow->GetListOfAllocatedRBs ()->at (rb)));

              estimatedSinrValues.push_back (sinr);
            }

          //compute the effective sinr
          double effectiveSinr = GetEesmEffectiveSinr (estimatedSinrValues);

          //get the MCS for transmission

          int mcs = amc->GetMCSFromCQI (amc->GetCQIFromSinr (effectiveSinr));

          //define the amount of bytes to transmit
          //int transportBlockSize = amc->GetTBSizeFromMCS (mcs);
          int transportBlockSize = amc->GetTBSizeFromMCS (mcs, flow->GetListOfAllocatedRBs ()->size ());
          double bitsToTransmit = transportBlockSize;
          flow->UpdateAllocatedBits (bitsToTransmit);

#ifdef SCHEDULER_DEBUG
		  std::cout << "\t\t --> flow "	<< flow->GetBearer ()->GetApplication ()->GetApplicationID ()
				  << " has been scheduled: " <<
				  "\n\t\t\t nb of RBs " << flow->GetListOfAllocatedRBs ()->size () <<
				  "\n\t\t\t effectiveSinr " << effectiveSinr <<
				  "\n\t\t\t tbs " << transportBlockSize <<
				  "\n\t\t\t bitsToTransmit " << bitsToTransmit
				  << std::endl;
#endif

                credit[flow->GetBearer ()->GetApplication ()->GetApplicationID()/nbFlows]+=bitsToTransmit;               //new add

		  //create PDCCH messages
		  for (int rb = 0; rb < flow->GetListOfAllocatedRBs ()->size (); rb++ )
		    {
			  pdcchMsg->AddNewRecord (PdcchMapIdealControlMessage::DOWNLINK,
					  flow->GetListOfAllocatedRBs ()->at (rb),
									  flow->GetBearer ()->GetDestination (),
									  mcs);
		    }
	    }
    }

  if (pdcchMsg->GetMessage()->size () > 0)
    {
      GetMacEntity ()->GetDevice ()->GetPhy ()->SendIdealControlMessage (pdcchMsg);
    }
  delete pdcchMsg;
        
        for (int i = 0; i< nbUE ; i++ )                         //new add
	{
	        credit[i]-=guarantee_bit;
	}       
}

void
DL_my_algo_PacketScheduler2::DoSchedule (void)
{
#ifdef SCHEDULER_DEBUG
	std::cout << "Start DL packet scheduler for node "
			<< GetMacEntity ()->GetDevice ()->GetIDNetworkNode()<< std::endl;
#endif

  UpdateAverageTransmissionRate ();
  CheckForDLDropPackets ();
  SelectFlowsToSchedule ();

  if (GetFlowsToSchedule ()->size() == 0)
	{}
  else
	{
	  RBsAllocation ();
	}

  StopSchedule ();
}

void
DL_my_algo_PacketScheduler2::DoStopSchedule (void)
{
#ifdef SCHEDULER_DEBUG
  std::cout << "\t Creating Packet Burst" << std::endl;
#endif

  PacketBurst* pb = new PacketBurst ();

  //Create Packet Burst
  FlowsToSchedule *flowsToSchedule = GetFlowsToSchedule ();

  for (FlowsToSchedule::iterator it = flowsToSchedule->begin (); it != flowsToSchedule->end (); it++)
    {
	  FlowToSchedule *flow = (*it);

	  int availableBytes = flow->GetAllocatedBits ()/8;

	  if (availableBytes > 0)
	    {

		  flow->GetBearer ()->UpdateTransmittedBytes (availableBytes);

#ifdef SCHEDULER_DEBUG
	      std::cout << "\t  --> add packets for flow "
	    		  << flow->GetBearer ()->GetApplication ()->GetApplicationID () << std::endl;
#endif

	      RlcEntity *rlc = flow->GetBearer ()->GetRlcEntity ();
	      PacketBurst* pb2 = rlc->TransmissionProcedure (availableBytes);

#ifdef SCHEDULER_DEBUG
	      std::cout << "\t\t  nb of packets: " << pb2->GetNPackets () << std::endl;
#endif

	      if (pb2->GetNPackets () > 0)
	        {
	    	  std::list<Packet*> packets = pb2->GetPackets ();
	    	  std::list<Packet* >::iterator it;
	    	  for (it = packets.begin (); it != packets.end (); it++)
	    	    {
#ifdef SCHEDULER_DEBUG
	    		  std::cout << "\t\t  added packet of bytes " << (*it)->GetSize () << std::endl;
	    		  //(*it)->Print ();
#endif

	    		  Packet *p = (*it);
	    		  pb->AddPacket (p->Copy ());
	    	    }
	        }
	      delete pb2;
	    }
	  else
	    {}
    }

  //UpdateAverageTransmissionRate ();

  //SEND PACKET BURST

#ifdef SCHEDULER_DEBUG
  if (pb->GetNPackets () == 0)
    std::cout << "\t Send only reference symbols" << std::endl;
#endif

  GetMacEntity ()->GetDevice ()->SendPacketBurst (pb);
}
