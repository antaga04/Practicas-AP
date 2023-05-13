#include <stdio.h>
#include <stdlib.h>
#include <emmintrin.h>

#include <opencv/cv.h>
#include <opencv/highgui.h>

int main(int argc, char** argv) {

    if (argc != 2) {
        printf("Usage: %s image_file_name\n", argv[0]);
        return EXIT_FAILURE;
    }

    IplImage* imagenInicial = cvLoadImage(argv[1], CV_LOAD_IMAGE_UNCHANGED);
    if (!imagenInicial) {
        printf("Error: %s fichero no leido", argv[1]);
        return EXIT_FAILURE;
    }

    IplImage* imagenInicialFloat = cvCreateImage(cvSize(imagenInicial->width, imagenInicial ->height), IPL_DEPTH_32F, imagenInicial->nChannels);
    cvConvertScale(imagenInicial, imagenInicialFloat, 1.0 / 255.0, 0);
    cvShowImage(argv[1], imagenInicialFloat);
    cvWaitKey(0);


    IplImage* imagenFinal = cvLoadImage(argv[2], CV_LOAD_IMAGE_UNCHANGED);
    if (!imagenFinal) {
        printf("Error: %s fichero no leida", argv[2]);
        return EXIT_FAILURE;
    }

    IplImage* imagenFinalFloat = cvCreateImage(cvSize(imagenFinal->width, imagenFinal ->height), IPL_DEPTH_32F, imagenFinal->nChannels);
    cvConvertScale(imagenFinal, imagenFinalFloat, 1.0 / 255.0, 0);

    IplImage* imagenTransicionFloat = cvCreateImage(cvSize(imagenInicialFloat->width, imagenInicialFloat ->height), IPL_DEPTH_32F, imagenInicialFloat->nChannels);
    cvShowImage("imagenTransicionFloat", imagenTransicionFloat);
    cvWaitKey(10);


    int paso;

    int iteraciones = 100;

    int fil, cc;

    for (paso = 0; paso <= iteraciones; paso++) {

        float pasoFloat = ((float) paso) / (float) (iteraciones);
        __m128 pasoSSE = _mm_set_ps1(pasoFloat);
        __m128 pasoMenosUnoSSE = _mm_set_ps1(1.0 - pasoFloat);

        for (fil = 0; fil < imagenInicialFloat->height; fil++) {

            __m128 *pimagenInicialFloat = (__m128 *) (imagenInicialFloat->imageData + fil * imagenInicialFloat->widthStep);
            __m128 *pimagenTransicionFloat = (__m128 *) (imagenTransicionFloat->imageData + fil * imagenTransicionFloat->widthStep);

            for (cc = 0; cc < imagenInicialFloat->widthStep; cc = cc + 16) {

                __m128 ccInicial, ccFinal, ccTransicion;

                ccInicial = *pimagenInicialFloat++;
                ccInicial = _mm_div_ps(ccInicial, pasoMenosUnoSSE);
                *pimagenTransicionFloat++ = ccInicial;
            }


        }
        cvWaitKey(30);
        cvShowImage("imagenTransicionFloat", imagenTransicionFloat);
    }



    return (EXIT_SUCCESS);
}
