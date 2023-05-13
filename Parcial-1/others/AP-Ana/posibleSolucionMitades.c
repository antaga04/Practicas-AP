#include <stdio.h>
#include <stdlib.h>

#include <opencv/cv.h>
#include <opencv/highgui.h>

int main(int argc, char **argv) {

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
    cvNamedWindow(argv[1],  CV_WINDOW_AUTOSIZE);
    // Img is shown in 'image' window
    cvShowImage(argv[1], Img);
    cvWaitKey(0);

    // Crea la imagen para la componete azul
    IplImage* ImgMitad = cvCreateImage(cvSize(Img->width, Img->height), IPL_DEPTH_8U, 3);

    int fila, columna, pos;

    for (fila = 0; fila < Img->height; fila++) {

        unsigned char *pImg = (unsigned char *) Img->imageData + fila * Img->widthStep;
        unsigned char *pImgMitad = (unsigned char *) ImgMitad->imageData + fila * ImgMitad->widthStep;

        *pImg = (Img -> height)/2;
        pos = (Img -> height)/2;

        for (columna = 0; columna < Img->width; columna++) {
            *pImgMitad++ = *pImg++;
            pos++;
            if (pos > Img->height){
                pos = 0;
                *pImg = (unsigned char *) Img->imageData + fila * Img->widthStep;
            }
        }
    }

    // crea y muestras las ventanas con las im genes
    cvNamedWindow("Componente Mitad", CV_WINDOW_AUTOSIZE);
    cvShowImage("Componente Mitad", ImgMitadE);

    cvWaitKey(0);

    // memory release for images before exiting the application
    cvReleaseImage(&Img);
    cvReleaseImage(&ImgMitad);

    // Self-explanatory
    cvDestroyWindow(argv[1]);
    cvDestroyWindow("Componente Mitad");

    return EXIT_SUCCESS;
}
