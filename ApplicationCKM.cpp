
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

	//
	srand(10);
	//
	//FILE * fid;


	//
	printf("\n");
	printf("ApplicationCKM end.\n");

	getchar();
	return 0; 

}
//

