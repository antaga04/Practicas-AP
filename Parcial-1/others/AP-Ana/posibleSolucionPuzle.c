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
    
    IplImage* imagenPuzle = cvLoadImage(argv[1], CV_LOAD_IMAGE_UNCHANGED);
    if(!imagenPuzle){
        printf("Error: fichero no leido", argv[1]);
        return EXIT_FAILURE;
    }
    
    IplImage* imagenPuzleFloat = cvCreateImage(cvSize(imagenPuzle->width, imagenPuzle ->height), IPL_DEPTH_32F, 4);
    //cvConvertScale(imagenPuzle, imagenInicialFloat, 1.0 / 255.0, 0);
    //cvShowImage(argv[1], imagenPuzleFloat);
    //cvWaitKey(0);

    IplImage* imagenMascaraFloat = cvCreateImage(cvSize(imagenPuzle->width, imagenPuzle ->height), IPL_DEPTH_32F, 4);
    
    
    IplImage* imagenFinal = cvLoadImage(argv[2], CV_LOAD_IMAGE_UNCHANGED);
    if(!imagenFinal){
        printf("Error: fichero no leido", argv[2]);
        return EXIT_FAILURE;
    }
    
    IplImage* imagenFinalFloat = cvCreateImage(cvSize(imagenFinal->width, imagenFinal ->height), IPL_DEPTH_32F, 4);
    //cvConvertScale(imagenFinal, imagenFinalFloat, 1.0 / 255.0, 0);
    //cvShowImage(argv[2], imagenFinalFloat);
    //cvWaitKey(0);
    
    IplImage* imagenTransicionFloat = cvCreateImage(cvSize(imagenPuzleFloat->width, imagenPuzleFloat ->height), IPL_DEPTH_32F, 4);
    //cvShowImage("imagenTransicionFloat", imagenTransicionFloat);
    //cvWaitKey(0);
    
    
    int paso;
    
    int iteraciones = 100;
    
    int fil, cc;
    
        
    //float pasoFloat = ((float) paso) / (float) (iteraciones);
    //__m128 pasoSSE = _mm_set_ps1(pasoFloat);
    //__m128 pasoMenosUnoSSE = _mm_set_ps1(1.0 - pasoFloat);

    //Bucle para crear la mascara
    for (fil = 0; fil < imagenInicialFloat->height; fil++) {
        __m128 *pimagenPuzleFloat = (__m128 *) (imagenPuzleFloat->imageData + fil * imagenPuzleFloat->widthStep);
        __m128 *pimagenMascaraFloat = (__m128 *) (imagenMascaraFloat->imageData + fil * imagenMascaraFloat->widthStep);
        __m128 *pimagenFinalFloat = (__m128*) (imagenFinalFloat->imageData + fil * imagenFinalFloat->widthStep);
        __m128 *pimagenTransicionFloat = (__m128 *) (imagenTransicionFloat->imageData + fil * imagenTransicionFloat->widthStep);

        for ( cc = 0; cc < imagenInicialFloat->widthStep; cc = cc + 16){
            for(paso = 0; paso < 3; paso++){
                *pImagenPuzle++;
            }
            for(paso = 0; paso < 3; paso++){
                *pImagenMascaraFloat = *pImagenPuzle;
                pImagenMascaraFloat++;
            }
            pImagenPuzle++;
        }
    }

    //Bucle para hacer las operaciones de unir imagenes
    for (fil = 0; fil < imagenInicialFloat->height; fil++) {
        *pimagenPuzleFloat = (__m128 *) (imagenPuzleFloat->imageData + fil * imagenPuzleFloat->widthStep);
        *pimagenMascaraFloat = (__m128 *) (imagenMascaraFloat->imageData + fil * imagenMascaraFloat->widthStep);
        *pimagenFinalFloat = (__m128*) (imagenFinalFloat->imageData + fil * imagenFinalFloat->widthStep);
        *pimagenTransicionFloat = (__m128 *) (imagenTransicionFloat->imageData + fil * imagenTransicionFloat->widthStep);            
        for ( cc = 0; cc < imagenInicialFloat->widthStep; cc = cc + 16){
            __m128 ccInicial, ccFinal, ccTransicion, ccMascara, alfa;

            ccInicial = *pimagenInicialFloat++;
            ccMascara = *pimagenMascaraFloat++;
            ccInicial = _mm_mul_ps(ccInicial, ccMascara);

            ccFinal = *pimagenFinalFloat++;
            ccMascaraMenos = _mm_subs_ps(255,ccMascara);
            alfa = ccMascaraMenos*1/255;
            ccFinal = _mm_mul_ps(ccFinal, alfa);

            ccTransicion = _mm_add_ps(ccInicial, ccFinal);

            *pimagenTransicionFloat++ = ccTransicion;
        }
    }
    
    // crea y muestras las ventanas con las im genes
    cvNamedWindow("Componente Puzle", CV_WINDOW_AUTOSIZE);
    cvShowImage("Componente Puzle", ImgPuzle);
    
    cvNamedWindow("Componente Imagen", CV_WINDOW_AUTOSIZE);
    cvShowImage("Componente Imagen", ImgFinal);
    
    cvNamedWindow("Componente Transicion", CV_WINDOW_AUTOSIZE);
    cvShowImage("Componente Transicion", ImgTransicionFloat);

    cvWaitKey(0);

    // memory release for images before exiting the application
    cvReleaseImage(&ImgPuzle);
    cvReleaseImage(&ImgFinal);
    cvReleaseImage(&ImgTransicion);

    // Self-explanatory
    cvDestroyWindow("Componente Puzle");
    cvDestroyWindow("Componente Imagen");
    cvDestroyWindow("Componente Transicion");
    
    return (EXIT_SUCCESS);
}
