#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <cstdio>

#define FILL_IPL(img, uchar, x) for (int i = 0; i < img->height; ++i) for (int j = 0; j < img->width; ++j) CV_IMAGE_ELEM(img, uchar, i, j) = x
#define PRINT_IPL(img, uchar) for (int i = 0; i < img->height; ++i){ for (int j = 0; j < img->width; ++j){ cout << CV_IMAGE_ELEM(img, uchar, i, j) - '\0' << " "; } cout << endl; }

#define min(a, b) (a > b) ? b : a
#define max(a, b) (a > b) ? a : b

using namespace std;

typedef pair <int,int> ii;
typedef pair <double,double> dd;
typedef pair <double,dd> tripla;
typedef vector < vector < int > > vvi;
typedef long long ll;

IplImage *esq, *dir, *disp, *res, *erro;
ll ** C, M, N;

int pass, maior = 31;
int bits = 8;
bool DEBUG = false;

ll sqr(ll a){ return (a) * (a); }

double module(tripla v){
	return abs(v.first) + abs(v.second.first) + abs(v.second.second);
}

double dotProd(double a, double b, double c, double x, double y, double z){
	return (a)*(x)+(b)*(y)+(c)*(z);
}

tripla crossProd(double a, double b, double c, double x, double y, double z){
	return tripla(b * z - c * y, dd(c * x - a * z, a * y - b * x));
}

/**
 * BUSCANDO PIXEL CORRESPONDENTE DISPARIDADE
 * DISPARIDADE ANTERIOR : (dir->width - j) 
 * ATUAL : 31 = 255 / 8 -> ate onde consegue-se alcançar uma vez que d eh multiplicado por 8.
 * A diferença absoluta teve mais acurácia do que o quadrado da diferença
 * 
 * @param i pixel position
 * @param j pixel position
 * @param m size of window
 * @param f function of operation in the pixels. Example: abs - absolute value of diference, sqr - square of diference
 */
// int getMatching(int i, int j, int mh, int mv, int range, ll (*f)(ll)){
int getMatching(int i, int j, int mh, int mv, int range){
	int d = 0, angMin = INT_MAX, base = mh / 2, base2 = mv / 2;
	double dist, distmin = INT_MAX;
	for(int k = j; k < j + range && k < esq->width; k++){
		dist = 0;
		for(int o = -base2; o <= base2; o++){
			for(int a = -base; a <= base; a++){
				if(i + o < 0 || k + a < 0 || i + o > dir->height || k + a > dir->width)
					continue;
				dist += sqrt(abs(CV_IMAGE_ELEM(esq, uchar, i+o, (k+a)*3  ) - CV_IMAGE_ELEM(dir, uchar, i+o, (j+a)*3  )) +
							 abs(CV_IMAGE_ELEM(esq, uchar, i+o, (k+a)*3+1) - CV_IMAGE_ELEM(dir, uchar, i+o, (j+a)*3+1)) +
							 abs(CV_IMAGE_ELEM(esq, uchar, i+o, (k+a)*3+2) - CV_IMAGE_ELEM(dir, uchar, i+o, (j+a)*3+2)));
			}
		}
		if(dist < distmin) {
			distmin = dist;
			d = k-j;
		}
	}
	return d;
}

/**
* CODIGO DO BASE DO PROFESSOR
*/
double average(int mh, int mv, bool lcs){
	int n=0, i, j;
	double avgerro = 0;
	if(DEBUG) printf("AVERAGE: %dx%d\n",mh,mv);
	for(i=0; i < esq->height; i++) {
		for(j=0; j < esq->width; j++) {
			if(!lcs){
				int d = getMatching(i, j, mh, mv, maior);
				CV_IMAGE_ELEM(res,uchar,i,j) = d * bits; //NAO MUDAR
			}
			if(CV_IMAGE_ELEM(disp,uchar,i,j)){
				CV_IMAGE_ELEM(erro,uchar,i,j) = abs(CV_IMAGE_ELEM(disp,uchar,i,j)-CV_IMAGE_ELEM(res,uchar,i,j));
				avgerro += CV_IMAGE_ELEM(erro,uchar,i,j);
				n++;
			}else CV_IMAGE_ELEM(erro,uchar,i,j) = 0;
		}
	}
	return avgerro / n;
}

bool pxEquals(int i, int j, int a, int b){
	int dist =  abs(CV_IMAGE_ELEM(esq, uchar, i, j*3  ) - CV_IMAGE_ELEM(dir, uchar, a, b*3  )) +
				abs(CV_IMAGE_ELEM(esq, uchar, i, j*3+1) - CV_IMAGE_ELEM(dir, uchar, a, b*3+1)) +
				abs(CV_IMAGE_ELEM(esq, uchar, i, j*3+2) - CV_IMAGE_ELEM(dir, uchar, a, b*3+2));
	return dist < 26;
}

void backtrack(int i, int j, bool flag){
    int a, b;
    if(i == 0 && j == 0){
        return;
    }else if(i == 0){
        backtrack(i,j-1,flag);
        int d = abs(i - (j-1));
        if(d > maior) maior = d;
        CV_IMAGE_ELEM(res, uchar, pass, i) = d * bits;
        if(flag){
	        CV_IMAGE_ELEM(esq, uchar, pass, i*3  ) = CV_IMAGE_ELEM(dir, uchar, pass, (j-1)*3  );
	        CV_IMAGE_ELEM(esq, uchar, pass, i*3+1) = CV_IMAGE_ELEM(dir, uchar, pass, (j-1)*3+1);
	        CV_IMAGE_ELEM(esq, uchar, pass, i*3+2) = CV_IMAGE_ELEM(dir, uchar, pass, (j-1)*3+2);
    	}
    }else if(j == 0){
        backtrack(i-1,j,flag);
        int d = abs((i-1) - j);
        if(d > maior) maior = d;
        CV_IMAGE_ELEM(res,  uchar, pass, i-1) = d * bits;
        if(flag){
		    CV_IMAGE_ELEM(esq, uchar, pass, (i-1)*3  ) = CV_IMAGE_ELEM(esq, uchar, pass, j*3  );
	        CV_IMAGE_ELEM(esq, uchar, pass, (i-1)*3+1) = CV_IMAGE_ELEM(esq, uchar, pass, j*3+1);
	        CV_IMAGE_ELEM(esq, uchar, pass, (i-1)*3+2) = CV_IMAGE_ELEM(esq, uchar, pass, j*3+2);
	    }
    }else if(pxEquals(pass, i-1, pass, j-1)){
        backtrack(i-1,j-1,flag);
        int d = abs((i-1) - (j-1));
        if(d > maior) maior = d;
        CV_IMAGE_ELEM(res,  uchar, pass, i-1) = d * bits;
        if(flag){
		    CV_IMAGE_ELEM(esq, uchar, pass, (i-1)*3  ) = CV_IMAGE_ELEM(dir, uchar, pass, (j-1)*3  );
	        CV_IMAGE_ELEM(esq, uchar, pass, (i-1)*3+1) = CV_IMAGE_ELEM(dir, uchar, pass, (j-1)*3+1);
	        CV_IMAGE_ELEM(esq, uchar, pass, (i-1)*3+2) = CV_IMAGE_ELEM(dir, uchar, pass, (j-1)*3+2);
	    }
    }else if(C[i][j-1] > C[i-1][j]){
        backtrack(i,j-1,flag);
        int d = abs(i - (j-1));
        if(d > maior) maior = d;
        CV_IMAGE_ELEM(res,  uchar, pass, i) = d * bits;
        if(flag){
		    CV_IMAGE_ELEM(esq, uchar, pass, i*3  ) = CV_IMAGE_ELEM(dir, uchar, pass, (j-1)*3  );
	        CV_IMAGE_ELEM(esq, uchar, pass, i*3+1) = CV_IMAGE_ELEM(dir, uchar, pass, (j-1)*3+1);
	        CV_IMAGE_ELEM(esq, uchar, pass, i*3+2) = CV_IMAGE_ELEM(dir, uchar, pass, (j-1)*3+2);
	    }
    }else if(C[i][j-1] <= C[i-1][j]){
        backtrack(i-1,j,flag);
	    int d = abs((i-1) - j);
	    if(d > maior) maior = d;
	    CV_IMAGE_ELEM(res,  uchar, pass, i-1) = d * bits;
        if(flag){
		    CV_IMAGE_ELEM(esq, uchar, pass, (i-1)*3  ) = CV_IMAGE_ELEM(esq, uchar, pass, j*3  );
	        CV_IMAGE_ELEM(esq, uchar, pass, (i-1)*3+1) = CV_IMAGE_ELEM(esq, uchar, pass, j*3+1);
	        CV_IMAGE_ELEM(esq, uchar, pass, (i-1)*3+2) = CV_IMAGE_ELEM(esq, uchar, pass, j*3+2);
	    }
	}
}

void printC(){
    for (unsigned int i = 0; i <= N; i++){
        for (unsigned int j = 0; j <= N; j++){
            cout << C[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
}

void fill_C(int k){
    for (unsigned int x = 0; x <= N; ++x)
        for (unsigned int y = 0; y <= N; ++y)
            C[x][y] = k;
}

void lcs(){
	fill_C(0);
    for (unsigned int x = 0; x < N; ++x){
        for (unsigned int y = 0; y < N; ++y){
        	if(pxEquals(pass, x, pass, y)){
                C[x+1][y+1] = C[x][y] + 1;
            }else{
                C[x+1][y+1] = max(C[x+1][y], C[x][y+1]);
            }
        }
    }
}

void initC(){
	C = (ll **)malloc(sizeof(ll *) * (N + 1));
    for (unsigned int x = 0; x <= N; ++x){
    	C[x] = (ll *)malloc(sizeof(ll) * (N + 1));
	}
}

void solve(int flag){
	initC();

	if(flag == 1 || flag == 2){
		for (int i = 0; i < M; ++i){
			pass = i;
			lcs();
			backtrack(N, N, flag);
			int  j = 0;
			while(CV_IMAGE_ELEM(res, uchar, i, j) <= 104)
				j++;
			// cout << "J:" << j << endl;
			for (int x = 0; x < j; ++x){
				CV_IMAGE_ELEM(res, uchar, i, x) = CV_IMAGE_ELEM(res, uchar, i, j);
			}
		}
	}

	if(DEBUG) printf("MAIOR: %d\n", maior);

	double menor = 200;
	if(flag == 1 || flag == 3){
		dd mdc(200,201), mdd(202,203); //menor media de erro crescente e decrescente
		int vec[] = {7,9,11,17,19}, tam = 5, cresc = 0, dec = tam - 1;
		for (int i = 0; i < tam && dec >= cresc; i++){

			if(DEBUG) printf("1. mdcAtual: %lf, mdcAnt: %lf, mddAtual: %lf, mddAnt: %lf\n", mdc.first, mdc.second, mdd.first, mdd.second);

			if(i % 2 == 0){
				mdc.second = mdc.first;
				mdc.first = average(vec[cresc], 11, false);

				if(DEBUG) printf("1. %lf <=? %lf\n", mdc.second, mdc.first);

				if(mdc.second <= mdc.first){
					if(DEBUG) printf("media anterior <= media atual.\n");
					break;
				}
				if(menor > mdc.first){
					menor = mdc.first;
				}
				cresc++;
			}else{
				mdd.second = mdd.first;
				mdd.first = average(vec[dec], 11, false);

				if(DEBUG) printf("2. %lf <=? %lf\n", mdd.second, mdd.first);

				if(menor > mdd.first){
					menor = mdd.first;
				}
				dec--;
			}
			if(DEBUG) printf("2. mdcAtual: %lf, mdcAnt: %lf, mddAtual: %lf, mddAnt: %lf\n", mdc.first, mdc.second, mdd.first, mdd.second);
			//Media decrecente maior que a crescente?
			if(mdc.first > mdd.first){
				if(DEBUG) printf("media i do vetor eh maior que a media N-i.\n");
				break;
			}
		}
	}else menor = average(0,0,true);
	printf("Erro médio: %lf\n", menor);
}

/**
 * Formato para executar:
 *
 * <executável> <imagem_esq> <imagem_direita> <imagem_disp> <default=ambos, jdm=executar JDM, lcs=LCS> <8 ou 4> <abs ou sqr>
 * ./a.out test<X>/esq.<formatoImagem> text<X>/dir.<formatoImagem> text<X>/disp.<formatoImagem> <boolean> <bits profundidade> <baseCalc>
 * 
 * ./a.out test1/esq.pgm test1/dir.pgm test1/disp.pgm true 8
 * ./a.out test6/esq.ppm test6/dir.ppm test6/disp6.pgm true 4
 *
 */
int main(int argc, char **argv) {

	if(argc < 4) return 0;

	esq = cvLoadImage(argv[1], 1);
	dir = cvLoadImage(argv[2], 1);
	disp = cvLoadImage(argv[3], 0);

	res = cvCreateImage(cvSize(esq->width,esq->height), IPL_DEPTH_8U, 1);
	erro = cvCreateImage(cvSize(esq->width,esq->height), IPL_DEPTH_8U, 1);

	int flag = 1; //AMBOS lcs + JDM

	if(argc > 4 && strcmp(argv[4],"debug") == 0) DEBUG = true;
	if(argc > 4 && strcmp(argv[4],"lcs") == 0)  flag = 2;
	if(argc > 4 && strcmp(argv[4],"jdm") == 0) flag = 3;
	if(argc > 5 && (argv[5][0] == '4' || argv[5][0] == '8')) bits = argv[5][0] - '0';
	if(argc > 6 && strcmp(argv[6],"true") == 0) DEBUG = true;

	if(DEBUG) printf("bits: %d, flag: %d, maior inicial: %d\n", bits, flag, maior);

	M = esq->height; N = esq->width;

	const clock_t begin_time = clock();
	// cout << dotProd(3,4,5,6,8,10) << endl;
	// iii v = crossProd(3,4,5,6,8,10);
	// cout << (sqr(v.first) + sqr(v.second.first) + sqr(v.second.second)) << endl; 
	// cout << v.first << ", " << v.second.first << ", " << v.second.second << endl;
	solve(flag);
	printf("Runtime: %.2f secs.\n", float( clock () - begin_time ) /  CLOCKS_PER_SEC);

	while(cvWaitKey(10) < 0) {
		cvShowImage("Direita", dir);
		cvShowImage("Esquerda", esq);
		cvShowImage("Disparidade", disp);
		cvShowImage("Resultado", res);
		cvShowImage("Erro", erro);
	}

	cvReleaseImage(&dir);
	cvReleaseImage(&esq);
	cvReleaseImage(&disp);
	cvReleaseImage(&res);
	cvReleaseImage(&erro);

	return 0;
}

