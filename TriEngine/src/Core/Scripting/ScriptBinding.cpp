#include "tripch.h"

#include "ScriptBinding.h"

#include "Core/Base/Random.h"

#include "glm/glm.hpp"

#include "scriptmath/scriptmath.h"
#include "scriptstdstring/scriptstdstring.h"
#include "scriptarray/scriptarray.h"
#include "datetime/datetime.h"
#include "scripthelper/scripthelper.h"
#include "scriptdictionary/scriptdictionary.h"
#include "scriptfile/scriptfile.h"
#include "scriptfile/scriptfilesystem.h"

static void log_trace(const std::string& in) {
    TRI_TRACE("{0}", in.c_str());
}

static void log_info(const std::string& in) {
    TRI_INFO("{0}", in.c_str());
}

static void log_warn(const std::string& in) {
    TRI_WARN("{0}", in.c_str());
}

static void log_error(const std::string& in) {
    TRI_ERROR("{0}", in.c_str());
}

static void Vec2ConstructorProxy(void* memory) {
    new(memory) glm::vec2();
}

static void Vec2ConstructorSingleProxy(float value, void* memory) {
    new(memory) glm::vec2(value);
}

static void Vec2ConstructorDoubleProxy(float x, float y, void* memory) {
    new(memory) glm::vec2(x, y);
}

static void Vec2DestructorProxy(void* memory) {
    delete memory;
}

namespace TriEngine {
    void Utils::ConfigureScriptEngine(asIScriptEngine* engine) 
    {
        RegisterScriptMath(engine);
        RegisterStdString(engine);
        RegisterScriptArray(engine, true);
        RegisterScriptDictionary(engine);
        RegisterScriptDateTime(engine);
        RegisterExceptionRoutines(engine);
        RegisterScriptFile(engine);
        RegisterScriptFileSystem(engine);

        int32_t r;

        // GLM types
        r = engine->RegisterObjectType("Vec2", sizeof(glm::vec2), asOBJ_VALUE | asGetTypeTraits<glm::vec2>() | asOBJ_APP_CLASS_ALLFLOATS); TRI_CORE_ASSERT(r >= 0, "Failed to register type");
        r = engine->RegisterObjectBehaviour("Vec2", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(Vec2ConstructorProxy), asCALL_CDECL_OBJLAST);
        r = engine->RegisterObjectBehaviour("Vec2", asBEHAVE_CONSTRUCT, "void f(float value)", asFUNCTION(Vec2ConstructorSingleProxy), asCALL_CDECL_OBJLAST);
        r = engine->RegisterObjectBehaviour("Vec2", asBEHAVE_CONSTRUCT, "void f(float x, float y)", asFUNCTION(Vec2ConstructorDoubleProxy), asCALL_CDECL_OBJLAST);
        r = engine->RegisterObjectBehaviour("Vec2", asBEHAVE_DESTRUCT, "void f()", asFUNCTION(Vec2DestructorProxy), asCALL_CDECL_OBJLAST); TRI_CORE_ASSERT( r >= 0, "Failed to register method" );

        // Logging
        r = engine->RegisterGlobalFunction("void log_trace(const string &in)", asFUNCTION(log_info), asCALL_CDECL); TRI_CORE_ASSERT(r >= 0, "Failed to register function");
        r = engine->RegisterGlobalFunction("void log_info(const string &in)", asFUNCTION(log_info), asCALL_CDECL); TRI_CORE_ASSERT(r >= 0, "Failed to register function");
        r = engine->RegisterGlobalFunction("void log_warn(const string &in)", asFUNCTION(log_info), asCALL_CDECL); TRI_CORE_ASSERT(r >= 0, "Failed to register function");
        r = engine->RegisterGlobalFunction("void log_error(const string &in)", asFUNCTION(log_info), asCALL_CDECL); TRI_CORE_ASSERT(r >= 0, "Failed to register function");

        // Random
        r = engine->RegisterGlobalFunction("float RandFloat(float min = 0.0f, float max = 1.0f)", asFUNCTION(Random::Float), asCALL_CDECL);
        r = engine->RegisterGlobalFunction("int RandInt(int min, int max)", asFUNCTION(Random::Int), asCALL_CDECL);
        r = engine->RegisterGlobalFunction("uint RandUint(uint min, uint max)", asFUNCTION(Random::Uint), asCALL_CDECL);
        r = engine->RegisterGlobalFunction("bool RandBool()", asFUNCTION(Random::Bool), asCALL_CDECL);

    }
}