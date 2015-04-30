#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <cstring>
#include <sstream>
#include <utility>
#include <algorithm>
#include <climits>

using namespace std;

#define sqr(a) (a) * (a)
#define menor(a,b,c) a < b && a < c ? a : b < a && b < c ? b : c 

typedef pair <long,long> ll;

bool kmeans(IplImage * img, IplImage * out, int * v, int k){

	ll m;
	int px, sum = 0;
	for (int i = 0; i < img->height; ++i){
		for (int j = 0; j < img->width; ++j){
			px = CV_IMAGE_ELEM(img, uchar, i, j);
			m = ll(LONG_MAX,-1);
			for(int x = 0; x < k; x++){
				long d = sqr(v[x] - px);
				if(d < m.first){
					m.first = d;
					m.second = v[x];
				}
				// cout << v[x] << ": " << d << " " ;
			}
			// cout << (int)CV_IMAGE_ELEM(out, uchar, i, j) << ", m: " << m.second << endl;
			if(((int)CV_IMAGE_ELEM(out, uchar, i, j)) != m.second){
				sum++;
				// 	mudou = true;
				CV_IMAGE_ELEM(out, uchar, i, j) = m.second;
			}
		}
	}
	
	bool mudou = false;// = changed(img, out);
	if(sum / img->width * img->height > 0.05)
		mudou = true;

	// cout << sum << endl;
	if(mudou){
		int * md;
		int c = 1;
		md = (int*)calloc(k,sizeof(int));
		for(int x = 0; x < k; x++){
			c = 0;
			for (int i = 0; i < img->height; ++i)
				for (int j = 0; j < img->width; ++j)
					if(CV_IMAGE_ELEM(out, uchar, i, j) == v[x]){
						md[x] += CV_IMAGE_ELEM(img, uchar, i, j);
						c++;
					}
			if(c)
				md[x] /= c;
		}
		for(int x = 0; x < k; v[x] = md[x], x++);
		// kmeans(img, out, md, k);
	}

	return mudou;
}

int main(int argc, char ** argv){
    IplImage * img, * out;
	if(argc > 1 && (img  = cvLoadImage(argv[1], 0)) != 0){
		out = cvCreateImage(cvSize(img->width,img->height),IPL_DEPTH_8U,1);

		int * v = (int*)calloc(3,sizeof(int));
		
		v[0] = 0; 
		v[1] = 127;
		v[2] = 255;

		bool mudou = true;

		for (int i = 0; i < img->height; ++i)
			for (int j = 0; j < img->width; ++j)
				CV_IMAGE_ELEM(out, uchar, i, j) = 0;

		while(mudou){
			// for (int i = 0; i < 3; ++i){
			// 	cout << v[i] << " ";
			// }
			// cout << endl;
			mudou = kmeans(img, out, v, 3);
			while(cvWaitKey(10) < 0){
				cvNamedWindow("view2",1);
				cvShowImage("view2",out);
			}
		}
		cvReleaseImage(&out);
	}
	return 0;
}
