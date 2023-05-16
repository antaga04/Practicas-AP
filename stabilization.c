#include <stdio.h>
#include <stdlib.h>

#include <opencv/cv.h>
#include <opencv/highgui.h>

void cvSet(IplImage* img, CvScalar value) {
    int i, j;
    for (i = 0; i < img->height; i++) {
        uchar* ptr = (uchar*)(img->imageData + i * img->widthStep);
        for (j = 0; j < img->width; j++) {
            ptr[j * img->nChannels + 0] = (uchar)value.val[0];
            ptr[j * img->nChannels + 1] = (uchar)value.val[1];
            ptr[j * img->nChannels + 2] = (uchar)value.val[2];
        }
    }
}

void cvCopy(IplImage* src, IplImage* dst) {
    int i, j;
    for (i = 0; i < src->height; i++) {
        uchar* srcPtr = (uchar*)(src->imageData + i * src->widthStep);
        uchar* dstPtr = (uchar*)(dst->imageData + i * dst->widthStep);
        for (j = 0; j < src->width; j++) {
            dstPtr[j * dst->nChannels + 0] = srcPtr[j * src->nChannels + 0];
            dstPtr[j * dst->nChannels + 1] = srcPtr[j * src->nChannels + 1];
            dstPtr[j * dst->nChannels + 2] = srcPtr[j * src->nChannels + 2];
        }
    }
}


int main(int argc, char** argv) {


    if (argc != 3)
        exit(-1);

    // Creamos las imagenes a mostrar
    CvCapture* capture = cvCaptureFromAVI(argv[1]);

    // Always check if the program can find a file
    if (!capture) {
        printf("Error: fichero %s no leido\n", argv[1]);
        return EXIT_FAILURE;
    }

    CvCapture* capture2 = cvCaptureFromAVI(argv[2]);

    if (!capture2) {
        printf("Error: fichero %s no leido\n", argv[1]);
        return EXIT_FAILURE;
    }

    printf("Pulsa una tecla para empezar...\n");
    cvWaitKey(0);

    IplImage *InFrameNew;
    IplImage *InFrameNew2;

    while ((InFrameNew = cvQueryFrame(capture)) != NULL && (InFrameNew2 = cvQueryFrame(capture2)) != NULL) {
        cvShowImage("Frame Video", InFrameNew);
        cvShowImage("Frame Video 2", InFrameNew2);
        cvWaitKey(1);
    }
}
