#include "parser/Parser.h"
#include "engine/Scene.h"
#include <iostream>
#include "engine/RayTracer.h"
#include "stb/stb_image_write.h"
#include "engine/Sphere.h"
#include "engine/Plane.h"
#include <filesystem>

// default: no debug info
bool debug = false;
bool FFT_MODE = false;
const int MAX_RECURSION_DEPTH = 5;
const int SCREEN_PIXEL_WIDTH = 1000;
const int SCREEN_PIXEL_HEIGHT = 1000;

double getGrayScale(double r, double g, double b)
{
    return 0.299 * r + 0.587 * g + 0.114 * b;
}

std::vector<double> convertToGrayscale(const std::vector<unsigned char> &image, int width, int height)
{
    std::vector<double> grayImage(width * height); // RGBA

    for (int i = 0; i < width * height; ++i)
    {
        // Extract RGB components (from RGBA) to compute grayscale [0-1]
        double r = image[i * 4] / 255.0;
        double g = image[i * 4 + 1] / 255.0;
        double b = image[i * 4 + 2] / 255.0;

        double gray = getGrayScale(r, g, b);

        grayImage[i] = gray;
    }

    return grayImage;
}

void printParsingDebugInfo(const Scene &scene)
{
    printf("Scene loaded successfully with %zu objects and %zu lights.\n",
           scene.objs.size(), scene.lights.size());
    printf("Camera position: (%.2f, %.2f, %.2f)\n",
           scene.cam.getPosition().x,
           scene.cam.getPosition().y,
           scene.cam.getPosition().z);
    printf("Ambient light intensity: (%.2f, %.2f, %.2f)\n",
           scene.ambientLight.x,
           scene.ambientLight.y,
           scene.ambientLight.z);
    for (size_t i = 0; i < scene.objs.size(); ++i)
    {
        const auto &obj = scene.objs[i];
        if (!obj)
        {
            printf("Object %zu is NULL!\n", i + 1);
            continue;
        }
        if (dynamic_cast<Plane *>(obj.get()))
        {
            printf("Object %zu is a Plane.\n", i + 1);
            printf("Plane normal: (%.2f, %.2f, %.2f)\n", dynamic_cast<Plane *>(obj.get())->normal.x,
                   dynamic_cast<Plane *>(obj.get())->normal.y,
                   dynamic_cast<Plane *>(obj.get())->normal.z);
            printf("Plane d constant: %.2f\n", dynamic_cast<Plane *>(obj.get())->d);
        }
        else if (dynamic_cast<Sphere *>(obj.get()))
        {
            printf("Object %zu is a Sphere. Radius of %.2f\nCenter of the sphere is at (%.2f, %.2f, %.2f)\n", i + 1, dynamic_cast<Sphere *>(obj.get())->radius,
                   dynamic_cast<Sphere *>(obj.get())->center.x,
                   dynamic_cast<Sphere *>(obj.get())->center.y,
                   dynamic_cast<Sphere *>(obj.get())->center.z);
        }
        else
        {
            printf("Object %zu is of unknown type.\n", i + 1);
        }
        printf("Object %zu color: (%.2f, %.2f, %.2f)\n",
               i + 1,
               obj->color.x,
               obj->color.y,
               obj->color.z);
        printf("Reflective constant: %.2f\n", obj->reflectiveConst);
        printf("Refractive constant: %.2f\n", obj->refractiveConst);
        printf("Shininess: %.2f\n", obj->shininess);
    }
}

int main(int argc, char *argv[])
{
    std::string filePath;
    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " <scene_file>" << std::endl;
        return 1;
    }
    if (argc >= 2 && std::string(argv[1]) == "-fft")
    {
        FFT_MODE = true;
        std::cout << "FFT mode enabled." << std::endl;
        // Resolve scene path robustly relative to the executable and common locations
        std::filesystem::path relPath("sceneFiles/fft_scene.txt");
        std::filesystem::path exeDir = std::filesystem::absolute(argv[0]).parent_path();
        std::filesystem::path candidate1 = exeDir / relPath;                          // e.g. if running from bin/
        std::filesystem::path candidate2 = exeDir.parent_path() / relPath;            // e.g. if running from bin/ where exe is in bin/
        std::filesystem::path candidate3 = std::filesystem::current_path() / relPath; // current working dir

        if (std::filesystem::exists(candidate1))
            filePath = candidate1.string();
        else if (std::filesystem::exists(candidate2))
            filePath = candidate2.string();
        else if (std::filesystem::exists(candidate3))
            filePath = candidate3.string();
        else
        {
            // Fallback to a simple relative path; user can adjust as needed
            filePath = relPath.string();
        }
        std::cout << "Using scene file: " << filePath << std::endl;
    }
    else
    {
        filePath = argv[1];
    }
    if (argc >= 3 && std::string(argv[2]) == "-d")
    {
        debug = true;
    }
    try
    {
        auto scene = buildFromFile(filePath);
        if (debug)
        {
            printParsingDebugInfo(*scene.get());
        }
        // build ray tracer
        RayTracer rayTracer(*scene.get(), SCREEN_PIXEL_WIDTH, SCREEN_PIXEL_HEIGHT, MAX_RECURSION_DEPTH);
        // render image
        std::vector<unsigned char> image = rayTracer.renderImage();

        // save the rendered image as PNG
        std::string outputFilename = "output.png";
        stbi_flip_vertically_on_write(1); // Flip the image vertically for correct orientation
        int success = stbi_write_png(outputFilename.c_str(),
                                     SCREEN_PIXEL_WIDTH,
                                     SCREEN_PIXEL_HEIGHT,
                                     4, // RGBA components
                                     image.data(),
                                     SCREEN_PIXEL_WIDTH * 4); // stride

        if (success)
        {
            std::cout << "Image saved successfully to " << outputFilename << std::endl;

            if (FFT_MODE)
            {
                std::cout << "FFT mode: Performing additional processing on output.png..." << std::endl;
                // Convert to grayscale
                std::vector<double> grayImage = convertToGrayscale(image, SCREEN_PIXEL_WIDTH, SCREEN_PIXEL_HEIGHT);
                // to be continued: perform FFT on grayImage using FFTW3
            }
        }
        else
        {
            std::cerr << "Failed to save image to " << outputFilename << std::endl;
            return 1;
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
