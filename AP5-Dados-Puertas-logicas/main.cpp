/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   puertaslogicas.c
 * Author: Pebb
 *
 * Created on 26 de marzo de 2023, 18:55
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


    // Copiar imagen
    IplImage* ImgRojo = cvCreateImage(cvSize(Img->width, Img->height), IPL_DEPTH_8U, 4);
    IplImage* ImgVerde = cvCreateImage(cvSize(Img->width, Img->height), IPL_DEPTH_8U, 4);
    IplImage* ImgAzul = cvCreateImage(cvSize(Img->width, Img->height), IPL_DEPTH_8U, 4);
    IplImage* ImgAlfa = cvCreateImage(cvSize(Img->width, Img->height), IPL_DEPTH_8U, 4);
    IplImage* ImgEj4 = cvCreateImage(cvSize(Img->width, Img->height), IPL_DEPTH_8U, 4);

    int fila, cc;
    for (fila = 0; fila < Img->height; fila++) {
        
        __m128i *pImg = (__m128i *) (Img->imageData + fila * Img-> widthStep);
        __m128i *pImgRojo = (__m128i *) (ImgRojo->imageData + fila * ImgRojo->widthStep);
        __m128i *pImgVerde = (__m128i *) (ImgVerde->imageData + fila * ImgVerde->widthStep);
        __m128i *pImgAzul = (__m128i *) (ImgAzul->imageData + fila * ImgAzul->widthStep);
        __m128i *pImgAlfa = (__m128i *) (ImgAlfa->imageData + fila * ImgAlfa->widthStep);
        __m128i *pImgEj4 = (__m128i *) (ImgEj4->imageData + fila * ImgEj4->widthStep);
        
        for (cc = 0; cc < Img->widthStep; cc = cc + 16) {
            *pImgRojo++ = *pImg;
            *pImgAzul++ = *pImg;
            *pImgVerde++ = *pImg;
            *pImgAlfa++ = *pImg;
            *pImgEj4++ = *pImg;
            pImg++;
        }
    }
    
    //Solo componente roja
    
    __m128i Rojo = _mm_set1_epi32(0xFF0000);

    for (fila = 0; fila < Img->height; fila++) {
        
        __m128i *pImgRojo = (__m128i *) (ImgRojo->imageData + fila * ImgRojo->widthStep);
        
        for (cc = 0; cc < ImgRojo->widthStep; cc+=16) {
            __m128i PuertaRoja = _mm_and_si128(Rojo, *pImgRojo);
            
            _mm_storeu_si128(pImgRojo, PuertaRoja);
            
            pImgRojo++;
        }

    }
    
    cvNamedWindow("Dados en rojo", CV_WINDOW_AUTOSIZE);
    cvShowImage("Dados en rojo", ImgRojo);
    
    //Solo componente verde
    
    __m128i Verde = _mm_set1_epi32(0x00FF00);
    
    for (fila = 0; fila < Img->height; fila++) {
        
        __m128i *pImgVerde = (__m128i *) (ImgVerde->imageData + fila * ImgVerde->widthStep);
        
        for (cc = 0; cc < ImgVerde->widthStep; cc+=16) {
            __m128i PuertaVerde = _mm_and_si128(Verde, *pImgVerde);
            
            _mm_storeu_si128(pImgVerde, PuertaVerde);
            
            pImgVerde++;
        }
    }

    cvNamedWindow("Dados en verde", CV_WINDOW_AUTOSIZE);
    cvShowImage("Dados en verde", ImgVerde);
    
    //Solo componente azul
    
    __m128i Azul = _mm_set1_epi32(0x0000FF);
    
    for (fila = 0; fila < Img->height; fila++) {
        
        __m128i *pImgAzul = (__m128i *) (ImgAzul->imageData + fila * ImgAzul->widthStep);
        
        for (cc = 0; cc < ImgAzul->widthStep; cc+=16) {
            __m128i PuertaAzul = _mm_and_si128(Azul, *pImgAzul);
            
            *pImgAzul++ = PuertaAzul;
        }
    }
    
    cvNamedWindow("Dados en azul", CV_WINDOW_AUTOSIZE);
    cvShowImage("Dados en azul", ImgAzul);
    cvWaitKey(0);
    
    //Solo componente alfa
    
    __m128i Alfa = _mm_set1_epi32(0xFF000000);
    
    for (fila = 0; fila < Img->height; fila++) {
        
        __m128i *pImgAlfa = (__m128i *) (ImgAlfa->imageData + fila * ImgAlfa->widthStep);
        
        for (cc = 0; cc < ImgAlfa->widthStep; cc+=16) {
            __m128i Canal_Alfa = _mm_and_si128(Alfa, *pImgAlfa);
            __m128i Shift1 = _mm_srli_epi32(Canal_Alfa, 8);
            __m128i Shift2 = _mm_srli_epi32(Shift1, 8);
            __m128i Shift3 = _mm_srli_epi32(Shift2, 8);
            
            __m128i suma1 = _mm_set1_epi32(0x00);
            
            suma1 = _mm_or_si128(Canal_Alfa, Shift1);
            __m128i suma2 = _mm_or_si128(suma1, Shift2);
            __m128i suma3 = _mm_or_si128(suma2, Shift3);
            
            *pImgAlfa++ = suma3;
            
        }
    }

    cvNamedWindow("Dados canal alfa", CV_WINDOW_AUTOSIZE);
    cvShowImage("Dados canal alfa", ImgAlfa);
    cvWaitKey(0);
    
    //Poner componente en todos la foto como valor 0x80h
    
    __m128i NoRojo = _mm_set1_epi32(0xFF00FFFF);
    __m128i SumarRojo = _mm_set1_epi32(0x00800000);
    
    for (fila = 0; fila < Img->height; fila++) {
        
        __m128i *pCambiarRojo = (__m128i *)(ImgEj4->imageData + fila * ImgEj4->widthStep);
        
        for (cc = 0; cc < ImgEj4->widthStep; cc+=16) {
            __m128i quitarRojo = _mm_and_si128(NoRojo, *pCambiarRojo);
            __m128i ponerRojo = _mm_or_si128(quitarRojo,SumarRojo);
            
            *pCambiarRojo++ = ponerRojo;
        }
    }

    cvNamedWindow("Dados ejericio 4", CV_WINDOW_AUTOSIZE);
    cvShowImage("Dados ejericio 4", ImgEj4);
    cvWaitKey(0);

    // memory release for images before exiting the application
    cvReleaseImage(&ImgRojo);
    cvReleaseImage(&ImgVerde);
    cvReleaseImage(&ImgAzul);
    cvReleaseImage(&ImgAlfa);
    cvReleaseImage(&ImgEj4);
    
    // Self-explanatory
    cvDestroyWindow("Dados en rojo");
    cvDestroyWindow("Dados en verde");
    cvDestroyWindow("Dados en azul");
    cvDestroyWindow("Dados canal alfa");
    cvDestroyWindow("Dados ejericio 4");

    return (EXIT_SUCCESS);
}
