#include "RessourceLoader.h"
#include <fstream>
#include <iostream>

unsigned char* RessourceLoader::LoadBMP32(std::string& _path, int& _outWidth, int& _outHeight)
{
    std::ifstream file(_path, std::ios::binary);
    if (!file)
    {
        std::cout << "Erreur: Impossible d'ouvrir " << _path << std::endl;
        return nullptr;
    }

    unsigned char header[54];
    file.read(reinterpret_cast<char*>(header), 54);

    _outWidth = *(int*)&header[18];
    _outHeight = *(int*)&header[22];

    int dataOffset = *(int*)&header[10];

    int imageSize = _outWidth * _outHeight * 4;
   
    unsigned char* pixels = new unsigned char[imageSize];

    file.seekg(dataOffset, std::ios::beg);
    file.read(reinterpret_cast<char*>(pixels), imageSize);
    file.close();

    for (int i = 0; i < imageSize; i += 4)
    {
        unsigned char temp = pixels[i];         
        pixels[i] = pixels[i + 2];              
        pixels[i + 2] = temp;                   
    }
    return pixels;
}
