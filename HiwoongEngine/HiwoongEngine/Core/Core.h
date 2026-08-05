#pragma once

#pragma warning(disable: 4251)

#define DLLEXPORT __declspec(dllexport)
#define DLLIMPORT __declspec(dllimport)

//This macro can export and import Engine to our Project
#if defined(ENGINE_BUILD_DLL)
#define Hiwoong_API DLLEXPORT
#else
#define Hiwoong_API DLLIMPORT
#endif