#include <stdio.h>
#include <stdlib.h>

#include <opencv/cv.h>
#include <opencv/highgui.h>

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
