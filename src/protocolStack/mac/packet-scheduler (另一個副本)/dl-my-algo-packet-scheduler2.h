/* -*- Mode:C++; c-double my_ComputeSchedulingMetric (double max,double min,double avg)file-style:"gnu"; indent-tabs-mode:nil; -*- */
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


#ifndef  DLMYALGOPACKETSCHEDULER2_H_  //dl-my-algo-packet-scheduler
#define  DLMYALGOPACKETSCHEDULER2_H_

#include "downlink-packet-scheduler.h"
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <vector>
#include <string>
#include <string.h>
#include <map>
#include <algorithm>

class DL_my_algo_PacketScheduler2 : public DownlinkPacketScheduler {
public:
	DL_my_algo_PacketScheduler2();
	virtual ~DL_my_algo_PacketScheduler2();

        virtual void DoSchedule (void);
        void RBsAllocation ();
        void DoStopSchedule (void);

	virtual double ComputeSchedulingMetric (RadioBearer *bearer, double spectralEfficiency, int subChannel);
	double mlwdf_ComputeSchedulingMetric (RadioBearer *bearer, double spectralEfficiency, int subChannel,double credit_rate,double avg_credit,int demand);
	double ComputeSchedulingMetric1 (RadioBearer *bearer, double spectralEfficiency, int subChannel,double demand , double total,double credit_rate,double avg_credit);



private:
    int m_roundRobinId;
};

#endif /* DLMYALGOPACKETSCHEDULER_H__ */
