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
    IplImage* ImgEj = cvCreateImage(cvSize(Img->width, Img->height), IPL_DEPTH_8U, 4);

    int fila, cc;
    for (fila = 0; fila < Img->height; fila++) {

        __m128i *pImg = (__m128i *) (Img->imageData + fila * Img-> widthStep);
        __m128i *pImgEj = (__m128i *) (ImgEj->imageData + fila * ImgEj->widthStep);

        for (cc = 0; cc < Img->widthStep; cc = cc + 16) {

            *pImgEj++ = *pImg;
            pImg++;
        }
    }

    // Componentes RG (segundo cuadrante)
    __m128i rg = _mm_set1_epi32(0xFFFF00);
    // Componentes RG (tercer cuadrante)
    __m128i gb = _mm_set1_epi32(0x00FFFF);
    // Componentes RG (cuarto cuadrante)
    __m128i rb = _mm_set1_epi32(0xFF00FF);
    // white
    __m128i w = _mm_set1_epi32(0x00);

    for (fila = 0; fila < Img->height; fila++) {
        __m128i *pImg = (__m128i *) (Img->imageData + fila * Img-> widthStep);
        __m128i *pImgEj = (__m128i *) (ImgEj->imageData + fila * ImgEj->widthStep);

        for (cc = 0; cc < Img->widthStep; cc = cc + 16) {
            if (cc < 1601 && fila > 200) {
                __m128i PuertaW = _mm_and_si128(w, *pImgEj);
                _mm_storeu_si128(pImgEj, PuertaW);
            }
            if (cc > 1600 && fila < 200) {
                __m128i PuertaRG = _mm_and_si128(rg, *pImgEj);
                _mm_storeu_si128(pImgEj, PuertaRG);
            }

            if (cc > 1600 && fila > 200) {
                __m128i PuertaGB = _mm_and_si128(gb, *pImgEj);
                _mm_storeu_si128(pImgEj, PuertaGB);
            }
            if (cc > 1600 && fila > 400) {
                __m128i PuertaRB = _mm_and_si128(rb, *pImgEj);
                _mm_storeu_si128(pImgEj, PuertaRB);
            }
            pImgEj++;
        }
    }

    cvNamedWindow("Ejericio 1", CV_WINDOW_AUTOSIZE);
    cvShowImage("Ejericio 1", ImgEj);
    cvWaitKey(0);

    // memory release for images before exiting the application
    cvReleaseImage(&ImgEj);

    // Self-explanatory
    cvDestroyWindow("Ejericio 1");

    return (EXIT_SUCCESS);
}
