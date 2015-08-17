/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2010 TELEMATICS LAB, Politecnico di Bari
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

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(int argc, char* argv[])
{
	FILE *finput;
	FILE *foutput;

        int n = 0;
        double goodput;
	double sum_goodput = 0;
        double avg_goodput = 0;
        double max_goodput = 0;
        double min_goodput = 0;

	double fi = 0;

	finput=fopen(argv[1],"r");
	while (feof(finput)==0) 
          {
		fscanf(finput,"%lf\n",&goodput);
		sum_goodput += goodput;
            	
            	if(n==0){
            	    max_goodput = goodput;
            	    min_goodput = goodput;   
            	}else{
            	    if(max_goodput < goodput ){
            	        max_goodput = goodput;
            	    }
            	    if(min_goodput > goodput){
            	        min_goodput = goodput;
            	    }
            	}
	    	n+=1;
	  }

	fclose(finput);

        avg_goodput = sum_goodput / n;
        fi = 1 - ((max_goodput - min_goodput) / sum_goodput);
		
 
       printf("tot %2.9lf\n",sum_goodput);
       printf("avg %2.9lf\n",avg_goodput);
       printf("FI %2.9lf\n",fi);

	return 0;
}
