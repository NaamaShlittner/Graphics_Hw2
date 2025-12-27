#include "parser/Parser.h"
#include "engine/Scene.h"
#include <iostream>

int main(int argc, char* argv[])
{
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <scene_file>" << std::endl;
        return 1;
    }
    try {
        auto scene = buildFromFile(argv[1]);
        printf("Scene loaded successfully with %zu objects and %zu lights.\n",
               scene->objs.size(), scene->lights.size());
        printf("Camera position: (%.2f, %.2f, %.2f)\n",
               scene->cam.getPosition().x,
               scene->cam.getPosition().y,
               scene->cam.getPosition().z);
        printf("Ambient light intensity: (%.2f, %.2f, %.2f)\n",
               scene->ambientLight.x,
               scene->ambientLight.y,
               scene->ambientLight.z);
        for (size_t i = 0; i < scene->objs.size(); ++i) {
            const auto& obj = scene->objs[i];
            printf("Object %zu color: (%.2f, %.2f, %.2f)\n",
                   i + 1,
                   obj->color.x,
                   obj->color.y,
                   obj->color.z);
            printf("Reflective constant: %.2f\n", obj->reflectiveConst);
            printf("Refractive constant: %.2f\n", obj->refractiveConst);
            printf("Shininess: %.2f\n", obj->shininess);
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}