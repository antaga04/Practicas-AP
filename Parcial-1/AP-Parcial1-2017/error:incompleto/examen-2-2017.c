
#include <stdio.h>
#include <stdlib.h>

#include <opencv/cv.h>
#include <opencv/highgui.h>

int main(int argc, char *argv) {

    IplImage ImgOrigen = cvLoadImage(argv[1], CV_LOAD_IMAGE_UNCHANGED);
    IplImage* ImgDestino = cvCreateImage(cvSize(ImgOrigen->width, ImgOrigen->height), ImgOrigen->depth, ImgOrigen->nChannels);
    int fila, cc;
    for (fila = 0; fila < ImgOrigen->height; fila++) {
        m128i *pImgOrigen = (m128i ) (ImgOrigen->imageData + fila ImgOrigen-> widthStep);
        m128i *pImgDestino = (m128i ) (ImgDestino->imageData + fila ImgDestino->widthStep);
        __m128i registro;

        for (cc = 0; cc < ImgOrigen->widthStep; cc = cc + 16) {

            //Opcion 1
            pImgDestino++ =pImgOrigen++;

            //Opcion 2
            //registro = pImgOrigen++;
            //pImgDestino++ = registro;

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
Pebb — hoy a las 0:01
/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

Expandir
message.txt
5 KB
f
/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   puzzle.c
 * Author: Pebb
 *
 * Created on 26 de marzo de 2023, 22:05
 */

#include <stdio.h>
#include <stdlib.h>

#include <emmintrin.h>
#include <smmintrin.h>

#include <opencv/cv.h>
#include <opencv/highgui.h>

int main(int argc, char **argv) {

    if (argc != 3) {
        printf("Error: Usage %s image_file_name\n", argv[0]);
        return EXIT_FAILURE;
    }

    //CV_LOAD_IMAGE_COLOR = 1 forces the resultant IplImage to be colour.
    //CV_LOAD_IMAGE_GRAYSCALE = 0 forces a greyscale IplImage.
    //CV_LOAD_IMAGE_UNCHANGED = -1
    IplImage* Img = cvLoadImage(argv[1], CV_LOAD_IMAGE_UNCHANGED);

    // Always check if the program can find the image file
    if (!Img) {
        printf("Error: file %s not found\n", argv[1]);
        return EXIT_FAILURE;
    }

    // a visualization window is created with title: image file name
    cvNamedWindow(argv[1], CV_WINDOW_AUTOSIZE);
    // Img is shown in 'image' window
    cvShowImage(argv[1], Img);
    cvWaitKey(0);
    
    IplImage* Img2 = cvLoadImage(argv[2], CV_LOAD_IMAGE_UNCHANGED);

    // Always check if the program can find the image file
    if (!Img2) {
        printf("Error: file %s not found\n", argv[2]);
        return EXIT_FAILURE;
    }

    // a visualization window is created with title: image file name
    cvNamedWindow(argv[2], CV_WINDOW_AUTOSIZE);
    // Img is shown in 'image' window
    cvShowImage(argv[2], Img2);
    cvWaitKey(0);

    // Crea la imagen para la componete azul
    IplImage* ImgFinal = cvCreateImage(cvSize(Img->width, Img->height), Img->depth, Img->nChannels);
    IplImage* ImgBlancoNegro = cvCreateImage(cvSize(Img->width, Img->height), Img->depth, Img->nChannels);

    cvNamedWindow("Foto B", CV_WINDOW_AUTOSIZE);

    //Ejercicio 1

    int fila, columna,cc;

    for (fila = 0; fila < Img->height; fila += 2) {

        //__m128i *pImg = (__m128i *) (Img->imageData + fila * Img->widthStep);
        //__m128i *pImgFinal = (__m128i *) (ImgFinal->imageData + fila * ImgFinal->widthStep);
        
        unsigned char *pImg = (unsigned char *) Img->imageData + fila * Img->widthStep;
        unsigned char *pImgFinal = (unsigned char *) ImgFinal->imageData + fila * ImgFinal->widthStep;

        for (cc = 0; cc < Img->width; cc += 1) {

            *pImgFinal = *pImg;
            pImg +=2;
            pImgFinal +=2;

        }

        cvShowImage("Foto B", ImgFinal);
        cvWaitKey(10);

    }

    //Ejercicio 2
    
    __m128i blanco = _mm_set1_epi8(0x00);
    __m128i negro = _mm_set1_epi8(0xFF);
    __m128i cien = _mm_set1_epi8(100);
    
    for (fila = 0; fila < Img->height; fila += 1) {

        __m128i *pImg = (__m128i *) (Img->imageData + fila * Img->widthStep);
        __m128i *pImg2 = (__m128i *) (Img2->imageData + fila * Img2->widthStep);
        __m128i *pImgBlancoNegro = (__m128i *) (ImgBlancoNegro->imageData + fila * ImgBlancoNegro->widthStep);

        for (cc = 0; cc < Img->widthStep; cc += 16) {
            __m128i pixel1 = *pImg; 
            __m128i pixel2 = *pImg2; 
            __m128i resta = _mm_sub_epi8(pixel1, pixel2);
            __m128i comparacion = _mm_cmpgt_epi8(resta, cien);
            __m128i resultado = _mm_or_si128(_mm_and_si128(comparacion, blanco), _mm_andnot_si128(comparacion, negro));
            _mm_store_si128(pImgBlancoNegro, resultado);
            
            pImg++;
            pImg2++;
            pImgBlancoNegro++;

        }

        cvShowImage("Foto ByN", ImgBlancoNegro);
        cvWaitKey(10);

    }

    // memory release for images before exiting the application
    cvReleaseImage(&Img);
    cvReleaseImage(&ImgFinal);

    // Self-explanatory
    cvDestroyWindow(argv[1]);
    cvDestroyWindow("Foto B");

    return EXIT_SUCCESS;
}
