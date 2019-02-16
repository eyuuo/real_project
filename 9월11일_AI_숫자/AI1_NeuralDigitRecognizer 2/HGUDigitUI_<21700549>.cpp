/***
	Copyright 2017 Injung Kim

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
#include <math.h>

#ifdef _WINDOWS
#include <conio.h>
#endif	// _WINDOWS

#include "Console.h"

#include "HGULayer.h"
#include "HGUNeuralNetwork.h"

#include "HGUDigitImage.h"
#include "HGUDigitUI.h"

//DigitSize=35
int noLayer = 2;
int aNetStruct[] = { DigitSize, 32, 10 };

HGUDigitUI::~HGUDigitUI()
{
	if(m_pNN != NULL){
		delete m_pNN;
		m_pNN = NULL;
	}
}

int HGUDigitUI::Run()
{
	m_windowWidth = getWindowWidth();
	m_windowHeight = getWindowHeight();

	m_sx = (m_windowWidth - DigitWidth * m_hScale) / 2;
	m_sy = (m_windowHeight - DigitHeight * m_vScale) / 2;

	// draw boundary
	DrawScreen();

	int x = 0, y = 0;
	int key = 0;

	do {
		gotoxy(m_sx + x * m_hScale, m_sy + y * m_vScale);
		key = _getch();

		if(key >= '0' && key <= '9'){
			m_digit.SetDigit(key - '0');
			DisplayDigit(m_sx, m_sy, &m_digit, m_hScale, m_vScale);
		} else if(key != 27 && key != 'q'){
			switch(key){
				case 75:		// left
				case 'h':		// left
					if(x > 0)
						x--;
					break;

				case 77:		// right
				case 'l':		// right
					if(x < DigitWidth - 1)
						x++;
					break;

				case 72:		// up
				case 'k':		// up
					if(y > 0)
						y--;
					break;

				case 80:		// down
				case 'j':		// down
					if(y < DigitHeight - 1)
						y++;
					break;

				case 83:		// del		clear
				case 'c':		// del		clear
					m_digit.Clear(0.F);
					DisplayDigit(m_sx, m_sy, &m_digit, m_hScale, m_vScale);
					break;

				case 'd':					// draw
					m_digit.Pixel(x, y) = 1.F;
					DrawBigDot(m_sx + x * m_hScale, m_sy + y * m_vScale, m_hScale, m_vScale, '=');
					break;

				case 'e':					// erase
					m_digit.Pixel(x, y) = 0.F;
					DrawBigDot(m_sx + x * m_hScale, m_sy + y * m_vScale, m_hScale, m_vScale, ' ');
					break;

				case ' ':					// toggle
					m_digit.Pixel(x, y) = 1.F - m_digit.Pixel(x, y);
					DrawBigDot(m_sx + x * m_hScale, m_sy + y * m_vScale, m_hScale, m_vScale, (m_digit.Pixel(x, y) == 0. ? ' ' : '='));
					break;

				case 'f':
					gotoxy(1, 1);
					m_digit.DisplayAsArray();
					break;

				case 'r':
                    RecognizeDigit();
					break;

				case 'i':				// reset model
					m_pNN->InitializeWeight();
					gotoxy(1, 4);
					printf("Model was reinitialized!");
					break;

				case 't':
					TrainRecognizer(1000000);
					DrawScreen();
					break;
			}
		}
	} while(key != 27 && key != 'q');

	clrscr();
	printf("Bye!\n");

	return 0;
}

void HGUDigitUI::DrawScreen()
{
    clrscr();

   	int sx = (m_windowWidth - DigitWidth * m_hScale) / 2;
	int sy = (m_windowHeight - DigitHeight * m_vScale) / 2;

 	if (sx > 0 && sy > 0)
		DrawBox(sx - 1, sy - 1, sx + DigitWidth * m_hScale, sy + DigitHeight * m_vScale, '#');
	DisplayDigit(sx, sy, &m_digit, m_hScale, m_vScale);

	int j = 1;
	gotoxy(2, sy + j++);
	printf("h, j, k, l keys: move");
	gotoxy(2, sy + j++);
	printf("(left, down, up, right)");

	gotoxy(2, sy + j++);
	printf("space: toggle pixel");

	j++;
	gotoxy(2, sy + j++);
	printf("0-9: set digit shape");

	j++;
	gotoxy(2, sy + j++);
	printf("r: recognize");

	gotoxy(2, sy + j++);
	printf("t: train");

	gotoxy(2, sy + j++);
	printf("i: init neural network");

	j++;
	gotoxy(2, sy + j++);
	printf("ESC: quit");
}

void HGUDigitUI::DisplayDigit(int sx, int sy, HGUDigitImage *pDigit, int hScale, int vScale)
{
	if (hScale == 0)
		hScale = m_hScale;
	if (vScale == 0)
		vScale = m_vScale;

	int i = 0;
	for (int y = 0; y < DigitHeight; y++){
		for (int x = 0; x < DigitWidth; x++, i++)
			DrawBigDot(sx + x * hScale, sy + y * vScale, hScale, vScale, ((*pDigit)[i] >= 0.5F ? '=' : ' '));
	}
}

void HGUDigitUI::DrawBigDot(int sx, int sy, int width, int height, char v)
{
	for (int y = 0; y < height; y++){
		gotoxy(sx, sy + y);
		for (int j = 0; j < width; j++)
			putchar(v);
	}
}

void HGUDigitUI::DrawBox(int left, int top, int right, int bottom, char v)
{
	int x = 0, y = 0;
	for (y = top; y <= bottom; y++){
		gotoxy(left, y);
		putchar(v);
		gotoxy(right, y);
		putchar(v);
	}

	gotoxy(left, top);
	for (x = left; x <= right; x++)
		putchar(v);

	gotoxy(left, bottom);
	for (x = left; x <= right; x++)
		putchar(v);
}

int HGUDigitUI::LoadRecognizer(int noLayer, int pNetStruct[], const char *weightFile)
{
	m_pNN = new HGUNeuralNetwork;
	if (m_pNN == NULL || m_pNN->Alloc(noLayer, pNetStruct, NULL) == FALSE){
		printf("Failed to allocate memory in %s (%s %d)\n", __FUNCTION__, __FILE__, __LINE__);
		if (m_pNN)
			m_pNN = NULL;
		return FALSE;
	}

	if (m_pNN->Load(weightFile) == FALSE){
		gotoxy(1, 4);
		printf("Failed to load neural network from %s in %s (%s %d)\n", weightFile, __FUNCTION__, __FILE__, __LINE__);
		return FALSE;
	} else {
		gotoxy(1, 4);
		printf("Succeeded to load neural network from %s.\n", weightFile);
	}

	return TRUE;
}

int HGUDigitUI::RecognizeDigit() //밑에 설명 써있는거. 프로파게이트라는 함수를 호출해야한다. m_pNN..GetVector
{

    // Propagate neural network feeding m_digit as the input
	//      use GetVector() of HGUDigitImage  (m_digit)
    //      use Propagate(float *pInput) of HGUNeuralNetwork (m_pNN)//m_digit.GetVector()
	m_pNN->Propagate(m_digit.GetVector());
	
    // Display output nodes of neural network
    //      use GetOutput(int nodeIdx) of HGUNeuralNetwork (m_pNN)
    //      use gotoxy(int x, int y) and printf() to display the output values
	gotoxy(0, 20);
	for(int i= 0;i<=4;i++){
		printf("[%d] : %f  ",i,m_pNN->GetOutput(i));
	}
	printf("\n");
	for(int i= 5;i<=9;i++){
		printf("[%d] : %f  ",i,m_pNN->GetOutput(i));
	}
	printf("\n");
	printf("-> %d [%f]  ",m_pNN->GetMaxOutputIndex(),m_pNN->GetOutput(m_pNN->GetMaxOutputIndex()));
	//printf("Function %s was not implemented yet.\n", __FUNCTION__);
		MyPause();
		DrawScreen();
//커서를 보내는 작동을 한다. 보내고 그자리에 출력을 하면 된다.
    // Display the recognition result and the score
    //      use GetMaxOutputIndex() of HGUNeuralNetwork (m_pNN) to retrieve recognition result
    //      use GetOutput(int nodeIdx) of HGUNeuralNetwork (m_pNN)
	//m_pNN->GetMaxOutputIndex();
    return TRUE;
}


int HGUDigitUI::TrainRecognizer(int maxEpoch)
{
	gotoxy(1, 4);
	printf("Function %s was not implemented yet.\n", __FUNCTION__);

    // clear screen and display a message to notice the start of training

	// allocate neural network if necessary //뉴런이 없으면 이렇게 하라~//어려울 것 같아서 교수님이 짜심.
	if(m_pNN == NULL){
		m_pNN = new HGUNeuralNetwork;
		if (m_pNN == NULL || m_pNN->Alloc(noLayer, aNetStruct, NULL) == FALSE){
			printf("Failed to allocate neural network in %s (%s %d)\n", __FUNCTION__, __FILE__, __LINE__);
			if (m_pNN != NULL){
				delete m_pNN;
				m_pNN = NULL;
			}
			return FALSE;
		}

		m_pNN->InitializeWeight();
	}

	// training samples 
  	//m_pNN->Load(ModelFile);//
	const int noSample = 10;
	HGUDigitImage aSample[10]; //10개를 생성. 0~9번 이미지를 이걸로 만든다. 
	//HGUDigitImage.SetDigit이라는 함수를 사용.>>특정한 모양의 디짓으로 그림을 그려준다.
	//setdigit(0)이렇게 넣으면 되는데... //SetDigit(i)이렇게 하고 루프를 돌리자.
	
	// set training data and desired labels
    //      use SetDigit(int d) of HGUDigitImage to make training data
    //          see SetDigit() to understand how it works.
	//      fill aDesiredOutput with proper values
	
	for(int i=0; i<10;i++)//
	aSample[i].SetDigit(i);//

	float aDesiredOutput[10][10] = { 0 }; //전부 다 0값이 된다.
	for(int i=0; i<10;i++)//
	{aDesiredOutput[i][i] = 1;}//

	// Repeat up to maxEpoch
	maxEpoch = 1000000;
    //      for each sample
    //          compute gradient using ComputeGradient() of HGUNeuralNetwork
    //          check MSE loss and recognition result by GetError() and GetMaxOutputIndex() of HGUNeuralNetwork
	printf("=== Training MLP...\n");
	float error = 0.F;
	int n = 0;
	int correct = 0;
	
	for(int epoch = 0; epoch < maxEpoch; epoch++){
        // compute gradient on each sample
        for(int i = 0; i < noSample; i++){
			m_pNN->ComputeGradient(aSample[i].GetVector(),aDesiredOutput[i]);  // compute and accumulate gradient
			error += m_pNN->GetError(aDesiredOutput[i]);            // accumulate MSE loss
			if(m_pNN->GetMaxOutputIndex() == i)
                correct++;
		}
		n += noSample;//

		//      update weight by UpdateWeight(0.01F / epochSample) of HGUNeuralNetwork
		// update once an epoch
		m_pNN->UpdateWeight(0.01F / noSample);                      // update weight with average gradient

		if(error / n < 0.0001F)
			break;

		if(epoch == 0 || (epoch + 1) % 10000 == 0){
			printf("epoch = %d, loss = %f, recognition rate = %f\n", epoch + 1, error / n, correct / (float)n);
			n = 0;
			correct = 0;
			error = 0.F;
		}
	}

	m_pNN->Save(ModelFile);

	printf("=== Training MLP... Done.\n");

	system("PAUSE");

	printf("=== Testing MLP...\n");
	for(int j = 0; j < noSample; j++){
		m_pNN->Propagate(aSample[j].GetVector()); 
		float *output = m_pNN->GetOutput(); 
		printf("sample %d: (%.3f %.3f) --> ", j, aSample[j][0], aSample[j][1]);
		printf("(%.3f, %.3f, %.3f, %.3f, %.3f, %.3f, %.3f, %.3f, %.3f, %.3f)\n", output[0], output[1],output[2], output[3],output[4], output[5],output[6], output[7],output[8], output[9]); 
	}

	MyPause();
	return TRUE;
}


int RunDigitUI()
{
	HGUDigitUI digitUI;
	digitUI.LoadRecognizer(noLayer, aNetStruct, ModelFile);
	digitUI.Run();

	return TRUE;
}
