#include <stdio.h>
#include <stdlib.h>

#include <opencv/cv.h>
#include <opencv/highgui.h>

int main(int argc, char **argv) {

    if (argc != 2) {
        printf("Error: Usage %s image_file_name\n", argv[0]);
        return EXIT_FAILURE;
    }

    IplImage* Img = cvLoadImage(argv[1], CV_LOAD_IMAGE_UNCHANGED);

    if (!Img) {
        printf("Error: file %s not found\n", argv[1]);
        return EXIT_FAILURE;
    }

    cvNamedWindow(argv[1], CV_WINDOW_AUTOSIZE);
    cvShowImage(argv[1], Img);
    cvWaitKey(0);

    //COPIA LA IMAGEN TAL CUAL 
    IplImage* ImgFade = cvCloneImage(Img);


    int fila, columna, pasos;

    for (pasos = 0; pasos < 256; pasos++) {

        for (fila = 0; fila < Img->height; fila++) {

            unsigned char *pImgFade = (unsigned char *) ImgFade->imageData + fila * ImgFade->widthStep;

            for (columna = 0; columna < Img->width; columna++) {

                if (*pImgFade >= 0) {
                    *pImgFade = *pImgFade - 1;
                } else {
                    *pImgFade = 0;
                }

                pImgFade++;


                if (*pImgFade >= 0) {
                    *pImgFade = *pImgFade - 1;
                } else {
                    *pImgFade = 0;
                }
                pImgFade++;


                if (*pImgFade >= 0) {
                    *pImgFade = *pImgFade - 1;
                } else {
                    *pImgFade = 0;
                }
                pImgFade++;

            }

        }

        cvNamedWindow("Fade", CV_WINDOW_AUTOSIZE);
        cvShowImage("Fade", ImgFade);
        cvWaitKey(10);
    }


    cvReleaseImage(&Img);
    cvReleaseImage(&ImgFade);


    cvDestroyWindow(argv[1]);
    cvDestroyWindow("Componente Fade");

    return EXIT_SUCCESS;
}
