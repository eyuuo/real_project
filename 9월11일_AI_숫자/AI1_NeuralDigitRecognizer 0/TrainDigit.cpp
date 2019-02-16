#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#ifdef WINDOWS
#include <windows.h>
#endif // WINDOWS


#include "HGULayer.h"
#include "HGUNeuralNetwork.h"

#include "HGUDigitImage.h"
#include "HGUDigitUI.h"

extern int noLayer;
extern int aNetStruct[];

int TrainDigit()
{
	srand((unsigned int)time(NULL));

	const int noSample = 10;

	HGUDigitImage aSample[10];
	float aDesiredOutput[10][10] = { 0 };

	for (int i = 0; i < 10; i++){
		aSample[i].SetDigit(i);
		aDesiredOutput[i][i] = 1.F;
	}

	HGUNeuralNetwork nn;
	nn.Alloc(noLayer, aNetStruct, NULL);

/*
	int ret = nn.Load("digit.wgt");
	if (ret == FALSE){
		printf("Failed to load neural network weights. Initializing.\n");
		nn.InitializeWeight();
	}
*/

	printf("=== Training MLP...\n");
	const int maxEpoch = 100000;

	int noCorrect = 0;
	float error = 0.F;
	int n = 0;
	for (int epoch = 0; epoch < maxEpoch; epoch++){
		for (int i = 0; i < noSample; i++){
			nn.ComputeGradient(aSample[i].GetVector(), aDesiredOutput[i]);
			error += nn.GetError(aDesiredOutput[i]);
			int intResult = nn.GetMaxOutputIndex();
			if (i == intResult)
				noCorrect++;
		}
		n += noSample;

		nn.UpdateWeight(0.05F / noSample);
		if (epoch == 0 || (epoch + 1) % 1000 == 0)
			printf("epoch = %d, error = %f\n", epoch + 1, error / n);

		if (error / n < 0.0001F)
			break;
	}

	printf("=== Training MLP... Done.\n");

	nn.Save("digit.wgt");

	system("PAUSE");

	return TRUE;
}
