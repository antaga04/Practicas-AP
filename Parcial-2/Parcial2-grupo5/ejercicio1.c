#include <stdio.h>
#include <stdlib.h>

#include <opencv/cv.h>
#include <opencv/highgui.h>

#include <Windows.h>

// Definición variables globales
#define ALTO 16  //BLOQUE
#define ANCHO 16 //BLOQUE

void moverBloque(IplImage* imgA, int filaA, int colA, IplImage* imgB, int filaB, int colB) {
    __m128i ceros;
    ceros = _mm_set1_epi8((unsigned char) 0);
    int fila, cc;

    for (fila = 0; fila < ALTO; fila++) {
        __m128i *pImg1 = (__m128i *) (imgA->imageData + (fila + filaA) * imgA->widthStep + colA * imgA->nChannels);
        __m128i *pImg2 = (__m128i *) (imgB->imageData + (fila + filaB) * imgB->widthStep + colB * imgB->nChannels);
        for (cc = 0; cc < ANCHO * imgA->nChannels; cc += 16) {
            *pImg2++ = *pImg1;
            *pImg1 = ceros;
            *pImg1++;
        }
    }
}

int main(int argc, char **argv) {

    if (argc != 3) {
        printf("Error: Usage %s image_file_name\n", argv[0]);
        return EXIT_FAILURE;
    }

    IplImage* ImgB = cvLoadImage(argv[1], CV_LOAD_IMAGE_UNCHANGED);
    IplImage* transicion = cvCreateImage(cvSize(ImgB->width, ImgB->height), ImgB->depth, ImgB->nChannels);

    if (!ImgB) {
        printf("Error: file %s not found\n", argv[1]);
        return EXIT_FAILURE;
    }

    /*
        cvShowImage("Imagen Origen", ImgB);
        cvShowImage("Imagen Destino", transicion);
        cvWaitKey(0);
     */

    int fila1, fila2, columna1, columna2;
    int alto = 16, ancho = 16;

    for (fila1 = 0; fila1 < ImgB->height; fila1 += alto) {
        for (columna1 = 0; columna1 < ImgB->width; columna1 += ancho) {
            fila2 = fila1;
            columna2 = columna1;

            moverBloque(ImgB, fila1, columna1, transicion, fila2, columna2);

            cvShowImage("Imagen Origen", ImgB);
            cvShowImage("Imagen Destino", transicion);
            cvWaitKey(15);
        }
    }
    // memory release for images before exiting the application
    cvReleaseImage(&ImgB);
    cvReleaseImage(&transicion);
    cvWaitKey(0);

    // Self-explanatory
    cvDestroyWindow("Imagen Origen");
    cvDestroyWindow("Imagen Destino");

    return EXIT_SUCCESS;
}
