#pragma once

#include <iostream>
#include<Windows.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <d3dcompiler.h>
#include <wrl/client.h>
#include <DirectXMath.h>
#include <comdef.h>

#include<tchar.h>
#include<assert.h>
#include <chrono>
#include <vector>
#include <map>

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "comsuppwd.lib")

#ifdef _DEBUG
#include <dxgidebug.h>
#endif

using namespace std;
using namespace Microsoft::WRL;
using namespace DirectX;
using namespace std::chrono;

//ゲーム内で使用する基本的なファイル
#include "Object.h"

//using namespace RNEngine;