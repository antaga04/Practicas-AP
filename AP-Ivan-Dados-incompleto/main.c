#include <stdio.h>
#include <stdlib.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>

int main(int argc, char** argv) {
    if (argc < 2) {
        printf("Usage: %s image_file_name\n", argv[0]);
        return EXIT_FAILURE;
    }
    //CV_LOAD_IMAGE_COLOR = 1 forces the resultant IplImage to be colour.
    //CV_LOAD_IMAGE_GRAYSCALE = 0 forces a greyscale IplImage.
    //CV_LOAD_IMAGE_UNCHANGED = -1
    IplImage* ImgOriginal = cvLoadImage(argv[1], CV_LOAD_IMAGE_UNCHANGED);
    IplImage* ImgResR = cvCloneImage(ImgOriginal);
    IplImage* ImgResG = cvCloneImage(ImgOriginal);
    IplImage* ImgResB = cvCloneImage(ImgOriginal);
    int fila, columna;
    for (fila = 0; fila < ImgResR->height; fila++) {
        __m128i *pImgResR = (__m128i *) (ImgResR->imageData + fila * ImgResR->widthStep);
        __m128i *pImgResG = (__m128i *) (ImgResG->imageData + fila * ImgResG->widthStep);
        __m128i *pImgResB = (__m128i *) (ImgResB->imageData + fila * ImgResB->widthStep);
        for (columna = 0; columna < ImgResR->widthStep; columna += 16) {
            __m128i registroR = *pImgResR;
            __m128i registroG = *pImgResG;
            __m128i registroB = *pImgResB;
            registroR = _mm_and_si128(registroR, _mm_set1_epi32(0xFF00FFFF));
            registroR = _mm_or_si128(registroR, _mm_set1_epi32(0x00800000));
            registroG = _mm_and_si128(registroG, _mm_set1_epi32(0xFFFF00FF));
            registroG = _mm_or_si128(registroG, _mm_set1_epi32(0x00008000));
            registroB = _mm_and_si128(registroB, _mm_set1_epi32(0xFFFFFF00));
            registroB = _mm_or_si128(registroB, _mm_set1_epi32(0x00000080));
            *pImgResR++ = registroR;
            *pImgResG++ = registroG;
            *pImgResB++ = registroB;
        }
    }
    cvNamedWindow("IMG RED", CV_WINDOW_AUTOSIZE);
    cvShowImage("IMG RED", ImgResR);
    cvNamedWindow("IMG BLUE", CV_WINDOW_AUTOSIZE);
    cvShowImage("IMG BLUE", ImgResB);
    cvNamedWindow("IMG GREEN", CV_WINDOW_AUTOSIZE);
    cvShowImage("IMG GREEN", ImgResG);
    
    
    cvWaitKey(0);
    cvReleaseImage(&ImgResR);
    cvReleaseImage(&ImgResG);
    cvReleaseImage(&ImgResB);
    cvDestroyAllWindows();
    return EXIT_SUCCESS;
}