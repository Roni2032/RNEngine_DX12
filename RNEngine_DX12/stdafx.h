#pragma once

//ComPtr
#include <wrl/client.h>

//std系統
#include <iostream>
#include <fstream>
#include <Windows.h>
#include <tchar.h>
#include <assert.h>
#include <chrono>
#include <vector>
#include <map>
#include <unordered_map>
#include <array>
#include <thread>
#include <codecvt>
#include <type_traits>
#include <filesystem>
#include <random>

//DirectX系統
#include <d3d12.h>
#include <dxgi1_6.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include<DirectXTex.h>
#include<d3dx12.h>
#include<DirectXColors.h>

//assimp
#include <assimp/Importer.hpp>
#include <assimp/Scene.h>
#include <assimp/postprocess.h>

//imgui
#define IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_IMPL_API
#include "include/imgui/backends/imgui_impl_win32.h"
#include "include/imgui/backends/imgui_impl_dx12.h"
#include "include/imgui/imgui.h"

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "DirectXTex.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "comsuppwd.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "assimp-vc143-mtd.lib")
#pragma comment(lib, "zlibstaticd.lib")

//デバッグ系統
#ifdef _DEBUG
#include <dxgidebug.h>
#endif
#include <comdef.h>

//名前空間定義
using namespace std;
using namespace Microsoft::WRL;
using namespace DirectX;
using namespace std::chrono;
using namespace std::filesystem;

//ゲーム内で使用する基本的なファイル
#include "Object.h"
#include "Util.h"
#include "Vector.h"

//using namespace RNEngine;