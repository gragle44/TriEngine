#pragma once

#include "ScriptEngine.h"

#include <string>

namespace ScriptUtils
{
    template <class Stream>
    void printEnumList(const asIScriptEngine* engine, Stream& stream);

    template <class Stream>
    void printClassTypeList(const asIScriptEngine* engine, Stream& stream);

    template <class Stream>
    void printGlobalFunctionList(const asIScriptEngine* engine, Stream& stream);

    template <class Stream>
    void printGlobalPropertyList(const asIScriptEngine* engine, Stream& stream);

    template <class Stream>
    void printGlobalTypedef(const asIScriptEngine* engine, Stream& stream);

    void GenerateScriptPredefined(const asIScriptEngine* engine, const std::string& path);
}