#include <stdio.h>
#include <stdlib.h>

#include <opencv/cv.h>
#include <opencv/highgui.h>


void copiarBloqueSSE(int filaOriginal, int colOriginal, IplImage * imgOrigen, int filaDestino, int colDestino, IplImage *imgDestino, int alto, int ancho) {
  
    int fila, columna;
    for (fila = 0; fila < alto; fila++) {
        __m128i *pImgOrigen = (__m128i *) (imgOrigen->imageData + (fila + filaOriginal) * imgOrigen->widthStep + colOriginal * imgOrigen->nChannels);
        __m128i *pImgDestino = (__m128i *) (imgDestino->imageData + (fila + filaDestino) * imgDestino->widthStep + colDestino * imgDestino->nChannels);
        for (columna = 0; columna < ancho * 6; columna+= 16) {
            *pImgDestino++ = *pImgOrigen++;
        }
    }
}

int compararBloqueSSE(IplImage* img1, int img1f, int img1c, IplImage* img2, int img2f, int img2c, int alto, int ancho) {
    __m128i resultadoProvisional, resultadoProvisionalParteAlta, sadCC16;

    //inicializa resultadoProvisional a cero
    resultadoProvisional = _mm_xor_si128(resultadoProvisional, resultadoProvisional);
    
    int fila, cc;

    for (fila = 0; fila < alto; fila++) {

        __m128i* pImg1 = (__m128i*) (img1->imageData + (img1f + fila) * img1->widthStep + img1c * img1->nChannels);
        __m128i* pImg2 = (__m128i*) (img2->imageData + (img2f + fila) * img2->widthStep + img2c * img2->nChannels);
        
        

        for (cc = 0; cc < ancho * 6; cc += 16) {

            sadCC16 = _mm_sad_epu8(*pImg1++, *pImg2++);
            resultadoProvisional = _mm_add_epi32(resultadoProvisional, sadCC16);

        }
    }

    resultadoProvisionalParteAlta = _mm_srli_si128(resultadoProvisional, 8);
    resultadoProvisional = _mm_add_epi32(resultadoProvisionalParteAlta, resultadoProvisional);
    return _mm_cvtsi128_si32(resultadoProvisional);


}

int main(int argc, char **argv) {

    if (argc < 2) {
        printf("Error: Usage %s image_file_name\n", argv[0]);
        return EXIT_FAILURE;
    }

    IplImage* Img1 = cvLoadImage(argv[1], CV_LOAD_IMAGE_COLOR);
    
    if (!Img1) {
        printf("Error: file %s not found\n", argv[1]);
        return EXIT_FAILURE;
    }

    IplImage* Img2 = cvLoadImage(argv[2], CV_LOAD_IMAGE_UNCHANGED);
    
    if (!Img2) {
        printf("Error: fichero %s no leido\n", argv[2]);
        return EXIT_FAILURE;
    }


    cvShowImage(argv[2], Img2);
    cvShowImage(argv[1], Img1);
    cvWaitKey(0);

    int alto = 16, ancho = 32, filaBloque = 0;
   

    int fila1, fila2, columna1, columna2;
    for (fila1 = 0; fila1 < Img1->height; fila1 = fila1 + alto) {
        for (columna1 = 0; columna1 < Img1->width; columna1 = columna1 + ancho) {
            int diferencia = INT_MAX;
            int nuevaDiferencia = diferencia;
            int mejorFila, mejorColumna;

            for (fila2 = 0; fila2 < Img2->height; fila2 = fila2 + alto) {
                for (columna2 = 0; columna2 < Img2->width; columna2 = columna2 + ancho) {
                    nuevaDiferencia = compararBloqueSSE(Img1, fila1, columna1, Img2, fila2, columna2, alto, ancho);
                    if (nuevaDiferencia < diferencia) {
                        diferencia = nuevaDiferencia;
                        mejorFila = fila2;
                        mejorColumna = columna2;
                    }
                }
            }
            
            if (filaBloque % 2 == 0){
                copiarBloqueSSE(mejorFila, mejorColumna, Img2, fila1, (columna1 +16)*Img1->nChannels, Img1, alto, ancho);
            }
            else{
                copiarBloqueSSE(mejorFila, mejorColumna, Img2, fila1, columna1, Img1, alto, ancho);
            }
            
            cvShowImage(argv[1], Img1);
            cvWaitKey(1);
        }
        filaBloque++;
    }

    cvShowImage(argv[1], Img1);
    cvShowImage(argv[2], Img2);
    cvWaitKey(0);

    return EXIT_SUCCESS;
}
