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
#include <sstream>
#include <stdexcept>
#include <vector>
#include <string>
#include <string.h>
#include <map>
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
#include "../../../flows/QoS/QoSForM_LWDF.h"
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
    double metric = (spectralEfficiency * 180000.)
					  /
					  bearer->GetAverageTransmissionRate();
    //std::cout << "spectralEfficiency : " << spectralEfficiency << std::endl;
    //std::cout << "subChannel : " << subChannel << std::endl;
    //int nbOfRBs = GetMacEntity ()->GetDevice ()->GetPhy ()->GetBandwidthManager ()->GetUlSubChannels ().size ();
    //std::cout << "nbOfRBs= " << nbOfRBs << std::endl ;

  //metric = spectralEfficiency * 180000; 
  return metric;
}
double
DL_my_algo_PacketScheduler2::hybridSchedulingMetric (RadioBearer *bearer, double spectralEfficiency,int users){
     
     double beta;
     
     if(users <= 5){
        beta=0.2;
     }else if(users > 5 && users <= 12){
        beta=0.45;
     }else{
        beta=0.75;
     }
     double metric = beta * bearer->GetNowTransmissionRate() + 
                (1-beta) * bearer->GetNowTransmissionRate() / bearer->GetAverageTransmissionRate();

  return metric; 

}
double
DL_my_algo_PacketScheduler2::mlwdf_ComputeSchedulingMetric (RadioBearer *bearer, double spectralEfficiency, int subChannel){
     
     //double metric = (((4.0/7)*(max-avg)+(3.0/7)*(max-min))* 180000.);
       /*
   * For the M-LWDF scheduler the metric is computed
   * as follows:
   *
   * metric = -log(dropProbability)/targetDelay *
   *  			* HOL * availableRate/averageRate
   */

  double metric;

  if ((bearer->GetApplication ()->GetApplicationType () == Application::APPLICATION_TYPE_INFINITE_BUFFER)
	  ||
	  (bearer->GetApplication ()->GetApplicationType () == Application::APPLICATION_TYPE_CBR))
    {
	  metric = (spectralEfficiency * 180000.)
				/
	    	    bearer->GetAverageTransmissionRate();

#ifdef SCHEDULER_DEBUG
	  std::cout << "METRIC: " << bearer->GetApplication ()->GetApplicationID ()
			 << " " << spectralEfficiency
			 << " " << bearer->GetAverageTransmissionRate ()
			 << " --> " << metric
			 << std::endl;
#endif

    }
  else
    {

     QoSForM_LWDF *qos = (QoSForM_LWDF*) bearer->GetQoSParameters ();

     double a = (-log10 (qos->GetDropProbability())) / qos->GetMaxDelay ();
     double HOL = bearer->GetHeadOfLinePacketDelay ();

	 metric = (a * HOL)
			 *
			 ((spectralEfficiency * 180000.)
			 /
			 bearer->GetAverageTransmissionRate ());

#ifdef SCHEDULER_DEBUG
	 std::cout << "METRIC: " << bearer->GetApplication ()->GetApplicationID ()
			 << " " << a
			 << " " << Simulator::Init()->Now()
			 << " " << bearer->GetMacQueue()->Peek().GetTimeStamp()
			 << " " << HOL
			 << " " << spectralEfficiency
			 << " " << bearer->GetAverageTransmissionRate ()
			 << " --> " << metric
			 << std::endl;
#endif

    }

  return metric;

}
double
DL_my_algo_PacketScheduler2::my_ComputeSchedulingMetric (double max,double min,double avg,double fraction1,double fraction2){
     
     double metric = (((fraction1)*(max-avg)+(fraction2)*(max-min))* 180000.);

  return metric; 

}

double
DL_my_algo_PacketScheduler2::my_ComputeSchedulingMetric (double max,double min,double avg,RadioBearer *bearer,double ue_position){

    double metric;
    if ((bearer->GetApplication ()->GetApplicationType () == Application::APPLICATION_TYPE_INFINITE_BUFFER) ||
	  (bearer->GetApplication ()->GetApplicationType () == Application::APPLICATION_TYPE_CBR))
    {
	        metric = ue_position * (((4.0/7)*(max)+(3.0/7)*(max-min))* 180000.);
    }
    else
    {
          //QoSForMY_algo2 *qos = (QoSForMY_algo2*) bearer->GetQoSParameters ();   
          //double a = (-log10 (qos->GetDropProbability())) / qos->GetMaxDelay ();
          //double HOL = bearer->GetHeadOfLinePacketDelay ();
     
          //metric = (a*HOL)*((((4.0/7)*(max)+(3.0/7)*(max-min))* 180000.));//bearer->GetAverageTransmissionRate());
          metric = ue_position * ((((4.0/7)*(max)+(3.0/7)*(max-min))* 180000.));//bearer->GetAverageTransmissionRate());

    }
     return metric; 
}
     



void
DL_my_algo_PacketScheduler2::print ()
{
           
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------
//
//
//
//
//-----------------------------------------------------------        my algo        ----------------------------------------------------------------

#ifdef my_algo
void
DL_my_algo_PacketScheduler2::RBsAllocation ()
{

int count=0;
   double static count_num=0;
   double static count_num1=0;
   double static count_num2=0;
   double static total_num=0;

#ifdef SCHEDULER_DEBUG
	std::cout << " ---- DL_my_algo_PacketScheduler2::my_algo";
#endif

  FlowsToSchedule* flows = GetFlowsToSchedule ();
  
  int nbOfRBs = GetMacEntity ()->GetDevice ()->GetPhy ()->GetBandwidthManager ()->GetDlSubChannels ().size ();
    
   //get average spectral efficiency
  std::vector<double>  avg_spectral_Eff(nbOfRBs,0);  
  for (int i = 0; i < nbOfRBs; i++)
    {
	  for (int j = 0; j < flows->size (); j++)
	    {
		  avg_spectral_Eff[i] = avg_spectral_Eff[i]+ flows->at (j)->GetSpectralEfficiency ().at (i);
	    }
	    avg_spectral_Eff[i]=avg_spectral_Eff[i]/flows->size ();
    }
    
    
  //create a matrix of flow metrics
  double metrics[nbOfRBs][flows->size ()];
  for (int i = 0; i < nbOfRBs; i++)
    {
	  for (int j = 0; j < flows->size (); j++)
	    {
		  metrics[i][j] = ComputeSchedulingMetric (flows->at (j)->GetBearer (),
				                                   flows->at (j)->GetSpectralEfficiency ().at (i),
	    		                                   i);
	    }
    }

#ifdef SCHEDULER_DEBUG
  std::cout << ", available RBs " << nbOfRBs << ", flows " << flows->size () << std::endl;
  for (int ii = 0; ii < flows->size (); ii++)
    {
	  std::cout << "\t metrics for flow "
			  << flows->at (ii)->GetBearer ()->GetApplication ()->GetApplicationID () << ":";
	  for (int jj = 0; jj < nbOfRBs; jj++)
	    {
		  std::cout << " " << metrics[jj][ii];
	    }
	  std::cout << std::endl;
    }
#endif


  AMCModule *amc = GetMacEntity ()->GetAmcModule ();
  double l_dAllocatedRBCounter = 0;

  int l_iNumberOfUsers = ((ENodeB*)this->GetMacEntity()->GetDevice())->GetNbOfUserEquipmentRecords();

  bool * l_bFlowScheduled = new bool[flows->size ()];
  int l_iScheduledFlows = 0;
  std::vector<double> * l_bFlowScheduledSINR = new std::vector<double>[flows->size ()];
  for (int k = 0; k < flows->size (); k++)
  {
      l_bFlowScheduled[k] = false;
  }
  /*
  int ue_num=flows->at (1)->GetBearer ()->GetApplication ()->GetApplicationID ();
  int radius=NetworkManager::Init()->GetCellByID (0)->GetRadius();
  std::cout << "Radius : " << radius << std::endl;
  std::cout << "UE : " << ue_num << std::endl;
          
  std::vector<UserEquipment*>* users = NetworkManager::Init()->GetUserEquipmentContainer ();
  std::cout << "\tx: " << users->at(ue_num)->GetMobilityModel ()->GetAbsolutePosition()->GetCoordinateX() << std::endl;
  std::cout << "\ty: " << users->at(ue_num)->GetMobilityModel ()->GetAbsolutePosition()->GetCoordinateY() << std::endl;
  std::cout << "\tSpeedDirection: " << users->at(ue_num)->GetMobilityModel ()->GetSpeedDirection ()  << std::endl;
  */
  int radius=NetworkManager::Init()->GetCellByID (0)->GetRadius()*1000;              //get radius
  std::vector<UserEquipment*>* users = NetworkManager::Init()->GetUserEquipmentContainer();
  int nbUE=users->size();
  int nbCell = NetworkManager::Init()->GetNbCell();
  int nbFlows = total_flow::Init()->get_total_flows();  
  static double * ue_position = new double[nbUE*nbFlows*nbCell]();
  static std::vector<vector<double> >  cqi(nbFlows*nbUE*nbCell,vector<double>());
  static std::vector<vector<double> >  avg_cqi(nbFlows*nbUE*nbCell,vector<double>(2));
  static std::vector<double>  diff_cqi(nbFlows*nbUE*nbCell);
  static std::vector<vector<vector<double> > >  efficiency(nbFlows*nbUE*nbCell,vector<vector<double> >(nbOfRBs,vector<double>(2)));
  static std::vector<double> ue_trans_bit(nbUE*nbFlows*nbCell , 0.0 );
  std::vector<double> transbit_rec(nbUE*nbFlows*nbCell , 0.0 );
  static std::vector<bool> IsCell_Center(nbUE*nbFlows*nbCell , false);
  
  
  
  static bool first_time_flag=false;
  
  static double start_time=0;                                                      //get time
  double now_time=Simulator::Init()->Now();
  if((now_time-start_time)>=0 || first_time_flag==false){
        start_time=now_time;
        
        //------------------------ cell  range  -----------------------------------------------
        for (int i=0;i<nbUE*nbCell;i++)
        {
                //---------------get ue radius----------
                double x=users->at(i)->GetMobilityModel ()->GetAbsolutePosition()->GetCoordinateX();
                double y=users->at(i)->GetMobilityModel ()->GetAbsolutePosition()->GetCoordinateY();
                             
                double ue_radius=sqrt(x*x+y*y);          
                
                
                //---------------partition with radius-------------
                
                if(ue_radius <= radius*2/6){
                        ue_position[i]=( ue_radius/(radius*2.0/6) )/3.0 + 0.3;
                }else if(ue_radius > radius*2/6 && ue_radius <= radius*5/6){
                        if(users->at(i)->GetMobilityModel ()->GetSpeedDirection () >3.14 ){             //went out
                                ue_position[i]= ( (ue_radius-(radius*2.0/6)) / ((radius*5.0/6)-(radius*2.0/6)) )/3.0 + 0.6;
                        }else{                                                                          //went inside
                                ue_position[i]= ( (ue_radius-(radius*2.0/6)) / ((radius*5.0/6)-(radius*2.0/6)) )/3.0 + 0.6;
                        }
                }else{       //ue_radius > radius*5/6 && ue_radius <= radius*6/6
                        ue_position[i]= ( (ue_radius-(radius*5.0/6)) / ((radius)-(radius*5.0/6)) )/3.0 + 0.0;
                }  
                
                
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
        
        if(first_time_flag==false){
                //init avg_cqi and diff_cqi
                for (int s = 0; s < nbOfRBs; s++)
                {
                        for (int i = 0; i < nbUE ; i++)
                        {
                                avg_cqi[i][1]=flows->at (i)->GetCqiFeedbacks ().at (s);
                                diff_cqi[i]=flows->at (i)->GetCqiFeedbacks ().at (s);
                        }
                        
                        for (int i = 0; i < flows->size() ; i++)
                        {                  
                                int id=flows->at (i)->GetBearer ()->GetApplication ()->GetApplicationID ();
                                efficiency[id][s][0]=flows->at (i)->GetSpectralEfficiency ().at (s);
                                efficiency[id][s][1]=flows->at (i)->GetSpectralEfficiency ().at (s);
                        }
                        
                }
        }else{
                 //--------------get cqi avg ----------------------------
                for(int i=0;i<nbUE;i++){
                        double total=0;
                        for(int j=0;j<cqi[i].size();j++){
                                total+=cqi[i][j];                                
                        }
                        avg_cqi[i][0]=avg_cqi[i][1];
                        avg_cqi[i][1]=total/cqi[i].size();
                }
                
                //--------------get ue cqi diff--------------------------
                for(int i=0;i<nbUE;i++){
                        double last=avg_cqi[i].at(0);
                        double last2=avg_cqi[i].at(1);
                        diff_cqi[i]=last-last2 ;
                }
             
                //--------------clear cqi queue--------------------------
                for(int i=0;i<nbUE;i++){
                        cqi[i].clear();
                }
                
                //----------calculate efficiency---------------------
                for(int i=0;i<flows->size();i++){
                     for (int s = 0; s < nbOfRBs; s++)
                        {   
                                int id=flows->at (i)->GetBearer ()->GetApplication ()->GetApplicationID ();
                                efficiency[i][s][0]=efficiency[id][s][1];
                                efficiency[i][s][1]=flows->at (i)->GetSpectralEfficiency ().at (s);
                        }
                }
        }
        
        first_time_flag=true;
        for(int i=0;i<flows->size();i++)
        {
               for (int s = 0; s < nbOfRBs; s++)
               {   
                       for(int j=0;j<2;j++)
                       {                            
                              //std::cout << "efficiency["<<i<<"]["<<s<<"]["<<j<<"] : " << efficiency[i][s][j] << "\t" ;
                       }
                       //std::cout << "avg_spectral_Eff[" << i << "]: " <<avg_spectral_Eff[i] << "\t";
                       //std::cout <<std::endl;
               }
         }
        
        
         for (int s = 0; s < nbOfRBs; s++)
         {
                //std::cout << "cqi at RB " << s << " : ";
                for(int i=0;i<flows->size();i++)
                {
                    //std::cout << flows->at (i)->GetSpectralEfficiency ().at (s) << " ,";
                }
                //std::cout << std::endl;
         }
        
        /*
        //------------------
        std::cout << "---------------------------------------  now time = : "  << now_time << std::endl;
        for(int i=0;i<nbUE;i++){
                for(int j=0;j<2;j++){                            
                        std::cout << "avg_cqi["<< i << "][" << j << "] : " << avg_cqi[i].at(j) << "\t";
                }
                std::cout <<std::endl;      
        }     
        for(int i=0;i<nbUE;i++){                     
                std::cout << "diff_cqi["<< i << "] : " << diff_cqi.at(i)<< std::endl;
        }  
        //------------------
        */
       
        
  }
  
    
  //create a matrix of flow metrics
  double my_metrics[nbOfRBs][flows->size ()];
  for (int i = 0; i < nbOfRBs; i++)
    {
          //std::cout << "RB " << i << " : " ; 
	  for (int j = 0; j < flows->size (); j++)
	    {
		  //my_metrics[i][j] = my_ComputeSchedulingMetric (efficiency[j][i][1],efficiency[j][i][0],avg_spectral_Eff[i],fr1,fr2);
		  //std::cout <<"flow " << j << " has metrics : " << my_metrics[i][j] << "\t";
	    }
	    //std::cout << std::endl;
    }
    //------------------------------------test
  std::vector<double> copy_ue_trans_bit(nbUE*nbFlows*nbCell , 0.0 );
  copy_ue_trans_bit.assign(ue_trans_bit.begin(), ue_trans_bit.end()); 
        //--------------------------------test
  
  
  //RBs allocation
  for (int s = 0; s < nbOfRBs; s++)
    {
                
        for (int j = 0; j < flows->size (); j++)
	    {
		  //my_metrics[s][j] = my_ComputeSchedulingMetric (efficiency[j][s][1],efficiency[j][s][0],avg_spectral_Eff[s],flows->at (j)->GetBearer (),ue_position[j]);
		  my_metrics[s][j] = mlwdf_ComputeSchedulingMetric (flows->at (j)->GetBearer (),
				                                   flows->at (j)->GetSpectralEfficiency ().at (s),
	    		                                   s);
		  //std::cout <<"flow " << j << " has metrics : " << my_metrics[i][j] << "\t";
	    }
    
    
      if (l_iScheduledFlows == flows->size ()){
          break;
      }
      
      double targetMetric = 0;
      bool RBIsAllocated = false;
      FlowToSchedule* scheduledFlow;
      int l_iScheduledFlowIndex = 0;

      if(s==0){
           for (int k = 0; k < flows->size () ; k++)
           {
               cqi[k].push_back(flows->at (k)->GetCqiFeedbacks ().at (s));
               //cqi[k].push_back(flows->at (k)->GetSpectralEfficiency ().at (s));
           }

           
      }
    
    FlowToSchedule* scheduledFlow1;
     for (int k = 0; k < flows->size (); k++)
   {    
        if (metrics[s][k] >= targetMetric && !l_bFlowScheduled[k] && flows->at (k)->GetDataToTransmit() >0 )
        {
                targetMetric = metrics[s][k];
                RBIsAllocated = true;
                scheduledFlow1 = flows->at (k);
                l_iScheduledFlowIndex = k;
                //std::cout << "\tschedule to flow : " << flows->at (k)->GetBearer ()->GetApplication ()->GetApplicationID () << std::endl;  
        }
                   
   }
   //std::cout << "PF : " << scheduledFlow1->GetBearer ()->GetApplication ()->GetApplicationID () << "\t";
   RBIsAllocated = false;
   l_iScheduledFlowIndex = 0;
   targetMetric=my_metrics[s][0];
   for (int k = 0; k < flows->size (); k++)
   {    
        if (my_metrics[s][k] >= targetMetric && !l_bFlowScheduled[k] && flows->at (k)->GetDataToTransmit() >0 )
        {
                targetMetric = my_metrics[s][k];
                RBIsAllocated = true;
                scheduledFlow = flows->at (k);
                l_iScheduledFlowIndex = k;
                //std::cout << "\tschedule to flow : " << flows->at (k)->GetBearer ()->GetApplication ()->GetApplicationID () << std::endl;  
        }
                   
   }
   if (!RBIsAllocated){
        targetMetric = my_metrics[s][0];
        RBIsAllocated = true;
        scheduledFlow = flows->at (0);
        l_iScheduledFlowIndex = 0;
   }
   
   //---------------------------------test
        int max_edge_RB=0;
        double edge=0;
        for(int k=0;k<flows->size ();k++)
        {
                if(IsCell_Center[flows->at (k)->GetBearer ()->GetApplication ()->GetApplicationID ()]==false){
                      edge++; 
                }
        }
        if( ( edge/flows->size() ) * nbOfRBs <  1.0/5.0*nbOfRBs){
                max_edge_RB=( edge/flows->size() ) * nbOfRBs;
        } else{
                max_edge_RB=1.0/5.0*nbOfRBs;
        }
        
   
        int min=0;  
        RBIsAllocated = false;
        if(s<max_edge_RB){
            targetMetric=0;
            for(int k=0;k<flows->size ();k++)
            {
                if(metrics[s][min] >= targetMetric && !l_bFlowScheduled[min] && flows->at (min)->GetDataToTransmit() >0 
                        && IsCell_Center[flows->at (k)->GetBearer ()->GetApplication ()->GetApplicationID ()]==false)
                {
                        RBIsAllocated = true;
                        scheduledFlow = flows->at (min);
                        l_iScheduledFlowIndex = min;
                        min=k;
                }
            }
        }
        if(!RBIsAllocated)
        {
               targetMetric=my_metrics[s][0];
               for (min = 0; min < flows->size (); min++)
                {    
                        if (my_metrics[s][min] >= targetMetric && !l_bFlowScheduled[min] && flows->at (min)->GetDataToTransmit() >0 )
                        {
                                targetMetric = my_metrics[s][min];
                                RBIsAllocated = true;
                                scheduledFlow = flows->at (min);
                                l_iScheduledFlowIndex = min;
                                //std::cout << "\tschedule to flow : " << flows->at (k)->GetBearer ()->GetApplication ()->GetApplicationID () << std::endl;  
                        }                   
                }     
        }
        min=l_iScheduledFlowIndex;
        /*
        std::cout << "RB = " << s <<
                 " , MIN = " << flows->at (min)->GetBearer ()->GetApplication ()->GetApplicationID () << 
                " , transbit = " << copy_ue_trans_bit[flows->at (min)->GetBearer ()->GetApplication ()->GetApplicationID ()] << std::endl;
        
        RBIsAllocated = true;
        scheduledFlow = flows->at (min);
        l_iScheduledFlowIndex = min;
        */
   //---------------------------------test
  
   //std::cout << "My algo : " << scheduledFlow->GetBearer ()->GetApplication ()->GetApplicationID () << "\t";
   

   if(scheduledFlow1->GetBearer ()->GetApplication ()->GetApplicationID () != scheduledFlow->GetBearer ()->GetApplication ()->GetApplicationID ()){
        //std::cout << "pf's effciency : " << scheduledFlow1->GetSpectralEfficiency ().at (s) << "\t" << "my effciency : " << scheduledFlow->GetSpectralEfficiency ().at (s) << "\n";
        if(scheduledFlow1->GetSpectralEfficiency ().at (s) > scheduledFlow->GetSpectralEfficiency ().at (s) ){
               count_num++; 
        }else if(scheduledFlow1->GetSpectralEfficiency ().at (s) == scheduledFlow->GetSpectralEfficiency ().at (s)){
                count_num1++;
        }else{
                count_num2++;
        }
   }else{
        count_num1++;
        //std::cout << "\n";
   }
   total_num++;
                                
      if (RBIsAllocated)
        {
          l_dAllocatedRBCounter++;

          scheduledFlow->GetListOfAllocatedRBs()->push_back (s); // the s RB has been allocated to that flow!          
          
          //std::cout << "\tRB " << s << " schedule to flow : " << scheduledFlow->GetBearer ()->GetApplication ()->GetApplicationID () << "\tcqi : "<< scheduledFlow->GetSpectralEfficiency ().at (s)<<std::endl; 
          /*
          int ue_num=scheduledFlow->GetBearer ()->GetApplication ()->GetApplicationID ();
          int radius=NetworkManager::Init()->GetCellByID (0)->GetRadius();
          std::cout << "Radius : " << radius << std::endl;
          std::cout << "UE : " << ue_num << std::endl;
          
          std::vector<UserEquipment*>* users = NetworkManager::Init()->GetUserEquipmentContainer ();
          std::cout << "\tx: " << users->at(ue_num)->GetMobilityModel ()->GetAbsolutePosition()->GetCoordinateX() << std::endl;
          std::cout << "\ty: " << users->at(ue_num)->GetMobilityModel ()->GetAbsolutePosition()->GetCoordinateY() << std::endl;
          */
#ifdef SCHEDULER_DEBUG
          std::cout << "\t *** RB " << s << " assigned to the "
                  " flow " << scheduledFlow->GetBearer ()->GetApplication ()->GetApplicationID ()
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
          
          //-------------------------------------test
                int last_transportBlockSize = amc->GetTBSizeFromMCS (mcs, scheduledFlow->GetListOfAllocatedRBs ()->size ()-1);
                int diff = transportBlockSize - last_transportBlockSize;
                copy_ue_trans_bit[scheduledFlow->GetBearer ()->GetApplication ()->GetApplicationID ()] += diff;
                
          //-------------------------------------test
         

        }
     
    }
    transbit_rec.clear();
   
    //std::cout << "total_tr_block : " << total_tr_block<<std::endl;
    
       //std::cout << "count1 : " << count1 << "  count2 : " << count2<<"   total : " << total << " count1 use rate : " << count1/total <<" count2 use rate : " << count2/total <<std::endl;
       //std::cout << "cqi win rate : " << count_cqi/count_cqi_total <<std::endl;
       //std::cout << "count : " << count_num <<"   total : " << total_num << "\tcqi win rate : " << count_num/total_num <<"\tcqi equil rate : " << count_num1/total_num <<"\tcqi lose rate : " << count_num2/total_num <<std::endl;
       
       

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
        ue_trans_bit[flow->GetBearer ()->GetApplication ()->GetApplicationID()]+=bitsToTransmit;                 //new add
        transbit_rec[flow->GetBearer ()->GetApplication ()->GetApplicationID()]=bitsToTransmit;
        
        
        

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
    
    FlowsToSchedule* flowss = GetFlowsToSchedule ();
     /* for(int hh=0;hh<nbUE*nbFlows*nbCell;hh++)
      {
                std::cout << hh << " : ue_trans_bit[" << hh << "] = " << 
                        ue_trans_bit[hh];
                if(transbit_rec[hh]>0)
                        std::cout << "   add  " << transbit_rec[hh];
                std::cout << std::endl;       
      }
        */

  if (pdcchMsg->GetMessage()->size () > 0)
    {
      GetMacEntity ()->GetDevice ()->GetPhy ()->SendIdealControlMessage (pdcchMsg);
    }
  delete pdcchMsg;
}
#endif

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

