#include <iostream>
#include <cstdarg>
#include <cmath>
#include <cstdlib>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;
using namespace std;

#define SQR(a) (a)*(a)

// -----------------Codigo de Java-----------------

void copy(IplImage * src, IplImage * dst, int step, char flag){
	int i, j;
    for(i = 0; i < src->height; ++i){
		for(j = 0; j < src->width; ++j){
    		if(flag == 'v')
        		CV_IMAGE_ELEM(dst, uchar, step + i, j) = CV_IMAGE_ELEM(src, uchar, i, j);
        	else
        		CV_IMAGE_ELEM(dst, uchar, i, step + j) = CV_IMAGE_ELEM(src, uchar, i, j);
    	}
    } 
}

IplImage * join(char flag, int esp, IplImage * base, int n, ... ){

	IplImage * join;
	if(flag == 'h')
		join = cvCreateImage(cvSize(base->width * (n + 1) + esp * n, base->height), base->depth, base->nChannels);
	else
		join = cvCreateImage(cvSize(base->width, base->height * (n + 1) + esp  * n), base->depth, base->nChannels);

	copy(base, join, 0, flag);

	const int dim = (flag == 'h') ? base->width : base->height;
	int passo = (flag == 'h') ? base->width : base->height; 

	va_list args;
	va_start(args, n);
	for(int x = 0; x < n; x++){
        IplImage * im = va_arg(args, IplImage *);
        copy(im, join, passo + x * dim + esp * (x + 1), flag);
	}
    va_end(args);
	return join; 
}

// ------------------------------------------------
int base(IplImage *esq, IplImage *dir, int i, int j, int k)
{
	// -------------- Correspondencia por Cor mais proxima --------------
	int dist = SQR(CV_IMAGE_ELEM(esq,uchar,i,k*3)-CV_IMAGE_ELEM(dir,uchar,i,j*3)) + SQR(CV_IMAGE_ELEM(esq,uchar,i,k*3+1)-CV_IMAGE_ELEM(dir,uchar,i,j*3+1)) + SQR(CV_IMAGE_ELEM(esq,uchar,i,k*3+2)-CV_IMAGE_ELEM(dir,uchar,i,j*3+2));
	return dist;
}
// ----------------------- Erro medio ~= 41 -------------------------

int integralSum(IplImage* real, int h, int k, int n)
{
	// cout << "h: " << min(h + n - 1,real->height-1) << " " << h << " | k: " << min(k + n - 1, real->width-1) << " " << k << " | n: " << n << endl;
	int dist = CV_IMAGE_ELEM(real, int, min(h + n - 1,real->height-1), min(k + n - 1, real->width-1));
	// cout << "1:" << dist;
	if (k > 0)
		dist -= CV_IMAGE_ELEM(real, int, min(h + n - 1,real->height-1), k - 1);
	// cout << " 2:" << dist;
	if (h > 0)
		dist -= CV_IMAGE_ELEM(real, int, h - 1, min(k + n - 1, real->width-1));
	// cout << " 3:" << dist;
	if (h > 0 && k > 0)
		dist += CV_IMAGE_ELEM(real, int, h - 1, k - 1);
	// cout << " 4:" << dist << endl;
	
	return dist;
}

void integralImage(IplImage *esq, IplImage* dir, IplImage *integral,int d)
{
	for (int i = 0; i < esq->height; i++)
		for (int j = 0; j+d < esq->width; j++)
		{
				// cout << "i: " << i << "| j: " << j << " | ";
				CV_IMAGE_ELEM(integral, int, i, j) = abs(CV_IMAGE_ELEM(esq,uchar,i,(j+d)*3)-CV_IMAGE_ELEM(dir,uchar,i,j*3)) +
			 									     abs(CV_IMAGE_ELEM(esq,uchar,i,(j+d)*3+1)-CV_IMAGE_ELEM(dir,uchar,i,j*3+1)) +
				 									 abs(CV_IMAGE_ELEM(esq,uchar,i,(j+d)*3+2)-CV_IMAGE_ELEM(dir,uchar,i,j*3+2));
				// cout << "integral: " << (int)CV_IMAGE_ELEM(integral, int, i, j) << " ";
				if (j > 0)
					CV_IMAGE_ELEM(integral, int, i, j) += CV_IMAGE_ELEM(integral, int, i, j-1);
				// cout << "integral: " << (int)CV_IMAGE_ELEM(integral, int, i, j) << " ";
				if (i > 0)
					CV_IMAGE_ELEM(integral, int, i, j) += CV_IMAGE_ELEM(integral, int, i-1, j);
				// cout << "integral: " << (int)CV_IMAGE_ELEM(integral, int, i, j) << " ";
				if (i > 0 && j > 0)
					CV_IMAGE_ELEM(integral, int, i, j) -= CV_IMAGE_ELEM(integral, int, i-1, j-1);
				// cout << "integral: " << (int)CV_IMAGE_ELEM(integral, int, i, j) << endl;
		}
}

// Correspondencia por vizinhança de N pixels
int stereoI(IplImage *esq, IplImage *dir, int i, int j, int k, int n)
{
	int dist = 0;
	for (int x = 0; x < n; x++)
		for (int y = 0; y < n; y++)
			if (i+x >= 0 && i+x < esq->height && k+y >= 0 && k+y < dir->width && j+y >= 0 && j+y < esq->width)
				dist += abs(CV_IMAGE_ELEM(esq,uchar,i+x,(k+y)*3)-CV_IMAGE_ELEM(dir,uchar,i+x,(j+y)*3)) +
						abs(CV_IMAGE_ELEM(esq,uchar,i+x,(k+y)*3+1)-CV_IMAGE_ELEM(dir,uchar,i+x,(j+y)*3+1)) +
						abs(CV_IMAGE_ELEM(esq,uchar,i+x,(k+y)*3+2)-CV_IMAGE_ELEM(dir,uchar,i+x,(j+y)*3+2));	
			
	// cout << "dist:" << dist << " i: " << i+n-1 << " j: " << j+n-1 << " k: " << k+n-1 << " n: " << n << endl;
	return dist;	
}

void meanNeighbours(IplImage* img, IplImage *dest, int n)
{
	int sum[3],qnt;

	int dist = 0;
	for (int i = 0; i < img->height; i++)
		for (int j = 0; j < img->width; j++)
		{
			sum[0] = sum[1] = sum[2] = qnt = 0;

			for (int k = 0; k < n; k++)
				for (int h = 0; h < n; h++)
					if (i+k < img->height && j+h < img->width)
					{
						sum[0] += CV_IMAGE_ELEM(img,uchar,i+k,(j+h)*3);
						sum[1] += CV_IMAGE_ELEM(img,uchar,i+k,(j+h)*3+1);
						sum[2] += CV_IMAGE_ELEM(img,uchar,i+k,(j+h)*3+2);
						qnt++;
					}

			sum[0] /= qnt;
			sum[1] /= qnt;
			sum[2] /= qnt;

			CV_IMAGE_ELEM(dest,uchar,i,j*3) = CV_IMAGE_ELEM(img,uchar,i,j*3) - max(CV_IMAGE_ELEM(img,uchar,i,j*3) - sum[0],0);
			CV_IMAGE_ELEM(dest,uchar,i,j*3+1) = CV_IMAGE_ELEM(img,uchar,i,j*3+1) - max(CV_IMAGE_ELEM(img,uchar,i,j*3+1) - sum[1],0);
			CV_IMAGE_ELEM(dest,uchar,i,j*3+2) = CV_IMAGE_ELEM(img,uchar,i,j*3+2) - max(CV_IMAGE_ELEM(img,uchar,i,j*3+2) - sum[2],0);
		}
}

int main(int argc, char **argv) {
	IplImage *esq, *dir, *disp, *res, *erro, *nEsq, *nDir;
	int i, j, k, n, d, dist,dist2, distmin, avgerro;
	string file = argv[1];

	const clock_t begin = clock();

	// esq = cvLoadImage((file+"esq.ppm").c_str(), 1);
	// dir = cvLoadImage((file+"dir.ppm").c_str(), 1);
	// disp = cvLoadImage((file+"disp.pgm").c_str(), 0);
	esq = cvLoadImage(argv[1], 1);
	dir = cvLoadImage(argv[2], 1);
	disp = cvLoadImage(argv[3], 0);
	res = cvCreateImage(cvSize(esq->width,esq->height), IPL_DEPTH_8U, 1);
	// IplImage *res2 = cvCreateImage(cvSize(esq->width,esq->height), IPL_DEPTH_8U, 1);
	erro = cvCreateImage(cvSize(esq->width,esq->height), IPL_DEPTH_8U, 1);
	nEsq = cvCreateImage(cvSize(esq->width,esq->height), IPL_DEPTH_8U, 3);
	nDir = cvCreateImage(cvSize(dir->width,dir->height), IPL_DEPTH_8U, 3);

	int neighbours = 11;
	meanNeighbours(esq,nEsq,3);
	meanNeighbours(dir,nDir,3);
	
	IplImage **integral = (IplImage**) malloc(dir->width * sizeof(IplImage*));
	for (int i = 0; i < dir->width; i++)
	{
		integral[i] = cvCreateImage(cvSize(dir->width,dir->height), IPL_DEPTH_32S, 1);
		integralImage(nEsq,nDir,integral[i],i);
	}

	int lowerMask = 2, higherMask = 32, avg = INT_MAX;
	avgerro=0; n=0;
	// for (int neighbours = 3; neighbours < 32; neighbours+=2)
	// {
		for(i=0; i < esq->height; i++) 
		{
			for(j=0; j < esq->width; j++) 
			{
				distmin = INT_MAX;
				d = 0;
				for(k = j + lowerMask; k < j + higherMask; k++)
				// for (k = j; k < esq->width; k++)
				{	
					// cout << "i: " << i << " | j: " << j << " | k: " << k << " | dist: " << dist << " | real: " << (int)CV_IMAGE_ELEM(integral[k-j],uchar,i+neighbours/2,j+neighbours/2) << endl;
					// if ((k + neighbours / 2) > esq->width-1)
					// {
					// 	// cout << "*" << endl;
					// 	//dist = integralSum(integral[k-j], i - neighbours / 2 , (j - neighbours / 2) - (neighbours / 2 + k - (esq->width-1)), neighbours);					
					// 	dist = stereoI(nEsq, nDir, i - neighbours / 2, j - neighbours / 2, k - neighbours / 2, neighbours); //stereo(nEsq,nDir,i,j,k);				
								
					// }
					// else
						dist = integralSum(integral[k-j], i - neighbours / 2, j - neighbours / 2, neighbours);
					
					if(dist < distmin) 
					{
						distmin = dist;
						d = k-j;
					}

					//dist2 = stereoI(nEsq, nDir, i - neighbours / 2, j - neighbours / 2, k - neighbours / 2, neighbours); //stereo(nEsq,nDir,i,j,k);				
					// cout << "dist: " << dist << endl;
					// if(dist2 < newDistMin) 
					// {
					// 	newDistMin = dist2;
					// 	newD = k-j;
					// }

					// if (dist2 != dist)
					// {
						// for (int p = 0; p < dir->height; p++)
						// {
						// 	for (int o = 0; o < dir->width; o++)
						// 		cout << (int)CV_IMAGE_ELEM(integral[k-j],int,p,o) << ";";
						// 	cout << endl;
						// }

						// cout << "i: " << i  << " j: " << j << " k: " << k << " d: " << dist << " d2: " << dist2 << " s: " << (j - neighbours / 2) << " v: " << (neighbours / 2 + k - (esq->width-1)) << endl;
						// dist = CV_IMAGE_ELEM(integral[k-j], int, min(i-neighbours / 2 + n - 1,integral[k-j]->height-1), min(j-neighbours / 2 + n - 1, integral[k-j]->width-1));
						// cout << "h: " << min(i-neighbours / 2 + n - 1,integral[k-j]->height-1) << " | k: " << min(j-neighbours / 2 + n - 1, integral[k-j]->width-1) << " | n: " << n << endl;
						// cout << "1:" << dist;
						// if (j-neighbours / 2 > 0)
						// 	dist -= CV_IMAGE_ELEM(integral[k-j], int, min(i-neighbours / 2 + n - 1,integral[k-j]->height-1), j-neighbours / 2 - 1);
						// cout << " 2:" << dist;
						// if (i-neighbours / 2 > 0)
						// 	dist -= CV_IMAGE_ELEM(integral[k-j], int, i-neighbours / 2 - 1, min(j-neighbours / 2 + n - 1, integral[k-j]->width-1));
						// cout << " 3:" << dist;
						// if (i-neighbours / 2 > 0 && j-neighbours / 2 > 0)
						// 	dist += CV_IMAGE_ELEM(integral[k-j], int, i-neighbours / 2 - 1, j-neighbours / 2 - 1);
						// cout << " 4:" << dist << endl;
					
						// cout << i-neighbours / 2 << " " << j-neighbours / 2 << " " << k-neighbours / 2 << endl;
						// for (int x = 0; x < neighbours; x++)
						// {
						// 	for (int y = 0; y < neighbours; y++)
						// 		if ((i-neighbours / 2 + x) >= 0 && (i-neighbours / 2 + x) < esq->height && (k-neighbours / 2 +y) >= 0 && (k-neighbours / 2 +y) < esq->width && (j-neighbours / 2 +y) >= 0 && (j-neighbours / 2 +y) < dir->width)
						// 		{
						// 			 cout << (int)(abs(CV_IMAGE_ELEM(nEsq,uchar,i-neighbours / 2 + x,(j-neighbours / 2 +y)*3)-CV_IMAGE_ELEM(nDir,uchar,i-neighbours / 2 + x,(k-neighbours / 2 +y)*3)) +
						// 					abs(CV_IMAGE_ELEM(nEsq,uchar,i-neighbours / 2 + x,(j-neighbours / 2 +y)*3+1)-CV_IMAGE_ELEM(nDir,uchar,i-neighbours / 2 + x,(k-neighbours / 2 +y)*3+1)) +
						// 					abs(CV_IMAGE_ELEM(nEsq,uchar,i-neighbours / 2 + x,(j-neighbours / 2 +y)*3+2)-CV_IMAGE_ELEM(nDir,uchar,i-neighbours / 2 + x,(k-neighbours / 2 +y)*3+2))) << ";";	
						// 			// cout << "dist:" << dist << " i: " << i+x << " j: " << j-neighbours/2+y << " k: " << (k-neighbours / 2 +y) << " n: " << n << endl;
						// 			 cout << "?" << (i-neighbours / 2 + x) << " " << (k-neighbours / 2 +y) << " " << (j-neighbours / 2 +y) << "?" << endl;
						// 		}
						// 		else
						// 		{
						// 			cout << "|" << (i-neighbours / 2 + x) << " " << (k-neighbours / 2 +y) << " " << (j-neighbours / 2 +y) << "|" << endl;
						// 		}
						// 	cout << endl;
						// }
						// cout << i-neighbours / 2 + neighbours - 1 << " " << j-neighbours / 2 + neighbours -1 << " " << k-neighbours / 2 + neighbours -1 << endl;

						// exit(0);					
					// }
				}
				// cout << "MinDist: " << distmin << " | d: " << d << " | newDistMin: " << newDistMin << " | newD: " << newD << endl;

	//			------------------------------------------------------------------
	//			------------------------ Calculo do Erro -------------------------
				CV_IMAGE_ELEM(res,uchar,i,j) = d*8;
				// CV_IMAGE_ELEM(res2,uchar,i,j) = newD*8;
				if(CV_IMAGE_ELEM(disp,uchar,i,j)) 
				{
					CV_IMAGE_ELEM(erro,uchar,i,j) = abs(CV_IMAGE_ELEM(disp,uchar,i,j)-CV_IMAGE_ELEM(res,uchar,i,j));
					avgerro += CV_IMAGE_ELEM(erro,uchar,i,j);
					n++;
				}
				else
					CV_IMAGE_ELEM(erro,uchar,i,j) = 0;
	//			------------------------------------------------------------------
			}
		}

	// 	if (avgerro < avg)
	// 		avg = avgerro;
	// 	else
	// 		break;
	// }
//  Para pixels dispares entre esq->dir e dir->esq calcular o valor baseado na media da vizinhança 

	cout << "Erro médio: " << (double) avgerro/n << endl << "Exec time: " << float( clock() - begin) / CLOCKS_PER_SEC << endl;
	// cout << "Erro médio: " << (double) avg/n << endl << "Exec time: " << float( clock() - begin) / CLOCKS_PER_SEC << endl;


    IplImage * out = join('h',30,disp,2,res,erro);
 //    IplImage * t = cvCreateImage(cvSize(esq->width,esq->height), IPL_DEPTH_8U, 1);
 //    IplImage * o = cvCreateImage(cvSize(esq->width,esq->height), IPL_DEPTH_8U, 1);
 //    for(i=0; i < esq->height; i++) 
 //    {
	// 	for(j=0; j < esq->width; j++)
	// 	{ 
	// 		CV_IMAGE_ELEM(t,uchar,i,j) = CV_IMAGE_ELEM(integral[29],int,i,j) -
	// 									 CV_IMAGE_ELEM(integral[29],int,i-1,j) - 
	// 									 CV_IMAGE_ELEM(integral[29],int,i,j-1) + 
	// 									 CV_IMAGE_ELEM(integral[29],int,i-1,j-1) ;

	// 		CV_IMAGE_ELEM(o,uchar,i,j) = abs(CV_IMAGE_ELEM(nEsq,uchar,i,j*3)-CV_IMAGE_ELEM(nDir,uchar,i,(j+1)*3)) +
	// 									 abs(CV_IMAGE_ELEM(nEsq,uchar,i,j*3+1)-CV_IMAGE_ELEM(nDir,uchar,i,(j+1)*3+1)) +
	// 									 abs(CV_IMAGE_ELEM(nEsq,uchar,i,j*3+2)-CV_IMAGE_ELEM(nDir,uchar,i,(j+1)*3+2));	
	// 	}
	// }

	while(cvWaitKey(10) < 0) {
		// cvShowImage("Direita", dir);
		// cvShowImage("Esquerda", esq);
		cvShowImage("meanedEsq", nEsq);
		cvShowImage("meanedDir", nDir);
		// cvShowImage("resIntegral", res);
		// cvShowImage("resSimple", res2);
		cvShowImage("Disp / Res / Erro", out);         
		// cvShowImage("integral[x]", t);
		// cvShowImage("manual_integral[x]", o);
	}
	cvDestroyAllWindows();
} 