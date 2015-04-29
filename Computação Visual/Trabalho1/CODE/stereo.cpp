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
typedef pair <double, double> dd;
typedef vector < vector < int > > vvi;
typedef long long ll;

IplImage *esq, *dir, *disp, *res, *erro;
ll ** C, m, n;

int pass, maior = 31;
int bits = 8;

ll sqr(ll a){ 
	return (a) * (a); 
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
int getMatching(int i, int j, int mh, int mv, int range, ll (*f)(ll)){
	int d = 0, distmin = INT_MAX, dist, base = mh / 2, base2 = mv / 2;
	for(int k = j; k < j + range /* 255 / 8 */ && k < dir->width; k++) {
		dist = 0;
		// int o = 0;
		for(int o = -base2; o <= base2; o++){
			for(int a = -base; a <= base; a++){
				if(i + o < 0 || k + a < 0 || i + o > dir->height || k + a > dir->width)
					continue;
				dist += (*f)(CV_IMAGE_ELEM(esq, uchar, i+o, (k+a)*3  ) - CV_IMAGE_ELEM(dir, uchar, i+o, (j+a)*3  )) +
						(*f)(CV_IMAGE_ELEM(esq, uchar, i+o, (k+a)*3+1) - CV_IMAGE_ELEM(dir, uchar, i+o, (j+a)*3+1)) +
						(*f)(CV_IMAGE_ELEM(esq, uchar, i+o, (k+a)*3+2) - CV_IMAGE_ELEM(dir, uchar, i+o, (j+a)*3+2));
			}
		}
		if(dist  < distmin) {
			distmin = dist;
			d = k-j;
		}
	}
	return d;
}

/**
* CODIGO DO BASE DO PROFESSOR
*/
double average(int mh, int mv, ll (*f)(ll)){
	int n=0, i, j, d;
	double avgerro = 0;
	for(i=0; i < esq->height; i++) {
		for(j=0; j < esq->width; j++) {
			d = getMatching(i, j, mh, mv, maior, f);
			CV_IMAGE_ELEM(res,uchar,i,j) = d * bits; //NAO MUDAR
			if(CV_IMAGE_ELEM(disp,uchar,i,j)) {
				CV_IMAGE_ELEM(erro,uchar,i,j) = abs(CV_IMAGE_ELEM(disp,uchar,i,j)-CV_IMAGE_ELEM(res,uchar,i,j));
				avgerro += CV_IMAGE_ELEM(erro,uchar,i,j);
				n++;
			}
			else
				CV_IMAGE_ELEM(erro,uchar,i,j) = 0;
		}
	}
	return avgerro / n;
}

double averageLCS(){
	int n=0, i, j, d;
	double avgerro = 0;
	for(i=0; i < esq->height; i++) {
		for(j=0; j < esq->width; j++) {
			if(CV_IMAGE_ELEM(disp,uchar,i,j)) {
				CV_IMAGE_ELEM(erro,uchar,i,j) = abs(CV_IMAGE_ELEM(disp,uchar,i,j)-CV_IMAGE_ELEM(res,uchar,i,j));
				avgerro += CV_IMAGE_ELEM(erro,uchar,i,j);
				n++;
			}
			else
				CV_IMAGE_ELEM(erro,uchar,i,j) = 0;
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

int normalize(int x){
	return (((ll)x * 255) / C[n][n]);
}

void backtrack(int i, int j, bool flag){
    int a, b;
    if(i == 0 && j == 0){
        return;
    }else if(i == 0){
        backtrack(i,j-1,flag);
        int d = abs(i - (j-1));
        if(d > maior && d < (0.50 * esq->width))
        	maior = d;
        CV_IMAGE_ELEM(res, uchar, pass, i) = d * bits;
        if(flag){
	        CV_IMAGE_ELEM(esq, uchar, pass, i*3  ) = CV_IMAGE_ELEM(dir, uchar, pass, (j-1)*3  );
	        CV_IMAGE_ELEM(esq, uchar, pass, i*3+1) = CV_IMAGE_ELEM(dir, uchar, pass, (j-1)*3+1);
	        CV_IMAGE_ELEM(esq, uchar, pass, i*3+2) = CV_IMAGE_ELEM(dir, uchar, pass, (j-1)*3+2);
    	}
    }else if(j == 0){
        backtrack(i-1,j,flag);
        int d = abs((i-1) - j);
        if(d > maior && d < (0.50 * esq->width))
        	maior = d;
        CV_IMAGE_ELEM(res,  uchar, pass, i-1) = d * bits;
        if(flag){
		    CV_IMAGE_ELEM(esq, uchar, pass, (i-1)*3  ) = CV_IMAGE_ELEM(esq, uchar, pass, j*3  );
	        CV_IMAGE_ELEM(esq, uchar, pass, (i-1)*3+1) = CV_IMAGE_ELEM(esq, uchar, pass, j*3+1);
	        CV_IMAGE_ELEM(esq, uchar, pass, (i-1)*3+2) = CV_IMAGE_ELEM(esq, uchar, pass, j*3+2);
	    }
    }else if(pxEquals(pass, i-1, pass, j-1)){
        backtrack(i-1,j-1,flag);
        int d = abs((i-1) - (j-1));
        if(d > maior && d < (0.50 * esq->width))
        	maior = d;
        CV_IMAGE_ELEM(res,  uchar, pass, i-1) = d * bits;
        if(flag){
		    CV_IMAGE_ELEM(esq, uchar, pass, (i-1)*3  ) = CV_IMAGE_ELEM(dir, uchar, pass, (j-1)*3  );
	        CV_IMAGE_ELEM(esq, uchar, pass, (i-1)*3+1) = CV_IMAGE_ELEM(dir, uchar, pass, (j-1)*3+1);
	        CV_IMAGE_ELEM(esq, uchar, pass, (i-1)*3+2) = CV_IMAGE_ELEM(dir, uchar, pass, (j-1)*3+2);
	    }
    }else if(C[i][j-1] > C[i-1][j]){
        backtrack(i,j-1,flag);
        int d = abs(i - (j-1));
        if(d > maior && d < (0.50 * esq->width))
        	maior = d;
        CV_IMAGE_ELEM(res,  uchar, pass, i) = d * bits;
        if(flag){
		    CV_IMAGE_ELEM(esq, uchar, pass, i*3  ) = CV_IMAGE_ELEM(dir, uchar, pass, (j-1)*3  );
	        CV_IMAGE_ELEM(esq, uchar, pass, i*3+1) = CV_IMAGE_ELEM(dir, uchar, pass, (j-1)*3+1);
	        CV_IMAGE_ELEM(esq, uchar, pass, i*3+2) = CV_IMAGE_ELEM(dir, uchar, pass, (j-1)*3+2);
	    }
    }else if(C[i][j-1] <= C[i-1][j]){//C[i][j-1] == C[i-1][j] && C[i][j] == C[i-1][j]){
        backtrack(i-1,j,flag);
	    int d = abs((i-1) - j);
	    if(d > maior && d < (0.50 * esq->width))
	    	maior = d;
	    CV_IMAGE_ELEM(res,  uchar, pass, i-1) = d * bits;
        if(flag){
		    CV_IMAGE_ELEM(esq, uchar, pass, (i-1)*3  ) = CV_IMAGE_ELEM(esq, uchar, pass, j*3  );
	        CV_IMAGE_ELEM(esq, uchar, pass, (i-1)*3+1) = CV_IMAGE_ELEM(esq, uchar, pass, j*3+1);
	        CV_IMAGE_ELEM(esq, uchar, pass, (i-1)*3+2) = CV_IMAGE_ELEM(esq, uchar, pass, j*3+2);
	    }
	}
}

void printC(){
    for (unsigned int i = 0; i <= n; i++){
        for (unsigned int j = 0; j <= n; j++){
            cout << C[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
}

void fill_C(int k){
    for (unsigned int x = 0; x <= n; ++x)
        for (unsigned int y = 0; y <= n; ++y)
            C[x][y] = k;
}

void lcs(){

	fill_C(0);

    for (unsigned int x = 0; x < n; ++x){
        for (unsigned int y = 0; y < n; ++y){
        	if(pxEquals(pass, x, pass, y)){
                C[x+1][y+1] = C[x][y] + 1;
            }else{
                C[x+1][y+1] = max(C[x+1][y], C[x][y+1]);
            }
        }
    }

}

void initC(){
	C = (ll **)malloc(sizeof(ll *) * (n + 1));
    for (unsigned int x = 0; x <= n; ++x){
    	C[x] = (ll *)malloc(sizeof(ll) * (n + 1));
	}
}

void solve(bool flag){
	m = esq->height;
	n = esq->width;
	initC();
	for (int i = 0; i < m; ++i){
		pass = i;
		lcs();
		backtrack(n,n,flag);
	}
	double menor = 200;
	dd mdc(200,201), mdd(201,202); //menor media de erro crescente e decrescente
	if(flag){
		// ii meno = ii(-1,-1); //A janela com o menor erro.
		int vec[] = {7,9,11,17,19}, tam = 5, cresc = 0, dec = tam - 1;
		for (int i = 0; i < tam && dec > cresc; i++){
			int aux;
			if(i % 2 == 0){
				// cout << vec[cresc];
				mdc.second = mdc.first;
				mdc.first = average(vec[cresc], 11, abs);
				if(mdc.second <= mdc.first){
					i = 20;
					continue;
				}
				if(menor > mdc.first){
					menor = mdc.first;
					// meno = ii(vec[cresc],11);
				}
				cresc++;
			}else{
				// cout << vec[dec];
				mdd.second = mdd.first;
				mdd.first = average(vec[dec], 11, abs);
				if(mdd.second <= mdd.first){
					i = 20;
					continue;
				}
				if(menor > mdd.first){
					menor = mdd.first;
					// meno = ii(vec[dec],11);
				}
				dec--;
			}
			// cout << "x" << 11 << endl;
			//Media decrecente maior que a crescente?
			if(mdc.first > mdd.first){
				i = tam * 2;
				continue;
			}
		}
		// cout << "MENOR " << meno.first << "x" << meno.second << endl;
	}
	else
		menor = averageLCS();

	printf("Erro médio: %lf\n", menor);
}

int main(int argc, char **argv) {

	esq = cvLoadImage(argv[1], 1);
	dir = cvLoadImage(argv[2], 1);
	disp = cvLoadImage(argv[3], 0);

	res = cvCreateImage(cvSize(esq->width,esq->height), IPL_DEPTH_8U, 1);
	erro = cvCreateImage(cvSize(esq->width,esq->height), IPL_DEPTH_8U, 1);
	res = cvCreateImage(cvSize(esq->width,esq->height), IPL_DEPTH_8U, 1);

	bool flag = false;

	if(argc > 4 && strcmp(argv[4],"true") == 0)
		flag = true;

	if(argc > 5)
		bits = argv[5][0] - '0';

	// printf("DIM: %dx%d\n", esq->width, esq->height);
	const clock_t begin_time = clock();
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

