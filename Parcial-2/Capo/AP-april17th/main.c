#include <stdio.h>
#include <stdlib.h>

#include <opencv/cv.h>
#include <opencv/highgui.h>

#include <Windows.h>

void copiarBloque(int filaOrigen, int colOrigen, IplImage* ImgOrigen, int filaDestino,
        int colDestino, IplImage *ImgDestino, int alto, int ancho) {

    int fila, columna;

    for (fila = 0; fila < alto; fila++) {

        unsigned char *pImgOrigen = (unsigned char *) ImgOrigen->imageData + (filaOrigen + fila) * ImgOrigen->widthStep + colOrigen * ImgOrigen->nChannels;
        unsigned char *pImgDestino = (unsigned char *) ImgDestino->imageData + (filaDestino + fila) * ImgDestino->widthStep + colDestino * ImgDestino->nChannels;

        for (columna = 0; columna < ancho; columna++) {
            *pImgDestino++ = *pImgOrigen++; // Blue Channel
            *pImgDestino++ = *pImgOrigen++; // Green Channel
            *pImgDestino++ = *pImgOrigen++; // Red Channel
        }
    }
}

//Funciona

int compararBloque(int filaOrigen, int colOrigen, IplImage* ImgOrigen, int filaDestino,
        int colDestino, IplImage *ImgDestino, int alto, int ancho) {
    int fila, columna;
    int diferencia = 0;

    for (fila = 0; fila < alto; fila++) {

        unsigned char *pImgOrigen = (unsigned char *) ImgOrigen->imageData + (filaOrigen + fila) * ImgOrigen->widthStep + colOrigen * ImgOrigen->nChannels;
        unsigned char *pImgDestino = (unsigned char *) ImgDestino->imageData + (filaDestino + fila) * ImgDestino->widthStep + colDestino * ImgDestino->nChannels;

        for (columna = 0; columna < ancho; columna++) {
            diferencia += abs(*pImgOrigen++ - *pImgDestino++);
            diferencia += abs(*pImgOrigen++ - *pImgDestino++);
            diferencia += abs(*pImgOrigen++ - *pImgDestino++);
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

    int fila1, fila2, columna1, columna2;
    int alto = 16;
    int ancho = 16;
    int mejorFila, mejorColumna;

    for (fila1 = 0; fila1 < ImgC->height; fila1 += alto) {

        for (columna1 = 0; columna1 < ImgC->width; columna1 += ancho) {

            int diferencia;
            int diferenciaProvisional = INT_MAX;
            int filaProvisional, columnaProvisional;

            for (fila2 = 0; fila2 < ImgB->height; fila2 += alto) {
                for (columna2 = 0; columna2 < ImgB->width; columna2 += ancho) {

                    diferencia = compararBloque(fila1, columna1, ImgB, fila2, columna2, ImgC, alto, ancho);

                    if (diferencia < diferenciaProvisional) {
                        diferenciaProvisional = diferencia;
                        mejorFila = fila2;
                        mejorColumna = columna2;
                    }
                }
            }
            copiarBloque(mejorFila, mejorColumna, ImgC, fila1, columna1, ImgB, alto, ancho);
        }
        cvShowImage("Imagen Destino", ImgB);
        cvWaitKey(10);
    }
    // memory release for images before exiting the application
    cvReleaseImage(&ImgB);
    cvReleaseImage(&ImgC);

    // Self-explanatory
    cvDestroyWindow("Imagen Origen");
    cvDestroyWindow("Imagen Destino");

    return EXIT_SUCCESS;
}
