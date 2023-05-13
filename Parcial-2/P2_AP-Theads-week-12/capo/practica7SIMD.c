#include <stdio.h>
#include <stdlib.h>

#include <emmintrin.h>

#include <opencv/cv.h>
#include <opencv/highgui.h>

void copiarBloque(int filaOrigen, int ccOrigen, IplImage* ImgOrigen, int filaDestino,
        int ccDestino, IplImage *ImgDestino, int alto, int ancho) {

    int fila, cc;

    for (fila = 0; fila < alto; fila += alto) {

        __m128i *pImgOrigen = (__m128i *) (ImgOrigen->imageData + (filaOrigen + fila) * ImgOrigen->widthStep + ccOrigen * ImgOrigen->nChannels);
        __m128i *pImgDestino = (__m128i *) (ImgDestino->imageData + (filaDestino + fila) * ImgDestino->widthStep + ccDestino * ImgDestino->nChannels);
        
        //printf("fila copia = %d\n", fila);

        for (cc = 0; cc < ancho; cc += 16) {
            
            //printf("cc copia = %d\n", cc);

            *pImgDestino++ = *pImgOrigen++; //Pixel entero

        }
    }
}

//Mía

int compararBloque(int filaOrigen, int colOrigen, IplImage* ImgOrigen, int filaDestino,
        int colDestino, IplImage *ImgDestino, int alto, int ancho) {
    int fila, cc;
    int sad;

    for (fila = 0; fila < alto; fila++) {

        __m128i *pImgOrigen = (__m128i *) (ImgOrigen->imageData + (filaOrigen + fila) * ImgOrigen->widthStep + colOrigen * ImgOrigen->nChannels);
        __m128i *pImgDestino = (__m128i *) (ImgDestino->imageData + (filaDestino + fila) * ImgDestino->widthStep + colDestino * ImgDestino->nChannels);

        __m128i A, B, C, D, E;
        
        //printf("fila comp = %d\n", fila);

        for (cc = 0; cc < ancho; cc += 16) {
            
            //printf("cc comp = %d\n", cc);

            A = _mm_load_si128(pImgOrigen);
            B = _mm_load_si128(pImgDestino);
            C = _mm_sad_epu8(A, B);
            D = _mm_srli_si128(C, 8);
            E = _mm_add_epi32(C, D);
            sad = _mm_cvtsi128_si32(E);
            
            //printf("El valor de sad es: %d\n", sad);
            
            //if(fila == 15) exit(1);

            pImgOrigen++;
            pImgDestino++;
            
            //printf("sad = %d\n", sad);

        }
    }

    return (sad);
}

int main(int argc, char **argv) {

    if (argc != 3) {
        printf("Error: Usage %s image_file_name\n", argv[0]);
        return EXIT_FAILURE;
    }

    //CV_LOAD_IMAGE_COLOR = 1 forces the resultant IplImage to be colour.
    //CV_LOAD_IMAGE_GRAYSCALE = 0 forces a greyscale IplImage.
    //CV_LOAD_IMAGE_UNCHANGED = -1

    IplImage* ImgB = cvLoadImage(argv[1], CV_LOAD_IMAGE_UNCHANGED);

    // Always check if the program can find the image file
    if (!ImgB) {
        printf("Error: file %s not found\n", argv[1]);
        return EXIT_FAILURE;
    }

    // a visualization window is created with title: image file name
    cvNamedWindow("Imagen Origen", CV_WINDOW_AUTOSIZE);
    // Img is shown in 'image' window
    cvShowImage("Imagen Origen", ImgB);
    cvWaitKey(0);

    IplImage* ImgC = cvLoadImage(argv[2], CV_LOAD_IMAGE_UNCHANGED);

    // Always check if the program can find the image file
    if (!ImgC) {
        printf("Error: file %s not found\n", argv[1]);
        return EXIT_FAILURE;
    }

    // a visualization window is created with title: image file name
    cvNamedWindow("Imagen Destino", CV_WINDOW_AUTOSIZE);
    // Img is shown in 'image' window
    cvShowImage("Imagen Destino", ImgC);
    cvWaitKey(0);

    //IplImage* ImgTransicion = cvCreateImage(cvSize(ImgB->width, ImgB->height), ImgB->depth, ImgB->nChannels);

    int fila1, fila2, cc1, cc2,sad;
    int alto = 16;
    int ancho = 16;
    int mejorFila, mejorColumna;
    int pixel = INT_MAX;
    int bloque = 0;

    for (fila1 = 0; fila1 < ImgB->height; fila1 += alto) {
        
        //printf("La fila 1 es: %d\n", fila1);

        for (cc1 = 0; cc1 < ImgB->widthStep; cc1 += ancho) {
            
            //printf("La col 1 es: %d\n", cc1);

            for (fila2 = 0; fila2 < ImgC->height; fila2 += alto) {
                
                //printf("La fila 2 es: %d\n", fila2);

                for (cc2 = 0; cc2 < ImgC->widthStep; cc2 += ancho) {
                    
                    //printf("La col 2 es: %d\n", cc2);
                    
                    sad = compararBloque(fila1, cc1, ImgB, fila2, cc2, ImgC, alto, ancho);
                    
                    if (sad < pixel) {
                        pixel = sad;
                        mejorFila = fila2;
                        mejorColumna = cc2;
                    }
                    
                }
            }
            bloque++;
            printf("Bloque a copiar %d\n", bloque);

            copiarBloque(mejorFila, mejorColumna, ImgC, fila1, cc1, ImgB, alto, ancho);
            
            pixel = INT_MAX;
        }

        //cvShowImage("Mosaico", ImgB);
        //cvWaitKey(10);

    }

    cvShowImage("Mosaico", ImgB);
    cvWaitKey(0);

    // memory release for images before exiting the application
    cvReleaseImage(&ImgB);
    cvReleaseImage(&ImgC);

    // Self-explanatory
    cvDestroyWindow("Imagen Origen");
    cvDestroyWindow("Imagen Destino");

    return EXIT_SUCCESS;
}