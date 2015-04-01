#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <opencv/highgui.h>
#include <opencv/cv.h>
#include <stdarg.h>

typedef unsigned char px;

#define MAX_GRAY_BINS 256

/**
 * Comparacao
 * @param
 * @param
 * @return
 */
int compare (const void * a, const void * b){
  return ( *(int*)a - *(int*)b );
}

/**
 * Rearranje a lista de forma que todos os elementos anteriores ao pivo sejam menores que ele, 
 * e todos os elementos posteriores ao pivô sejam maiores que ele.
 * @param
 * @param
 * @param
 * @return posicao do pivo e haverah duas sublistas nao ordenadas.
 */
int partition(int * a, int l, int r) {
	int pivot = a[l], i = l, j = r + 1, t;
	while(1){
		do ++i; while( a[i] <= pivot && i <= r );
		do --j; while( a[j] > pivot );
   		if( i >= j ) break;
   		t = a[i]; a[i] = a[j]; a[j] = t;
	}
	t = a[l]; a[l] = a[j]; a[j] = t;
	return j;
}

/**
 * Dispensa apresentacao. Algoritmo de ordenacao teta(nlogn) no melhor e medio caso e teta(n^2) no pior caso.
 * Intervalo [l,r ] - Inicia em l e termina em r inclusivo
 * @param
 * @param
 * @param
 */
void quickSort( int * a, int l, int r){
	int j;
	if(l < r) {
		j = partition(a, l, r);
    	quickSort( a, l, j-1);
    	quickSort( a, j+1, r);
	}
}

/**
 * calcula o histograma da imagem
 * @param a imagem fonte
 * @param o histograma de entrada e saida
 * @param largura
 * @param altura
 */
void calcHistogram(IplImage * img, int * hist, int w, int h){
	int i, j;
	for(i = 0; i < MAX_GRAY_BINS; i++)
		hist[i] = 0;
	for(i = 0; i < w; i++)
		for(j = 0; j < h; j++)
			hist[CV_IMAGE_ELEM(img, uchar, i, j)]++;
}

/**
 * Borra a imagem utilizando a media aritmetica
 * @param a imagem fonte
 * @param a mascara de entrada e saida da imagem
 * @param filtro 3[3x3], 5[5x5] ...
 */
void averageBlur(IplImage * img, IplImage * mask, int m){
	int i, j, a, b, w = img->width, h = img->height;
	for(a = 0; a < h; a++){
		for(b = 0; b < w; b++){
			long soma = 0, cont = 0, base = m / 2;
			for(i = -base; i <= base; i++){
				for(j = -base; j <= base; j++){
					if(a + i < 0 || b + j < 0 || a + i > h || b + j > w)
						continue;
					soma += CV_IMAGE_ELEM(img, uchar, a + i , b + j);
					cont++;
				}
			}
			CV_IMAGE_ELEM(mask, uchar, a, b) = (unsigned char)(soma / cont);
		}
	}
}

/**
 * Borra a imagem utilizando a mediana dos pixels, eliminando ruidos na imagem.
 * @param a imagem fonte
 * @param a mascara de entrada e saida da imagem
 * @param filtro 3[3x3], 5[5x5] ...
 */
void medianBlur(IplImage * img, IplImage * mask, int m){
	int i, j, a, b, w = img->width, h = img->height;
	int vet[m * m];
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

/**
 * Faz a copia da imagem 'src' para 'dst' com espaco 'step' acima, se flag for 'v'(vertical)
 * ou por default flag eh 'h'(horizontal) e o espaco aparece a esquerda.
 * @param a imagem fonte
 * @param a imagem destino
 * @param tamanho do espaco
 * @param 'v' - vertical ou por default 'h' - horizontal
 */
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

/**
 * Junta a imagem base com as n outras imagens.
 * @param 'v' - vertical ou por default 'h' - horizontal
 * @param espaco entre as imagens
 * @param a imagem fonte
 * @param quantidade das imagens a serem unidas
 * @param as N imagens a serem unidas com a imagem fonte.
 */
IplImage * joinIplImage(char flag, int esp, IplImage * base, int n, ... ){
	int x, i, j;

	IplImage * join;
	if(flag == 'h')
		join = cvCreateImage(cvSize(base->width * (n + 1) + esp * n, base->height), base->depth, base->nChannels);
	else
		join = cvCreateImage(cvSize(base->width, base->height * (n + 1) + esp  * n), base->depth, base->nChannels);

	copy(base, join, 0, flag);

	int passo = (flag == 'h') ? base->width : base->height; 

	va_list args;
	va_start(args, n);
	for(x = 0; x < n; x++){
        IplImage * im = va_arg(args, IplImage *);
        copy(im, join, passo + x * base->width + esp * (x + 1), flag);
    }
    va_end(args);
    return join; 
}

//TODO
// void drawHistogram(){
// 	int bin_w = cvRound( (double) hist_w / MAX_GRAY_BINS);
// 	IplImage * histImage = cvCreateImage(cvSize(img->width, img->height),IPL_DEPTH_8U,1);
// 	for (int i = 1; i < n; ++i){
// 		line(histImage,Point( bin_w*(i-1), hist_h - cvRound(hist[i-1])) ) ,
//                        Point( bin_w*(i), hist_h - cvRound(b_hist.at<float>(i)) ),
//                        Scalar( 255, 0, 0), 2, 8, 0  );
		
// 	}
// }

int main(int argc, char ** argv){
    IplImage * img;	//aloca
	if(argc == 2 && (img  = cvLoadImage(argv[1],0)) != 0){

		int * hist = (int*) malloc(sizeof(int) * MAX_GRAY_BINS);
		calcHistogram(img, hist, img->width, img->height);

		IplImage * mask = cvCreateImage(cvSize(img->width, img->height),IPL_DEPTH_8U,1);
		averageBlur(img, mask, 11);

		IplImage * mask2 = cvCreateImage(cvSize(img->width, img->height),IPL_DEPTH_8U,1);
		medianBlur(img, mask2, 11);

		cvLine(img,cvPoint(0,0),cvPoint(img->width,img->height),cvScalar(0,0,10,10), 1, 8, 0);

		cvNamedWindow(argv[1], CV_WINDOW_AUTOSIZE); 						//aloca memoria
		cvShowImage(argv[1], joinIplImage('h', 10, mask, 2, img, mask2));	//exibe imagem
		cvWaitKey(0);		            			 						//sleep
		cvDestroyWindow(argv[1]);					 						//desaloca memoria

		cvReleaseImage(&mask2);
		cvReleaseImage(&mask);
		cvReleaseImage(&img);		    			 						//desaloca espaco da imagem
	}
	return 0;
}
