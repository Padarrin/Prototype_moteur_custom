#pragma once
#include "string";
class RessourceLoader
{

public : 

	static unsigned char* LoadBMP32(std::string& _path, int& _outWidth, int& _outHeight);
};

