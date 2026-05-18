#include "../../Component.h"
#include "../../ReflectionGenerater.h"
#include "../../Reflection.h"
#include "../../Serialize.h"

namespace RNEngine {
	struct TransformReflect {
		TransformReflect() {
			
			//FieldInfo info = { "m_Position",offsetof(Transform,m_Position),FieldInfo::Type::Vec3,{} };
		}
	};

	static TransformReflect g_TransformReflect;
}