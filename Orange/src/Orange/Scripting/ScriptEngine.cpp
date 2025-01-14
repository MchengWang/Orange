#include "ogpch.h"
#include "ScriptEngine.h"

#include "ScriptGlue.h"

#include "mono/jit/jit.h"
#include "mono/metadata/assembly.h"
#include "mono/metadata/object.h"

namespace Orange {

	namespace Utils {

		// TODO: move to FileSystem class
		static char* ReadBytes(const std::filesystem::path& filepath, uint32_t* outSize)
		{
			std::ifstream stream(filepath, std::ios::binary | std::ios::ate);

			if (!stream)
			{
				// Failed to open the file
				return nullptr;
			}

			std::streampos end = stream.tellg();
			stream.seekg(0, std::ios::beg);
			uint64_t size = end - stream.tellg();

			if (size == 0)
			{
				// File is empty
				return nullptr;
			}

			char* buffer = new char[size];
			stream.read((char*)buffer, size);
			stream.close();

			*outSize = (uint32_t)size;
			return buffer;
		}

		static MonoAssembly* LoadMonoAssembly(const std::filesystem::path& assemblyPath)
		{
			uint32_t fileSize = 0;
			char* fileData = ReadBytes(assemblyPath, &fileSize);

			// NOTE: We can't use this image for anything other than loading the assembly because this image doesn't have a reference to the assembly
			MonoImageOpenStatus status;
			MonoImage* image = mono_image_open_from_data_full(fileData, fileSize, 1, &status, 0);

			if (status != MONO_IMAGE_OK)
			{
				const char* errorMessage = mono_image_strerror(status);
				// Log some error message using the errorMessage data
				return nullptr;
			}

			std::string pathString = assemblyPath.string();
			MonoAssembly* assembly = mono_assembly_load_from_full(image, pathString.c_str(), &status, 0);
			mono_image_close(image);

			// Don't forget to free the file data
			delete[] fileData;

			return assembly;
		}

		void PrintAssemblyTypes(MonoAssembly* assembly)
		{
			MonoImage* image = mono_assembly_get_image(assembly);
			const MonoTableInfo* typeDefinitionsTable = mono_image_get_table_info(image, MONO_TABLE_TYPEDEF);
			int32_t numTypes = mono_table_info_get_rows(typeDefinitionsTable);

			for (int32_t i = 0; i < numTypes; i++)
			{
				uint32_t cols[MONO_TYPEDEF_SIZE];
				mono_metadata_decode_row(typeDefinitionsTable, i, cols, MONO_TYPEDEF_SIZE);

				const char* nameSpace = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAMESPACE]);
				const char* name = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAME]);

				OG_CORE_TRACE("{}.{}", nameSpace, name);
			}
		}

	}

	struct ScriptEngineData
	{
		MonoDomain* RootDomain = nullptr;
		MonoDomain* AppDomain = nullptr;

		MonoAssembly* CoreAssembly = nullptr;
		MonoImage* CoreAssemblyImage = nullptr;

		ScriptClass EntityClass;
	};

	static ScriptEngineData* sed_Data = nullptr;

	void ScriptEngine::Init()
	{
		sed_Data = new ScriptEngineData();

		InitMono();
		LoadAssembly("Resources/Scripts/Orange-scriptCode.dll");

		ScriptGlue::RegisterFunctions();

		// Retrieve and instantiate class (with constructor)
		sed_Data->EntityClass = ScriptClass("Orange", "Entity");

		MonoObject* instance = sed_Data->EntityClass.Instantiate();

		// Call method
		MonoMethod* printMessageFunc = sed_Data->EntityClass.GetMethod("PrintMessage", 0);
		sed_Data->EntityClass.InvokeMethod(instance, printMessageFunc);

		// Call method with param
		MonoMethod* printIntFunc = sed_Data->EntityClass.GetMethod("PrintInt", 1);

		int value = 5;
		void* param = &value;

		sed_Data->EntityClass.InvokeMethod(instance, printIntFunc, &param);

		MonoMethod* printIntsFunc = sed_Data->EntityClass.GetMethod("PrintInts", 2);
		int value2 = 508;
		void* params[2] =
		{
			&value,
			&value2
		};
		sed_Data->EntityClass.InvokeMethod(instance, printIntsFunc, params);

		MonoString* monoString = mono_string_new(sed_Data->AppDomain, "Hello World from C++!");
		MonoMethod* printCustomMessageFunc = sed_Data->EntityClass.GetMethod("PrintCustomMessage", 1);
		void* stringParam = monoString;
		sed_Data->EntityClass.InvokeMethod(instance, printCustomMessageFunc, &stringParam);

		//OG_CORE_ASSERT(false);
	}

	void ScriptEngine::Shutdown()
	{
		ShutdownMono();
		delete sed_Data;
	}


	void ScriptEngine::InitMono()
	{
		mono_set_assemblies_path("mono/lib");

		MonoDomain* rootDomain = mono_jit_init("OrangeJITRuntime");
		OG_CORE_ASSERT(rootDomain);

		// Store the root domain pointer
		sed_Data->RootDomain = rootDomain;
	}

	void ScriptEngine::ShutdownMono()
	{
		// NOTE(Yan): mono is a little confusing to shutdown, so maybe come back to this

		// mono_domain_unload(s_Data->AppDomain);
		sed_Data->AppDomain = nullptr;

		// mono_jit_cleanup(s_Data->RootDomain);
		sed_Data->RootDomain = nullptr;
	}

	void ScriptEngine::LoadAssembly(const std::filesystem::path& filepath)
	{
		// Create an App Domain
		sed_Data->AppDomain = mono_domain_create_appdomain("OrangeScriptRuntime", nullptr);
		mono_domain_set(sed_Data->AppDomain, true);

		// Move this maybe
		sed_Data->CoreAssembly = Utils::LoadMonoAssembly(filepath);
		sed_Data->CoreAssemblyImage = mono_assembly_get_image(sed_Data->CoreAssembly);
		// Utils::PrintAssemblyTypes(s_Data->CoreAssembly);
	}

	MonoObject* ScriptEngine::InstantiateClass(MonoClass* monoClass)
	{
		MonoObject* instance = mono_object_new(sed_Data->AppDomain, monoClass);
		mono_runtime_object_init(instance);
		return instance;
	}

	ScriptClass::ScriptClass(const std::string& classNamespace, const std::string& className)
		: o_ClassNamespace(classNamespace), o_ClassName(className)
	{
		o_MonoClass = mono_class_from_name(sed_Data->CoreAssemblyImage, classNamespace.c_str(), className.c_str());
	}

	MonoObject* ScriptClass::Instantiate()
	{
		return ScriptEngine::InstantiateClass(o_MonoClass);
	}

	MonoMethod* ScriptClass::GetMethod(const std::string& name, int parameterCount)
	{
		return mono_class_get_method_from_name(o_MonoClass, name.c_str(), parameterCount);
	}

	MonoObject* ScriptClass::InvokeMethod(MonoObject* instance, MonoMethod* method, void** params)
	{
		return mono_runtime_invoke(method, instance, params, nullptr);
	}

}
