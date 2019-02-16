/***
	Copyright 2012 Injung Kim

	Licensed under the Apache License, Version 2.0 (the "License");
	you may not use this file except in compliance with the License.
	You may obtain a copy of the License at

		http://www.apache.org/licenses/LICENSE-2.0

	Unless required by applicable law or agreed to in writing, software
	distributed under the License is distributed on an "AS IS" BASIS,
	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
	See the License for the specific language governing permissions and
	limitations under the License.
***/


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#ifdef WINDOWS
#include <Windows.h>
#endif // WINDOWS


#include "HGULayer.h"
#include "HGUNeuralNetwork.h"

#include "HGUDigitImage.h"
#include "HGUDigitUI.h"


int TestXOR();

int main(int argc, char *argv[])
{
    //TestXOR();

 	RunDigitUI();

	return 0;
}

int TestXOR()
{
    srand((unsigned int)time(NULL));

    // neural network model
    const char *modelFile = "xor.wgt";
    int noLayer = 2;                    // # of layers not including input layer
	int aNetStruct[] = { 2, 4, 2 };     // (inputDim, hiddenDim, outputDim)

	HGUNeuralNetwork nn;
	nn.Alloc(noLayer, aNetStruct, NULL);
//    nn.Load(modelFile);

    // training samples and desired output
	const int noSample = 4;
	float aSample[4][2] = {
		{ 0.F, 0.F },
		{ 0.F, 1.F },
		{ 1.F, 0.F },
		{ 1.F, 1.F }
	};

	float aDesiredOutput[4][2] = {
	    { 1, 0 },
	    { 0, 1 },
	    { 0, 1 },
	    { 1, 0 }
    };    // outputDim = 2


	printf("=== Training MLP...\n");
	const int maxEpoch = 1000000;

	float error = 0.F;
	int n = 0;
	int correct = 0;

	for(int epoch = 0; epoch < maxEpoch; epoch++){
        // compute gradient on each sample
        for(int i = 0; i < noSample; i++){
			nn.ComputeGradient(aSample[i], aDesiredOutput[i]);  // compute and accumulate gradient

			error += nn.GetError(aDesiredOutput[i]);            // accumulate MSE loss

			if(nn.GetMaxOutputIndex() == aDesiredOutput[i][1])
                correct++;
		}
		n += noSample;

		// update once an epoch
		nn.UpdateWeight(0.01F / noSample);                      // update weight with average gradient

		if(error / n < 0.0001F)
			break;

		if(epoch == 0 || (epoch + 1) % 10000 == 0){
			printf("epoch = %d, loss = %f, recognition rate = %f\n", epoch + 1, error / n, correct / (float)n);
			n = 0;
			correct = 0;
			error = 0.F;
		}
	}

//	nn.Save(modelFile);

	printf("=== Training MLP... Done.\n");

	system("PAUSE");

	printf("=== Testing MLP...\n");
	for(int j = 0; j < noSample; j++){
		nn.Propagate(aSample[j]);
		float *hidden = nn[0]->GetOutput();
		float *output = nn[1]->GetOutput();
		printf("sample %d: (%.3f %.3f) --> (%.3f %.3f %.3f %.3f) --> (%.3f, %.3f)\n", j, aSample[j][0], aSample[j][1],  hidden[0], hidden[1], hidden[2], hidden[3], output[0], output[1]);
	}

	system("PAUSE");

	return TRUE;
}
