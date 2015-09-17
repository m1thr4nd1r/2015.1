#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>

using namespace cv;
using namespace std;

/** @function main */
int main(int argc, char** argv)
{
  Mat src, src_gray;

  /// Read the image
  src = imread( argv[1], 1 );

  if( !src.data )
    { cout << " error " << endl;
      return -1; }

  /// Convert it to gray
  cvtColor( src, src_gray, CV_BGR2GRAY );

  /// Reduce the noise so we avoid false circle detection
  GaussianBlur( src_gray, src_gray, Size(9, 9), 2, 2 );

  vector<Vec3f> circles;

  /// Apply the Hough Transform to find the circles
  HoughCircles( src_gray, circles, CV_HOUGH_GRADIENT, 1, src_gray.rows/8, 256, 30, 0, 0 );
  // src_gray: Input image (grayscale)
  // circles: A vector that stores sets of 3 values: x_{c}, y_{c}, r for each detected circle.
  // CV_HOUGH_GRADIENT: Define the detection method. Currently this is the only one available in OpenCV
  // dp = 1: The inverse ratio of resolution
  // min_dist = src_gray.rows/8: Minimum distance between detected centers
  // param_1 = 200: Upper threshold for the internal Canny edge detector
  // param_2 = 100*: Threshold for center detection.
  // min_radius = 0: Minimum radio to be detected. If unknown, put zero as default.
  // max_radius = 0: Maximum radius to be detected. If unknown, put zero as default


  /// Draw the circles detected
  cout << circles.size() << endl;
  for( size_t i = 0; i < circles.size(); i++ )
  {
      Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
      int radius = cvRound(circles[i][2]);
      // circle center
      circle( src, center, 3, Scalar(0,255,0), -1, 8, 0 );
      // circle outline
      circle( src, center, radius, Scalar(0,0,255), 3, 8, 0 );
   }

  /// Show your results
  namedWindow( "Hough Circle Transform Demo", CV_WINDOW_AUTOSIZE );
  imshow( "Hough Circle Transform Demo", src );

  waitKey(0);
  return 0;
}