#include <stdio.h>
#include <stdlib.h>

#include <opencv/cv.h>
#include <opencv/highgui.h>

#include <Windows.h>

void calcularMediaColor(IplImage* img, int fila, int col, int r, int g, int b) {
    r = g = b = 0;
    int total_pixeles = 0;
    int i, j;
    for (i = fila; i < fila + 16; i++) {
        for (j = col; j < col + 16; j++) {
            uchar* pixel = (uchar*) (img->imageData + i * img->widthStep + j * img->nChannels);
            r += *pixel++;
            g += *pixel++;
            b += *pixel++;
            total_pixeles++;
        }
    }
    r /= total_pixeles;
    g /= total_pixeles;
    b /= total_pixeles;

    for (i = fila; i < fila + 16; i++) {
        for (j = col; j < col + 16; j++) {
            uchar* pixel = (uchar*) (img->imageData + i * img->widthStep + j * img->nChannels);
            *pixel++ = r;
            *pixel++ = g;
            *pixel++ = b;
        }
    }
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

    cvShowImage("Imagen Origen", ImgB);
    cvWaitKey(0);

    int fila, columna, r, g, b;
    int alto = 16, ancho = 16;
    int mejorFila, mejorColumna;

    for (fila = 0; fila < ImgB->height; fila += alto) {
        uchar* pixel = (uchar*) (ImgB->imageData + fila * ImgB->widthStep);

        for (columna = 0; columna < ImgB->width; columna += ancho) {
            calcularMediaColor(ImgB, fila, columna, r, g, b);
        }
        cvShowImage("Imagen Destino", ImgB);
        cvWaitKey(15);
    }
    // memory release for images before exiting the application
    cvReleaseImage(&ImgB);
    cvWaitKey(0);
    // Self-explanatory
    cvDestroyWindow("Imagen Origen");
    cvDestroyWindow("Imagen Destino");

    return EXIT_SUCCESS;
}
