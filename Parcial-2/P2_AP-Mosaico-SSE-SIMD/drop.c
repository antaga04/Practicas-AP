#include <stdio.h>
#include <stdlib.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <emmintrin.h>

#define BLOCK_SIZE 32

int compararBloqueSSE(int img1f, int img1c, IplImage* img1, int img2f, int img2c, IplImage* img2, int alto, int ancho) {
    __m128i resultadoProvisional, resultadoProvisionalParteAlta, sadCC16;
    // Inicializa resultadoProvisional a cero
    resultadoProvisional = _mm_setzero_si128();
    int fila, cc;

    for (fila = 0; fila < alto; fila++) {
        __m128i* pImg1 = (__m128i*)(img1->imageData + (img1f + fila) * img1->widthStep + img1c * img1->nChannels);
        __m128i* pImg2 = (__m128i*)(img2->imageData + (img2f + fila) * img2->widthStep + img2c * img2->nChannels);

        for (cc = 0; cc < ancho * 3; cc += 16) {
            sadCC16 = _mm_sad_epu8(*pImg1++, *pImg2++);
            resultadoProvisional = _mm_add_epi32(resultadoProvisional, sadCC16);
        }
    }

    resultadoProvisionalParteAlta = _mm_srli_si128(resultadoProvisional, 8);
    resultadoProvisional = _mm_add_epi32(resultadoProvisionalParteAlta, resultadoProvisional);
    return _mm_cvtsi128_si32(resultadoProvisional);
}

int main(int argc, char** argv) {
    if (argc != 2) {
        printf("Error: Usage %s video_file_name\n", argv[0]);
        return EXIT_FAILURE;
    }

    CvCapture* capture = cvCaptureFromAVI(argv[1]);

    if (!capture) {
        printf("Error: file %s not found or cannot be read\n", argv[1]);
        return EXIT_FAILURE;
    }

    int frameCount = cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_COUNT);
    int fps = cvGetCaptureProperty(capture, CV_CAP_PROP_FPS);
    int width = cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH);
    int height = cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT);

    // Crear el video de salida con los mismos parámetros que el video de entrada
    CvVideoWriter* writer = cvCreateVideoWriter("estabilizado.avi", CV_FOURCC('X', 'V', 'I', 'D'), fps, cvSize(width, height), 1);

    IplImage* previousFrame = NULL;
    IplImage* currentFrame = NULL;
    IplImage* stabilizedFrame = NULL;

    while ((currentFrame = cvQueryFrame(capture)) != NULL) {
        if (previousFrame == NULL) {
            previousFrame = cvCloneImage(currentFrame);
            stabilizedFrame = cvCloneImage(currentFrame);
        } else {
            // Buscar el bloque central en el frame siguiente
            int rowOffset = (height - BLOCK_SIZE) / 2;
            int colOffset = (width - BLOCK_SIZE) / 2;
            int bestRowOffset = rowOffset;
            int bestColOffset = colOffset
            int bestMatch = compararBloqueSSE(rowOffset, colOffset, previousFrame, rowOffset, colOffset, currentFrame, BLOCK_SIZE, BLOCK_SIZE);

            // Buscar el bloque en las proximidades del frame siguiente
            for (int dr = -1; dr <= 1; dr++) {
                for (int dc = -1; dc <= 1; dc++) {
                    int newRowOffset = rowOffset + dr;
                    int newColOffset = colOffset + dc;
                    int match = compararBloqueSSE(newRowOffset, newColOffset, previousFrame, rowOffset, colOffset, currentFrame, BLOCK_SIZE, BLOCK_SIZE);

                    if (match < bestMatch) {
                        bestMatch = match;
                        bestRowOffset = newRowOffset;
                        bestColOffset = newColOffset;
                    }
                }
            }

            // Mover el frame actual para centrar el bloque
            cvSet(stabilizedFrame, cvScalar(0, 0, 0));  // Pintar píxeles negros en el frame estabilizado
            cvSetImageROI(currentFrame, cvRect(bestColOffset, bestRowOffset, width - 2 * bestColOffset, height - 2 * bestRowOffset));
            cvSetImageROI(stabilizedFrame, cvRect(bestColOffset, bestRowOffset, width - 2 * bestColOffset, height - 2 * bestRowOffset));
            cvCopy(currentFrame, stabilizedFrame);
            cvResetImageROI(currentFrame);
            cvResetImageROI(stabilizedFrame);

            // Escribir el frame estabilizado en el video de salida
            cvWriteFrame(writer, stabilizedFrame);

            // Mostrar el frame estabilizado (opcional, si no se incluye la opción --showoff)
            if (argc != 3 || strcmp(argv[2], "--showoff") != 0) {
                cvShowImage("Estabilizado", stabilizedFrame);
                cvWaitKey(1);
            }

            // Actualizar el frame anterior
            cvCopy(currentFrame, previousFrame);
        }
    }

    // Liberar memoria y recursos
    cvReleaseCapture(&capture);
    cvReleaseVideoWriter(&writer);
    cvReleaseImage(&previousFrame);
    cvReleaseImage(&currentFrame);
    cvReleaseImage(&stabilizedFrame);

    return EXIT_SUCCESS;
}
