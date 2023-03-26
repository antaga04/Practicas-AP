#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main(int argc, char** argv) {
    Mat imgA, imgB, imgC;
    imgA = imread(argv[1], IMREAD_GRAYSCALE);
    imgB = imread(argv[2], IMREAD_GRAYSCALE);

    if (imgA.empty() || imgB.empty()) {
        cout << "Could not read the images" << endl;
        return -1;
    }

    imgC = Mat::zeros(imgA.size(), CV_8UC1);

    for (int i = 0; i < imgA.rows; i++) {
        for (int j = 0; j < imgA.cols; j++) {
            if (abs(imgA.at<uchar>(i, j) - imgB.at<uchar>(i, j)) < 100) {
                imgC.at<uchar>(i, j) = 255;
            }
        }
    }

    namedWindow("Image A", WINDOW_AUTOSIZE);
    imshow("Image A", imgA);

    namedWindow("Image B", WINDOW_AUTOSIZE);
    imshow("Image B", imgB);

    namedWindow("Image C", WINDOW_AUTOSIZE);
    imshow("Image C", imgC);

    waitKey(0);

    return 0;
}
