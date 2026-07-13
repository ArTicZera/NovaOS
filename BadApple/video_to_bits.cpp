#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <stdint.h>

typedef unsigned char BYTE;

#define WIDTH  160
#define HEIGHT 120

#define FRAME_SIZE (WIDTH * HEIGHT / 8)

int main()
{
    cv::VideoCapture cap("bad_apple.mp4");

    if (!cap.isOpened()) {
        printf("Erro ao abrir video\n");
        return 1;
    }

    int total_frames = (int)cap.get(cv::CAP_PROP_FRAME_COUNT);
    double fps = 15;//cap.get(cv::CAP_PROP_FPS);

    printf("Frames: %d | FPS: %.2f\n", total_frames, fps);
    printf("Frame size: %d bytes\n", FRAME_SIZE);

    FILE* out = fopen("badapple.bin", "wb");

    if (!out) {
        printf("Erro ao criar arquivo\n");
        return 1;
    }

    cv::Mat frame, gray, bw;
    int frame_count = 0;

    while (true)
    {
        if (!cap.read(frame))
            break;

        frame_count++;

        if (frame.empty()) {
            printf("Frame vazio em %d\n", frame_count);
            continue;
        }

        // Resize fixo
        cv::resize(frame, frame, cv::Size(WIDTH, HEIGHT), 0, 0, cv::INTER_AREA);

        // Grayscale
        cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);

        // Preto/branco
        cv::threshold(gray, bw, 127, 255, cv::THRESH_BINARY);

        // Buffer de saída do frame
        BYTE buffer[FRAME_SIZE];
        int index = 0;

        // Packing seguro (8 pixels → 1 byte)
        for (int y = 0; y < HEIGHT; y++)
        {
            for (int x = 0; x < WIDTH; x += 8)
            {
                BYTE byte = 0;

                for (int b = 0; b < 8; b++)
                {
                    BYTE pixel = bw.at<BYTE>(y, x + b);

                    if (pixel) // branco
                        byte |= (1 << (7 - b));
                }

                buffer[index++] = byte;
            }
        }

        // Segurança: garantir tamanho correto
        if (index != FRAME_SIZE) {
            printf("Erro: frame %d com tamanho errado (%d)\n", frame_count, index);
            break;
        }

        fwrite(buffer, 1, FRAME_SIZE, out);

        if (frame_count % 100 == 0)
            printf("Processado: %d frames\n", frame_count);
    }

    fclose(out);
    cap.release();

    printf("Finalizado! Frames processados: %d\n", frame_count);

    return 0;
}