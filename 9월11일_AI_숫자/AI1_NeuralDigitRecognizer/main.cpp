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
    //TestXOR(); //이건 닫고 9.11 //이건 입력패턴.

	
	RunDigitUI();//이건 열고 실행을 해보자>> 그러면 실행됩니다. 9.11 //이건실행.

	return 0;
}
//epoch : 학습을 연습한 횟수 , loss : 틀린 횟수, recognition rate : 알아본 비율 
int TestXOR()
{
    srand((unsigned int)time(NULL));

    // neural network model
    const char *modelFile = "xor.wgt";
    int noLayer = 2;                    // # of layers not including input layer
	int aNetStruct[] = { 2, 4, 2 };     // (inputDim, hiddenDim, outputDim) 입력2개 히든노드 4개 결과2개

	HGUNeuralNetwork nn;
	nn.Alloc(noLayer, aNetStruct, NULL);
//    nn.Load(modelFile);

    // training samples and desired output//들어올 값과 결과 값을 준다.
	const int noSample = 4;
	float aSample[4][2] = {//입력값
		{ 0.F, 0.F },
		{ 0.F, 1.F },
		{ 1.F, 0.F },
		{ 1.F, 1.F }
	};

	float aDesiredOutput[4][2] = {//정답값 >> xor 정답값
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
	//방향을 찾는, 미분과 적분.
	//셈플이 위에서 4개 였으니깐, 4번 돌아가고 그것에 대해서 미분을 한다.
	//결과를 누적한다.
	for(int epoch = 0; epoch < maxEpoch; epoch++){
        // compute gradient on each sample
        for(int i = 0; i < noSample; i++){
			nn.ComputeGradient(aSample[i], aDesiredOutput[i]);  // compute and accumulate gradient
			//파라미터(입력샘플,정답)을 넣고 학습시키는 것. 미분해서 방향성으로 찾는다.
			error += nn.GetError(aDesiredOutput[i]);            // accumulate MSE loss
			//에러를 계속 더하고 계수만큼 나눠주면 평균값이 나온다 >> 밑에 있다.
			if(nn.GetMaxOutputIndex() == aDesiredOutput[i][1])
			//if(aDesiredOutput[i][nn.GetMaxOutputIndex]== 1.F)
                correct++;
		}
		n += noSample;

		// update once an epoch
		nn.UpdateWeight(0.01F / noSample);                      // update weight with average gradient
		//위에서 발견한 그 찾아낸 방향으로 이동하라...//업데이트
		//미분,이동을 발견하고 점점 노스가 줄어들고 정확도가 높아진다.
		if(error / n < 0.0001F)
			break;

		if(epoch == 0 || (epoch + 1) % 10000 == 0){
			printf("epoch = %d, loss = %f, recognition rate = %f\n", epoch + 1, error / n, correct / (float)n);//에러를 나누는 걸 볼수 있다.
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
		nn.Propagate(aSample[j]); //뉴런네트워크: nn //계산하는.
		//노드들은 float array로 저장되어진다.
		float *hidden = nn[0]->GetOutput();//structure pointer ...
		float *output = nn[1]->GetOutput(); //getoutput>>결과출력 시 사용.
		printf("sample %d: (%.3f %.3f) --> (%.3f %.3f %.3f %.3f) --> (%.3f, %.3f)\n", j, aSample[j][0], aSample[j][1],  hidden[0], hidden[1], hidden[2], hidden[3], output[0], output[1]);
	}

	system("PAUSE");

	return TRUE;
}
//학습은 정비례하는 것이 아니다