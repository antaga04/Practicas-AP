/*
author: Anta
*/
#include <stdio.h>
#include <stdlib.h>

#include <emmintrin.h>

#include <opencv/cv.h>
#include <opencv/highgui.h>

int main(int argc, char** argv) {

    if (argc != 2) {
        printf("Error: Usage %s image_file_name\n", argv[0]);
        return EXIT_FAILURE;
    }

    IplImage* Img = cvLoadImage(argv[1], CV_LOAD_IMAGE_UNCHANGED);

    //check if the program can find the image file
    if (!Img) {
        printf("Error: file %s not found\n", argv[1]);
        return EXIT_FAILURE;
    }

    // a visualization window is created
    cvNamedWindow(argv[1], CV_WINDOW_AUTOSIZE);
    // Img is shown
    cvShowImage(argv[1], Img);
    cvWaitKey(0);

    // Crear imagen que pondremos en verde
    IplImage* ImgVerde = cvCreateImage(cvSize(Img->width + 200, Img->height), IPL_DEPTH_8U, 4);

    int fila, cc;
    // Set Imagen verde
    __m128i Verde = _mm_set1_epi32(0x0000FF00);

    for (fila = 0; fila < Img->height; fila++) {

        __m128i *pImgVerde = (__m128i *) (ImgVerde->imageData + fila * ImgVerde->widthStep);

        for (cc = 0; cc < ImgVerde->widthStep; cc += 16) {
            __m128i PuertaVerde = _mm_and_si128(Verde, Verde);
            _mm_storeu_si128(pImgVerde, PuertaVerde);
            pImgVerde++;
        }
    }

    // Copiamos la imagen original a partir de una distancia
    // para dejar los bordes verdes
    for (fila = 0; fila < Img->height; fila++) {

        __m128i *pImgVerde = (__m128i *) (ImgVerde->imageData +(ImgVerde->nChannels*100)+ fila * ImgVerde->widthStep);
        __m128i *pImg = (__m128i *) (Img->imageData + fila * Img-> widthStep);

        for (cc = 0; cc < Img->widthStep; cc += 16) {
            // No empezamos a copiar la imagen hasta llega
                *pImgVerde = *pImg;
    

        }
    }

    cvNamedWindow("Dados en verde", CV_WINDOW_AUTOSIZE);
    cvShowImage("Dados en verde", ImgVerde);
    cvWaitKey(0);


    // memory release for images before exiting the application
    cvReleaseImage(&ImgVerde);

    // Self-explanatory
    cvDestroyWindow("Dados en verde");

    return (EXIT_SUCCESS);
}
