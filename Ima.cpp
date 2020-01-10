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

void CIma::Contour(unsigned char * ImagTrait, int Larg, int Haut)
{
	int Coul = 0;	
	int P11, P12, P21, P22;
	for (int i = 0; i <Haut - 1; i++) {
		for (int j = 0; j < Larg - 1; j++) {

		for (Coul = 0; Coul < 3; Coul++) {
				P11 = (i * Larg + j) * 3 + Coul;	
				P12 = (i * Larg + (j+1)) * 3 + Coul;
				P21 = ((i+1) * Larg  + j) * 3 + Coul;
				P22 = ((i+1) * Larg + (j+1)) * 3 + Coul;
				
				ImagTrait[P11] = (int) sqrt((ImagTrait[P11] - ImagTrait[P22])*(ImagTrait[P11] - ImagTrait[P22])  +  (ImagTrait[P12] - ImagTrait[P21])*(ImagTrait[P12] - ImagTrait[P21]));
				
			}
		}
	}
}

void CIma::Contour2(unsigned char * ImagTrait, int Larg, int Haut)
{
	int Coul = 0;
	int P11, P12, P21, P22, P13, x12, P31, P32, P33;
	for (int i = 0; i < Haut - 1; i++) {
		for (int j = 0; j < Larg - 1; j++) {

			for (Coul = 0; Coul < 3; Coul++) {
				P11 = (i * Larg + j) * 3 + Coul;
				P12 = (i * Larg + (j + 1)) * 3 + Coul;
				P21 = ((i + 1) * Larg + j) * 3 + Coul;
				P22 = ((i + 1) * Larg + (j + 1)) * 3 + Coul;
				P13 = ((i)* Larg + (j + 2)) * 3 + Coul;
				x12 = ((i + 1) * Larg + (j + 2)) * 3 + Coul;
				P31 = ((i + 2) * Larg + (j)) * 3 + Coul;
				P32 = ((i + 2) * Larg + (j + 1)) * 3 + Coul;
				P33 = ((i + 2) * Larg + (j + 2)) * 3 + Coul;
				
				ImagTrait[P11] = (int)sqrt(ImagTrait[P11] + ImagTrait[P21] + ImagTrait[P33] - ImagTrait[P13] - ImagTrait[x12] - ImagTrait[P33]);
			}
		}
	}
}

void CIma::inversion(unsigned char * ImagTrait, int Larg, int Haut)
{
	int Coul = 0;
	int P11, P12, P21, P22;
	for (int i = 0; i < Haut - 1; i++) {
		for (int j = 0; j < Larg - 1; j++) {

			for (Coul = 0; Coul < 3; Coul++) {
				P11 = (i * Larg + j) * 3 + Coul;
				ImagTrait[P11] = 255 - ImagTrait[P11];
			}
		}
	}
}

void CIma::seuillage(unsigned char * ImagTrait, int Larg, int Haut)
{
	int Coul = 0;
	int P11, P12, P21, P22;
	for (int i = 0; i < Haut - 1; i++) {
		for (int j = 0; j < Larg - 1; j++) {

			for (Coul = 0; Coul < 3; Coul++) {
				P11 = (i * Larg + j) * 3 + Coul;
				if (ImagTrait[P11] > 130) {
					ImagTrait[P11] = 255;
				}
				else ImagTrait[P11] = 0;
			}
		}
	}

}
unsigned char * ImagTrait2;
void CIma::rotation(unsigned char * ImagTrait, int Larg, int Haut)
{
	
	int In, Jn,i,j, tita = 3.1415, Tx = Larg/2, Ty = Haut/2;
	int Coul = 0, k=0;
	int P11, P11n;
	ImagTrait2 = (unsigned char *)malloc(Larg * Haut * 3 * sizeof(unsigned char *));
	
	for (int In = 0; In < Haut - 1; In++) {
		for (int Jn = 0; Jn < Larg - 1; Jn++) {
			i =(int)( In * cos(tita) - Jn * sin(tita) + Tx);
			j = (int)(Jn * sin(tita) + Jn * cos(tita) + Ty);
			
				for (Coul = 0; Coul < 3; Coul++) {

					if ((i >= 0 && i <= Larg - 1) && (j >= 0 && j <= Haut - 1))
					{
						P11n = (In * Larg + Jn) * 3 + Coul;
						P11 = (i * Larg + j) * 3 + Coul;
						ImagTrait2[P11n] = ImagTrait[P11];
					}
					else {
						P11n = (In * Larg + Jn) * 3 + Coul;
						ImagTrait2[P11n] = 0;
					}
					k++;


				}
			
			
			
				
				
			
		}
	}

	ImagTrait = ImagTrait2;



}


void CIma::Flou(unsigned char * ImagTrait, int Larg, int Haut)
{
	int Coul = 0;
	int k = 0;
	int P11, P12, P13, P21, P22, P23, P30, P31, P33;
	while (k < 20)
	{
		for (int i = 0; i < Haut - 4; i++) {
			for (int j = 0; j < Larg - 4; j++) {

				for (Coul = 0; Coul < 3; Coul++) {
					P11 = (i * Larg + j) * 3 + Coul;
					P12 = (i * Larg + (j + 1)) * 3 + Coul;
					P13 = (i * Larg + (j + 2)) * 3 + Coul;

					P21 = ((i + 1) * Larg + j) * 3 + Coul;
					P22 = ((i + 1) * Larg + (j + 1)) * 3 + Coul;
					P23 = ((i + 1) * Larg + (j + 2)) * 3 + Coul;

					P30 = ((i + 2) * Larg + j) * 3 + Coul;
					P31 = ((i + 2) * Larg + (j + 1)) * 3 + Coul;
					P33 = ((i + 2) * Larg + (j + 2)) * 3 + Coul;

					ImagTrait[P11] = (int)((1 * ImagTrait[P11] + 1 * ImagTrait[P12] + 1 * ImagTrait[P13]
						+ 1 * ImagTrait[P21] + 1 * ImagTrait[P22] + 1 * ImagTrait[P23]
						+ 1 * ImagTrait[P30] + 1 * ImagTrait[P31] + 1 * ImagTrait[P33]) / 9);
				}


			}
		}
		k++;

	}
}

