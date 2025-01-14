#include "ogpch.h"
#include "ScriptGlue.h"

#include "mono/metadata/object.h"

namespace Orange
{

#define OG_ADD_INTERNAL_CALL(Name) mono_add_internal_call("Orange.InternalCalls::" #Name, Name)

	static void NativeLog(MonoString* string, int parameter)
	{
		char* cStr = mono_string_to_utf8(string);
		std::string str(cStr);
		mono_free(cStr);
		std::cout << str << ", " << parameter << std::endl;
	}

	static void NativeLog_Vector(glm::vec3* parameter, glm::vec3* outResult)
	{
		float a = parameter->x, b = parameter->y, c = parameter->z;
		OG_CORE_WARN("Value: {0}, {1}, {2}", a, b, c);
		*outResult = glm::normalize(*parameter);
	}

	static float NativeLog_VectorDot(glm::vec3* parameter)
	{
		float a = parameter->x, b = parameter->y, c = parameter->z;
		OG_CORE_WARN("Value: {0}, {1}, {2}", a, b, c);
		return glm::dot(*parameter, *parameter);
	}

	void ScriptGlue::RegisterFunctions()
	{
		OG_ADD_INTERNAL_CALL(NativeLog);
		OG_ADD_INTERNAL_CALL(NativeLog_Vector);
		OG_ADD_INTERNAL_CALL(NativeLog_VectorDot);
	}

}
