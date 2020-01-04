#include "stdafx.h"
#include "Ima.h"


CIma::CIma()
{
}

CIma::~CIma()
{
}

byte CIma::clip(int a)
{
	if (a>255) return  (byte)255;
	if (a<0) return  (byte)0;
	return (byte)a;
}

void CIma::ConvertYUY2RGB(BYTE *PtrIn, BYTE *ptrOut, int Larg, int Haut)
{


	int Larg2 = Larg / 2;
	byte r, g, b;
	byte y, u, v;
	byte *pIm = PtrIn;
	int p, l, c, col, c4;
	int y0, u0, v0, y1;

	p = 0;
	for (l = 0; l<Haut; l++)
	{
		for (c = 0; c<Larg2; c++)
		{
			y0 = pIm[0];
			u0 = pIm[1];
			y1 = pIm[2];
			v0 = pIm[3];
			pIm += 4;
			int c = y0 - 16;
			int d = u0 - 128;
			int e = v0 - 128;
			ptrOut[p + 0] = clip((298 * c + 516 * d + 128) >> 8); // blue
			ptrOut[p + 1] = clip((298 * c - 100 * d - 208 * e + 128) >> 8); // green
			ptrOut[p + 2] = clip((298 * c + 409 * e + 128) >> 8); // red

			p += 3;

			c = y1 - 16;
			ptrOut[p + 0] = clip((298 * c + 516 * d + 128) >> 8); // blue
			ptrOut[p + 1] = clip((298 * c - 100 * d - 208 * e + 128) >> 8); // green
			ptrOut[p + 2] = clip((298 * c + 409 * e + 128) >> 8); // red

			p += 3;
		}
	}

}

void CIma::ConvertRGBYUY2(BYTE *ptrIn, BYTE *pyuv, int Larg, int Haut)
{

	float R, G, B, Y, U, V;
	int p, l, c, col, c4;
	int Larg2 = Larg / 2;
	byte *pIm = ptrIn;
	byte y, u, v;
	col = 0;
	p = 0;

	for (l = 0; l < Haut; l++)
	{
		for (c = 0; c < Larg; c++)
		{
			R = (float)pIm[p + 2];   //R
			G = (float)pIm[p + 1]; // G
			B = (float)pIm[p];  // B
			Y = (0.256788 * R + 0.504129 * G + 0.097906 * B) + 16;
			U = (-0.148223 * R - 0.290993 * G + 0.439216 * B) + 128;
			V = (0.439216 * R - 0.367788 * G - 0.071427 * B) + 128;

			pyuv[col + 0] = (byte)Y;
			pyuv[col + 1] = (byte)U;
			pyuv[col + 3] = (byte)V;
			c++;
			p += 3;
			R = (float)pIm[p + 2];   //R
			G = (float)pIm[p + 1]; // G
			B = (float)pIm[p];  // B
			Y = (0.256788 * R + 0.504129 * G + 0.097906 * B) + 16;
			y = (byte)Y;
			pyuv[col + 2] = y;
			col += 4;
			p += 3;
		}
	}

}

void CIma::Contour(unsigned char * ImrgbPtr, int Larg, int Haut)
{
	int colorIdx = 0;	
	int x00, x01, x10, x11;
	for (int i = 0; i <Haut - 1; i++) {
		for (int j = 0; j < Larg - 1; j++) {

		for (colorIdx = 0; colorIdx < 3; colorIdx++) {
				x00 = (i * Larg + j) * 3 + colorIdx;	
				x01 = (i * Larg + (j+1)) * 3 + colorIdx;
				x10 = ((i+1) * Larg  + j) * 3 + colorIdx;
				x11 = ((i+1) * Larg + (j+1)) * 3 + colorIdx;
				
				ImrgbPtr[x00] = (int) sqrt((ImrgbPtr[x00] - ImrgbPtr[x11])*(ImrgbPtr[x00] - ImrgbPtr[x11])  +  (ImrgbPtr[x01] - ImrgbPtr[x10])*(ImrgbPtr[x01] - ImrgbPtr[x10]));
				
			}
		}
	}
}

void CIma::Contour2(unsigned char * ImrgbPtr, int Larg, int Haut)
{
	int colorIdx = 0;
	int x00, x01, x10, x11, x02, x12, x20, x21, x22;
	for (int i = 0; i < Haut - 1; i++) {
		for (int j = 0; j < Larg - 1; j++) {

			for (colorIdx = 0; colorIdx < 3; colorIdx++) {
				x00 = (i * Larg + j) * 3 + colorIdx;
				x01 = (i * Larg + (j + 1)) * 3 + colorIdx;
				x10 = ((i + 1) * Larg + j) * 3 + colorIdx;
				x11 = ((i + 1) * Larg + (j + 1)) * 3 + colorIdx;
				x02 = ((i)* Larg + (j + 2)) * 3 + colorIdx;
				x12 = ((i + 1) * Larg + (j + 2)) * 3 + colorIdx;
				x20 = ((i + 2) * Larg + (j)) * 3 + colorIdx;
				x21 = ((i + 2) * Larg + (j + 1)) * 3 + colorIdx;
				x22 = ((i + 2) * Larg + (j + 2)) * 3 + colorIdx;
				
				ImrgbPtr[x00] = (int)sqrt(ImrgbPtr[x00] + ImrgbPtr[x10] + ImrgbPtr[x22] - ImrgbPtr[x02] - ImrgbPtr[x12] - ImrgbPtr[x22]);
			}
		}
	}
}

void CIma::inversion(unsigned char * ImrgbPtr, int Larg, int Haut)
{
	int colorIdx = 0;
	int x00, x01, x10, x11;
	for (int i = 0; i < Haut - 1; i++) {
		for (int j = 0; j < Larg - 1; j++) {

			for (colorIdx = 0; colorIdx < 3; colorIdx++) {
				x00 = (i * Larg + j) * 3 + colorIdx;
				ImrgbPtr[x00] = 255 - ImrgbPtr[x00];
			}
		}
	}



}

void CIma::seuillage(unsigned char * ImrgbPtr, int Larg, int Haut)
{
	int colorIdx = 0;
	int x00, x01, x10, x11;
	for (int i = 0; i < Haut - 1; i++) {
		for (int j = 0; j < Larg - 1; j++) {

			for (colorIdx = 0; colorIdx < 3; colorIdx++) {
				x00 = (i * Larg + j) * 3 + colorIdx;
				if (ImrgbPtr[x00] > 130) {
					ImrgbPtr[x00] = 255;
				}
				else ImrgbPtr[x00] = 0;
			}
		}
	}

}
