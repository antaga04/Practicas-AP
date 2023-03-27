#include <stdio.h>
#include <stdlib.h>

#include <opencv/cv.h>
#include <opencv/highgui.h>

int main(int argc, char **argv)
{

  if (argc != 2)
  {
    printf("Error: Usage %s image_file_name\n", argv[0]);
    return EXIT_FAILURE;
  }

  IplImage *Img = cvLoadImage(argv[1], CV_LOAD_IMAGE_UNCHANGED);

  if (!Img)
  {
    printf("Error: file %s not found\n", argv[1]);
    return EXIT_FAILURE;
  }

  cvNamedWindow(argv[1], CV_WINDOW_AUTOSIZE);
  cvShowImage(argv[1], Img);
  cvWaitKey(0);

  IplImage *ImgFinal = cvCreateImage(cvSize(Img->width, Img->height + 200), Img->depth, Img->nChannels);
  memset(ImgFinal->imageData, 0x00FF0000, ImgFinal->imageSize);

  int fila, columna, pasos;

  for (fila = 100; fila < Img->height; fila++)
  {

    unsigned char *pImg = (unsigned char *)Img->imageData + fila * Img->widthStep;
    unsigned char *pImgFinal = (unsigned char *)ImgFinal->imageData + fila * ImgFinal->widthStep;

    for (columna = 0; columna < Img->width; columna++)
    {

      if (*pImgFinal < *pImg)
      {
        (*pImgFinal)++;
      }
      pImgFinal++;
      pImg++;

      if (*pImgFinal < *pImg)
      {
        *pImgFinal = *pImgFinal + 1;
      }
      pImgFinal++;
      pImg++;

      if (*pImgFinal < *pImg)
      {
        *pImgFinal = *pImgFinal + 1;
      }
      pImgFinal++;
      pImg++;
    }

    cvNamedWindow("Fade", CV_WINDOW_AUTOSIZE);
    cvShowImage("Fade", ImgFinal);
    cvWaitKey(10);
  }

  cvReleaseImage(&Img);
  cvReleaseImage(&ImgFinal);

  cvDestroyWindow(argv[1]);
  cvDestroyWindow("Componente Fade");

  return EXIT_SUCCESS;
}
