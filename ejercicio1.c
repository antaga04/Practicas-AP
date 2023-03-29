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
    IplImage* ImgEj1 = cvCreateImage(cvSize(Img->width, Img->height), IPL_DEPTH_8U, 4);
  
    // Componentes RG (segundo cuadrante)
    __m128i rg = _mm_set1_epi32(0xFFFF00);
    // Componentes RG (tercer cuadrante)
    __m128i gb = _mm_set1_epi32(0x00FFFF);
    // Componentes RG (cuarto cuadrante)
    __m128i rb = _mm_set1_epi32(0xFF00FF);
  
    int fila, cc;
    for (fila = 0; fila < Img->height; fila++) {
        
        __m128i *pImgEj4 = (__m128i *) (ImgEj4->imageData + fila * ImgEj4->widthStep);
        
        for (cc = 0; cc < Img->widthStep; cc = cc + 16) {
          if(cc < 800 && fila < 800){
            // Primer cuadrante RGB
            *pImgEj4++ = *pImg;
          }else if(cc >800 && fila < 800){
            __m128i PuertaRG = _mm_and_si128(rg, *pImgEj4);
            _mm_storeu_si128(pImgEj4, PuertaRG);
          }
          else if(cc > 800 && fila > 800 && fila < 1300){
            __m128i PuertaRG = _mm_and_si128(rg, *pImgEj4);
            _mm_storeu_si128(pImgEj4, PuertaRG);
          }
          else if(cc > 800 && fila > 1300){
            __m128i PuertaRG = _mm_and_si128(rg, *pImgEj4);
            _mm_storeu_si128(pImgEj4, PuertaRG);
          }
            pImg++;
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
