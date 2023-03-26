#include <stdio.h>
#include <stdlib.h>

#include <opencv/cv.h>
#include <opencv/highgui.h>

int main(int argc, char **argv) {

    IplImage* imgA = cvLoadImage(argv[1], CV_LOAD_IMAGE_GRAYSCALE);
    IplImage* imgB = cvLoadImage(argv[2], CV_LOAD_IMAGE_GRAYSCALE);

    IplImage* imgC = cvCreateImage(cvSize(imgA->width, imgA->height), imgA->depth, 1);

    int row, col;
    for (row = 0; row < imgA->height; row++) {
        for (col = 0; col < imgA->width; col++) {
            int valA = cvGet2D(imgA, row, col).val[0];
            int valB = cvGet2D(imgB, row, col).val[0];
            int diff = abs(valA - valB);
            if (diff < 100) {
                cvSet2D(imgC, row, col, cvScalar(255));
            } else {
                cvSet2D(imgC, row, col, cvScalar(0));
            }
        }
    }

    cvNamedWindow("Imagen A", CV_WINDOW_AUTOSIZE);
    cvShowImage("Imagen A", imgA);

    cvNamedWindow("Imagen B", CV_WINDOW_AUTOSIZE);
    cvShowImage("Imagen B", imgB);

    cvNamedWindow("Imagen C", CV_WINDOW_AUTOSIZE);
    cvShowImage("Imagen C", imgC);

    cvWaitKey(0);

    cvReleaseImage(&imgA);
    cvReleaseImage(&imgB);
    cvReleaseImage(&imgC);

    cvDestroyAllWindows();

    return EXIT_SUCCESS;
}
