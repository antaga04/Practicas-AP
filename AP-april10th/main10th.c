#include <stdio.h>
#include <stdlib.h>

#include <opencv/cv.h>
#include <opencv/highgui.h>

void copiarBloque(int filaOrigen, int colOrigen, IplImage* ImgOrigen, int filaDestino,
int colDestino, IplImage *ImgDestino, int alto, int ancho){
    
    int fila, columna;
    
    for (fila = 0; fila < alto; fila++) {

        unsigned char *pImgOrigen = (unsigned char *) ImgOrigen->imageData + (filaOrigen+fila) * ImgOrigen->widthStep + colOrigen * ImgOrigen->nChannels;
        unsigned char *pImgDestino = (unsigned char *) ImgDestino->imageData + (filaDestino+fila) * ImgDestino->widthStep + colDestino * ImgDestino->nChannels;

        for (columna = 0; columna < ancho; columna++) {
            
            *pImgDestino++ = *pImgOrigen++; //Blue Channel
            *pImgDestino++ = *pImgOrigen++; //Green Channel
            *pImgDestino++ = *pImgOrigen++; //Red Channel
            
        }
    }
}

/*
void compararBloque(int filaOrigen, int colOrigen, IplImage* ImgOrigen, int filaDestino,
int colDestino, IplImage *ImgDestino, int alto, int ancho){
    int fila, columna;
    
    for (fila = 0; fila < alto; fila++) {

        unsigned char *pImgOrigen = (unsigned char *) ImgOrigen->imageData + (filaOrigen+fila) * ImgOrigen->widthStep + colOrigen * ImgOrigen->nChannels;
        unsigned char *pImgDestino = (unsigned char *) ImgDestino->imageData + (filaDestino+fila) * ImgDestino->widthStep + colDestino * ImgDestino->nChannels;

        for (columna = 0; columna < ancho; columna++) {
            
            *pImgDestino++ = *pImgOrigen++; //Blue Channel
            *pImgDestino++ = *pImgOrigen++; //Green Channel
            *pImgDestino++ = *pImgOrigen++; //Red Channel
            
        }
    }
}
*/

int main(int argc, char **argv) {

    if (argc != 3) {
        printf("Error: Usage %s image_file_name\n", argv[0]);
        return EXIT_FAILURE;
    }

    //CV_LOAD_IMAGE_COLOR = 1 forces the resultant IplImage to be colour.
    //CV_LOAD_IMAGE_GRAYSCALE = 0 forces a greyscale IplImage.
    //CV_LOAD_IMAGE_UNCHANGED = -1
    
    //IplImage* ImgB = cvLoadImage(argv[1], CV_LOAD_IMAGE_UNCHANGED);
    IplImage* ImgB = GetOpenFileNameA("aves.jpg");

    // Always check if the program can find the image file
    if (!ImgB) {
        printf("Error: file %s not found\n", argv[1]);
        return EXIT_FAILURE;
    }

    // a visualization window is created with title: image file name
    cvNamedWindow("Imagen Origen",  CV_WINDOW_AUTOSIZE);
    // Img is shown in 'image' window
    cvShowImage("Imagen Origen", ImgB);
    cvWaitKey(0);
    
    IplImage* ImgC = cvLoadImage(argv[2], CV_LOAD_IMAGE_UNCHANGED);

    // Always check if the program can find the image file
    if (!ImgC) {
        printf("Error: file %s not found\n", argv[1]);
        return EXIT_FAILURE;
    }

    // a visualization window is created with title: image file name
    cvNamedWindow("Imagen Destino",  CV_WINDOW_AUTOSIZE);
    // Img is shown in 'image' window
    cvShowImage("Imagen Destino", ImgC);
    cvWaitKey(0);
    
    //IplImage* ImgTransicion = cvCreateImage(cvSize(ImgB->width, ImgB->height), ImgB->depth, ImgB->nChannels);
    
    int fila, columna;
    
    for (fila = 0; fila < ImgB->height; fila+=64) {
        
        for (columna = 0; columna < ImgB->width; columna+=64) {
            
            copiarBloque(fila,columna,ImgB,fila,columna,ImgC,64,64);
        }
        
        cvShowImage("Imagen Destino", ImgC);
        cvWaitKey(100);
    }


    // memory release for images before exiting the application
    cvReleaseImage(&ImgB);
    cvReleaseImage(&ImgC);

    // Self-explanatory
    cvDestroyWindow("Imagen Origen");
    cvDestroyWindow("Imagen Destino");

    return EXIT_SUCCESS;
}
