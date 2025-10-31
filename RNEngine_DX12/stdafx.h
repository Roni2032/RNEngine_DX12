#pragma once

//ComPtr
#include <wrl/client.h>

//std�n��
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

//DirectX�n��
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

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "DirectXTex.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "comsuppwd.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "assimp-vc143-mtd.lib")
#pragma comment(lib, "zlibstaticd.lib")

//�f�o�b�O�n��
#ifdef _DEBUG
#include <dxgidebug.h>
#endif
#include <comdef.h>

//���O��Ԓ�`
using namespace std;
using namespace Microsoft::WRL;
using namespace DirectX;
using namespace std::chrono;

//�Q�[�����Ŏg�p�����{�I�ȃt�@�C��
#include "Object.h"

//using namespace RNEngine;