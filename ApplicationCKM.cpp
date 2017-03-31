
//
#include <direct.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

//
#include "FloatMat.h"
#include "CKM_Model.h"
//

//
void loadConfiguration();
//
int FlagLoadFromFile;
int FlagTraining;
int FlagFiles;
//
int FlagCenterType;
int NumCentersBottom;
int NumCentersTop;
//
int NumFeatures;
int NumTypes;
//
int MaxIter;
float loss_tol;
//


//
int main()  
{  
	//printf("\n");
	printf("ApplicationCKM begin ...\n\n");
	//
	// direct
	mkdir("AutoCKM_working_direct");
	chdir("AutoCKM_working_direct");
	//
	char WORK_DIRECT[128];
	getcwd(WORK_DIRECT, sizeof(WORK_DIRECT)); 
	//
	// configuration
	loadConfiguration();
	//
	printf("Configuration loaded.\n\n");
	//
	printf("FlagLoadFromFile: %d\n", FlagLoadFromFile);
	printf("FlagTraining: %d\n", FlagTraining);
	printf("FlagFiles: %d\n", FlagFiles);
	printf("\n");
	//
	printf("FlagCenterType: %d\n", FlagCenterType);
	printf("NumCentersBottom: %d\n", NumCentersBottom);
	printf("NumCentersTop: %d\n", NumCentersTop);
	printf("\n");
	//
	printf("NumFeatures: %d\n", NumFeatures);
	printf("NumTypes: %d\n", NumTypes);
	printf("\n");
	//
	printf("MaxIter: %d\n", MaxIter);
	printf("LossTol: %.4f\n", loss_tol);
	printf("\n");
	//
	getchar();
	//

	//
	char TrainingSamples_Filename[32];
	char TrainingLabels_Filename[32];
	char TestSamples_Filename[32];
	char TestLabels_Filename[32];
	//
	char CKM_Filename[32];
	//
	if (FlagFiles == 0)
	{
		strcpy(TrainingSamples_Filename, "TrainingSamples.txt");
		strcpy(TrainingLabels_Filename, "TrainingLabels.txt");
		strcpy(TestSamples_Filename, "TestSamples.txt");
		strcpy(TestLabels_Filename, "TestLabels.txt");
		//
		strcpy(CKM_Filename, "CKM_File.txt");
	}
	else if (FlagFiles == 1)
	{
		strcpy(TrainingSamples_Filename, "TrainingSamples_Ascend.txt");
		strcpy(TrainingLabels_Filename, "TrainingLabels_Ascend.txt");
		strcpy(TestSamples_Filename, "TestSamples_Ascend.txt");
		strcpy(TestLabels_Filename, "TestLabels_Ascend.txt");
		//
		strcpy(CKM_Filename, "CKM_File_Ascend.txt");
	}
	else if (FlagFiles == -1)
	{
		strcpy(TrainingSamples_Filename, "TrainingSamples_Descend.txt");
		strcpy(TrainingLabels_Filename, "TrainingLabels_Descend.txt");
		strcpy(TestSamples_Filename, "TestSamples_Descend.txt");
		strcpy(TestLabels_Filename, "TestLabels_Descend.txt");
		//
		strcpy(CKM_Filename, "CKM_File_Descend.txt");
	}
	else
	{
		strcpy(TrainingSamples_Filename, "TrainingSamples.txt");
		strcpy(TrainingLabels_Filename, "TrainingLabels.txt");
		strcpy(TestSamples_Filename, "TestSamples.txt");
		strcpy(TestLabels_Filename, "TestLabels.txt");
		//
		strcpy(CKM_Filename, "CKM_File.txt");
	}

	// model
	CKM_Model ckm;

	// Load
	if (FlagLoadFromFile == 1)
	{
		// load
		int iLoad = ckm.loadFromFile(CKM_Filename);   //
		if (iLoad == 0)
		{
			//printf("\n");
			printf("Model Loaded from %s.\n", CKM_Filename);
			//fnn.display();
			//
		}
		else
		{
			//printf("\n");
			printf("Error when loading model from %s.\n", CKM_Filename);
		}
		//
		getchar();
		//
	}
	else
	{
		printf("FlagLoadFromFile == 0.\n");
		printf("\n");
	}

	// model configuration
	ckm.FlagCenterType = FlagCenterType;
	//
	ckm.MaxIter = MaxIter;
	ckm.loss_tol = loss_tol;
	//

	// Log
	char CKM_Log_Filename[128];
	//

	// Training
	if (FlagTraining == 1)
	{
		//
		printf("FlagTraining == 1.\n\n");
		//
		// Log
		if (FlagFiles == 1)
		{
			sprintf(CKM_Log_Filename, "CKM_Log_Training_Ascend_%d_%d_%d_%d.txt", FlagCenterType, NumCentersBottom, NumCentersTop, MaxIter);
		}
		else if (FlagFiles == -1)
		{
			sprintf(CKM_Log_Filename, "CKM_Log_Training_Descend_%d_%d_%d_%d.txt", FlagCenterType, NumCentersBottom, NumCentersTop, MaxIter);
		}
		else
		{
			sprintf(CKM_Log_Filename, "CKM_Log_Training_%d_%d_%d_%d.txt", FlagCenterType, NumCentersBottom, NumCentersTop, MaxIter);
		}
		//
		FILE * fid = fopen(CKM_Log_Filename, "w");
		fclose(fid);
		//

		//
		// TrainingSamples
		FloatMat TrainingSamples(1, NumFeatures);
		TrainingSamples.loadAllDataInFile(TrainingSamples_Filename);
		printf("TrainingSamples loaded.\n");
		//
		int NumRowsSamples, NumCols;
		TrainingSamples.getMatSize(NumRowsSamples, NumCols);
		printf("TrainingSamples NumRows: %d\n", NumRowsSamples);
		//
		// TrainingLabels
		FloatMat TrainingLabels(1, NumTypes);
		TrainingLabels.loadAllDataInFile(TrainingLabels_Filename);
		printf("TrainingLabels loaded.\n");
		//
		int NumRowsLabels;
		TrainingLabels.getMatSize(NumRowsLabels, NumCols);
		printf("TrainingLabels NumRows: %d\n", NumRowsLabels);
		//
		if (NumRowsSamples < 2)
		{
			printf("\n");
			printf("WARNING: not enough samples!\n");
		}
		//
		getchar();
		//

		//
		// Training Process
		printf("Training Process:\n\n");
		ckm.setMatSamples(TrainingSamples);
		//
		for (int c = NumCentersBottom; c <= NumCentersTop; c++)
		{
			printf("Current NumCenters: %d\n", c);
			//
			ckm.setNumCenters(c);
			//
			FunctionClusteringCKM(ckm);
			//
			printf("clustered, ");
			//
			FunctionClassifyingCKM(ckm);
			//
			printf("classified, silhouetting ... ");
			//
			FunctionSilhouetteCKM(ckm);
			//
			printf("silhouetted, ");
			//
			if (NumRowsSamples == NumRowsLabels)
			{
				FunctionValidatingCKM(ckm, TrainingLabels);
			}
			else
			{
				ckm.resetArrayPositiveRatios();
			}
			//
			printf("validated, ");
			//
			printf("AverSil: %.4f\n", ckm.AverageSilhouette);
			//

			//
			fid = fopen(CKM_Log_Filename, "a");
			//
			fprintf(fid, "NumCenters, %d, loss, %.6f, PositiveRatios, ", c, ckm.loss);
			for (int i = 0; i < c; i++) fprintf(fid, "%.4f, ", ckm.PositiveRatios[i]);
			fprintf(fid, "\n");
			//
			fprintf(fid, "MaxIter, %d, AverSil, %.6f, NumDistrib, ", ckm.MaxIter, ckm.AverageSilhouette);
			for (int i = 0; i < c; i++) fprintf(fid, "%d, ", ckm.NumDistributed[i]);
			fprintf(fid, "\n\n");
			//
			fclose(fid);
			//

		}// for c
		//
		FunctionNearestInstancesCKM(ckm);
		//
		ckm.writeToFile(CKM_Filename);
		//
		printf("\n");
		printf("Training Process Ended.\n");
		//
		getchar();
		//
	}
	else
	{
		//
		printf("FlagTraining == 0.\n\n");
		//

		// Log
		if (FlagFiles == 1)
		{
			sprintf(CKM_Log_Filename, "CKM_Log_Test_Ascend_%d_%d_%d_%d.txt", FlagCenterType, NumCentersBottom, NumCentersTop, MaxIter);
		}
		else if (FlagFiles == -1)
		{
			sprintf(CKM_Log_Filename, "CKM_Log_Test_Descend_%d_%d_%d_%d.txt", FlagCenterType, NumCentersBottom, NumCentersTop, MaxIter);
		}
		else
		{
			sprintf(CKM_Log_Filename, "CKM_Log_Test_%d_%d_%d_%d.txt", FlagCenterType, NumCentersBottom, NumCentersTop, MaxIter);
		}
		//
		FILE * fid = fopen(CKM_Log_Filename, "w");
		fclose(fid);
		//

		//
		// TestSamples
		FloatMat TestSamples(1, NumFeatures);
		TestSamples.loadAllDataInFile(TestSamples_Filename);
		printf("TestSamples loaded.\n");
		//
		int NumRowsSamples, NumCols;
		TestSamples.getMatSize(NumRowsSamples, NumCols);
		printf("TestSamples NumRows: %d\n", NumRowsSamples);
		//
		// TestLabels
		FloatMat TestLabels(1, NumTypes);
		TestLabels.loadAllDataInFile(TestLabels_Filename);
		printf("TestLabels loaded.\n");
		//
		int NumRowsLabels;
		TestLabels.getMatSize(NumRowsLabels, NumCols);
		printf("TestLabels NumRows: %d\n", NumRowsLabels);
		//
		if (NumRowsSamples < 2)
		{
			printf("\n");
			printf("WARNING: not enough samples!\n");
		}
		//
		getchar();
		//

		//
		printf("Test Process ...\n");
		ckm.setMatSamples(TestSamples);
		//
		FunctionClassifyingCKM(ckm);
		FunctionSilhouetteCKM(ckm);
		//
		if (NumRowsSamples == NumRowsLabels)
		{
			FunctionValidatingCKM(ckm, TestLabels);
		}
		else
		{
			ckm.resetArrayPositiveRatios();
		}
		//
		int NumCentersLoaded = ckm.getNumCenters();
		//
		fid = fopen(CKM_Log_Filename, "a");
		//
		fprintf(fid, "NumCenters, %d, loss, %.6f, PositiveRatios, ", NumCentersLoaded, ckm.loss);
		for (int i = 0; i < NumCentersLoaded; i++) fprintf(fid, "%.4f, ", ckm.PositiveRatios[i]);
		fprintf(fid, "\n");
		//
		fprintf(fid, "MaxIter, %d, AverSil, %.6f, NumDistrib, ", ckm.MaxIter, ckm.AverageSilhouette);
		for (int i = 0; i < NumCentersLoaded; i++) fprintf(fid, "%d, ", ckm.NumDistributed[i]);
		fprintf(fid, "\n\n");
		//
		fclose(fid);
		//
		//
		//FunctionNearestInstancesCKM(ckm);
		//

		//
		printf("Test Process Ended.\n");
		//
		getchar();

	} //if FlagTraining
	//

	//
	printf("\n");
	printf("ApplicationCKM end.\n");

	getchar();
	return 0; 

}
//

//
void loadConfiguration()
{
	// Ä¬ÈÏÖµ
	FlagLoadFromFile = 0;
	FlagTraining = 0;
	FlagFiles = 0;
	//
	FlagCenterType = 2;
	NumCentersBottom = 2;
	NumCentersTop = 2;
	//
	NumFeatures =2;
	NumTypes = 2;
	//
	MaxIter = 100;
	loss_tol = 0.0001;
	//

	//
	FILE * fid = fopen("AutoCKM_Configuration.txt","r");

	if (fid == NULL)
	{
		fid = fopen("AutoCKM_Configuration.txt","w");

		fprintf(fid, "FlagLoadFromFile: %d\n", FlagLoadFromFile);
		fprintf(fid, "FlagTraining: %d\n", FlagTraining);
		fprintf(fid, "FlagFiles: %d\n", FlagFiles);
		//
		fprintf(fid, "FlagCenterType: %d\n", FlagCenterType);
		fprintf(fid, "NumCentersBottom: %d\n", NumCentersBottom);
		fprintf(fid, "NumCentersTop: %d\n", NumCentersTop);
		//
		fprintf(fid, "NumFeatures: %d\n", NumFeatures);
		fprintf(fid, "NumTypes: %d\n", NumTypes);
		//
		fprintf(fid, "MaxIter: %d\n", MaxIter);
		fprintf(fid, "LossTol: %.4f\n", loss_tol);
		//
		fprintf(fid, "End.");
		//

	}
	else
	{
		int LenBuff = 64;

		char * buff = new char[LenBuff];
		int curr;

		//
		while(fgets(buff, LenBuff, fid) != NULL)
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
			if (strcmp(buff, "FlagLoadFromFile") == 0)         //
			{
				sscanf(buff + curr, "%d", &FlagLoadFromFile);
			}
			else if (strcmp(buff, "FlagTraining") == 0)
			{
				sscanf(buff + curr, "%d", &FlagTraining);
			}
			else if (strcmp(buff, "FlagFiles") == 0)
			{
				sscanf(buff + curr, "%d", &FlagFiles);
			}
			else if (strcmp(buff, "FlagCenterType") == 0)     //
			{
				sscanf(buff + curr, "%d", &FlagCenterType);
			}
			else if (strcmp(buff, "NumCentersBottom") == 0)
			{
				sscanf(buff + curr, "%d", &NumCentersBottom);
			}
			else if (strcmp(buff, "NumCentersTop") == 0)
			{
				sscanf(buff + curr, "%d", &NumCentersTop);
			}
			else if (strcmp(buff, "NumFeatures") == 0)             //
			{
				sscanf(buff + curr, "%d", &NumFeatures);
			}
			else if (strcmp(buff, "NumTypes") == 0)
			{
				sscanf(buff + curr, "%d", &NumTypes);
			}
			else if (strcmp(buff, "MaxIter") == 0)
			{
				sscanf(buff + curr, "%d", &MaxIter);
			}
			else if (strcmp(buff, "LossTol") == 0)
			{
				sscanf(buff + curr, "%f", &loss_tol);
			}

		}// while fgets

		//
		delete [] buff;

	}

	fclose(fid);
	//
}
//

