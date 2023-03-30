#include <stdio.h>
#include <stdlib.h>

#include <opencv/cv.h>
#include <opencv/highgui.h>

int main(int argc, char **argv) {

    IplImage* inicial = cvLoadImage(argv[1], CV_LOAD_IMAGE_COLOR);

    IplImage* transicion = cvCreateImage(cvSize(inicial->width, inicial->height), IPL_DEPTH_32F, 3);

    float *pinicial, *ptransicion;
    int fila, cc, pasos;

    for (fila = 0; fila < inicial->height; fila++) {
        pinicial = (float *) (inicial->imageData + fila * inicial->widthStep);
        ptransicion = (float *) (transicion->imageData + fila * transicion->widthStep);

        for (cc = 0; cc < inicial->width * 3; cc += 3) {
            ptransicion[cc] = pinicial[cc] / 255.0;
            ptransicion[cc + 1] = pinicial[cc + 1] / 255.0;
            ptransicion[cc + 2] = pinicial[cc + 2] / 255.0;
        }
    }

    for (pasos = 0; pasos < 256; pasos++) {
        for (fila = 0; fila < transicion->height; fila++) {
            ptransicion = (float *) (transicion->imageData + fila * transicion->widthStep);

            for (cc = 0; cc < transicion->width * 3; cc += 3) {
                ptransicion[cc] += 1.0 / 255.0;
                ptransicion[cc + 1] += 1.0 / 255.0;
                ptransicion[cc + 2] += 1.0 / 255.0;
            }
        }

        cvNamedWindow("Fade a Blanco", CV_WINDOW_AUTOSIZE);
        cvShowImage("Fade a Blanco", transicion);
        cvWaitKey(20);
    }

    cvReleaseImage(&transicion);
    cvReleaseImage(&inicial);

    cvDestroyWindow(argv[1]);
    cvDestroyWindow("Fade a Blanco");

    return EXIT_SUCCESS;
}
