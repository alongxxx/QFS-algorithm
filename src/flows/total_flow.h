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

#ifndef TOTALFLOW_H_
#define TOTALFLOW_H_

#include <iostream>

using namespace std;

class total_flow {
private:
        int nbVoIP;
        int nbVideo;
        int nbBE;
        int nbCBR;
        double data_rate;
        static total_flow *ptr;

public:
	total_flow();
	total_flow(int VoIP,int Video,int BE,int CBR,double demand_data_Rate);
	
	virtual ~total_flow();
	
	static total_flow*
	Init (void)
	  {
		if (ptr==NULL)
	      {
		    ptr = new total_flow;
	   	  }
		return ptr;
	  }
	
	int get_nbVoIP();
	int get_nbVideo();
	int get_nbBE();
	int get_nbCBR();
	int get_total_flows();
        double get_data_rate();
	void set_all_flows(int VoIP,int Video,int BE,int CBR,double demand_data_Rate);
	
	
};

#endif /* TOTALFLOW_H_ */
