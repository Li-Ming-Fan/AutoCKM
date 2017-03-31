
#ifndef CKM_Model_H
#define CKM_Model_H

#include "FloatMat.h"

#include <string.h>
#include <stdio.h>

//
class CKM_Model
{
private:
	//
	int NumSamples;
	int NumFeatures;
	//
	int NumCenters;
	//

public:
	FloatMat Samples;
	//
	FloatMat * Centers;   // of length NumCenters
	//
	int * NumDistributed;       // of length NumCenters
	float * PositiveRatios;        // for pattern assessment, of length NumCenters
	int * NearestInstances;       // of length NumCenters
	//
	int * ArrayClassifyings;     // of length NumSamples
	//
	float * ArraySilhouettes;    // of length NumSamples
	float AverageSilhouette;
	//

	//
	int FlagCenterType;
	//
	static const int CKM_MEDOID = 0;
	static const int CKM_MEDIAN = 1;
	static const int CKM_MEAN = 2;
	//
	int MaxIter;
	float loss_tol;
	//
	float loss;
	//

	//
	CKM_Model(void)
	{
		Samples.setMatSize(1, 1);
		NumSamples = 1;
		NumFeatures = 1;
		//
		NumCenters = 1;
		Centers = new FloatMat[1];
		//
		ArrayClassifyings = new int[1];
		//
		ArraySilhouettes = new float[1];
		AverageSilhouette = -1000;
		//
		NumDistributed = new int[1];
		PositiveRatios = new float[1];
		NearestInstances = new int[1];
		//

		//
		FlagCenterType = 1;
		//
		MaxIter = 1000;
		loss_tol = 0.0001;
		//
		loss = 100;
		//

	}
	//
	~CKM_Model()
	{
		delete [] Centers;
		delete [] ArrayClassifyings;
		delete [] ArraySilhouettes;
		delete [] NumDistributed;
		delete [] PositiveRatios;
		delete [] NearestInstances;
	}
	//
	void setNumCenters(int Num)
	{
		NumCenters = Num;
		//
		delete [] Centers;
		Centers = new FloatMat[NumCenters];
		//
		delete [] NumDistributed;
		NumDistributed = new int[NumCenters];
		//
		delete [] PositiveRatios;
		PositiveRatios = new float[NumCenters];
		//
		delete [] NearestInstances;
		NearestInstances = new int[NumCenters];
		//
	}
	void setMatSamples(FloatMat MatSamples)
	{
		Samples = MatSamples;
		//
		Samples.getMatSize(NumSamples, NumFeatures);
		//
		delete [] ArrayClassifyings;
		delete [] ArraySilhouettes;
		//
		ArrayClassifyings = new int[NumSamples];
		ArraySilhouettes = new float[NumSamples];
		//
	}
	//
	int getNumCenters()
	{
		return NumCenters;
	}
	int getNumSamples()
	{
		return NumSamples;
	}
	int getNumFeatures()
	{
		return NumFeatures;
	}
	//
	void resetArrayPositiveRatios()
	{
		for (int c = 0; c < NumCenters; c++) PositiveRatios[c] = 0;
	}
	//

	// interface functions
	void display()
	{
		//
		printf("FlagCenterType: %d\n", FlagCenterType);
		printf("NumCenters: %d\n", NumCenters);
		printf("NumFeatures: %d\n", NumFeatures);
		//
		printf("NumDistributed: ");
		for (int c = 0; c < NumCenters; c++)
		{
			printf("%d, ", NumDistributed[c]);
		}
		printf("\n");
		//
		printf("PositiveRatios: ");
		for (int c = 0; c < NumCenters; c++)
		{
			printf("%.4f, ", PositiveRatios[c]);
		}
		printf("\n");
		//
		printf("NearestInstances: ");
		for (int c = 0; c < NumCenters; c++)
		{
			printf("%d, ", NearestInstances[c]);
		}
		printf("\n");
		//
		printf("Centers:\n");
		for (int c = 0; c < NumCenters; c++)
		{
			Centers[c].display();
		}
		//
		printf("NumSamples: %d\n", NumSamples);
		//
		printf("AverageSilhouette: %.6f\n", AverageSilhouette);
		//
		printf("CKM_End.\n");

	}
	int loadFromFile(char * file_str)
	{
		FILE * fp = fopen(file_str, "r");
		//
		if (fp == NULL) return -1;

		//
		int NumRead = 0;
		//

		//
		int LenBuff = 4096;

		char * buff = new char[LenBuff];
		int curr;
		//
		while(fgets(buff, LenBuff, fp) != NULL)
		{
			if (strlen(buff) < 5) continue;

			//
			curr = 0;
			while (buff[curr] != ':')
			{
				curr++;
			}
			//
			buff[curr] = '\0';
			curr++;
			//
			if (strcmp(buff, "FlagCenterType") == 0)
			{
				sscanf(buff + curr, "%d", &FlagCenterType);    //
			}
			else if (strcmp(buff, "NumCenters") == 0)
			{
				sscanf(buff + curr, "%d", &NumRead);
				//
				this->setNumCenters(NumRead);      //
				//
			}
			else if (strcmp(buff, "NumFeatures") == 0)
			{
				sscanf(buff + curr, "%d", &NumFeatures);    //
			}
			else if (strcmp(buff, "NumDistributed") == 0)
			{
				//
				int Posi = 0;
				char * str_begin = buff + curr;
				//
				while (buff[curr] != '\n')
				{
					if (buff[curr] == ',')
					{
						buff[curr] = '\0';

						sscanf(str_begin, "%d", NumDistributed + Posi);

						//
						Posi++;

						//
						curr++;

						str_begin = buff + curr;
					}
					else
					{
						curr++;
					}
				}
				//
			}
			else if (strcmp(buff, "PositiveRatios") == 0)
			{
				//
				int Posi = 0;
				char * str_begin = buff + curr;
				//
				while (buff[curr] != '\n')
				{
					if (buff[curr] == ',')
					{
						buff[curr] = '\0';

						sscanf(str_begin, "%f", PositiveRatios + Posi);

						//
						Posi++;

						//
						curr++;

						str_begin = buff + curr;
					}
					else
					{
						curr++;
					}
					//
				}
			}
			else if (strcmp(buff, "NearestInstances") == 0)
			{
				//
				int Posi = 0;
				char * str_begin = buff + curr;
				//
				while (buff[curr] != '\n')
				{
					if (buff[curr] == ',')
					{
						buff[curr] = '\0';

						sscanf(str_begin, "%d", NearestInstances + Posi);

						//
						Posi++;

						//
						curr++;

						str_begin = buff + curr;
					}
					else
					{
						curr++;
					}
				}
				//
			}
			else if (strcmp(buff, "Centers") == 0)
			{
				for (int c = 0; c < NumCenters; c++)
				{
					Centers[c].setMatSize(1, NumFeatures);
					Centers[c].loadFromFile(fp, 1);
				}
			}
			else if (strcmp(buff, "NumSamples") == 0)
			{
				sscanf(buff + curr, "%d", &NumSamples);    //
			}
			else if (strcmp(buff, "AverageSilhouette") == 0)
			{
				sscanf(buff + curr, "%f", &AverageSilhouette);
			}

		}// while

		return 0;
	}
	int writeToFile(char * file_str)
	{
		FILE * fp = fopen(file_str, "w");
		//
		if (fp == NULL) return -1;

		//
		fprintf(fp, "FlagCenterType: %d\n", FlagCenterType);
		fprintf(fp, "NumCenters: %d\n", NumCenters);
		fprintf(fp, "NumFeatures: %d\n", NumFeatures);
		//
		fprintf(fp, "NumDistributed: ");
		for (int c = 0; c < NumCenters; c++)
		{
			fprintf(fp, "%d, ", NumDistributed[c]);
		}
		fprintf(fp, "\n");
		//
		fprintf(fp, "PositiveRatios: ");
		for (int c = 0; c < NumCenters; c++)
		{
			fprintf(fp, "%.4f, ", PositiveRatios[c]);
		}
		fprintf(fp, "\n");
		//
		fprintf(fp, "NearestInstances: ");
		for (int c = 0; c < NumCenters; c++)
		{
			fprintf(fp, "%d, ", NearestInstances[c]);
		}
		fprintf(fp, "\n");
		//
		fprintf(fp, "Centers: \n");
		for (int c = 0; c < NumCenters; c++)
		{
			Centers[c].writeToFile(fp);
		}
		//
		fprintf(fp, "NumSamples: %d\n", NumSamples);
		fprintf(fp, "AverageSilhouette: %.6f\n", AverageSilhouette);
		//
		fprintf(fp, "CKM_End.\n");
		//

		//
		fclose(fp);
		//
		return 0;

	}
	//

	// internal functions
	int initializeCentersL1()
	{
		// first one
		int Posi = cblas_isamax(NumSamples, Samples.data, NumFeatures);
		//
		cblas_scopy(NumFeatures, Samples.data + Posi*NumFeatures, 1, Centers[0].data, 1);
		//

		// others
		float * ArrayMinDistances = new float[NumSamples];
		float min_distance, distance;
		//
		float * data_sample;
		//
		for (int c = 1; c < NumCenters; c++)
		{
			// calculate ArrayMinDistances
			//
			data_sample = Samples.data;
			//
			for (int s = 0; s < NumSamples; s++, data_sample += NumFeatures)
			{
				// L1
				min_distance = distanceFunctionL1(Centers[0].data, data_sample, NumFeatures);
				//
				for (int c_inner = 1; c_inner < c; c_inner++)
				{
					// L1
					distance = distanceFunctionL1(Centers[c_inner].data, data_sample, NumFeatures);
					//
					if (distance < min_distance) min_distance = distance;

				}// for c_inner

				// record
				ArrayMinDistances[s] = min_distance;

			}// for s

			// get Posi of max in ArrayMinDistances
			Posi = cblas_isamax(NumSamples, ArrayMinDistances, 1);
			//
			cblas_scopy(NumFeatures, Samples.data + Posi*NumFeatures, 1, Centers[c].data, 1);
			//

		}// for c
		//
		delete [] ArrayMinDistances;

		//
		return 0;
	}
	int initializeCentersL2()
	{
		// first one
		int Posi = cblas_isamax(NumSamples, Samples.data, NumFeatures);
		//
		cblas_scopy(NumFeatures, Samples.data + Posi*NumFeatures, 1, Centers[0].data, 1);
		//

		// others
		float * ArrayMinDistances = new float[NumSamples];
		float min_distance, distance;
		//
		float * data_sample;
		//
		for (int c = 1; c < NumCenters; c++)
		{
			// calculate ArrayMinDistances
			//
			data_sample = Samples.data;
			//
			for (int s = 0; s < NumSamples; s++, data_sample += NumFeatures)
			{
				// L2
				min_distance = distanceFunctionL2(Centers[0].data, data_sample, NumFeatures);
				//
				for (int c_inner = 1; c_inner < c; c_inner++)
				{
					// L2
					distance = distanceFunctionL2(Centers[c_inner].data, data_sample, NumFeatures);
					//
					if (distance < min_distance) min_distance = distance;

				}// for c_inner

				// record
				ArrayMinDistances[s] = min_distance;

			}// for s

			// get Posi of max in ArrayMinDistances
			Posi = cblas_isamax(NumSamples, ArrayMinDistances, 1);
			//
			cblas_scopy(NumFeatures, Samples.data + Posi*NumFeatures, 1, Centers[c].data, 1);
			//

		}// for c
		//
		delete [] ArrayMinDistances;

		//
		return 0;
	}
	//
	int classifySamplesL1()
	{
		// count
		for (int c = 0; c < NumCenters; c++)
		{
			NumDistributed[c] = 0;
		}

		// classify
		float distance_min, distance;
		int type;
		//
		float * sample_instance = Samples.data;
		//
		for (int s = 0; s < NumSamples; s++, sample_instance += NumFeatures)
		{
			distance_min = 10000000;
			type = 0;
			//
			for (int c = 0; c < NumCenters; c++)
			{
				// L1
				distance = distanceFunctionL1(sample_instance, Centers[c].data, NumFeatures);
				//
				if (distance < distance_min)
				{
					distance_min = distance;
					type = c;
				}
			}
			//
			ArrayClassifyings[s] = type;
			//
			NumDistributed[type]++;

		}// for s

		return 0;
	}
	int classifySamplesL2()
	{
		// count
		for (int c = 0; c < NumCenters; c++)
		{
			NumDistributed[c] = 0;
		}

		// classify
		float distance_min, distance;
		int type;
		//
		float * sample_instance = Samples.data;
		//
		for (int s = 0; s < NumSamples; s++, sample_instance += NumFeatures)
		{
			distance_min = 10000000;
			type = 0;
			//
			for (int c = 0; c < NumCenters; c++)
			{
				// L2
				distance = distanceFunctionL2(sample_instance, Centers[c].data, NumFeatures);
				//
				if (distance < distance_min)
				{
					distance_min = distance;
					type = c;
				}
			}
			//
			ArrayClassifyings[s] = type;
			//
			NumDistributed[type]++;

		}// for s

		return 0;
	}
	//
	int updateCentersMeans()
	{
		// L2 distance, component mean

		// initialize
		for (int c = 0; c < NumCenters; c++)
		{
			Centers[c].setMatConstant(0);
		}

		// sum
		int type;
		float * instance = Samples.data;
		//
		for (int s = 0; s < NumSamples; s++, instance += NumFeatures)
		{
			type = ArrayClassifyings[s];
			//
			cblas_saxpy(NumFeatures, 1.0, instance, 1, Centers[type].data, 1);

		}// for s

		// mean
		for (int c = 0; c < NumCenters; c++)
		{
			cblas_sscal(NumFeatures, 1.0 / NumDistributed[c], Centers[c].data, 1);
		}

		return 0;
	}
	int updateCentersMedians()
	{
		// L1 distance, component median
		//
		FloatMat SamplesExtracted;
		int NumInstances;
		//
		float * data_instance;
		float * data_sample;
		//

		//
		for (int c = 0; c < NumCenters; c++)
		{
			// NumInstances
			NumInstances = NumDistributed[c];

			// extract
			SamplesExtracted.setMatSize(NumFeatures, NumInstances);
			//
			data_instance = SamplesExtracted.data;
			data_sample = Samples.data;
			//
			for (int s = 0; s < NumSamples; s++, data_sample += NumFeatures)
			{
				if (ArrayClassifyings[s] == c)
				{
					//cblas_scopy (const MKL_INT n, const float *x, const MKL_INT incx, float *y, const MKL_INT incy);
					//
					// copy and transpose
					//
					cblas_scopy(NumFeatures, data_sample, 1, data_instance, NumInstances);
					//
					data_instance++;
				}
			}

			// median -- sort
			int posi_1 = 0;
			int posi_2 = NumInstances - 1;
			//
			for (int i = 0; i < NumFeatures; i++)
			{
				quickSort(SamplesExtracted.data, posi_1, posi_2);

				//
				posi_1 += NumInstances;
				posi_2 += NumInstances;

			}
			//
			// median --  copy and transpose
			//
			cblas_scopy(NumFeatures, SamplesExtracted.data + (int)(NumInstances/2), NumInstances, Centers[c].data, 1);
			//

		}// for c

		return 0;
	}
	//
	int updateCentersMedoids()
	{
		// L1 distance, instance minimizing distance sum
		//
		printf("Error, updateCentersMedoids() NOT implemented.\n");
		//

		return -1;
	}
	//

	// distance functions
	static float distanceFunctionL1(float * sample_1, float * sample_2, int Length)
	{
		// difference
		float * diff_data = new float[Length];
		//
		memcpy(diff_data, sample_1, sizeof(float) * Length);
		//
		cblas_saxpy(Length, -1.0, sample_2, 1, diff_data, 1);

		// NormL1
		float norm =  cblas_sasum(Length, diff_data, 1);   // abs, then sum
		//
		delete [] diff_data;

		//
		return norm;

	}
	static float distanceFunctionL2(float * sample_1, float * sample_2, int Length)
	{
		// difference
		float * diff_data = new float[Length];
		//
		memcpy(diff_data, sample_1, sizeof(float) * Length);
		//
		cblas_saxpy (Length, -1.0, sample_2, 1, diff_data, 1);

		// NormL2
		float norm =  cblas_snrm2 (Length, diff_data, 1);  // square, then sum, then square-root,
		//
		delete [] diff_data;

		//
		return norm;
	}
	//

	// quickSort
	static void quickSort(float * a, int left, int right)
	{
		if(left >= right)    /*如果左边索引大于或者等于右边的索引就代表已经整理完成一个组了*/
		{
			return ;
		}

		//
		int i = left;
		int j = right;
		float key = a[left];
		//
		while(i < j)
		{
			while(i < j && key <= a[j])
			{
				j--;
			}
			//
			a[i] = a[j];

			while(i < j && key >= a[i])
			{
				i++;
			}
			//
			a[j] = a[i];
		}
		//
		a[i] = key;   /*当在当组内找完一遍以后就把中间数key回归*/

		//
		quickSort(a, left, i - 1);           /*用同样的方式对分出来的左边的小组进行同上的做法*/
		quickSort(a, i + 1, right);       /*用同样的方式对分出来的右边的小组进行同上的做法*/
		//
	}
	//

};

// utility functions
int FunctionClusteringCKM(CKM_Model & ckm);
int FunctionClassifyingCKM(CKM_Model & ckm);
//
int FunctionSilhouetteCKM(CKM_Model & ckm);
int FunctionValidatingCKM(CKM_Model & ckm, FloatMat Labels);
//
int FunctionNearestInstancesCKM(CKM_Model & ckm);
//


#endif

