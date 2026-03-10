#pragma once

#ifndef NOMINMAX
#define NOMINMAX
#endif

//
// Windows
/////////////////////////
#include <winsock2.h>
#include <windows.h>
#include <Ws2tcpip.h>

#include <algorithm>
#include <chrono>
#include <cmath>
#include <emmintrin.h>
#include <functional>
#include <iostream>
#include <list>
#include <malloc.h>
#include <map>
#include <memory.h>
#include <stdlib.h>
#include <string>
#include <tchar.h>
#include <thread>
#include <typeindex>
#include <unordered_map>
#include <vector>



//
// DirectX
/////////////////////////
#include "DirectXMath.h"
using namespace DirectX;


//
// Forward declaration
/////////////////////////
class ECS;


//
// Variables
/////////////////////////
#define GRAVITY							9.81f
#define NONE							nullptr


//
// Float2
/////////////////////////
inline XMFLOAT2 VEC2_ZERO				= { 0.0f, 0.0f };
inline XMFLOAT2 VEC2_ONE				= { 1.0f, 1.0f };

//
// Float3
/////////////////////////
inline XMFLOAT3 VEC3_ZERO				= { 0.0f, 0.0f, 0.0f };
inline XMFLOAT3 VEC3_ONE				= { 1.0f, 1.0f, 1.0f };
inline XMFLOAT3 VEC3_FORWARD			= { 0.0f, 0.0f, 1.0f };
inline XMFLOAT3 VEC3_UP					= { 0.0f, 1.0f, 0.0f };
inline XMFLOAT3 VEC3_RIGHT				= { 1.0f, 0.0f, 0.0f };
inline XMFLOAT3 VEC3_GRAVITY			= { 0.0f, 9.81f, 0.0f };

//
// Float4
/////////////////////////
inline XMFLOAT4 VEC4_QUAT_ZERO			= { 0.0f, 0.0f, 0.0f, 1.0f };
inline XMFLOAT4 VEC4_QUAT_ONE			= { 1.0f, 1.0f, 1.0f, 1.0f };

//
// Float4x4
/////////////////////////
inline XMFLOAT4X4 VEC4X4_IDENTITY		= { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
inline XMFLOAT4X4 VEC4X4_ZERO			= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

//
// Core
/////////////////////////
#include "Utils.hpp"
using namespace Utils;
#include "../Render/Vertex.h"
#include "Geometry/Triangle.h"
#include "InputsEnums.h"
#include "../Render/GameTimer.h"
#include "GameManager.h"
#include "Scene.h"
#include "SceneManager.h"

//
// Component
/////////////////////////
#include "Component.h"
#include "Entity.h"
#include "Transform.h"
#include "Velocity.h"
#include "MeshRenderer.h"
#include "Health.h"

//
// System
/////////////////////////
#include "System.h"
#include "TransformSystem.h"
#include "MeshSystem.h"
#include "RenderSystem.h"
#include "HealthSystem.h"
#include "CleanUpSystem.h"

//
//Inputs
/////////////////////////
#include "InputSystem.h"

//
// Collider
/////////////////////////
#include "Collider.h"

//
// ECS
/////////////////////////
#include "Ecs.h"

//
//StateMachine
/////////////////////////
#include "StateMachine.h"


//
//COLORS
/////////////////////////
inline  XMFLOAT4 WHITE		= { 1.0f, 1.0f, 1.0f, 1.0f };
inline  XMFLOAT4 GREY		= { 0.5f, 0.5f, 0.5f, 1.0f };
inline  XMFLOAT4 BLACK		= { 0.0f, 0.0f, 0.0f, 1.0f };
inline  XMFLOAT4 RED		= { 1.0f, 0.0f, 0.0f, 1.0f };
inline  XMFLOAT4 GREEN		= { 0.0f, 1.0f, 0.0f, 1.0f };
inline  XMFLOAT4 BLUE		= { 0.0f, 0.0f, 1.0f, 1.0f };
inline  XMFLOAT4 GRAY		= { 0.5f, 0.5f, 0.5f, 1.0f };
inline  XMFLOAT4 MAGENTA	= { 1.0f, 0.0f, 1.0f, 1.0f };
inline  XMFLOAT4 ORANGE		= { 1.0f, 0.5f, 0.0f, 1.0f };
inline  XMFLOAT4 BROWN		= { 0.6f, 0.4f, 0.1f, 1.0f };
inline  XMFLOAT4 SAND		= { 0.92f, 0.85f, 0.73f,1.0f };
inline	XMFLOAT4 CACTUS		= { 0.0f, 0.8f, 0.0f, 1.0f };

