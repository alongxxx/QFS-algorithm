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


#include "total_flow.h"

total_flow* total_flow::ptr=NULL;

total_flow::total_flow()
{}

total_flow::total_flow(int VoIP,int Video,int BE,int CBR,double demand_data_Rate)
{
        nbVoIP=VoIP;
        nbVideo=Video;
        nbBE=BE;
        nbCBR=CBR;
        data_rate=demand_data_Rate;
}

total_flow::~total_flow()
{}

int 
total_flow::get_nbVoIP(){
       return nbVoIP; 
};

int 
total_flow::get_nbVideo(){
       return nbVideo; 
};

int 
total_flow::get_nbBE(){
       return nbBE; 
};

int 
total_flow::get_nbCBR(){
       return nbCBR; 
};

int 
total_flow::get_total_flows(){
       return (nbVoIP+nbVideo+nbBE+nbCBR); 
};

double 
total_flow::get_data_rate(){
       return data_rate; 
};

void total_flow::set_all_flows(int VoIP,int Video,int BE,int CBR,double demand_data_Rate){
        nbVoIP=VoIP;
        nbVideo=Video;
        nbBE=BE;
        nbCBR=CBR;
        data_rate=demand_data_Rate;
};




