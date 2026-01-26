#pragma once
//#include <span>
#include "RendererHeader.h"
#include "Buffer.h"

namespace RNEngine {
	//ˆê”Ê“I‚ÈŠÖ”ŒQ
	class Util
	{
	public:
		static wstring ConvertStrToWstr(const string& str);
		static string ConvertWstrToStr(const wstring& str);

		template<typename T>
		static T Random(T min = 0.0f, T max = 1.0f) {
			static thread_local random_device rd;
			static thread_local mt19937 gen(rd());
			
			if constexpr (is_integral_v<T>) {
				uniform_int_distribution<T> dist(min, max);
				return dist(gen);
			}
			else if constexpr (is_floating_point_v<T>) {
				uniform_real_distribution<T> dist(min, max);
				return dist(gen);
			}
		}
	};

	//DirectX—pŠÖ”ŒQ
	class DxUtil {
	public:
		static void CreateMeshBuffer(ID3D12Device* _dev, Mesh& mesh);
		/*template<typename T>
		static ComPtr<ID3D12Resource> CreateUploadBuffer(ID3D12Device* dev, std::span<const T> data){
			ComPtr<ID3D12Resource> buffer;

			const UINT64 size = sizeof(T) * data.size();

			const D3D12_HEAP_PROPERTIES heap{
				D3D12_HEAP_TYPE_UPLOAD
			};

			const D3D12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Buffer(size);

			ThrowIfFailed(dev->CreateCommittedResource(
				&heap,
				D3D12_HEAP_FLAG_NONE,
				&desc,
				D3D12_RESOURCE_STATE_GENERIC_READ,
				nullptr,
				IID_PPV_ARGS(&buffer)
			));

			void* mapped = nullptr;
			ThrowIfFailed(buffer->Map(0, nullptr, &mapped));
			std::memcpy(mapped, data.data(), size);
			buffer->Unmap(0, nullptr);

			return buffer;
		}*/
	};
}
