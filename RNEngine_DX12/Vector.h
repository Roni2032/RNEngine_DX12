#pragma once
namespace RNEngine {
	class Vector3 : public XMFLOAT3
	{
	public:
		Vector3(float _x, float _y, float _z):XMFLOAT3(_x,_y,_z) {}
		Vector3() : Vector3(0.0f,0.0f,0.0f) {}
	};
	class Vector2 : public XMFLOAT2
	{
	public:
		Vector2(float _x, float _y) :XMFLOAT2(_x, _y) {}
		Vector2() : Vector2(0.0f, 0.0f) {}
		
	};
}

