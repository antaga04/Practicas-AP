#include <stdio.h>
#include <stdlib.h>

#include <opencv/cv.h>
#include <opencv/highgui.h>

#include <emmintrin.h>
#include <emmintrin.h>

int main(int argc, char **argv) {

    __m128i ceros;
    ceros = _mm_set1_epi8((unsigned char) 0);

    __m128i unos;
    unos = _mm_set1_epi8((unsigned char) 1);

    IplImage* inicial = cvLoadImage(argv[1], CV_LOAD_IMAGE_UNCHANGED);
    IplImage* transicion = cvCreateImage(cvSize(inicial->width, inicial->height), inicial->depth, inicial->nChannels);

    int fila, cc, pasos;
    for (pasos = 0; pasos < 256; pasos++) {
        for (fila = 0; fila < transicion->height; fila++) {

            __m128i *pinicial = (__m128i *) (inicial->imageData + fila * inicial -> widthStep);
//            __m128i *ptransicion = (__m128i *) (transicion->imageData + fila * transicion->widthStep);

            for (cc = 0; cc < transicion->widthStep; cc = cc + 16) {

                __m128i A = _mm_loadu_si128(pinicial);
                __m128i B = _mm_subs_epu8(A,unos);
                _mm_storeu_si128(pinicial,B);
                
                pinicial++;
            }
        }

        cvNamedWindow("Componente Original", CV_WINDOW_AUTOSIZE);
        cvShowImage("Componente Original", inicial);
        cvWaitKey(20);
    }
    cvReleaseImage(&transicion);
    cvReleaseImage(&inicial);


    cvDestroyWindow(argv[1]);
    cvDestroyWindow("Componente Copiado");

    return EXIT_SUCCESS;
}
