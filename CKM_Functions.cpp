
//
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "CKM_Model.h"

//
void createLogCKM()
{
	FILE * fp = fopen("log_optimize_ckm.txt","w");
	fclose(fp);
}
void printStrToLogCKM(char * str)
{
	FILE * fp = fopen("log_optimize_ckm.txt","a");
	fprintf(fp,"%s",str);
	fclose(fp);
}
//


// utility functions
int FunctionClusteringCKM(CKM_Model & ckm, FloatMat Samples, int NumCenters)
{
	//
	int MaxIter = ckm.MaxIter;
	float loss_tol = ckm.loss_tol;
	//
	int FlagCenterType = ckm.FlagCenterType;
	//

	//
	ckm.setNumCenters(NumCenters);    //
	ckm.setMatSamples(Samples);          //
	//
	int NumSamples, NumFeatures;
	Samples.getMatSize(NumSamples, NumFeatures);
	//
	FloatMat * CentersReserved = new FloatMat[NumCenters];
	//
	float loss = 0;   //
	//

	//
	if (FlagCenterType == ckm.CKM_MEDIAN)
	{
		// initialize
		ckm.initializeCentersL1();    // L1
		//
		for (int c = 0; c < NumCenters; c++)
		{
			CentersReserved[c].copyFrom(ckm.Centers[c]);   // reserve
		}
		//
		// optimize
		int iter = 0;
		while (iter++ < MaxIter)
		{
			// classify samples
			ckm.classifySamplesL1();   // L1

			// update centers
			ckm.updateCentersMedians();    //

			// loss
			loss = 0;
			//
			for (int c = 0; c < NumCenters; c++)
			{
				loss += ckm.distanceFunctionL1(CentersReserved[c].data, ckm.Centers[c].data, NumFeatures);   // L1
				//
				CentersReserved[c].copyFrom(ckm.Centers[c]);   // reserve

			}// for c

			// criteria
			if (loss < loss_tol)
			{
				printf("loss < loss_tol = %f.\n", loss_tol);

				break; // while iter
			}

		}// while iter
		//
		if (iter >= MaxIter) printf("iter >= MaxIter = %d.\n", MaxIter);
		//
	}
	else if (FlagCenterType == ckm.CKM_MEAN)
	{
		// initialize
		ckm.initializeCentersL2();    // L2
		//
		for (int c = 0; c < NumCenters; c++)
		{
			CentersReserved[c].copyFrom(ckm.Centers[c]);   // reserve
		}
		//
		// optimize
		int iter = 0;
		while (iter++ < MaxIter)
		{
			// classify samples
			ckm.classifySamplesL2();     // L2

			// update centers
			ckm.updateCentersMeans();   //

			// loss
			loss = 0;
			//
			for (int c = 0; c < NumCenters; c++)
			{
				loss += ckm.distanceFunctionL2(CentersReserved[c].data, ckm.Centers[c].data, NumFeatures);   // L2
				//
				CentersReserved[c].copyFrom(ckm.Centers[c]);   // reserve

			}// for c

			// criteria
			if (loss < loss_tol)
			{
				printf("loss < loss_tol = %f.\n", loss_tol);

				break; // while iter
			}

		}// while iter
		//
		if (iter >= MaxIter) printf("iter >= MaxIter = %d.\n", MaxIter);
		//
	}
	else if (FlagCenterType == ckm.CKM_MEDOID)
	{
		printf("Error, FlagCenterType == ckm.CKM_MEDOID NOT implemented.\n");
	}
	else
	{
		printf("Error, FlagCenterType %d NOT defined.\n", ckm.FlagCenterType);

	}// if FlagCenterType

	//
	delete [] CentersReserved;

	//
	return 0;

}
//
int FunctionClassifyingCKM(CKM_Model & ckm)
{
	// classify
	if (ckm.FlagCenterType == ckm.CKM_MEDIAN || ckm.FlagCenterType == ckm.CKM_MEDOID)
	{
		ckm.classifySamplesL1();
	}
	else if (ckm.FlagCenterType == ckm.CKM_MEAN)
	{
		ckm.classifySamplesL2();
	}
	else
	{
		printf("Error, ckm.FlagCenterType %d NOT defined.\n", ckm.FlagCenterType);
		//
		return -1;
	}

	//
	return 0;
}
//
int FunctionSilhouetteCKM(CKM_Model & ckm)
{
	// classified

	// calculate distances
	int NumSamples, NumFeatures;
	ckm.Samples.getMatSize(NumSamples, NumFeatures);
	//
	FloatMat MatDistances(NumSamples, NumSamples);
	//
	float * distance_start = MatDistances.data;
	float * distance_curr = NULL;
	//
	float * data_sample = ckm.Samples.data;
	float * data_inner = NULL;
	//
	if (ckm.FlagCenterType == ckm.CKM_MEDIAN || ckm.FlagCenterType == ckm.CKM_MEDOID)
	{
		// L1
		for (int s = 0; s < NumSamples; s++, data_sample += NumFeatures, distance_start += NumSamples)
		{
			distance_curr = distance_start;
			//
			data_inner = data_sample;
			//
			for (int s_inner = 0; s_inner < s; s_inner++, data_inner += NumFeatures, distance_curr++)
			{
				distance_curr[0] = ckm.distanceFunctionL1(data_sample, data_inner, NumFeatures);   // L1
			}
		}// for s
	}
	else if (ckm.FlagCenterType == ckm.CKM_MEAN)
	{
		// L2
		for (int s = 0; s < NumSamples; s++, data_sample += NumFeatures, distance_start += NumSamples)
		{
			distance_curr = distance_start;
			//
			data_inner = data_sample;
			//
			for (int s_inner = 0; s_inner < s; s_inner++, data_inner += NumFeatures, distance_curr++)
			{
				distance_curr[0] = ckm.distanceFunctionL2(data_sample, data_inner, NumFeatures);   // L2
			}
		}// for s
	}
	else
	{
		printf("Error, ckm.FlagCenterType %d NOT defined.\n", ckm.FlagCenterType);
		//
		return -1;
	}

	//
	int NumCenters = ckm.getNumCenters();
	float * SumDistances = new float[NumCenters];
	//
	int type;
	float ad, min_ad_others;
	//

	// calculate silhouette
	distance_start = MatDistances.data;
	//
	for (int s = 0; s < NumSamples; s++, distance_start += NumSamples)
	{
		// initialize sum array
		for (int c = 0; c < NumCenters; c++) SumDistances[c] = 0;

		// sum
		distance_curr = distance_start;
		//
		for (int s_inner = 0; s_inner < s; s_inner++, distance_curr++)
		{
			SumDistances[ckm.ArrayClassifyings[s_inner]] += distance_curr[0];
		}
		//
		for (int s_inner = s+1; s < NumSamples; s_inner++, distance_curr += NumSamples)
		{
			SumDistances[ckm.ArrayClassifyings[s_inner]] += distance_curr[0];
		}

		// calculate silhouette
		type = ckm.ArrayClassifyings[s];
		//
		min_ad_others = 100000000;
		for (int c = 0; c < NumCenters; c++)
		{
			if (c != type)
			{
				ad = SumDistances[c] / ckm.NumDistributed[c];
				//
				if (ad < min_ad_others) min_ad_others = ad;
			}
		}
		//
		// ad_same
		ad = SumDistances[type] / ckm.NumDistributed[type];
		//
		// silhouette
		if (min_ad_others > ad)
		{
			ckm.ArraySilhouettes[s] = 1 - ad/min_ad_others;
		}
		else
		{
			ckm.ArraySilhouettes[s] = min_ad_others/ad - 1;
		}
		//ckm.ArraySilhouettes[s] = (min_ad_others - ad) / max(ad, min_ad_others);
		//

	}// for s

	//
	float sum = 0;
	for (int s = 0; s < NumSamples; s++) sum += ckm.ArraySilhouettes[s];
	//
	ckm.AverageSilhouette = sum/NumSamples;
	//

	//
	delete [] SumDistances;

	//
	return 0;
}
//
int FunctionValidatingCKM(CKM_Model & ckm, FloatMat Labels)
{
	// classified

	// initialize
	int NumCenters = ckm.getNumCenters();
	//
	float * CountPositive = new float[NumCenters];
	for (int c = 0; c < NumCenters; c++) CountPositive[c] = 0;
	//

	// count
	int NumSamples, NumTypes;
	Labels.getMatSize(NumSamples, NumTypes);
	//
	float * data_labels = Labels.data;
	//
	for (int s = 0; s < NumSamples; s++, data_labels += NumTypes)
	{
		if (data_labels[0] >= 0.5)   //
		{
			CountPositive[ckm.ArrayClassifyings[s]]++;
		}
	}

	// ratio
	for (int c = 0; c < NumCenters; c++)
	{
		ckm.PositiveRatios[c] = 1.0 * CountPositive[c] / ckm.NumDistributed[c];
	}

	//
	delete [] CountPositive;

	//
	return 0;
}
//









