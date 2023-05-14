#include <stdio.h>
#include <stdlib.h>

#include <opencv/cv.h>
#include <opencv/highgui.h>

#include <Windows.h>

void copiarBloqueSSE(int i, int j, IplImage* Img1, int k, int l, IplImage* Img2, int alto, int ancho) {
    int fila, cc;
    for (fila = 0; fila < alto; fila++) {
        __m128i *pImg1 = (__m128i *) (Img1->imageData + (fila + i) * Img1->widthStep + j * Img1->nChannels);
        __m128i *pImg2 = (__m128i *) (Img2->imageData + (fila + k) * Img2->widthStep + l * Img2->nChannels);
        for (cc = 0; cc < ancho * Img1->nChannels; cc += 16) {
            *pImg2++ = *pImg1++;
        }
    }
}

int compararBloqueSSE(int img1f, int img1c, IplImage* img1, int img2f, int img2c, IplImage* img2, int alto, int ancho) {
    __m128i resultadoProvisional, resultadoProvisionalParteAlta, sadCC16;
    //inicializa resultadoProvisional a cero
    resultadoProvisional = _mm_xor_si128(resultadoProvisional, resultadoProvisional);
    int fila, cc;

    for (fila = 0; fila < alto; fila++) {
        __m128i* pImg1 = (__m128i*) (img1->imageData + (img1f + fila) * img1->widthStep + img1c * img1->nChannels);
        __m128i* pImg2 = (__m128i*) (img2->imageData + (img2f + fila) * img2->widthStep + img2c * img2->nChannels);

        for (cc = 0; cc < ancho * 3; cc += 16) {
            sadCC16 = _mm_sad_epu8(*pImg1++, *pImg2++);
            resultadoProvisional = _mm_add_epi32(resultadoProvisional, sadCC16);
        }
    }

    resultadoProvisionalParteAlta = _mm_srli_si128(resultadoProvisional, 8);
    resultadoProvisional = _mm_add_epi32(resultadoProvisionalParteAlta, resultadoProvisional);
    return _mm_cvtsi128_si32(resultadoProvisional);
}

int diferenciaBloqueSIMD(int i, int j, IplImage* Img1, int k, int l, IplImage *Img2, int alto, int ancho) {
    int diferencia = 0;
    int fila, cc;
    __m128i A, B, C, D, E;

    for (fila = 0; fila < alto; fila++) {

        __m128i *pImg1 = (__m128i *) (Img1->imageData + (fila + i) * Img1->widthStep + j * Img1->nChannels);
        __m128i *pImg2 = (__m128i *) (Img2->imageData + (fila + k) * Img2->widthStep + l * Img2->nChannels);

        for (cc = 0; cc < ancho * Img1->nChannels; cc += 16) {
            A = _mm_load_si128(pImg1++); // carga los bytes de la imagen 1 en A
            B = _mm_load_si128(pImg2++); // carga los bytes de la imagen

            C = _mm_sad_epu8(A, B); // hace la suma y deja dos datos colocados en r1 y r3 (vacio dato vacio dato)
            D = _mm_srli_si128(C, 8); // en D se guarda en la esquina de la derecha el dato C
            E = _mm_add_epi32(C, D); // se suman el dato D y C y se convierte a entero de 32 bits
            diferencia += _mm_cvtsi128_si32(E); // convertir a int
        }
    }

    return (diferencia);
}

int main(int argc, char **argv) {

    if (argc != 3) {
        printf("Error: Usage %s image_file_name\n", argv[0]);
        return EXIT_FAILURE;
    }

    IplImage* ImgB = cvLoadImage(argv[1], CV_LOAD_IMAGE_UNCHANGED);

    if (!ImgB) {
        printf("Error: file %s not found\n", argv[1]);
        return EXIT_FAILURE;
    }

    IplImage* ImgC = cvLoadImage(argv[2], CV_LOAD_IMAGE_UNCHANGED);

    if (!ImgC) {
        printf("Error: file %s not found\n", argv[1]);
        return EXIT_FAILURE;
    }
    
    IplImage* transicion = cvCreateImage(cvSize(ImgB->width, ImgB->height), ImgB->depth, ImgB->nChannels);
    
/*
    cvShowImage("Imagen Origen", ImgB);
    cvShowImage("Imagen Destino", ImgC);
    cvShowImage("Imagen Black", transicion);
    cvWaitKey(0);
*/

    int fila1, fila2, columna1, columna2;
    int alto = 16, ancho = 32;
    int mejorFila, mejorColumna;
    int tupla = 2;
    
    for (fila1 = 0; fila1 < ImgC->height; fila1 += alto) {
        
        for (columna1 = 0; columna1 < ImgC->width; columna1 += ancho) {

            int diferencia = INT_MAX;
            int diferenciaProvisional = INT_MAX;
            int filaProvisional, columnaProvisional;

            for (fila2 = 0; fila2 < ImgB->height; fila2 += alto) {
                for (columna2 = 0; columna2 < ImgB->width; columna2 += ancho) {

                    diferencia = compararBloqueSSE(fila1, columna1, ImgB, fila2, columna2, ImgC, alto, ancho);

                    if (diferencia < diferenciaProvisional) {
                        diferenciaProvisional = diferencia;
                        mejorFila = fila2;
                        mejorColumna = columna2;
                    }
                }
            }
            if (tupla % 2 == 0) {
                copiarBloqueSSE(mejorFila, mejorColumna, ImgC, fila1, columna1, transicion, alto, ancho);
            }else {
                copiarBloqueSSE(mejorFila, mejorColumna, ImgC, fila1, columna1  + ancho/2, transicion, alto, ancho);
            }
        }
        tupla++;
        cvShowImage("Imagen Destino", transicion);
        cvWaitKey(20);
    }
    // memory release for images before exiting the application
    cvReleaseImage(&ImgB);
    cvReleaseImage(&ImgC);
    cvWaitKey(0);
    // Self-explanatory
    cvDestroyWindow("Imagen Origen");
    cvDestroyWindow("Imagen Destino");

    return EXIT_SUCCESS;
}
