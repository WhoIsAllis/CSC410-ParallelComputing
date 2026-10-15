#define _POSIX_C_SOURCE 200809L

#include <errno.h>
#include <limits.h>
#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

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

typedef struct {
    const unsigned char *input;
    unsigned char *output;
    int width;
    int start_row;  // First output row owned by this thread
    int end_row;    // Exclusive end of this thread's row range
} thread_data_t;

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

static void convolve_sobel_rows(const unsigned char *input, unsigned char *output, int width, int start_row, int end_row)
{
    const int channels = 3;

    for (int i = start_row; i < end_row; i++) 
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

static void *sobel_worker(void *arg)
{
    thread_data_t *data = (thread_data_t *)arg;

    convolve_sobel_rows(data->input, data->output, data->width, data->start_row, data->end_row);
    return NULL;
}

static double monotonic_time(void)
{
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    return (double)now.tv_sec + (double)now.tv_nsec / 1e9;
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
    // calloc keeps the one-pixel border black. This matches the sequential program
    unsigned char *output_image = (unsigned char *)calloc(output_size, sizeof(unsigned char));
    pthread_t *threads = (pthread_t *)malloc((size_t)thread_count * sizeof(pthread_t));
    thread_data_t *thread_data =
        (thread_data_t *)malloc((size_t)thread_count * sizeof(thread_data_t));

    if (output_image == NULL || threads == NULL || thread_data == NULL) 
    {
        fprintf(stderr, "Error! Could not allocate memory!\n");
        stbi_image_free(input_image);
        free(output_image);
        free(threads);
        free(thread_data);
        return EXIT_FAILURE;
    }

    int interior_rows = height > 2 ? height - 2 : 0;
    int rows_per_thread = interior_rows / thread_count;
    int remainder = interior_rows % thread_count;
    int created_threads = 0;
    int create_error = 0;

    printf("Loading color image: %s\n", argv[1]);
    printf("Running Sobel operator with %d pthread(s)\n", thread_count);

    // Include pthread creation and joining in the parallel work
    double start_time = monotonic_time();

    for (int i = 0; i < thread_count; i++) 
    {
        // Give an extra row to each of the first remainder threads
        int extra_row = i < remainder ? 1 : 0;
        int rows_before = i * rows_per_thread + (i < remainder ? i : remainder);

        thread_data[i].input = input_image;
        thread_data[i].output = output_image;
        thread_data[i].width = width;
        thread_data[i].start_row = 1 + rows_before;
        thread_data[i].end_row = thread_data[i].start_row + rows_per_thread + extra_row;

        int result = pthread_create(&threads[i], NULL, sobel_worker, &thread_data[i]);
        if (result != 0) 
        {
            fprintf(stderr, "Error! Could not create thread %d: %s\n", i, strerror(result));
            create_error = 1;
            break;
        }
        created_threads++;
    }

    for (int i = 0; i < created_threads; i++) 
    {
        int result = pthread_join(threads[i], NULL);
        if (result != 0) 
        {
            fprintf(stderr, "Error! Could not join thread %d: %s\n", i, strerror(result));
            create_error = 1;
        }
    }

    double elapsed_time = monotonic_time() - start_time;

    if (create_error) 
    {
        stbi_image_free(input_image);
        free(output_image);
        free(threads);
        free(thread_data);
        return EXIT_FAILURE;
    }

    printf("Time taken with pthreads: %.6f seconds\n", elapsed_time);

    const char *output_path = "output_sobel_pthread.bmp";
    if (!stbi_write_bmp(output_path, width, height, 3, output_image)) {
        fprintf(stderr, "Error! Could not write image!\n");
        stbi_image_free(input_image);
        free(output_image);
        free(threads);
        free(thread_data);
        return EXIT_FAILURE;
    }

    printf("Saved edge visual to '%s'\n", output_path);

    stbi_image_free(input_image);
    free(output_image);
    free(threads);
    free(thread_data);
    return EXIT_SUCCESS;
}
