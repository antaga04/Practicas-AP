#include <stdio.h>
#include <stdlib.h>
#include <emmintrin.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>
int main(int argc, char** argv) {

    if (argc != 3) {
        printf("Usage: %s image_file_name\n", argv[0]);
        return EXIT_FAILURE;
    }

    //CV_LOAD_IMAGE_COLOR = 1 forces the resultant IplImage to be colour.
    //CV_LOAD_IMAGE_GRAYSCALE = 0 forces a greyscale IplImage.
    //CV_LOAD_IMAGE_UNCHANGED = -1
    IplImage* Img1 = cvLoadImage(argv[1], CV_LOAD_IMAGE_COLOR);
    IplImage* Img2 = cvLoadImage(argv[2], CV_LOAD_IMAGE_COLOR);
    
    // Always check if the program can find a file
    if (!Img1) {
        printf("Error: fichero %s no leido\n", argv[1]);
        return EXIT_FAILURE;
    }
     if (!Img2) {
        printf("Error: fichero %s no leido\n", argv[2]);
        return EXIT_FAILURE;
    }
    // a visualization window is created with title 'image'
    cvNamedWindow("color", CV_WINDOW_AUTOSIZE);

    // img is shown in 'image' window
    cvShowImage("color", Img1);
   // cvShowImage("color", Img2);
    cvWaitKey(0);
    
  
    __m128i UNOS = _mm_set1_epi8 (1); // FUNDIDO A NEGRO
    
    int pixel,fila,columna;
    for ( pixel = 0; pixel < 256; pixel++) {
        for ( fila = 0; fila < Img1->height; fila++) {

            __m128i *pImg1 = (__m128i *) (Img1->imageData + fila * Img1->widthStep);
            for ( columna = 0; columna < Img1->widthStep; columna=columna+16) {
                __m128i A = _mm_loadu_si128(pImg1);
                __m128i B = _mm_subs_epu8(A,UNOS);
                _mm_storeu_si128(pImg1,B);
                //*pImg1=_mm_subs_epu8 (*pImg1, UNOS);  //FUNDIDO A NEGRO
                pImg1++;
            }
        }

        cvShowImage("color", Img1);
        cvWaitKey(1);
    }
    
     __m128i reg_temp;
    
    
    for ( pixel = 0; pixel < 256; pixel++) {
        for ( fila = 0; fila < Img1->height; fila++) {

            __m128i *pImg1 = (__m128i *) (Img1->imageData + fila * Img1->widthStep);
            __m128i *pImg2 = (__m128i *) (Img2->imageData + fila * Img1->widthStep);
            for ( columna = 0; columna < Img1->widthStep; columna=columna+16) {
                __m128i A = _mm_loadu_si128(pImg1);
                __m128i B = _mm_loadu_si128(pImg2);
                __m128i reg_temp = _mm_adds_epu8(A, UNOS);
                reg_temp = _mm_min_epu8(reg_temp, B);
                _mm_storeu_si128(pImg1,reg_temp);
                pImg1++;
                pImg2++;
                
            }
        }

        cvShowImage("color", Img1);
        cvWaitKey(1);
    }
   
    cvWaitKey(0);
    // memory release for img before exiting the application
    cvReleaseImage(&Img1);

    // Self-explanatory
    cvDestroyWindow(argv[1]);

    return EXIT_SUCCESS;

}
