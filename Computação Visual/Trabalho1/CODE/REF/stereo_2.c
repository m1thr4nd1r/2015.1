#include <stdio.h>
#include <cv.h>
#include <highgui.h>
#include <stdlib.h>

#define SQR(a) (a)*(a)

int compare (const void * a, const void * b){
  return ( *(int*)a - *(int*)b );
}

void medianBlur(IplImage * img, IplImage * mask, int m){
	int i, j, a, b, w = img->width, h = img->height;
	int vet[m * m];
	printf("MEDIAN: %d x %d\n", w,h);
	for(a = 0; a < h; a++){
		for(b = 0; b < w; b++){
			long cont = 0, base = m / 2;
			for(i = -base; i <= base; i++){
				for(j = -base; j <= base; j++){
					if(a + i < 0 || b + j < 0 || a + i > h || b + j > w)
						continue;
					vet[cont] = CV_IMAGE_ELEM(img, uchar, a + i , b + j);
					cont++;
				}
			}
			qsort(vet, cont, sizeof(int), compare); //quickSort
			CV_IMAGE_ELEM(mask, uchar, a, b) = (cont % 2 == 0) ? (vet[cont / 2] + vet[cont / 2 + 1]) / 2 : vet[cont / 2 + 1];
		}
	}
}

int main(int argc, char **argv) {
	IplImage *esq, *dir, *disp, *res, *erro;
	int i, j, k, n, d, dist, distmin, avgerro;

	esq = cvLoadImage(argv[1], 1);
	dir = cvLoadImage(argv[2], 1);
	disp = cvLoadImage(argv[3], 0);

	res = cvCreateImage(cvSize(esq->width,esq->height), IPL_DEPTH_8U, 1);
	erro = cvCreateImage(cvSize(esq->width,esq->height), IPL_DEPTH_8U, 1);
	res = cvCreateImage(cvSize(esq->width,esq->height), IPL_DEPTH_8U, 1);

	avgerro=0; n=0;
	for(i=0; i < esq->height; i++) {
		for(j=0; j < esq->width; j++) {
			distmin = INT_MAX;
			d = 0;
			
//------------------- :: VALOR DE COR MAIS PARECIDO :: ------------------

			for(k = j; k < dir->width; k++) {
				dist = SQR(CV_IMAGE_ELEM(esq,uchar,i,k*3) - CV_IMAGE_ELEM(dir,uchar,i,j*3)) + SQR(CV_IMAGE_ELEM(esq,uchar,i,k*3+1) - CV_IMAGE_ELEM(dir,uchar,i,j*3+1)) + SQR(CV_IMAGE_ELEM(esq,uchar,i,k*3+2) - CV_IMAGE_ELEM(dir,uchar,i,j*3+2));
				if(dist < distmin) {
					distmin = dist;
					d = k-j;
				}
			}

//---------------------- :: CALCULA VALOR ERRO :: ------------------------

			CV_IMAGE_ELEM(res,uchar,i,j) = d * 8; //NAO MUDAR
			if(CV_IMAGE_ELEM(disp,uchar,i,j)) {
				CV_IMAGE_ELEM(erro,uchar,i,j) = abs(CV_IMAGE_ELEM(disp,uchar,i,j)-CV_IMAGE_ELEM(res,uchar,i,j));
				avgerro += CV_IMAGE_ELEM(erro,uchar,i,j);
				n++;
			}
			else
				CV_IMAGE_ELEM(erro,uchar,i,j) = 0;
		}

	}
	
	printf("Erro médio: %lf\n", (double)avgerro / n);

	while(cvWaitKey(10) < 0) {
		cvShowImage("Direita", dir);
		cvShowImage("Esquerda", esq);
		cvShowImage("Disparidade", disp);
		cvShowImage("Resultado", res);
		cvShowImage("Erro", erro);
	}
}

