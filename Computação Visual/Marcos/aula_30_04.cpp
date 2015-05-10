#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <sstream>

#define SQR(a) (a) * (a)
#define MAX_DST 1275

using namespace cv;
using namespace std;

IplImage * img, * saida, *segmentada, *dist;
int limiar = 255;

void segmentation(int i, int j, int semente, int d){

	// cout << i << ", " << j << " s: " << semente << " d: " << d <<  " : " << CV_IMAGE_ELEM(segmentada, int, i, j) << ", " << CV_IMAGE_ELEM(dist, int, i, j) << " < " << d << endl;
	if(CV_IMAGE_ELEM(segmentada, int, i, j) != -1 && CV_IMAGE_ELEM(segmentada, int, i, j) == semente)
		return;

	CV_IMAGE_ELEM(segmentada, int, i, j) = semente;
	CV_IMAGE_ELEM(dist, int, i, j) = d;

	CV_IMAGE_ELEM(saida, uchar, i, j * 3  ) = CV_IMAGE_ELEM(img, uchar, i, j * 3  );
	CV_IMAGE_ELEM(saida, uchar, i, j * 3+1) = CV_IMAGE_ELEM(img, uchar, i, j * 3+1); 
	CV_IMAGE_ELEM(saida, uchar, i, j * 3+2) = CV_IMAGE_ELEM(img, uchar, i, j * 3+2);
	
	// cout << i << "," << j << endl;
	// cout << "Cor: " << (int)CV_IMAGE_ELEM(img, uchar, i, j * 3  ) << ", " << (int)CV_IMAGE_ELEM(img, uchar, i, j * 3+1) << ", " << (int)CV_IMAGE_ELEM(img, uchar, i, j * 3+2) << endl;

	if(j - 1 >= 0){
		d = abs(CV_IMAGE_ELEM(img,uchar,i,j*3  )-CV_IMAGE_ELEM(img, uchar, i, (j-1)*3  ))+
			abs(CV_IMAGE_ELEM(img,uchar,i,j*3+1)-CV_IMAGE_ELEM(img, uchar, i, (j-1)*3+1))+
			abs(CV_IMAGE_ELEM(img,uchar,i,j*3+2)-CV_IMAGE_ELEM(img, uchar, i, (j-1)*3+2));
		if(d <= limiar){
			// cout << "1. " << d << endl;
			segmentation(i,j-1,semente, d);
		}
	}

	if(i + 1 < img->height){
		d = abs(CV_IMAGE_ELEM(img, uchar, i, j*3  ) - CV_IMAGE_ELEM(img, uchar, i+1, j*3  ))+
			abs(CV_IMAGE_ELEM(img, uchar, i, j*3+1) - CV_IMAGE_ELEM(img, uchar, i+1, j*3+1))+
			abs(CV_IMAGE_ELEM(img, uchar, i, j*3+2) - CV_IMAGE_ELEM(img, uchar, i+1, j*3+2));
		if(d <= limiar){
			// cout << "2. " << d << endl;
			segmentation(i+1,j,semente, d);
		}
	}

	if(j + 1 < img->width){
		d = abs(CV_IMAGE_ELEM(img,uchar,i,j*3  )-CV_IMAGE_ELEM(img, uchar, i, (j+1)*3  ))+
			abs(CV_IMAGE_ELEM(img,uchar,i,j*3+1)-CV_IMAGE_ELEM(img, uchar, i, (j+1)*3+1))+
			abs(CV_IMAGE_ELEM(img,uchar,i,j*3+2)-CV_IMAGE_ELEM(img, uchar, i, (j+1)*3+2));
		if(d <= limiar){
			// cout << "3. " << d << endl;
			segmentation(i,j+1,semente, d);
		}
	}

	if(i - 1 >= 0){
		d = abs(CV_IMAGE_ELEM(img, uchar, i, j*3  ) - CV_IMAGE_ELEM(img, uchar, i-1, j*3  ))+
			abs(CV_IMAGE_ELEM(img, uchar, i, j*3+1) - CV_IMAGE_ELEM(img, uchar, i-1, j*3+1))+
			abs(CV_IMAGE_ELEM(img, uchar, i, j*3+2) - CV_IMAGE_ELEM(img, uchar, i-1, j*3+2));
		if(d <= limiar){
			// cout << "4. " << d << endl;
			segmentation(i-1,j,semente, d);
		}
	}

}

string itos(int Number){
	return static_cast<ostringstream*>( &(ostringstream() << Number) )->str();
}

int stoi(string s){
	int Number;
	if ( ! (istringstream(s) >> Number) ) Number = 0;
	return Number;
}

int countNotSegmented(){
	int count = 0;
	for(int i = 0; i < segmentada->height; ++i)
		for(int j = 0; j < segmentada->width; ++j)
			if(CV_IMAGE_ELEM(segmentada,int,i,j) == -1){
				count++;
			}
	return count;
}

bool isAllSegmented(){
	return countNotSegmented() == 0;
}

int main(int argc, char **argv) {

	if(argc > 1 && (img = cvLoadImage(argv[1], 1)) != 0){

		if(argc > 2)
			limiar = stoi(argv[2]);

		saida = cvCreateImage(Size(img->width, img->height),IPL_DEPTH_8U,3);
		dist = cvCreateImage(Size(img->width, img->height),IPL_DEPTH_32S,1);
		segmentada = cvCreateImage(Size(img->width, img->height),IPL_DEPTH_32S,1);

		for (int i = 0; i < img->height; ++i){
			for (int j = 0; j < img->width; ++j){
				CV_IMAGE_ELEM(segmentada, int, i, j) = -1;
				CV_IMAGE_ELEM(dist, int, i, j) = MAX_DST;
				CV_IMAGE_ELEM(saida, uchar, i, j * 3  ) = 0;
				CV_IMAGE_ELEM(saida, uchar, i, j * 3+1) = 127;
				CV_IMAGE_ELEM(saida, uchar, i, j * 3+2) = 255;
			}
		}

		int semente = 1;
		segmentation(img->height / 2, img->width / 2, semente, 0);

		while(!isAllSegmented() && limiar < MAX_DST){
			semente = 1;
			while(semente < countNotSegmented()){
				int j, i;
				bool found = false;
				// cout << "s: " << semente << endl;
				for(i = 0; i < img->height && !found; ++i){
					for (j = 0; j < img->width && CV_IMAGE_ELEM(segmentada,int,i,j) != -1; ++j);
					found = j != img->width;
				}
				// cout << i << "," << j << endl;
				segmentation(i, j, ++semente, 0);
			}
			limiar += 255;
		}
		
		cout << "Limiar: " << limiar << ", sementes: " << semente << endl;
		cout << "Qtd. px nao segmentado: " << countNotSegmented() << endl;
		cout << "Toda segmentada? " << (isAllSegmented() ? "True" : "False") << endl;

		while(cvWaitKey(10) < 0) {
			cvShowImage(argv[1], img);
			cvShowImage("Saida", saida);
		}

		cvDestroyWindow(argv[1]);
		cvDestroyWindow("Saida");
		cvReleaseImage(&img);
		cvReleaseImage(&saida);
	}
	return 0;
}
