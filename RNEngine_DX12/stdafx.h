#pragma once

#include <iostream>
#include<Windows.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <d3dcompiler.h>
#include <wrl/client.h>
#include <DirectXMath.h>

#include<tchar.h>
#include<assert.h>

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

#ifdef _DEBUG
#include <dxgidebug.h>
#endif

using namespace std;
using namespace Microsoft::WRL;
using namespace DirectX;
//using namespace RNEngine;