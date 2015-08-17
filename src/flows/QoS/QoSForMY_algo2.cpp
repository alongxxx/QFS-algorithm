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


#include "QoSForMY_algo2.h"
#include <iostream>

QoSForMY_algo2::QoSForMY_algo2()
{
  m_dropProbability = 0.005;
}

QoSForMY_algo2::QoSForMY_algo2(double targetDelay)
{
  SetMaxDelay (targetDelay);
  m_dropProbability = 0.005;
}

QoSForMY_algo2::~QoSForMY_algo2()
{}

void
QoSForMY_algo2::SetDropPorbability (double probability)
{
  m_dropProbability = probability;
}

double
QoSForMY_algo2::GetDropProbability (void) const
{
  return m_dropProbability;
}


//Debug
void
QoSForMY_algo2::Print ()
{
}
