#include <stdio.h>
#include <stdlib.h>

#include <opencv/cv.h>
#include <opencv/highgui.h>

int main(int argc, char **argv) {

    IplImage* inicial = cvLoadImage(argv[1], CV_LOAD_IMAGE_UNCHANGED);
    IplImage* transicion = cvCreateImage(cvSize(inicial->width, inicial->height), inicial->depth, inicial->nChannels);
    IplImage* negro = cvCreateImage(cvSize(inicial->width, inicial->height), inicial->depth, inicial->nChannels);
    cvZero(negro); // Imagen completamente negra

    int pasos;
    for (pasos = 0; pasos < 256; pasos++) {
        double alpha = (double)pasos / 255.0;
        double beta = 1.0 - alpha;

        cvConvertScale(inicial, transicion, alpha, 0); // Transición de la imagen original
        cvConvertScale(negro, negro, beta, 0); // Transición a negro

        cvMerge(transicion, transicion, transicion, NULL, transicion); // Mezcla de los canales
        cvAdd(negro, transicion, transicion, NULL); // Suma de las imágenes

        cvNamedWindow("Componente Original", CV_WINDOW_AUTOSIZE);
        cvShowImage("Componente Original", inicial);

        cvNamedWindow("Componente Copiado", CV_WINDOW_AUTOSIZE);
        cvShowImage("Componente Copiado", transicion);
        cvWaitKey(20);
    }

    cvReleaseImage(&transicion);
    cvReleaseImage(&inicial);
    cvReleaseImage(&negro);

    cvDestroyWindow(argv[1]);
    cvDestroyWindow("Componente Copiado");

    return EXIT_SUCCESS;
}
