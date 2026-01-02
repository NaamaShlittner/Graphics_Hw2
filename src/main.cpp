#include "parser/Parser.h"
#include "engine/Scene.h"
#include <iostream>
#include "engine/RayTracer.h"
#include "stb/stb_image_write.h"

// default: no debug info
bool debug = false;
const int MAX_RECURSION_DEPTH = 0;
const int SCREEN_PIXEL_WIDTH = 1000;
const int SCREEN_PIXEL_HEIGHT = 1000;

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
        if (dynamic_cast<Plane*>(obj.get()))
        {
            printf("Object %zu is a Plane.\n", i + 1);
            printf("Plane normal: (%.2f, %.2f, %.2f)\n", dynamic_cast<Plane*>(obj.get())->normal.x,
                   dynamic_cast<Plane*>(obj.get())->normal.y,
                   dynamic_cast<Plane*>(obj.get())->normal.z);
            printf("Plane d constant: %.2f\n", dynamic_cast<Plane*>(obj.get())->d);
        }
        else if (dynamic_cast<Sphere*>(obj.get()))
        {
            printf("Object %zu is a Sphere. Radius of %.2f\nCenter of the sphere is at (%.2f, %.2f, %.2f)\n", i + 1, dynamic_cast<Sphere*>(obj.get())->radius, 
                   dynamic_cast<Sphere*>(obj.get())->center.x,
                   dynamic_cast<Sphere*>(obj.get())->center.y,
                   dynamic_cast<Sphere*>(obj.get())->center.z);
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
    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " <scene_file>" << std::endl;
        return 1;
    }
    if (argc >= 3 && std::string(argv[2]) == "-d")
    {
        debug = true;
    }
    try
    {
        auto scene = buildFromFile(argv[1]);
        if (debug)
        {
            printParsingDebugInfo(*scene.get());
        }
        //build ray tracer
        RayTracer rayTracer(*scene.get(), SCREEN_PIXEL_WIDTH, SCREEN_PIXEL_HEIGHT, MAX_RECURSION_DEPTH);
        //render image
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
        
        if (success) {
            std::cout << "Image saved successfully to " << outputFilename << std::endl;
        } else {
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

