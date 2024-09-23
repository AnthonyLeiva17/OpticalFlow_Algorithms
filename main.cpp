#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/optflow.hpp>
#include "lucas-kanade.cpp"
#include "dense_optical_flow.cpp"
#include <sys/stat.h>
#include <chrono>

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{   
    //Variables para medir tiempos de ejecucion
    const int num_iterations = 200;
    double total_time_lucas_kanade = 0.0;
    double total_time_lucas_kanade_dense = 0.0;
    double total_time_farneback = 0.0;

    const string keys =
            "{ h help |      | print this help message }"
            "{ @video |  | path to image file }"
            "{ save | | save video frames }";
    CommandLineParser parser(argc, argv, keys);

    string filename = samples::findFile(parser.get<string>("@video"));
    if (!parser.check())
    {
        parser.printErrors();
        return 0;
    }

    
    bool save = false;
    if (parser.has("save")){
        save = true;
        mkdir("optical_flow_frames", 0777);
    }
    bool to_gray = true;

     for (int i = 0; i < num_iterations; ++i) {
        std::cout << "Iteración: " << i+1 << "\n";
        // Medir tiempo para Lucas-Kanade
        auto start_lk = std::chrono::high_resolution_clock::now();
        
        std::cout << "Running method: lucas_kanade..." << std::endl;
        lucas_kanade(filename, save);
        
        auto end_lk = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> iteration_time_lk = end_lk - start_lk;
        total_time_lucas_kanade += iteration_time_lk.count();

        // Medir tiempo para Lucas-Kanade Dense
        auto start_lkd = std::chrono::high_resolution_clock::now();
        
        std::cout << "Running method: lucaskanade_dense..." << std::endl;
        dense_optical_flow(filename, save, optflow::calcOpticalFlowSparseToDense, to_gray, 8, 128, 0.05f, true, 500.0f, 1.5f);
        
        auto end_lkd = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> iteration_time_lkd = end_lkd - start_lkd;
        total_time_lucas_kanade_dense += iteration_time_lkd.count();

        // Medir tiempo para Farneback
        auto start_fb = std::chrono::high_resolution_clock::now();
        
        std::cout << "Running method: Farneback..." << std::endl;
        dense_optical_flow(filename, save, calcOpticalFlowFarneback, to_gray, 0.5, 3, 15, 3, 5, 1.2, 0);
        
        auto end_fb = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> iteration_time_fb = end_fb - start_fb;
        total_time_farneback += iteration_time_fb.count();
    }

    // Calcular el tiempo promedio por iteración para cada método
    double average_time_lucas_kanade = total_time_lucas_kanade / num_iterations;
    double average_time_lucas_kanade_dense = total_time_lucas_kanade_dense / num_iterations;
    double average_time_farneback = total_time_farneback / num_iterations;

    // Mostrar los resultados
    std::cout << "Tiempo promedio por iteración para Lucas-Kanade: " << average_time_lucas_kanade << " segundos" << std::endl;
    std::cout << "Tiempo promedio por iteración para Lucas-Kanade Dense: " << average_time_lucas_kanade_dense << " segundos" << std::endl;
    std::cout << "Tiempo promedio por iteración para Farneback: " << average_time_farneback << " segundos" << std::endl;

    return 0;

}