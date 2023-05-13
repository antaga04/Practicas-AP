#include <stdio.h>
#include <stdlib.h>

#include <opencv/cv.h>
#include <opencv/highgui.h>

#include <emmintrin.h>
#include <emmintrin.h>

int main(int argc, char **argv) {

    __m128i unos;
    unos = _mm_set1_epi8((unsigned char) 1);

    IplImage* inicial = cvLoadImage(argv[1], CV_LOAD_IMAGE_UNCHANGED);
    IplImage* transicion = cvCreateImage(cvSize(inicial->width, inicial->height), inicial->depth, inicial->nChannels);

    int fila, cc,pasos;
    for ( pasos = 0; pasos < 256; pasos++) {
        for (fila = 0; fila < transicion->height; fila++) {

            __m128i *pinicial = (__m128i *) (inicial->imageData + fila * inicial -> widthStep);
            __m128i *ptransicion = (__m128i *) (transicion->imageData + fila * transicion->widthStep);

            for (cc = 0; cc < transicion->widthStep; cc = cc + 16) {

                *ptransicion = _mm_adds_epu8(*ptransicion, unos);
                *ptransicion = _mm_min_epu8(*ptransicion, *pinicial);

                ptransicion++;
                pinicial++;
            }
        }

        cvNamedWindow("Componente Original", CV_WINDOW_AUTOSIZE);
        cvShowImage("Componente Original", inicial);

        cvNamedWindow("Componente Copiado", CV_WINDOW_AUTOSIZE);
        cvShowImage("Componente Copiado", transicion);
        cvWaitKey(20);
    }
    cvReleaseImage(&transicion);
    cvReleaseImage(&inicial);


    cvDestroyWindow(argv[1]);
    cvDestroyWindow("Componente Copiado");

    return EXIT_SUCCESS;
}
