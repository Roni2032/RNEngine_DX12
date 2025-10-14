#pragma once
#include "Component.h"

namespace RNEngine
{
	class Image : public Component
	{
	public:
		Image() : Component() {}
		virtual ~Image() {}

		virtual void Start() override;
		virtual void Draw() override;

	};
}
