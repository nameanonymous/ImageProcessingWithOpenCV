#include <iostream>
#include <vector>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;
using namespace cv;

/**
 * This program reads biscuit images with shapes of rectangle, circle, and heart,
 * then detect shape and divide into three grounps.
 */

int main()
{
	Mat mask;
	vector<Mat> images;
	for (int i = 0; i < 16; ++i)
		images.push_back(imread("D:\\ProgramData\\handsign\\" + to_string(i) + ".jpg"));

	Mat samples(16, 3, CV_32F); //18 images (objects); 3 features.
	vector<int> Pointnumber;
	for (int i = 0; i < images.size(); ++i)
	{
		Mat img;
		cvtColor(images.at(i), img, COLOR_BGR2GRAY);

		threshold(img, mask, 200, 255, THRESH_BINARY_INV | THRESH_OTSU);
		medianBlur(mask, mask, 7);

		vector<vector<cv::Point>> conts;
		findContours(mask.clone(), conts, RETR_EXTERNAL, CHAIN_APPROX_NONE);
		int maxArea = 0;
		int maxIndex = 0;
		for (int j = 0; conts.size() > j; j++)
		{

			if (contourArea(conts[j]) > maxArea)
			{
				maxArea = contourArea(conts[j]);
				maxIndex = j;
			}
		}
		drawContours(mask, conts, maxIndex, Scalar(0, 0, 255), 1);
		vector<int> uhull;
		convexHull(conts[maxIndex], uhull, false, false);
		vector<Vec4i> omega;
		convexityDefects(conts[maxIndex], uhull,omega);
		int number =  0;
		for (int k = 0;omega.size() > k; k++)
		{
			Vec4i v = omega[k]; //get a defects,v[0]=first point,v[1]=second point,v[2]=defect point,v[3]= distance between defect point and �Ȃ�����(hull)
			double distance = (v[3] / 256.0);
			if (distance > 90)
			{
				Point defectPoint = conts[maxIndex][v[2]];
				circle(mask,defectPoint,2,Scalar(0,255,0),2);
				number++;
			}
			
		}
		Pointnumber.push_back(number);

	}
	int b = 0, c = 0, d = 0;
	for (int a = 0; a < 16; a++)
	{
		auto img = images.at(a);
		int count = 0;

		if (count < 16)
		{

			if (Pointnumber.at(a) >= 2) {
				string str = "Paper number" + to_string(b + 1);
				imshow(str, images.at(a));
				b++;
			}
			else if (Pointnumber.at(a) == 1) {
				string str = "Scissors number" + to_string(c + 1);
				imshow(str, images.at(a));
				c++;
			}
			else if (Pointnumber.at(a) == 0) {
				string str = "ROCK number" + to_string(d + 1);
				imshow(str, images.at(a));
				d++;
			}

		}
	}
	waitKey(0);
}
