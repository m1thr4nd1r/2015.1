#include <cstdarg>

using namespace cv;
using namespace std;

typedef unsigned char Pixel;
typedef unsigned char Cor[3];

#define MAX_GRAY_BINS 256
#define BRANCO 255
#define PRETO 0
#define SEMROTULO -1
#define FILL_IPL(img, type, x) for (int i = 0; i < img->height; ++i) for (int j = 0; j < img->width; ++j) CV_IMAGE_ELEM(img, type, i, j) = x
#define PRINT_IPL(img, type) for (int i = 0; i < img->height; ++i){ for (int j = 0; j < img->width; ++j){ cout << CV_IMAGE_ELEM(img, type, i, j) - '\0' << " "; } cout << endl; }
#define PRINTBIN_IPL(img, type) for (int i = 0; i < img->height; ++i){ for (int j = 0; j < img->width; ++j){ cout << (CV_IMAGE_ELEM(img, type, i, j) - '\0' == 0 ? 0 : 1) << " "; } cout << endl; }

/**
 * [print description]
 * @param
 * @param
 */
void print(int * p, int max){
	for (int i = 0; i < max; ++i){
		if(i != 0) cout << " ";
			cout << p[i];
	}
	cout << endl;
}

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
 * @return o tom que mais aparece
 */
int calcHistogram(IplImage * img, int * hist){
	int i, j, maior = 0;
	for(i = 0; i < MAX_GRAY_BINS; i++)
		hist[i] = 0;

	for(i = 0; i < img->width; i++)
		for(j = 0; j < img->height; j++)
			hist[CV_IMAGE_ELEM(img, uchar, i, j)]++;

	for(i = 0; i < img->width; i++)
		for(j = 0; j < img->height; j++)
			if(hist[CV_IMAGE_ELEM(img, uchar, i, j)] > maior)
				maior = CV_IMAGE_ELEM(img, uchar, i, j);

	return maior;
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

int normalize(int x, int A, int B, int nA, int nB){
	return nA + (x - A) * (nB - nA) / (B - A);
}

/**
 * Desenha o histograma
 * 
 * int * hist = (int*) malloc(sizeof(int) * MAX_GRAY_BINS);
 * calcHistogram(img, hist, img->width, img->height);
 * drawHistogram(hist, img->width, img->height);
 * 
 * @param
 * @param
 * @param
 *
 */
void drawHistogram(int hist[], int w, int h){
	int m = 0;
	for (int i = 0; i < MAX_GRAY_BINS; ++i)
		if(hist[i] > m)
			m = hist[i];

	int cpHist[MAX_GRAY_BINS];
	for (int i = 0; i < MAX_GRAY_BINS; ++i){
		cpHist[i] = normalize(hist[i], 0, m, 0, MAX_GRAY_BINS - 1);
	}

	Mat mat_img(Size(w,h),CV_8UC1);
	for (int i = 1; i < 256; ++i){
		line(mat_img,Point(i-1,h - cvRound(cpHist[i-1])),
					 Point(i, h - cvRound(cpHist[i])),
					 Scalar(127,127,127), 2, 8, 0);
	}
	imshow("Histogram",mat_img);
	cvWaitKey(0);
	cvDestroyWindow("Histogram");
}

void add(IplImage * src, IplImage * src2, IplImage * mask){
	for (int i = 0; i < src->height; ++i){
		for (int j = 0; j < src->width; ++j){
			CV_IMAGE_ELEM(mask, uchar, i, j) = CV_IMAGE_ELEM(src, uchar, i, j) + CV_IMAGE_ELEM(src2, uchar, i, j); 
		}
	}
}

void diff(IplImage * src, IplImage * src2, IplImage * mask){
	for (int i = 0; i < src->height; ++i){
		for (int j = 0; j < src->width; ++j){
			CV_IMAGE_ELEM(mask, uchar, i, j) = CV_IMAGE_ELEM(src, uchar, i, j) - CV_IMAGE_ELEM(src2, uchar, i, j); 
		}
	}
}

/**
 * String para int
 * @param  str o valor no formato string
 * @return     o valor no formato inteiro.
 */
int strToInt(string str){
	int v;
	stringstream(str) >> v;
	return v;
}

/**
 * Limiarizacao -> converter a imagem em dois tons a partir de um ponto de corte(limiar).
 * @param img     imagem fonte
 * @param binaria imagem de saida binarizada
 * @param limiar  limiar
 */
void threshold(IplImage * img, IplImage * binaria, Pixel limiar, int min, int max){
	for (int i = 0; i < img->height; ++i){
		for (int j = 0; j < img->width; ++j){
			CV_IMAGE_ELEM(binaria, uchar, i, j) = (CV_IMAGE_ELEM(img, uchar, i, j) > limiar) ? max : min; 
		}
	}
}

/**
 * Rotulacao - Cria componentes 4-conexa ou 8-conexa
 * @param bin      imagem binarizada
 * @param rotulada imagem a ser rotulada
 * @param i        pos. x do px da imagem a ser rotulada
 * @param j        pos. y do px da imagem a ser rotulada
 * @param cor      cor da componente
 * @param f        0: 4-conexa, 1: 8-conexa
 */
void labeledRecursive(IplImage * bin, IplImage * rotulada, int i, int j, int cor, char f){
	CV_IMAGE_ELEM(rotulada, int, i, j) = cor;
	if(i > 0 && CV_IMAGE_ELEM(rotulada, int, i-1,j) == SEMROTULO && CV_IMAGE_ELEM(bin,uchar,i,j) == CV_IMAGE_ELEM(bin,uchar,i-1,j))
		labeledRecursive(bin, rotulada, i-1, j, cor, f);
	if(i < bin->height - 1 && CV_IMAGE_ELEM(rotulada, int, i+1,j) == SEMROTULO && CV_IMAGE_ELEM(bin,uchar,i,j) == CV_IMAGE_ELEM(bin,uchar,i+1,j))
		labeledRecursive(bin, rotulada, i+1, j, cor, f);
	if(j > 0 && CV_IMAGE_ELEM(rotulada, int, i,j-1) == SEMROTULO && CV_IMAGE_ELEM(bin,uchar,i,j) == CV_IMAGE_ELEM(bin,uchar,i,j-1))
		labeledRecursive(bin, rotulada, i, j-1, cor, f);
	if(j < bin->width - 1 && CV_IMAGE_ELEM(rotulada, int, i,j+1) == SEMROTULO && CV_IMAGE_ELEM(bin,uchar,i,j) == CV_IMAGE_ELEM(bin,uchar,i,j+1))
		labeledRecursive(bin, rotulada, i, j+1, cor, f);
	if(f == 1){
		if(i > 0 && j > 0 && CV_IMAGE_ELEM(rotulada, int, i-1,j-1) == SEMROTULO && CV_IMAGE_ELEM(bin,uchar,i,j) == CV_IMAGE_ELEM(bin,uchar,i-1,j-1))
			labeledRecursive(bin, rotulada, i-1, j-1, cor, f);
		if(i < bin->height - 1 && j < bin->width - 1 && CV_IMAGE_ELEM(rotulada, int, i+1,j+1) == SEMROTULO && CV_IMAGE_ELEM(bin,uchar,i,j) == CV_IMAGE_ELEM(bin,uchar,i+1,j+1))
			labeledRecursive(bin, rotulada, i+1, j+1, cor, f);
		if(j > 0 && i < bin->height - 1 && CV_IMAGE_ELEM(rotulada, int, i+1,j-1) == SEMROTULO && CV_IMAGE_ELEM(bin,uchar,i,j) == CV_IMAGE_ELEM(bin,uchar,i+1,j-1))
			labeledRecursive(bin, rotulada, i+1, j-1, cor, f);
		if(j < bin->width - 1 && i > 0 && CV_IMAGE_ELEM(rotulada, int, i-1,j+1) == SEMROTULO && CV_IMAGE_ELEM(bin,uchar,i,j) == CV_IMAGE_ELEM(bin,uchar,i-1,j+1))
			labeledRecursive(bin, rotulada, i-1, j+1, cor, f);		
	}
}

int EQ(int * eq, int i){
	if(eq[i] == i)
		return i;
	EQ(eq, eq[i]);
}

int labeled(IplImage * bin, IplImage * rotulada, int i, int j){
	int rotulo = 1;
	int eq[bin->height * bin->width]; // equivalentes

	for (int i = 0; i < bin->height * bin->width; ++i){
		eq[i] = i;
	}

	for (int i = 0; i < bin->height; ++i){
		for (int j = 0; j < bin->width; ++j){

			if(CV_IMAGE_ELEM(bin, uchar, i, j) == 0){
				continue;
			}

			Pixel C = CV_IMAGE_ELEM(bin, uchar, i-1, j);
			Pixel E = CV_IMAGE_ELEM(bin, uchar, i, j-1);

			if(i == 0){
				if(j > 0)
					if(E == 0)
						CV_IMAGE_ELEM(rotulada, int, i, j) = ++rotulo;
					else
						CV_IMAGE_ELEM(rotulada, int, i, j) = CV_IMAGE_ELEM(rotulada, int, i, j-1);
			}else if(j == 0){
				if(C == 0)
					CV_IMAGE_ELEM(rotulada, int, i, j) = ++rotulo;
				else
					CV_IMAGE_ELEM(rotulada, int, i, j) = CV_IMAGE_ELEM(rotulada, int, i-1, j);	
			}else {
				if(C == 0 && E == 0)
					CV_IMAGE_ELEM(rotulada, int, i, j) = ++rotulo;
				else if(C != 0 && E == 0)
					CV_IMAGE_ELEM(rotulada, int, i, j) = CV_IMAGE_ELEM(rotulada, int, i-1, j);
				else if(E != 0 && C == 0)
					CV_IMAGE_ELEM(rotulada, int, i, j) = CV_IMAGE_ELEM(rotulada, int, i, j-1);
				else if(C != 0 && E != 0 && CV_IMAGE_ELEM(rotulada, int, i-1, j) == CV_IMAGE_ELEM(rotulada, int, i, j-1))
					CV_IMAGE_ELEM(rotulada, int, i, j) = CV_IMAGE_ELEM(rotulada, int, i-1, j);
				else{
					// cout << i << "," << j << " " << "C: " << C - '\0' << ", E: " << E - '\0' << ", " << "rC: " << CV_IMAGE_ELEM(rotulada, int, i-1, j) - '\0' << ", rE: " << CV_IMAGE_ELEM(rotulada, int, i, j-1) - '\0' << "; " << endl; 
					int m = max(CV_IMAGE_ELEM(rotulada, int, i-1, j),CV_IMAGE_ELEM(rotulada, int, i, j-1));
					int n = min(CV_IMAGE_ELEM(rotulada, int, i-1, j),CV_IMAGE_ELEM(rotulada, int, i, j-1));
					eq[m] = n;
					if(m == CV_IMAGE_ELEM(rotulada, int, i-1, j))
						CV_IMAGE_ELEM(rotulada, int, i, j) = CV_IMAGE_ELEM(rotulada, int, i, j-1);
					else
						CV_IMAGE_ELEM(rotulada, int, i, j) = CV_IMAGE_ELEM(rotulada, int, i-1, j);
				}
			}
		}
	}

	int vet[bin->height * bin->width];
	for (int i = 0; i < bin->height * bin->width; ++i){
		vet[i] = 0;
	}

	for (int i = 0; i < bin->height; ++i){
		for (int j = 0; j < bin->width; ++j){
			CV_IMAGE_ELEM(rotulada,int,i,j) = EQ(eq,CV_IMAGE_ELEM(rotulada,int,i,j));
			vet[CV_IMAGE_ELEM(rotulada,int,i,j)] = 1;
		}
	}

	int cont = 1;
	for (int i = 0; i < bin->height * bin->width; ++i){
		if(vet[i] == 1)
			cont++;
	}

	return cont;
}