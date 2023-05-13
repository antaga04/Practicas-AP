#include <stdio.h>
#include <stdlib.h>

#include <opencv/cv.h>
#include <opencv/highgui.h>

int main(int argc, char **argv) {

    IplImage* ImgOrigen = cvLoadImage(argv[1], CV_LOAD_IMAGE_UNCHANGED);
    IplImage* ImgDestino = cvCreateImage(cvSize(ImgOrigen->width, ImgOrigen->height), ImgOrigen->depth, ImgOrigen->nChannels);
    int fila, cc;
    for (fila = 0; fila < ImgOrigen->height; fila+=2) {
/*
        __m128i *pImgOrigen = (__m128i *) (ImgOrigen->imageData + fila * ImgOrigen-> widthStep);
        __m128i *pImgDestino = (__m128i *) (ImgDestino->imageData + fila * ImgDestino->widthStep);
*/
        unsigned char *pImgOrigen = ( unsigned char *) (ImgOrigen->imageData + fila * ImgOrigen-> widthStep);
        unsigned char *pImgDestino = ( unsigned char *) (ImgDestino->imageData + fila * ImgDestino->widthStep);
        
        __m128i registro;

        for (cc = 0; cc < ImgOrigen->widthStep; cc+=2) {

            //Opcion 1
            *pImgDestino = *pImgOrigen;
            pImgDestino+=2;
            pImgOrigen+=2;
            
            //Opcion 2
            //registro = *pImgOrigen++;
            //*pImgDestino++ = registro;

            //Opcion 3
            //registro = _mm_load_si128(pImgOrigen++);
            //_mm_store_si128(pImgDestino++,registro);

            //Opcion 4, para datos no alineados en memoria
            //registro = _mm_loadu_si128(pImgOrigen++);
            //_mm_storeu_si128(pImgDestino++,registro);
        }
    }

    cvNamedWindow("Componente Original", CV_WINDOW_AUTOSIZE);
    cvShowImage("Componente Original", ImgOrigen);
    
    cvNamedWindow("Componente Copiado", CV_WINDOW_AUTOSIZE);
    cvShowImage("Componente Copiado", ImgDestino);
    cvWaitKey(0);

    cvReleaseImage(&ImgOrigen);
    cvReleaseImage(&ImgDestino);


    cvDestroyWindow(argv[1]);
    cvDestroyWindow("Componente Copiado");

    return EXIT_SUCCESS;
}
