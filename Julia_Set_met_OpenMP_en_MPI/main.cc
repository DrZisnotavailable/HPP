#include <iostream>
#include <vector>
#include <complex>
#include <cmath>
#include <string>
#include <sstream>
#include <iomanip>
#include <omp.h>
#include <mpi.h>
#include <filesystem>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

const int WIDTH = 800;
const int HEIGHT = 800;
const int MAX_ITER = 500;
const int TOTAL_FRAMES = 120;

const double M_PI = 3.14159265358979323846;

struct Pixel {
    unsigned char r, g, b;
};

Pixel getColor(int iteration) {
    Pixel p;
    if (iteration == MAX_ITER) {
        p = {0, 0, 0};
    } else {
        int c = 255 * iteration / MAX_ITER;
        p = {static_cast<unsigned char>(c), static_cast<unsigned char>(c / 2), static_cast<unsigned char>(255 - c)};
    }
    return p;
}

Pixel computeJuliaPixel(int x, int y, int frameIndex) {
    double zx = 1.5 * (x - WIDTH / 2) / (0.5 * WIDTH);
    double zy = (y - HEIGHT / 2) / (0.5 * HEIGHT);

    double cx = -0.7 + 0.1 * std::cos(frameIndex * 2.0 * M_PI / TOTAL_FRAMES);
    double cy = 0.27015 + 0.1 * std::sin(frameIndex * 2.0 * M_PI / TOTAL_FRAMES);

    int iter = 0;
    while (zx * zx + zy * zy < 4.0 && iter < MAX_ITER) {
        double tmp = zx * zx - zy * zy + cx;
        zy = 2.0 * zx * zy + cy;
        zx = tmp;
        iter++;
    }

    return getColor(iter);
}

void renderFrame(int frameIndex) {
    std::vector<Pixel> image(WIDTH * HEIGHT);
    
    #pragma omp parallel for collapse(2)
    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            image[y * WIDTH + x] = computeJuliaPixel(x, y, frameIndex);
        }
    }

    // === aangepaste opslagstructuur ===
    std::stringstream pathBuilder;
    int numThreads = omp_get_max_threads();
    pathBuilder << "frames_" << numThreads;

    std::filesystem::create_directories(pathBuilder.str());

    std::stringstream filename;
    filename << pathBuilder.str() << "/frame_" << std::setw(3) << std::setfill('0') << frameIndex << ".png";
    std::string outputPath = filename.str();

    stbi_write_png(outputPath.c_str(), WIDTH, HEIGHT, 3, image.data(), WIDTH * 3);
}

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int numThreads = omp_get_max_threads();
    int framesPerProcess = TOTAL_FRAMES / size;
    int start = rank * framesPerProcess;
    int end = (rank == size - 1) ? TOTAL_FRAMES : start + framesPerProcess;

    // Start tijdsmeting
    auto t1 = std::chrono::high_resolution_clock::now();

    for (int i = start; i < end; ++i) {
        renderFrame(i);
    }

    // Stop tijdsmeting
    auto t2 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = t2 - t1;

    MPI_Barrier(MPI_COMM_WORLD); // Synchroniseer vóór print
    std::cout.flush(); // Zorg dat output direct komt
    std::cout << "Rank " << rank << " with " << numThreads
            << " threads finished in " << elapsed.count() << " seconds." << std::endl;


    MPI_Finalize();
    return 0;
}
