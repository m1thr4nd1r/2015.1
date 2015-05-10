#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <sstream>
#include "lib.h"

using namespace cv;
using namespace std;

double mat[3][3];

typedef pair<int,int> ii;
typedef pair<ii,int> cor;

/**
 * Realçar a borda da imagem
 * @param a imagem fonte
 * @param a mascara de entrada e saida da imagem
 */
void highlight(IplImage * img, IplImage * mask, cor lb, cor lg, int limiar, int flag){
	int a, b, w = img->width, h = img->height;
	for(a = 0; a < h; a++){
		for(b = 0; b < w; b++){
			//G[f(x,y)] ≅ | f{x,y}-f{x+1,y+1} | + | f(x+1,y)-f(x,y+1) |
			int grad = 0;

			grad =  abs(CV_IMAGE_ELEM(img, uchar, a, b * 3  ) - CV_IMAGE_ELEM(img, uchar, a+1, (b+1) * 3  )) +
					abs(CV_IMAGE_ELEM(img, uchar, a, b * 3+1) - CV_IMAGE_ELEM(img, uchar, a+1, (b+1) * 3+1)) +
					abs(CV_IMAGE_ELEM(img, uchar, a, b * 3+2) - CV_IMAGE_ELEM(img, uchar, a+1, (b+1) * 3+2)) +
					abs(CV_IMAGE_ELEM(img, uchar, a+1, b * 3  ) - CV_IMAGE_ELEM(img, uchar, a, (b+1) * 3  )) +
					abs(CV_IMAGE_ELEM(img, uchar, a+1, b * 3+1) - CV_IMAGE_ELEM(img, uchar, a, (b+1) * 3+1)) +
					abs(CV_IMAGE_ELEM(img, uchar, a+1, b * 3+2) - CV_IMAGE_ELEM(img, uchar, a, (b+1) * 3+2));
			
			switch(flag){
				case 1:
					CV_IMAGE_ELEM(mask, uchar, a, b * 3  ) = grad;
					CV_IMAGE_ELEM(mask, uchar, a, b * 3+1) = grad;
				   	CV_IMAGE_ELEM(mask, uchar, a, b * 3+2) = grad;
					break;
					
				case 2:
					if(grad >= limiar){
						CV_IMAGE_ELEM(mask, uchar, a, b * 3  ) = lg.first.first;
						CV_IMAGE_ELEM(mask, uchar, a, b * 3+1) = lg.first.second;
					   	CV_IMAGE_ELEM(mask, uchar, a, b * 3+2) = lg.second;
					}else{
						CV_IMAGE_ELEM(mask, uchar, a, b * 3  ) = CV_IMAGE_ELEM(img, uchar, a, b * 3  );
						CV_IMAGE_ELEM(mask, uchar, a, b * 3+1) = CV_IMAGE_ELEM(img, uchar, a, b * 3+1);
						CV_IMAGE_ELEM(mask, uchar, a, b * 3+2) = CV_IMAGE_ELEM(img, uchar, a, b * 3+2);
					}
					break;

				case 3:
					if(grad >= limiar){
						CV_IMAGE_ELEM(mask, uchar, a, b * 3  ) = grad;
						CV_IMAGE_ELEM(mask, uchar, a, b * 3+1) = grad;
					   	CV_IMAGE_ELEM(mask, uchar, a, b * 3+2) = grad;
				   	}else{
					   	CV_IMAGE_ELEM(mask, uchar, a, b * 3  ) = lb.first.first;
						CV_IMAGE_ELEM(mask, uchar, a, b * 3+1) = lb.first.second;
						CV_IMAGE_ELEM(mask, uchar, a, b * 3+2) = lb.second;
				   	}
					break;

				case 4:
					if(grad >= limiar){
						CV_IMAGE_ELEM(mask, uchar, a, b * 3  ) = lg.first.first;
						CV_IMAGE_ELEM(mask, uchar, a, b * 3+1) = lg.first.second;
					   	CV_IMAGE_ELEM(mask, uchar, a, b * 3+2) = lg.second;
				   	}else{
						CV_IMAGE_ELEM(mask, uchar, a, b * 3  ) = CV_IMAGE_ELEM(img, uchar, a, b * 3  );
						CV_IMAGE_ELEM(mask, uchar, a, b * 3+1) = CV_IMAGE_ELEM(img, uchar, a, b * 3+1);
						CV_IMAGE_ELEM(mask, uchar, a, b * 3+2) = CV_IMAGE_ELEM(img, uchar, a, b * 3+2);
				   	}
					break;
					
				case 5:
					if(grad >= limiar){
						CV_IMAGE_ELEM(mask, uchar, a, b * 3  ) = lg.first.first;
						CV_IMAGE_ELEM(mask, uchar, a, b * 3+1) = lg.first.second;
					   	CV_IMAGE_ELEM(mask, uchar, a, b * 3+2) = lg.second;
					}else{
						CV_IMAGE_ELEM(mask, uchar, a, b * 3  ) = lb.first.first;
						CV_IMAGE_ELEM(mask, uchar, a, b * 3+1) = lb.first.second;
						CV_IMAGE_ELEM(mask, uchar, a, b * 3+2) = lb.second;
					}
					break;
				
					
			}
			
		}
	}
}

int main(int argc, char **argv) {
 	IplImage * img, *dist;

    if(argc > 1 && (img = cvLoadImage(argv[1], 1)) != 0){
 	
 		int limiar = 50;

 		if(argc > 2)
 			limiar = stoi(argv[2]);

        dist = cvCreateImage(Size(img->width, img->height),IPL_DEPTH_8U,3);
		
		cout << limiar << endl;

        highlight(img, dist,cor(ii(255,0),0), cor(ii(0,0),255),limiar,1);
        cvShowImage("Saida", dist);
		highlight(img, dist,cor(ii(0,255),0), cor(ii(0,0),255),limiar,2);
        cvShowImage("Saida2", dist);
		highlight(img, dist,cor(ii(255,255),0), cor(ii(0,0),255),limiar,3);
        cvShowImage("Saida3", dist);
		highlight(img, dist,cor(ii(255,0),255), cor(ii(0,0),255),limiar,4);
        cvShowImage("Saida4", dist);
		highlight(img, dist,cor(ii(0,255),255), cor(ii(0,0),255),limiar,5);
        cvShowImage("Saida5", dist);

        cvShowImage(argv[1], img);

        cvWaitKey(10); 
        cvDestroyWindow(argv[1]);
        cvDestroyWindow("Saida");
        cvDestroyWindow("Saida2");        
        cvDestroyWindow("Saida3");
        cvDestroyWindow("Saida4");
        cvDestroyWindow("Saida5");
        cvReleaseImage(&img);
        cvReleaseImage(&dist);
    }
    return 0;
}

