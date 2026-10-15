#include <errno.h>
#include <limits.h>
#include <math.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

static const int Gx[3][3] = {
    {-1, 0, 1},
    {-2, 0, 2},
    {-1, 0, 1}
};

static const int Gy[3][3] = {
    {-1, -2, -1},
    { 0,  0,  0},
    { 1,  2,  1}
};

static int parse_thread_count(const char *text, int *thread_count)
{
    char *end = NULL;
    long value;

    errno = 0;
    value = strtol(text, &end, 10);

    if (errno != 0 || end == text || *end != '\0' || value < 1 || value > INT_MAX) 
    {
        return 0;
    }

    *thread_count = (int)value;
    return 1;
}

static void convolve_sobel_openmp(const unsigned char *input, unsigned char *output, int width, int height, int thread_count)
{
    const int channels = 3;

    // Every pixel is independent. Flatten the row/column loops for work sharing
    #pragma omp parallel for collapse(2) schedule(static) num_threads(thread_count)
    for (int i = 1; i < height - 1; i++) 
    {
        for (int j = 1; j < width - 1; j++) 
        {
            for (int c = 0; c < channels; c++) 
            {
                float sum_x = 0.0f;
                float sum_y = 0.0f;

                for (int ki = 0; ki < 3; ki++) 
                {
                    for (int kj = 0; kj < 3; kj++) 
                    {
                        int pixel_row = i + ki - 1;
                        int pixel_col = j + kj - 1;
                        int pixel_idx = (pixel_row * width + pixel_col) * channels + c;
                        unsigned char pixel_val = input[pixel_idx];

                        sum_x += (float)pixel_val * Gx[ki][kj];
                        sum_y += (float)pixel_val * Gy[ki][kj];
                    }
                }

                float magnitude = sqrtf(sum_x * sum_x + sum_y * sum_y) * 2.0f;

                if (magnitude > 255.0f) 
                {
                    magnitude = 255.0f;
                }

                int output_idx = (i * width + j) * channels + c;
                output[output_idx] = (unsigned char)magnitude;
            }
        }
    }
}

int main(int argc, char *argv[])
{
    if (argc != 3) 
    {
        fprintf(stderr, "Usage: %s <input_image_path> <num_threads>\n", argv[0]);
        return EXIT_FAILURE;
    }

    int thread_count;
    if (!parse_thread_count(argv[2], &thread_count)) 
    {
        fprintf(stderr, "Error! num_threads must be a positive integer!\n");
        return EXIT_FAILURE;
    }

    int width, height, actual_channels;
    unsigned char *input_image = stbi_load(argv[1], &width, &height, &actual_channels, 3);

    if (input_image == NULL) 
    {
        fprintf(stderr, "Error! Could not load image file '%s'!\n", argv[1]);
        return EXIT_FAILURE;
    }

    size_t output_size = (size_t)width * (size_t)height * 3u;
    // calloc keeps the one-pixel border black, matching the sequential program.
    unsigned char *output_image = (unsigned char *)calloc(output_size, sizeof(unsigned char));

    if (output_image == NULL) 
    {
        fprintf(stderr, "Error! Could not allocate output image memory!\n");
        stbi_image_free(input_image);
        return EXIT_FAILURE;
    }

    printf("Loading color image: %s\n", argv[1]);
    printf("Running Sobel operator with %d OpenMP thread(s) = \n", thread_count);

    double start_time = omp_get_wtime();
    convolve_sobel_openmp(input_image, output_image, width, height, thread_count);
    double elapsed_time = omp_get_wtime() - start_time;

    printf("Time taken with OpenMP: %.6f seconds\n", elapsed_time);

    const char *output_path = "output_sobel_openmp.bmp";
    if (!stbi_write_bmp(output_path, width, height, 3, output_image)) 
    {
        fprintf(stderr, "Error! Could not write image!\n");
        stbi_image_free(input_image);
        free(output_image);
        return EXIT_FAILURE;
    }

    printf("Saved visual to '%s'\n", output_path);

    stbi_image_free(input_image);
    free(output_image);
    return EXIT_SUCCESS;
}
