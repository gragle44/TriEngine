#include "tripch.h"

#include "ScriptBinding.h"

#include "Core/Base/Random.h"

#include "glm/glm.hpp"

#include "Core/Base/KeyCodes.h"
#include "Core/Base/MouseButtonCodes.h"

#include "Core/Base/Input.h"

#include "Core/Base/UUID.h"

#include "Core/Renderer/OrthographicCamera.h"

#include "Core/Resources/ResourceManager.h"

#include "GameObjectBinding.h"

#include "scriptmath/scriptmath.h"
#include "scriptstdstring/scriptstdstring.h"
#include "scriptarray/scriptarray.h"
#include "datetime/datetime.h"
#include "scripthelper/scripthelper.h"
#include "scriptdictionary/scriptdictionary.h"
#include "scriptfile/scriptfile.h"
#include "scriptfile/scriptfilesystem.h"

#include "asbind20/asbind.hpp"

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

static void Mat4Constructor_Proxy(glm::mat4* mem, const glm::vec4& v1, const glm::vec4& v2, const glm::vec4& v3, const glm::vec4& v4) {
    new(mem) glm::mat4(v1, v2, v3, v4);
}

static std::string formatVec2(glm::vec2 vec) {
    return std::format("({}, {})", vec.x, vec.y);
}

static std::string formatVec3(glm::vec3 vec)
{
    return std::format("({}, {}, {})", vec.x, vec.y, vec.z);
}

static std::string formatVec4(glm::vec4 vec)
{
    return std::format("({}, {}, {}, {})", vec.x, vec.y, vec.z, vec.w);
}

static void GetResource_Generic(asIScriptGeneric* gen) {
    std::string_view path = reinterpret_cast<const char*>(gen->GetArgAddress(0));

    void* resource = TriEngine::ResourceManager::Get(TriEngine::ResourceManager::GetIDFromPath(path)).get();

    void* retAddress = gen->GetAddressOfReturnLocation();

    memcpy(retAddress, &resource, sizeof(resource));
}

static void BindGLM(asIScriptEngine* engine) {
    asbind20::value_class<glm::vec2>(
        engine,
        "Float2",
        asOBJ_APP_CLASS_ALLFLOATS | asOBJ_APP_CLASS_MORE_CONSTRUCTORS
    )
        .behaviours_by_traits()
        .constructor<float>("float value")
        .constructor<float, float>("float x, float y")
        .opEquals()
        .opAdd()
        .opAddAssign()
        .opSub()
        .opSubAssign()
        .opMul()
        .opMulAssign()
        .opDiv()
        .opDivAssign()
        .property("float x", &glm::vec2::x)
        .property("float y", offsetof(glm::vec2, y));

    asbind20::value_class<glm::vec3>(
        engine,
        "Float3",
        asOBJ_APP_CLASS_ALLFLOATS | asOBJ_APP_CLASS_MORE_CONSTRUCTORS
    )
        .behaviours_by_traits()
        .constructor<float>("float value")
        .constructor<float, float, float>("float x, float y, float z")
        .opEquals()
        .opAdd()
        .opAddAssign()
        .opSub()
        .opSubAssign()
        .opMul()
        .opMulAssign()
        .opDiv()
        .opDivAssign()
        .property("float x", &glm::vec3::x)
        .property("float y", offsetof(glm::vec3, y))
        .property("float z", offsetof(glm::vec3, z))
        .property("float r", &glm::vec3::x)
        .property("float g", offsetof(glm::vec3, y))
        .property("float b", offsetof(glm::vec3, z));

    asbind20::value_class<glm::vec4>(
        engine,
        "Float4",
        asOBJ_APP_CLASS_ALLFLOATS | asOBJ_APP_CLASS_MORE_CONSTRUCTORS
    )
        .behaviours_by_traits()
        .constructor<float>("float value")
        .constructor<float, float, float, float>("float x, float y, float z, float w")
        .opEquals()
        .opAdd()
        .opAddAssign()
        .opSub()
        .opSubAssign()
        .opMul()
        .opMulAssign()
        .opDiv()
        .opDivAssign()
        .property("float x", &glm::vec4::x)
        .property("float y", offsetof(glm::vec4, y))
        .property("float z", offsetof(glm::vec4, z))
        .property("float w", offsetof(glm::vec4, w))
        .property("float r", offsetof(glm::vec4, r))
        .property("float g", offsetof(glm::vec4, g))
        .property("float b", offsetof(glm::vec4, b))
        .property("float a", offsetof(glm::vec4, a));

    asbind20::value_class<glm::mat4>(
        engine,
        "Mat4",
        asOBJ_APP_CLASS_ALLFLOATS
    )
        .behaviours_by_traits()
        .constructor_function("Float4, Float4, Float4, Float4", &Mat4Constructor_Proxy)
        .opEquals()
        .opAdd()
        .opAddAssign()
        .opSub()
        .opSubAssign()
        .opMul()
        .opMulAssign()
        .opDiv()
        .opDivAssign()
        .opNeg();

    asbind20::global(engine)
        .function("float Radians(float degrees)", &glm::radians<float>)
        .function("float Degrees(float radians)", &glm::degrees<float>);
    }

static void BindInput(asIScriptEngine* engine)
{
    using namespace TriEngine;

    asbind20::enum_<KeyCode>(engine, "KeyCode")
        .value(KeyCode::Space,       "Space")
        .value(KeyCode::Apostrophe,  "Apostrophe")
        .value(KeyCode::Comma,       "Comma")
        .value(KeyCode::Minus,       "Minus")
        .value(KeyCode::Period,      "Period")
        .value(KeyCode::Slash,       "Slash")
        .value(KeyCode::_0,          "_0")
        .value(KeyCode::_1,          "_1")
        .value(KeyCode::_2,          "_2")
        .value(KeyCode::_3,          "_3")
        .value(KeyCode::_4,          "_4")
        .value(KeyCode::_5,          "_5")
        .value(KeyCode::_6,          "_6")
        .value(KeyCode::_7,          "_7")
        .value(KeyCode::_8,          "_8")
        .value(KeyCode::_9,          "_9")
        .value(KeyCode::Semicolon,   "Semicolon")
        .value(KeyCode::Equal,       "Equal")
        .value(KeyCode::A,           "A")
        .value(KeyCode::B,           "B")
        .value(KeyCode::C,           "C")
        .value(KeyCode::D,           "D")
        .value(KeyCode::E,           "E")
        .value(KeyCode::F,           "F")
        .value(KeyCode::G,           "G")
        .value(KeyCode::H,           "H")
        .value(KeyCode::I,           "I")
        .value(KeyCode::J,           "J")
        .value(KeyCode::K,           "K")
        .value(KeyCode::L,           "L")
        .value(KeyCode::M,           "M")
        .value(KeyCode::N,           "N")
        .value(KeyCode::O,           "O")
        .value(KeyCode::P,           "P")
        .value(KeyCode::Q,           "Q")
        .value(KeyCode::R,           "R")
        .value(KeyCode::S,           "S")
        .value(KeyCode::T,           "T")
        .value(KeyCode::U,           "U")
        .value(KeyCode::V,           "V")
        .value(KeyCode::W,           "W")
        .value(KeyCode::X,           "X")
        .value(KeyCode::Y,           "Y")
        .value(KeyCode::Z,           "Z")
        .value(KeyCode::LeftBracket, "LeftBracket")
        .value(KeyCode::Backslash,   "Backslash")
        .value(KeyCode::RightBracket,"RightBracket")
        .value(KeyCode::GraveAccent, "GraveAccent")
        .value(KeyCode::World1,      "World1")
        .value(KeyCode::World2,      "World2")
        .value(KeyCode::Escape,      "Escape")
        .value(KeyCode::Enter,       "Enter")
        .value(KeyCode::Tab,         "Tab")
        .value(KeyCode::Backspace,   "Backspace")
        .value(KeyCode::Insert,      "Insert")
        .value(KeyCode::Delete,      "Delete")
        .value(KeyCode::Right,       "Right")
        .value(KeyCode::Left,        "Left")
        .value(KeyCode::Down,        "Down")
        .value(KeyCode::Up,          "Up")
        .value(KeyCode::PageUp,      "PageUp")
        .value(KeyCode::PageDown,    "PageDown")
        .value(KeyCode::Home,        "Home")
        .value(KeyCode::End,         "End")
        .value(KeyCode::CapsLock,    "CapsLock")
        .value(KeyCode::ScrollLock,  "ScrollLock")
        .value(KeyCode::NumLock,     "NumLock")
        .value(KeyCode::PrintScreen, "PrintScreen")
        .value(KeyCode::Pause,       "Pause")
        .value(KeyCode::F1,          "F1")
        .value(KeyCode::F2,          "F2")
        .value(KeyCode::F3,          "F3")
        .value(KeyCode::F4,          "F4")
        .value(KeyCode::F5,          "F5")
        .value(KeyCode::F6,          "F6")
        .value(KeyCode::F7,          "F7")
        .value(KeyCode::F8,          "F8")
        .value(KeyCode::F9,          "F9")
        .value(KeyCode::F10,         "F10")
        .value(KeyCode::F11,         "F11")
        .value(KeyCode::F12,         "F12")
        .value(KeyCode::F13,         "F13")
        .value(KeyCode::F14,         "F14")
        .value(KeyCode::F15,         "F15")
        .value(KeyCode::F16,         "F16")
        .value(KeyCode::F17,         "F17")
        .value(KeyCode::F18,         "F18")
        .value(KeyCode::F19,         "F19")
        .value(KeyCode::F20,         "F20")
        .value(KeyCode::F21,         "F21")
        .value(KeyCode::F22,         "F22")
        .value(KeyCode::F23,         "F23")
        .value(KeyCode::F24,         "F24")
        .value(KeyCode::F25,         "F25")
        .value(KeyCode::KP_0,        "KP_0")
        .value(KeyCode::KP_1,        "KP_1")
        .value(KeyCode::KP_2,        "KP_2")
        .value(KeyCode::KP_3,        "KP_3")
        .value(KeyCode::KP_4,        "KP_4")
        .value(KeyCode::KP_5,        "KP_5")
        .value(KeyCode::KP_6,        "KP_6")
        .value(KeyCode::KP_7,        "KP_7")
        .value(KeyCode::KP_8,        "KP_8")
        .value(KeyCode::KP_9,        "KP_9")
        .value(KeyCode::KP_Decimal,  "KP_Decimal")
        .value(KeyCode::KP_Divide,   "KP_Divide")
        .value(KeyCode::KP_Multiply, "KP_Multiply")
        .value(KeyCode::KP_Subtract, "KP_Subtract")
        .value(KeyCode::KP_Add,      "KP_Add")
        .value(KeyCode::KP_Enter,    "KP_Enter")
        .value(KeyCode::KP_Equal,    "KP_Equal")
        .value(KeyCode::LeftShift,   "LeftShift")
        .value(KeyCode::LeftControl, "LeftControl")
        .value(KeyCode::LeftAlt,     "LeftAlt")
        .value(KeyCode::LeftSuper,   "LeftSuper")
        .value(KeyCode::RightShift,  "RightShift")
        .value(KeyCode::RightControl,"RightControl")
        .value(KeyCode::RightAlt,    "RightAlt")
        .value(KeyCode::RightSuper,  "RightSuper")
        .value(KeyCode::Menu,        "Menu");

    asbind20::enum_<MouseButton>(engine, "MouseButton")
        .value(MouseButton::Button1, "Button1")
        .value(MouseButton::Button2, "Button2")
        .value(MouseButton::Button3, "Button3")
        .value(MouseButton::Button4, "Button4")
        .value(MouseButton::Button5, "Button5")
        .value(MouseButton::Button6, "Button6")
        .value(MouseButton::Button7, "Button7")
        .value(MouseButton::Button8, "Button8")
        .value(MouseButton::Right, "Right")
        .value(MouseButton::Middle, "Middle")
        .value(MouseButton::Left, "Left")
        .value(MouseButton::Last, "Last");

    // Input
    asbind20::global(engine)
        .function("bool IsKeyPressed(int keycode)", &Input::IsKeyPressed)
        .function("bool IsMouseButtonPressed(int button)", &Input::IsMouseButtonPressed)
        .function("Float2 GetMousePosition()", &Input::GetMousePosition)
        .function("bool IsGamepadButtonPressed(int32 joystick, int32 button)", &Input::IsGamepadButtonPressed)
        .function("float GetGamepadAxes(int32 joystick, int32 axis)", &Input::GetGamepadAxes);
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

        //UUID
        asbind20::value_class<UUID>(
            engine,
            "UUID",
            asOBJ_APP_CLASS_ALLINTS | asOBJ_APP_CLASS_MORE_CONSTRUCTORS
        )
            .behaviours_by_traits()
            .constructor<uint64_t>("uint64 id")
            .opEquals()
            .opCmp()
            .opImplConv<uint64_t>();

        BindGLM(engine);

        // Camera
        asbind20::value_class<OrthographicCamera>(
            engine,
            "OrthographicCamera",
            asOBJ_APP_CLASS_ALLFLOATS
        )
            .behaviours_by_traits()
            .method("const Mat4& GetProjection() const", &OrthographicCamera::GetProjection)
            .method("void SetSize(float zoom, float nearClip = -1.0f, float farClip = 1.0f)", &OrthographicCamera::SetSize)
            .method("void SetViewportSize(uint32 width, uint32 height, uint32 windowHeight = 0)", &OrthographicCamera::SetViewportSize);

        BindGameObjects(engine);

        BindInput(engine);

        int32_t r;

        // Logging
        r = engine->RegisterGlobalFunction("void LogTrace(const string &in)", asFUNCTION(log_trace), asCALL_CDECL); TRI_CORE_ASSERT(r >= 0, "Failed to register function");
        r = engine->RegisterGlobalFunction("void LogInfo(const string &in)", asFUNCTION(log_info), asCALL_CDECL); TRI_CORE_ASSERT(r >= 0, "Failed to register function");
        r = engine->RegisterGlobalFunction("void LogWarn(const string &in)", asFUNCTION(log_warn), asCALL_CDECL); TRI_CORE_ASSERT(r >= 0, "Failed to register function");
        r = engine->RegisterGlobalFunction("void LogError(const string &in)", asFUNCTION(log_error), asCALL_CDECL); TRI_CORE_ASSERT(r >= 0, "Failed to register function");

        // Random
        r = engine->RegisterGlobalFunction("float RandFloat(float min = 0.0f, float max = 1.0f)", asFUNCTION(Random::Float), asCALL_CDECL); TRI_CORE_ASSERT(r >= 0, "Failed to register function");
        r = engine->RegisterGlobalFunction("int RandInt(int min, int max)", asFUNCTION(Random::Int), asCALL_CDECL); TRI_CORE_ASSERT(r >= 0, "Failed to register function");
        r = engine->RegisterGlobalFunction("uint RandUint(uint min, uint max)", asFUNCTION(Random::Uint), asCALL_CDECL); TRI_CORE_ASSERT(r >= 0, "Failed to register function");
        r = engine->RegisterGlobalFunction("bool RandBool()", asFUNCTION(Random::Bool), asCALL_CDECL); TRI_CORE_ASSERT(r >= 0, "Failed to register function");

        asbind20::global(engine)
            .function("T@ GetResource<T>(const string& in)", &GetResource_Generic)
            .function("string formatFloat2(Float2 vec)", &formatVec2)
            .function("string formatFloat3(Float3 vec)", &formatVec3)
            .function("string formatFloat4(Float4 vec)", &formatVec4);

        asbind20::interface(engine, "ScriptInterface");
    }
}