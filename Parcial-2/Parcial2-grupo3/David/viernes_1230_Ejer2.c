#include <stdio.h>
#include <stdlib.h>

#include <opencv/cv.h>
#include <opencv/highgui.h>

//Definimos el numero de threads
#define NTHREADS 2

//Definimos las imagenes globales para poder acceder desde la funcion
IplImage* imagenOrigen;
IplImage* imagenDestino;

//Creamos la funcion que sera llamada por cada thread
void media_thread(void *ptr){

    //Convertimos el puntero a un tipo entero para dividir el trabajo
    int *thread = (int *) ptr;
    
    int fila, cc;
    int filaOrigen = *thread * imagenOrigen->height/NTHREADS;
    __m128i media;

    for (fila = *thread * imagenDestino->height/NTHREADS; fila < *thread * imagenDestino->height/NTHREADS + imagenDestino->height/NTHREADS; fila++) {
        
        __m128i *pImagenOrigen = (__m128i *) (imagenOrigen->imageData + (filaOrigen++) * imagenOrigen->widthStep);        
        __m128i *pImagenOrigenFilaSiguiente = (__m128i *) (imagenOrigen->imageData + (filaOrigen++) * imagenOrigen->widthStep);
        __m128i *pImagenDestino = (__m128i *) (imagenDestino->imageData + fila * imagenDestino->widthStep);              
        
        for (cc = 0; cc < imagenDestino->widthStep; cc += 16) {           
            
            media = _mm_avg_epu8(*pImagenOrigen, *pImagenOrigenFilaSiguiente);            
            *pImagenDestino = media;
            
            pImagenOrigen++;
            pImagenDestino++;
            pImagenOrigenFilaSiguiente++;       
        }
    }
}

int main(int argc, char **argv) {

    //Comprobamos que se pasa el parametro de imagen
    if (argc != 2) {
        printf("Error: Usage %s image_file_name\n", argv[0]);
        return EXIT_FAILURE;
    }
    
    //Cargamos la imagen original
    imagenOrigen = cvLoadImage(argv[1], CV_LOAD_IMAGE_UNCHANGED);

    //Comprobamos que se ha cargado la imagen correctamente
    if (!imagenOrigen) {
        printf("Error: file %s not found\n", argv[1]);
        return EXIT_FAILURE;
    }

    //Creamos la imagen destino con la mitad de filas
    imagenDestino = cvCreateImage(cvSize(imagenOrigen->width, imagenOrigen->height/2), imagenOrigen->depth, imagenOrigen->nChannels);
    
    // Creamos las ventanas
    cvNamedWindow(argv[1],  CV_WINDOW_AUTOSIZE);
    cvNamedWindow("Resultado",  CV_WINDOW_AUTOSIZE);   
    
    //Definimos un vector que va a contener los threads
    pthread_t threads[NTHREADS];
    
    int filas[NTHREADS];    
    int i;
    
    //Lanzamos cada thread con la llamada a la funcion
    for (i = 0; i < NTHREADS; i++) {
        filas[i] = i;
        printf("\nEl thread %d sustituye las filas %d", i, filas[i]);
        pthread_create(&threads[i], NULL, (void *) &media_thread, (void *) &filas[i]);
    }
    
    //El hilo principal espera a que acaben todos los threads
    for (i = 0; i < NTHREADS; i++) {
        pthread_join(threads[i], NULL);
    }
    

    // Mostramos las imagenes
    cvShowImage(argv[1], imagenOrigen);
    cvShowImage("Resultado", imagenDestino);
    cvWaitKey(0);

    //Limpiamos memoria de las imagenes
    cvReleaseImage(&imagenOrigen);
    cvReleaseImage(&imagenDestino);
    
    //Destruimos las ventanas
    cvDestroyWindow(argv[1]);
    cvDestroyWindow("Resultado");

    return EXIT_SUCCESS;
}
