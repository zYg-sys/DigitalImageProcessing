#pragma once
#include <windows.h>
#include <string>

namespace ImageUtil
{

	typedef struct ImageColor{
		BYTE r, g, b, a;
	}RGBA;

	typedef struct ImageData
	{
		BITMAPFILEHEADER fileHeader;
		BITMAPINFOHEADER infoHeader;
		RGBQUAD rgbquad[256];
		BYTE * pImg;
		int length;
		int width, height;

		ImageData& operator+(ImageData& d0);
		ImageData& operator*(float k);

	}IMGDATA;

	typedef struct GrayHistogram
	{
		double gray[256] = { 0 };
		int pixelCount = 0;
		void normalize();

	private:
		bool isNormalize = false;
	}GRAYHISTOGRAM;




	inline void GrayHistogram::normalize()
	{
		if (isNormalize)
			return;

		for (auto& i : gray)
		{
			i = i / pixelCount;
		}

		isNormalize = true;
	}

	ImageData loadImage(const std::string& path);
	ImageData loadImageToGray(const std::string& path);
	void outputImage(ImageData data, int clrUsed, const std::string&path);
	GRAYHISTOGRAM getHistogram(IMGDATA data);
	void outputHistogram(IMGDATA data, const std::string& path);



	int clamp(int c);
}
