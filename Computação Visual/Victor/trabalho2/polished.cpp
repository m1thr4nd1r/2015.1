#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <iomanip>

using namespace cv;
using namespace std;

string printCoins(int c[5])
{
    stringstream s;
    s.str("");
    for (int j = 0; j < 5; j++) s << c[j] << " ";
    return s.str();
}

/** @function main */
// ex: ./a.out ../../imgs/20150917_105911.jpg y(optional)
int main(int argc, char** argv)
{
    Mat src, src_gray, mask[4];
    int maxRadius = 0;
    bool debug = false;
    src = imread( argv[1], 1 );
    
    if( !src.data )
    { cout << " error " << endl;
      return -1; }

    if (argc > 2 && (argv[2][0] == 'y' || argv[2][0] == 'Y'))
        debug = true;

    // Resize
    resize(src,src,Size(), (double)0.245, (double)0.245, INTER_LANCZOS4);
    /// Convert it to gray
    cvtColor( src, src_gray, CV_BGR2GRAY );
    //--- Reduce the noise so we avoid false circle detection
    // GaussianBlur( src_gray, src_gray, Size(9, 9), 2, 2 );
    
    vector<Vec3f> circles, circlesTemp;    
    HoughCircles( src_gray, circlesTemp, CV_HOUGH_GRADIENT, 4/3, src_gray.rows, 100, 45, 10,maxRadius);
 
    int maxR = -1, minR = 100;
    while(circlesTemp.size() != 0)
    {
        Point center(cvRound(circlesTemp[0][0]), cvRound(circlesTemp[0][1]));
        int radius = cvRound(circlesTemp[0][2]);        
        // circle outline
        circle( src_gray, center, radius+5, Scalar(0,0,0), -1, 8, 0 );

        bool similar = false;
        for (int i = 0 ; i < circles.size(); i++)
            if (abs(center.x-circles[i][0]) < 15 &&
                abs(center.y-circles[i][1]) < 15)// &&
                {
                    if (debug)
                    {
                        cout << "(" << circles.size() << ") equals: " << center << "(" << radius << ") | " << Point(circles[i][0],circles[i][1]) << "(" << circles[i][2] << ")" << endl;
                        cout << abs(center.x-circles[i][0]) << " " << abs(center.y-circles[i][1]) << " " << abs(radius-circles[i][2]) << endl;
                    }
                    if (minR == cvRound(circles[i][2])) 
                        similar = true;

                    circles[i][0] = (circles[i][0] + center.x) / 2;
                    circles[i][1] = (circles[i][1] + center.y) / 2;
                    circles[i][2] = (circles[i][2] + radius) / 2;
                    radius = cvRound(circles[i][2]);

                    if (similar)
                        minR = radius;
                    else
                        similar = true;                    
                    break;
                }
        if (!similar)
            circles.push_back(circlesTemp.front());

        maxR = max(radius,maxR);
        minR = min(radius,minR);        
        
        HoughCircles( src_gray, circlesTemp, CV_HOUGH_GRADIENT, 4/3, src_gray.rows, 100, 45, 10,maxRadius);
    }
    
    vector< vector<int> > types(3, vector<int>());
    vector<Mat> bgr_planes;
    Mat b_hist[3], g_hist[3], r_hist[3], tempImg[3];
    int M[3][3][2];
    int coins[5] = {0,0,0,0,0}, chance[5];
    // -----------------------------------------------------------------------------
    // Process the circles found
    for( size_t i = 0; i < circles.size(); i++ )
    {
        Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
        int radius = cvRound(circles[i][2]);
        tempImg[0] = src(Rect( std::max(center.x-radius,0), std::max(center.y-radius,0), radius*2, radius*2 ));
        tempImg[0].copyTo(tempImg[1]);
        tempImg[2] = src(Rect( std::max(center.x-radius*0.7,0.0), std::max(center.y-radius*0.7,0.0), radius*1.4, radius*1.4 ));
        circle( tempImg[1], Point(radius,radius), radius*0.7, Scalar(0,0,0), -1, 8, 0 );        

        for (int j = 0; j < 3; j++)
        {
            // 0 - Original | 1 - Out | 2 - In
            bgr_planes.clear();
            /// Separate the image in 3 places ( B, G and R )
            split( tempImg[j], bgr_planes );
            /// Establish the number of bins
            int histSize = 10;
            /// Set the ranges ( for B,G,R) )
            float range[] = { (j == 1)? histSize : 0, 256 } ;
            const float* histRange = { range };
            bool uniform = true; bool accumulate = false;

            /// Compute the histograms:
            calcHist( &bgr_planes[0], 1, 0, Mat(), b_hist[j], 1, &histSize, &histRange, uniform, accumulate );
            calcHist( &bgr_planes[1], 1, 0, Mat(), g_hist[j], 1, &histSize, &histRange, uniform, accumulate );
            calcHist( &bgr_planes[2], 1, 0, Mat(), r_hist[j], 1, &histSize, &histRange, uniform, accumulate );

            minMaxIdx(b_hist[j],NULL,NULL,NULL,M[j][0]);
            minMaxIdx(g_hist[j],NULL,NULL,NULL,M[j][1]);
            minMaxIdx(r_hist[j],NULL,NULL,NULL,M[j][2]);
            if (debug)
                cout << M[j][0][0] << ";" << M[j][1][0] << ";" << M[j][2][0] << ";" << ((j==2)? radius*0.3 : radius) << ";" << j << ";";
        }
        if (debug)
            cout << "|" << endl;

        for (int j = 0; j < 5; j++) chance[j] = 0;
        int sumDiffOut,sumDiffIn,sumDiffTotal;    
        sumDiffOut = abs(M[1][0][0]-M[1][1][0])+abs(M[1][0][0]-M[1][2][0])+abs(M[1][1][0]-M[1][2][0]);
        sumDiffIn = abs(M[2][0][0]-M[2][1][0])+abs(M[2][0][0]-M[2][2][0])+abs(M[2][1][0]-M[2][2][0]);
        sumDiffTotal = abs(M[0][0][0]-M[0][1][0])+abs(M[0][0][0]-M[0][2][0])+abs(M[0][1][0]-M[0][2][0]);
        double mediaDiffs = (sumDiffOut + sumDiffIn + sumDiffTotal) / 3;

        if (sumDiffTotal == 0)
        {
            chance[0] += 1;
            chance[1] += 7;
            chance[2] += 1;
        }
        else if (sumDiffTotal == 2)
        {
            chance[0] += 4;
            chance[1] += 2;
            chance[2] += 2; // 3            
            chance[3] += 1;
            chance[4] += 4;
        }        
        else if (sumDiffTotal == 4)
        {
            chance[0] += 5;
            chance[2] += 5; // 4?            
            chance[3] += 5;
            chance[4] += 3;
        }
        else if (sumDiffTotal == 6)
        {
            chance[0]++;
            chance[2]++; // 3?            
            chance[3] += 4;
            chance[4] += 2;
        }
        else 
        {
            chance[2]++;
            chance[4]++;
        }
        
        if (debug)
        {
            cout << "sumDiffTotal = " << sumDiffTotal << " (" << printCoins(chance) << ")" << endl;
            cout << "in: " << sumDiffIn << " | out: " << sumDiffOut << endl;  
        }

        if (abs(sumDiffOut-sumDiffIn) > 2)
        {
            chance[0] += 4; // 10
            chance[2] += 4;
            if (debug) cout << "sumDiffOut - sumDiffIn > 2(" << printCoins(chance) << ")" << endl;
        }

        if (mediaDiffs < 2)
        {
            chance[0]++;
            chance[1]+= 10;
            chance[2]+= 2;
            if (debug) cout << "media < 2(" << printCoins(chance) << ")" << endl;
        }
        else if (mediaDiffs < 4)
        {
            chance[0] += 8;
            chance[2] += 4; // 5
            chance[3] += 2;
            chance[4] += 5;            
            if (debug) cout << "media < 4(" << printCoins(chance) << ")" << endl;            
        }
        else if (mediaDiffs < 6)
        {
            chance[0] += 1;
            chance[2] += 3; // 4
            chance[3] += 4;
            chance[4] += 2;            
            if (debug) cout << "media < 6(" << printCoins(chance) << ")" << endl;            
        }
        else if (mediaDiffs < 8)
        {
            chance[2] += 2; // 4
            chance[3] += 4;
            chance[4] += 3;            
            if (debug) cout << "media < 8(" << printCoins(chance) << ")" << endl;            
        }
        
        // se b,g ou r sao menores que 2 na moeda completa
        if (M[0][0][0] < 2 || M[0][1][0] < 2 || M[0][2][0] < 2)
        {
            if (M[0][0][0] < 1 || M[0][1][0] < 1 || M[0][2][0] < 1)
            {
                chance[3] += 8; // 3
                chance[4] += 8; // 3           
                if (debug) cout << "b|g|r == 0(" << printCoins(chance) << ")" << endl;
            }
            else
            {
                chance[2] += 7; // 2
                chance[3] += 7; // 2
                chance[4] += 7; // 2
                if (debug) cout << "b|g|r == 1(" << printCoins(chance) << ")" << endl;                
            }
        }        
        else if (M[0][0][0] > 5 || M[0][1][0] > 5 || M[0][2][0] > 5)
        {
            chance[0] += 7; // 3
            chance[2] += 2;
            if (debug) cout << "b|g|r > 5(" << printCoins(chance) << ")" << endl;            
        }
        else
        {
            chance[0] += 2;
            chance[1] += 3;
            chance[2]++;
            chance[3]++;
            chance[4]++;
            if (debug) cout << "b|g|r //(" << printCoins(chance) << ")" << endl;
        }
        
        if (debug) cout << "--------------" << endl;        

        int finalC = 0;
        for (int j = 0; j < 5; j++)
            if (chance[j] > chance[finalC])
                finalC = j; 

        coins[finalC]++;
        if (debug)
        {
            imshow("Coin",tempImg[0]);
            waitKey(0);        
        }
    }

    int sum = 0;
    sum += coins[0] * 100;
    sum += coins[1] * 50;
    sum += coins[2] * 25;
    sum += coins[3] * 10;
    sum += coins[4] * 5;
    cout << fixed << setprecision(2) << (double)sum / 100 << endl;

    // Write src_gray output to disk
    if (debug)
    {
        stringstream name;
        string n;
        name.str("");
        n = argv[1];
        name << maxRadius << "-" << n.substr(n.find_last_of("/")+1);
        imwrite(name.str(),src_gray);
    }
    // ----------------

    if (debug)
    {
        imshow("SRC",src);
        waitKey(0);
    }
    return 0;
}