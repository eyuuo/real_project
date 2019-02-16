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

#ifndef __DigitImage__
#define __DigitImage__

#define DigitWidth 5
#define DigitHeight 7
#define DigitSize 35

class HGUDigitImage {
	float vector[DigitSize];

public:
	HGUDigitImage();
	float& Pixel(int x, int y)	{ return vector[y * DigitWidth + x]; }
	float& operator[](int idx)  { return vector[idx]; }
	float* GetVector()			{ return vector; }

	void Clear(float v);
	void SetDigit(int d);		// 0 <= d <= 9
	void AddNoise(float flipProb);

	void SetImage(float *src);
	void Read(const char *file);

	HGUDigitImage& operator=(const HGUDigitImage &src);

	void Display();
	void DisplayAsArray();
};


#endif // !__DigitImage__
