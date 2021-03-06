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

#ifndef __DigitUI__
#define __DigitUI__

#define ModelFile "digit.wgt"

extern int noLayer;
extern int aNetStruct[];

class HGUDigitUI {
	int m_hScale, m_vScale;
	int m_windowWidth, m_windowHeight;
	int m_sx, m_sy;

	HGUDigitImage m_digit;
	HGUNeuralNetwork *m_pNN;

public:
	HGUDigitUI(){
		m_hScale = 4;
		m_vScale = 2;
		m_windowWidth = m_windowHeight = 0;
		m_sx = m_sy = 0;
		m_pNN = NULL;
	}

	virtual ~HGUDigitUI();

	void SetScale(int hScale, int vScale){
		m_hScale = hScale;
		m_vScale = vScale;
	}

	int Run();
	void DrawScreen();
	void DisplayDigit(int sx, int sy, HGUDigitImage *pDigit, int hScale = 0, int vScale = 0);
	void DrawBigDot(int x, int y, int width, int height, char v);
	//점을 찍어주는
	void DrawBox(int left, int top, int right, int bottom, char v);
	//테두릴. 박스를 그려주는,
	int LoadRecognizer(int noLayer, int pNetStruct[], const char *weightFile);
	int RecognizeDigit();//우리가 짜야하는 1
	int TrainRecognizer(int maxEpoch);//우리가 짜야하는2 //10가지 디짓패턴을 읽어와서 뉴런네트워크에 학습시켜야한다.
};

int RunDigitUI();

#endif // !__DigitUI__
