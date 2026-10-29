#pragma once

#include <map>

#include <d3d11.h>
#include <d3dx11.h>
#include <ImGui/imgui.h>

#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"d3dx11.lib")
#define DIRECTINPUT_VERSION 0x0800

class Options
{
public:
	ImFont* InterRegular;
	ImFont* InterRegular_small;
};

extern Options opt;
Options opt;  // Definition of the global variable `opt`