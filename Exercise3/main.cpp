#include <windows.h>
#include <string>
#include <iostream>
#include "../DigitalImageProcessing/ImageUtil.h"

using namespace ImageUtil;

IMGDATA advenage(IMGDATA data);
IMGDATA mid(IMGDATA data);
IMGDATA laplaceOstu(IMGDATA data);

void colorful(IMGDATA data, const std::string& path);
int getMid(int arr[9]);

int getRed(int g);
int getBlud(int g);
int getGreen(int g);

int main()
{
	std::string path;
	std::cin >> path;
	IMGDATA data = ImageUtil::loadImageToGray(path);
	IMGDATA laplace1 = data;
	ImageUtil::outputHistogram(data, "bitmap/before_advence_histogram.bmp");


	//----------------拉普拉斯滤波----------
	IMGDATA laplaceIMG = laplaceOstu(data);
	ImageUtil::outputImage(laplaceIMG, 256, "bitmap/laplace_step_1.bmp");
	laplaceIMG = data + (laplaceIMG * -1);
	ImageUtil::outputImage(laplaceIMG, 256, "bitmap/laplace.bmp");
	ImageUtil::outputHistogram(laplaceIMG, "bitmap/laplace_step_2.bmp");

	//---------------伪彩色化--------------
	colorful(data, "bitmap/colorful.bmp");

	//---------------中值滤波---------------
	IMGDATA midData = mid(data);
	int j = 0;
	while (++j < 4)
		midData = mid(midData);
	ImageUtil::outputHistogram(midData, "bitmap/after_mid_histogram.bmp");
	ImageUtil::outputImage(midData, 256, "bitmap/mid.bmp");


	//-------------平均滤波-----------------
	int i = 0;
	while (++i < 4)
		data = advenage(data);
	ImageUtil::outputHistogram(data, "bitmap/after_advence_histogram.bmp");
	ImageUtil::outputImage(data, 256, "bitmap/advenage.bmp");

	
	return 0;
}


void colorful(ImageUtil::IMGDATA data, const std::string& path)
{
	IMGDATA newData = data;
	newData.infoHeader.biBitCount = 24;
	newData.infoHeader.biClrUsed = 0;
	newData.infoHeader.biSizeImage = ((data.width * 3 + 3) / 4 * 4) * data.height;
	newData.fileHeader.bfOffBits = 54;
	newData.fileHeader.bfSize = 54 + newData.infoHeader.biSizeImage;

	BYTE * newImg = new BYTE[data.width * data.height * 3];
	int point = -1;
	for (int i = 0; i < data.height; i++)
	{
		for (int j = 0; j < data.width; j++)
		{
			//bgr
			newImg[++point] = getBlud(data.pImg[i*data.width + j]);
			newImg[++point] = getGreen(data.pImg[i*data.width + j]);
			newImg[++point] = getRed(data.pImg[i*data.width + j]);
		}
	}

	newData.pImg = newImg;
	outputImage(newData, 0, path);
}

int getGreen(int g)
{
	if (g <= 130)
		return clamp((255 / 130) * g);
	else
		return clamp((-2.04 * g) + 520.2);
}

int getBlud(int g)
{
	return clamp((-255/130) * g + 255);
}

int getRed(int g)
{
	return clamp(2.04 * g - 265.2);
}



IMGDATA laplaceOstu(IMGDATA data)
{
	BYTE * newData = new BYTE[data.length];

	for (int i = 0; i < data.height; i++)
	{
		for (int j = 0; j < data.width; j++)
		{
			int up, down, left, right;
			if (i == 0)
				up = 0;
			else
				up = i - 1;

			if (i == data.height - 1)
				down = data.height - 1;
			else
				down = i + 1;

			if (j == 0)
				left = 0;
			else
				left = j - 1;

			if (j == data.width - 1)
				right = data.width - 1;
			else
				right = j + 1;

			newData[i * data.width + j] = clamp(
				1 * data.pImg[up * data.width + left] + 1 * data.pImg[up * data.width + j] + 1 * data.pImg[up * data.width + right] +
				1 * data.pImg[i * data.width + left] + -8 * data.pImg[i * data.width + j] + 1 * data.pImg[i * data.width + right] +
				1 * data.pImg[down * data.width + left] + 1 * data.pImg[down * data.width + j] + 1 *data.pImg[down * data.width + right]);
		}

	}

	IMGDATA imgData = data;

	imgData.pImg = newData;
	return imgData;
}

IMGDATA mid(IMGDATA data)
{
	BYTE *newData = new BYTE[data.length];
	for(int i = 1;i < data.height - 1;i++)
	{
		for(int j = 1;j < data.width - 1;j++)
		{
			int arr[9] = {
				data.pImg[(i - 1) * data.width + j - 1],data.pImg[(i - 1) * data.width + j],data.pImg[(i - 1)*data.width + j + 1],
				data.pImg[(i)* data.width + j - 1],data.pImg[(i)* data.width + j],data.pImg[(i)*data.width + j + 1],
				data.pImg[(i + 1) * data.width + j - 1],data.pImg[(i + 1) * data.width + j],data.pImg[(i + 1)*data.width + j + 1] };
			newData[i * data.width + j] = getMid(arr);
			//delete[] arr;
		}
	}


	IMGDATA newImg = data;
	newImg.pImg = newData;
	return newImg;
}



int getMid(int arr[9])
{
	for(int i = 0;i < 9;i++)
	{
		for(int j = 0 ;j < 8;j++)
		{
			if(arr[j] > arr[j + 1])
			{
				const int temp = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = temp;
			}
		}
	}
	return arr[4];
}

IMGDATA advenage(IMGDATA data)
{
	BYTE * newData = new BYTE[data.length];

	for (int i = 0; i < data.height; i++)
	{
		for (int j = 0; j < data.width; j++)
		{
			int up, down,left,right;
			if (i == 0)
				up = 0;
			else
				up = i - 1;

			if (i == data.height - 1)
				down = data.height - 1;
			else
				down = i + 1;

			if (j == 0)
				left = 0;
			else
				left = j - 1;

			if (j == data.width - 1)
				right = data.width - 1;
			else
				right = j + 1;

			newData[i * data.width + j] = clamp(
				(data.pImg[up * data.width + left] + 2 * data.pImg[up * data.width + j] + data.pImg[up * data.width + right] +
					2 * data.pImg[i * data.width + left] + 4 * data.pImg[i * data.width + j] + 2 * data.pImg[i * data.width + right] +
					data.pImg[down * data.width + left] + 2 * data.pImg[down * data.width + j] + data.pImg[down * data.width + right]) / 16);
 		}

	}

	delete[] data.pImg;
	data.pImg = newData;
	return data;
}

