#include "ogpch.h"
#include "ScriptEngine.h"

#include "ScriptGlue.h"

#include "mono/jit/jit.h"
#include "mono/metadata/assembly.h"
#include "mono/metadata/object.h"
#include "mono/metadata/tabledefs.h"

namespace Orange {

	static std::unordered_map<std::string, ScriptFieldType> s_ScriptFieldTypeMap =
	{
		{ "System.Single", ScriptFieldType::Float },
		{ "System.Double", ScriptFieldType::Double },
		{ "System.Boolean", ScriptFieldType::Bool },
		{ "System.Char", ScriptFieldType::Char },
		{ "System.Int16", ScriptFieldType::Short },
		{ "System.Int32", ScriptFieldType::Int },
		{ "System.Int64", ScriptFieldType::Long },
		{ "System.Byte", ScriptFieldType::Byte },
		{ "System.UInt16", ScriptFieldType::UShort },
		{ "System.UInt32", ScriptFieldType::UInt },
		{ "System.UInt64", ScriptFieldType::ULong },
		{ "Hazel.Vector2", ScriptFieldType::Vector2 },
		{ "Hazel.Vector3", ScriptFieldType::Vector3 },
		{ "Hazel.Vector4", ScriptFieldType::Vector4 },
		{ "Hazel.Entity", ScriptFieldType::Entity },
	};

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

		ScriptFieldType MonoTypeToScriptFieldType(MonoType* monoType)
		{
			std::string typeName = mono_type_get_name(monoType);

			auto it = s_ScriptFieldTypeMap.find(typeName);
			if (it == s_ScriptFieldTypeMap.end())
			{
				OG_CORE_ERROR("Unknown type: {}", typeName);
				return ScriptFieldType::None;
			}

			return it->second;
		}

		const char* ScriptFieldTypeToString(ScriptFieldType type)
		{
			switch (type)
			{
				case ScriptFieldType::Float:   return "Float";
				case ScriptFieldType::Double:  return "Double";
				case ScriptFieldType::Bool:    return "Bool";
				case ScriptFieldType::Char:    return "Char";
				case ScriptFieldType::Byte:    return "Byte";
				case ScriptFieldType::Short:   return "Short";
				case ScriptFieldType::Int:     return "Int";
				case ScriptFieldType::Long:    return "Long";
				case ScriptFieldType::UByte:   return "UByte";
				case ScriptFieldType::UShort:  return "UShort";
				case ScriptFieldType::UInt:    return "UInt";
				case ScriptFieldType::ULong:   return "ULong";
				case ScriptFieldType::Vector2: return "Vector2";
				case ScriptFieldType::Vector3: return "Vector3";
				case ScriptFieldType::Vector4: return "Vector4";
				case ScriptFieldType::Entity:  return "Entity";
			}
			return "<Invalid>";
		}

	}

	struct ScriptEngineData
	{
		MonoDomain* RootDomain = nullptr;
		MonoDomain* AppDomain = nullptr;

		MonoAssembly* CoreAssembly = nullptr;
		MonoImage* CoreAssemblyImage = nullptr;

		MonoAssembly* AppAssembly = nullptr;
		MonoImage* AppAssemblyImage = nullptr;

		ScriptClass EntityClass;

		std::unordered_map<std::string, Ref<ScriptClass>> EntityClasses;
		std::unordered_map<UUID, Ref<ScriptInstance>> EntityInstances;
		// Runtime
		Scene* SceneContext = nullptr;
	};

	static ScriptEngineData* sed_Data = nullptr;

	void ScriptEngine::Init()
	{
		sed_Data = new ScriptEngineData();

		InitMono();

		LoadAssembly("Resources/Scripts/Orange-scriptCore.dll");
		LoadAppAssembly("SandboxProject/Assets/Scripts/Binaries/Sandbox.dll");
		LoadAssemblyClasses();


		ScriptGlue::RegisterComponents();
		ScriptGlue::RegisterFunctions();

		// Retrieve and instantiate class 
		sed_Data->EntityClass = ScriptClass("Orange", "Entity", true);
#if 0
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

		OG_CORE_ASSERT(false);
#endif
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

		// mono_domain_unload(sed_Data->AppDomain);
		sed_Data->AppDomain = nullptr;

		// mono_jit_cleanup(sed_Data->RootDomain);
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
		// Utils::PrintAssemblyTypes(sed_Data->CoreAssembly);
	}

	void ScriptEngine::LoadAppAssembly(const std::filesystem::path& filepath)
	{
		// Move this maybe
		sed_Data->AppAssembly = Utils::LoadMonoAssembly(filepath);
		auto assemb = sed_Data->AppAssembly;
		sed_Data->AppAssemblyImage = mono_assembly_get_image(sed_Data->AppAssembly);
		auto assembi = sed_Data->AppAssemblyImage;
		// Utils::PrintAssemblyTypes(sed_Data->AppAssembly);
	}

	void ScriptEngine::OnRuntimeStart(Scene* scene)
	{
		sed_Data->SceneContext = scene;
	}

	bool ScriptEngine::EntityClassExists(const std::string& fullClassName)
	{
		return sed_Data->EntityClasses.find(fullClassName) != sed_Data->EntityClasses.end();
	}
	
	void ScriptEngine::OnCreateEntity(Entity entity)
	{
		const auto& sc = entity.GetComponent<ScriptComponent>();
		if (ScriptEngine::EntityClassExists(sc.ClassName))
		{
			Ref<ScriptInstance> instance = CreateRef<ScriptInstance>(sed_Data->EntityClasses[sc.ClassName], entity);
			sed_Data->EntityInstances[entity.GetUUID()] = instance;
			instance->InvokeOnCreate();
		}
	}
	
	void ScriptEngine::OnUpdateEntity(Entity entity, Timestep ts)
	{
		UUID entityUUID = entity.GetUUID();
		OG_CORE_ASSERT(sed_Data->EntityInstances.find(entityUUID) != sed_Data->EntityInstances.end());
		Ref<ScriptInstance> instance = sed_Data->EntityInstances[entityUUID];
		instance->InvokeOnUpdate((float)ts);
	}
	
	Scene* ScriptEngine::GetSceneContext()
	{
		return sed_Data->SceneContext;
	}
	
	Ref<ScriptInstance> ScriptEngine::GetEntityScriptInstance(UUID entityID)
	{
		auto it = sed_Data->EntityInstances.find(entityID);
		if (it == sed_Data->EntityInstances.end())
			return nullptr;
		return it->second;
	}

	void ScriptEngine::OnRuntimeStop()
	{
		sed_Data->SceneContext = nullptr;
		sed_Data->EntityInstances.clear();
	}
	
	std::unordered_map<std::string, Ref<ScriptClass>> ScriptEngine::GetEntityClasses()
	{
		return sed_Data->EntityClasses;
	}
	
	void ScriptEngine::LoadAssemblyClasses()
	{
		sed_Data->EntityClasses.clear();

		const MonoTableInfo* typeDefinitionsTable = mono_image_get_table_info(sed_Data->AppAssemblyImage, MONO_TABLE_TYPEDEF);
		int32_t numTypes = mono_table_info_get_rows(typeDefinitionsTable);
		MonoClass* entityClass = mono_class_from_name(sed_Data->CoreAssemblyImage, "Orange", "Entity");

		for (int32_t i = 0; i < numTypes; i++)
		{
			uint32_t cols[MONO_TYPEDEF_SIZE];
			mono_metadata_decode_row(typeDefinitionsTable, i, cols, MONO_TYPEDEF_SIZE);

			const char* nameSpace = mono_metadata_string_heap(sed_Data->AppAssemblyImage, cols[MONO_TYPEDEF_NAMESPACE]);
			const char* className = mono_metadata_string_heap(sed_Data->AppAssemblyImage, cols[MONO_TYPEDEF_NAME]);
			std::string fullName;
			if (strlen(nameSpace) != 0)
				fullName = fmt::format("{}.{}", nameSpace, className);
			else
				fullName = className;

			MonoClass* monoClass = mono_class_from_name(sed_Data->AppAssemblyImage, nameSpace, className);

			if (monoClass == entityClass)
				continue;

			bool isEntity = mono_class_is_subclass_of(monoClass, entityClass, false);
			if (isEntity)
				continue;
			Ref<ScriptClass> scriptClass = CreateRef<ScriptClass>(nameSpace, className);
			sed_Data->EntityClasses[fullName] = scriptClass;

			// This routine is an iterator routine for retrieving the fields in a class.
			// You must pass a gpointer that points to zero and is treated as an opaque handle
			// to iterate over all of the elements. When no more values are available, the return value is NULL.
			int fieldCount = mono_class_num_fields(monoClass);
			OG_CORE_WARN("{} has {} fields:", className, fieldCount);
			void* iterator = nullptr;

			while (MonoClassField* field = mono_class_get_fields(monoClass, &iterator))
			{
				const char* fieldName = mono_field_get_name(field);
				uint32_t flags = mono_field_get_flags(field);
				if (flags & FIELD_ATTRIBUTE_PUBLIC)
				{
					MonoType* type = mono_field_get_type(field);
					ScriptFieldType fieldType = Utils::MonoTypeToScriptFieldType(type);
					OG_CORE_WARN("  {} ({})", fieldName, Utils::ScriptFieldTypeToString(fieldType));
					scriptClass->o_Fields[fieldName] = { fieldType, fieldName, field };
				}
			}
		}

		auto& entityClasses = sed_Data->EntityClasses;
		//mono_field_get_value()
	}
	
	MonoImage* ScriptEngine::GetCoreAssemblyImage()
	{
		return sed_Data->CoreAssemblyImage;
	}

	MonoObject* ScriptEngine::InstantiateClass(MonoClass* monoClass)
	{
		MonoObject* instance = mono_object_new(sed_Data->AppDomain, monoClass);
		mono_runtime_object_init(instance);
		return instance;
	}

	ScriptClass::ScriptClass(const std::string& classNamespace, const std::string& className, bool isCore)
		: o_ClassNamespace(classNamespace), o_ClassName(className)
	{
		o_MonoClass = mono_class_from_name(isCore ? sed_Data->CoreAssemblyImage : sed_Data->AppAssemblyImage, classNamespace.c_str(), className.c_str());
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

	ScriptInstance::ScriptInstance(Ref<ScriptClass> scriptClass, Entity entity)
		: o_ScriptClass(scriptClass)
	{
		o_Instance = scriptClass->Instantiate();
		o_Constructor = sed_Data->EntityClass.GetMethod(".ctor", 1);
		o_OnCreateMethod = scriptClass->GetMethod("OnCreate", 0);
		o_OnUpdateMethod = scriptClass->GetMethod("OnUpdate", 1);
		// Call Entity constructor
		{
			UUID entityID = entity.GetUUID();
			void* param = &entityID;
			o_ScriptClass->InvokeMethod(o_Instance, o_Constructor, &param);
		}
	
	}
	
	void ScriptInstance::InvokeOnCreate()
	{
		if (o_OnCreateMethod)
			o_ScriptClass->InvokeMethod(o_Instance, o_OnCreateMethod);
	}
	
	void ScriptInstance::InvokeOnUpdate(float ts)
	{
		if (o_OnUpdateMethod)
		{
			void* param = &ts;
			o_ScriptClass->InvokeMethod(o_Instance, o_OnUpdateMethod, &param);
		}
	}

	bool ScriptInstance::GetFieldValueInternal(const std::string& name, void* buffer)
	{
		const auto& fields = o_ScriptClass->GetFields();
		auto it = fields.find(name);
		if (it == fields.end())
			return false;
		const ScriptField& field = it->second;
		mono_field_get_value(o_Instance, field.ClassField, buffer);
		return true;
	}
	bool ScriptInstance::SetFieldValueInternal(const std::string& name, const void* value)
	{
		const auto& fields = o_ScriptClass->GetFields();
		auto it = fields.find(name);
		if (it == fields.end())
			return false;
		const ScriptField& field = it->second;
		mono_field_set_value(o_Instance, field.ClassField, (void*)value);
		return true;
	}

}
