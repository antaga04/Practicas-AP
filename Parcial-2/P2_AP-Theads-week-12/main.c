#include <stdio.h>
#include <stdlib.h>

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <time.h>

#include <pthread.h> 
#define NTHREADS 12

// Definición variables globales
#define ALTO 16  //BLOQUE
#define ANCHO 16 //BLOQUE
IplImage* Img1;
IplImage* Img2;
IplImage* Img3;

void copiarBloqueSSE(int i, int j, IplImage* Img1, int k, int l, IplImage* Img2);
void copiarBloque(int filaOrigen, int colOrigen, IplImage* ImgOrigen, int filaDestino,
        int colDestino, IplImage *ImgDestino, int alto, int ancho);
int diferenciaBloqueSSE2(int filaOrigen, int colOrigen, IplImage* ImgOrigen, int
        filaDestino, int colDestino, IplImage *ImgDestino, int alto, int ancho);
int diferenciaBloque(int filaOrigen, int colOrigen, IplImage* ImgOrigen, int
        filaDestino, int colDestino, IplImage *ImgDestino, int alto, int ancho);
void mosaico_thread(void *ptr);

int main(int argc, char **argv) {
    if (argc != 3) {
        printf("Error: Usage %s image_file_name\n", argv[0]);
        return EXIT_FAILURE;
    }

    Img1 = cvLoadImage(argv[1], CV_LOAD_IMAGE_UNCHANGED);

    if (!Img1) {
        printf("Error: file %s not found\n", argv[1]);
        return EXIT_FAILURE;
    }

    Img2 = cvLoadImage(argv[2], CV_LOAD_IMAGE_UNCHANGED);

    if (!Img2) {
        printf("Error: file %s not found\n", argv[2]);
        return EXIT_FAILURE;
    }

    Img3 = cvCreateImage(cvSize(Img1->width, Img1->height), IPL_DEPTH_8U,
            Img1->nChannels);

    //Controlador time
    struct timespec start, finish;
    float elapsed;
    clock_gettime(CLOCK_MONOTONIC, &start);

    pthread_t threads[NTHREADS];
    int filas[NTHREADS];
    int i;
    for (i = 0; i < NTHREADS; i++) {
        filas[i] = i;
        printf("\nEl thread %d sustituye las filas %d", i, filas[i]);
        pthread_create(&threads[i], NULL, (void *) &mosaico_thread, (void *) &filas
                [i]);
    }
    for (i = 0; i < NTHREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    //Controlador time
    clock_gettime(CLOCK_MONOTONIC, &finish);
    elapsed = (finish.tv_sec - start.tv_sec);
    elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
    printf("\nTiempo transcurrido: %f", elapsed);

    cvShowImage("Imagen Destino", Img3);
    cvWaitKey(0);

    cvDestroyAllWindows();
    cvReleaseImage(&Img1);
    cvReleaseImage(&Img2);

    return EXIT_SUCCESS;
}

void mosaico_thread(void *ptr) {
    //ptr apunta a un entero que indica el mutiplo de la fila a sustituir
    int *p = (int *) ptr;
    int fila1, columna1, fila2, columna2;
    int filaProvisional, columnaProvisional, diferencia;

    for (fila1 = (*p) * Img1->height / NTHREADS; fila1 < (*p) * Img1->height /
            NTHREADS + Img1->height / NTHREADS; fila1 += ALTO) {
        for (columna1 = 0; columna1 < Img1->width; columna1 += ANCHO) {

            int minDiff = INT_MAX;

            for (fila2 = 0; fila2 < Img2->height; fila2 += ALTO) {
                for (columna2 = 0; columna2 < Img2->width; columna2 += ALTO) {

                    int diff = diferenciaBloque(fila1, columna1, Img1, fila2,
                            columna2, Img2, ALTO, ANCHO);

                    if (diff < minDiff) {
                        minDiff = diff;
                        filaProvisional = fila2;
                        columnaProvisional = columna2;
                    }
                }
            }
            copiarBloque(filaProvisional, columnaProvisional, Img2, fila1, columna1,
                    Img3, ALTO, ANCHO);
        }
    }
}

void copiarBloqueSSE(int i, int j, IplImage* Img1, int k, int l, IplImage* Img2) {
    int fila, cc;
    for (fila = 0; fila < ALTO; fila++) {
        __m128i *pImg1 = (__m128i *) (Img1->imageData + (fila + i) * Img1->widthStep +
                j * Img1->nChannels);
        __m128i *pImg2 = (__m128i *) (Img2->imageData + (fila + k) * Img2->widthStep +
                l * Img2->nChannels);
        for (cc = 0; cc < ANCHO * Img1->nChannels; cc += 16) {
            *pImg1++ = *pImg2++;
        }
    }
}

int diferenciaBloqueSSE(int i, int j, IplImage* Img1, int k, int l, IplImage *Img2) {
    int diferencia = 0;
    int fila, cc;
    __m128i A, B, C, D, E;

    for (fila = 0; fila < ALTO; fila++) {

        __m128i *pImg1 = (__m128i *) (Img1->imageData + (fila + i) * Img1->widthStep +
                j * Img1->nChannels);
        __m128i *pImg2 = (__m128i *) (Img2->imageData + (fila + k) * Img2->widthStep +
                l * Img2->nChannels);

        for (cc = 0; cc < ANCHO * Img1->nChannels; cc += 16) {
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

int diferenciaBloque(int filaOrigen, int colOrigen, IplImage* ImgOrigen, int
        filaDestino, int colDestino, IplImage *ImgDestino, int alto, int ancho) {
    int diferencia = 0;
    int fila, columna;

    for (fila = 0; fila < alto; fila++) {

        unsigned char *pImgOrigen = (unsigned char *) ImgOrigen->imageData +
                (filaOrigen + fila) * ImgOrigen->widthStep + colOrigen * ImgOrigen->nChannels;
        unsigned char *pImgDestino = (unsigned char *) ImgDestino->imageData +
                (filaDestino + fila) * ImgDestino->widthStep + colDestino * ImgDestino->nChannels;

        for (columna = 0; columna < ancho; columna++) {
            diferencia += abs(*pImgOrigen++ - *pImgDestino++);
            diferencia += abs(*pImgOrigen++ - *pImgDestino++);
            diferencia += abs(*pImgOrigen++ - *pImgDestino++);
        }
    }

    return (diferencia);
}

void copiarBloque(int filaOrigen, int colOrigen, IplImage* ImgOrigen, int filaDestino,
        int colDestino, IplImage *ImgDestino, int alto, int ancho) {
    int fila, columna;

    for (fila = 0; fila < alto; fila++) {

        unsigned char *pImgOrigen = (unsigned char *) ImgOrigen->imageData +
                (filaOrigen + fila) * ImgOrigen->widthStep + colOrigen * ImgOrigen->nChannels;
        unsigned char *pImgDestino = (unsigned char *) ImgDestino->imageData +
                (filaDestino + fila) * ImgDestino->widthStep + colDestino * ImgDestino->nChannels;

        for (columna = 0; columna < ancho; columna++) {

            *pImgDestino++ = *pImgOrigen++; //Blue Channel
            *pImgDestino++ = *pImgOrigen++; //Green Channel
            *pImgDestino++ = *pImgOrigen++; //Red Channel

        }
    }
}
