#pragma once
#include <string> 
#include "DirectXMath.h"

struct Item
{
	std::string meshName;
	DirectX::XMFLOAT4X4 worldMatrix;
	bool isScreenSpace = false;
};

struct ItemUI
{
	std::string _text;
	DirectX::XMFLOAT4X4 orthoProj;
};