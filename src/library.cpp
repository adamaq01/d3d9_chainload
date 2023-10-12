#include <string>
#include <fstream>
#include <windows.h>
#include <filesystem>

TCHAR module_path[MAX_PATH] = {};
HINSTANCE__ *real_d3d9 = nullptr;

void chainload_libraries() {
    auto library_list = std::wifstream(
            // Strip the filename part from the module path, then add 'chainload.txt'.
            std::filesystem::path(module_path).remove_filename().append("chainload.txt")
    );

    if (library_list.is_open()) {
        // Read filenames from each line and call LoadLibrary.
        for (std::wstring line; std::getline(library_list, line);) {
            // Treat lines starting with '#' as comments.
            if (line.empty() || line.starts_with(L"#"))
                continue;

            LoadLibrary(line.c_str());
        }
    }
}

extern "C" __declspec(dllexport) class IDirect3D9 *Direct3DCreate9(UINT SDKVersion) {
    chainload_libraries();
    return reinterpret_cast<decltype(&Direct3DCreate9)>(GetProcAddress(real_d3d9, "Direct3DCreate9"))(SDKVersion);
}

extern "C" __declspec(dllexport) HRESULT Direct3DCreate9Ex(UINT SDKVersion, OUT void *unknown) {
    chainload_libraries();
    return reinterpret_cast<decltype(&Direct3DCreate9Ex)>(GetProcAddress(real_d3d9, "Direct3DCreate9Ex"))(SDKVersion,
                                                                                                          unknown);
}

extern "C" __declspec(dllexport) int WINAPI D3DPERF_BeginEvent(DWORD col, LPCWSTR wszName) {
    return reinterpret_cast<decltype(&D3DPERF_BeginEvent)>(GetProcAddress(real_d3d9, "D3DPERF_BeginEvent"))(col,
                                                                                                            wszName);
}

extern "C" __declspec(dllexport) int WINAPI D3DPERF_EndEvent(void) {
    return reinterpret_cast<decltype(&D3DPERF_EndEvent)>(GetProcAddress(real_d3d9, "D3DPERF_EndEvent"))();
}

extern "C" __declspec(dllexport) DWORD WINAPI D3DPERF_GetStatus(void) {
    return reinterpret_cast<decltype(&D3DPERF_GetStatus)>(GetProcAddress(real_d3d9, "D3DPERF_GetStatus"))();
}

extern "C" __declspec(dllexport) BOOL WINAPI D3DPERF_QueryRepeatFrame(void) {
    return reinterpret_cast<decltype(&D3DPERF_QueryRepeatFrame)>(GetProcAddress(real_d3d9,
                                                                                "D3DPERF_QueryRepeatFrame"))();
}

extern "C" __declspec(dllexport) void WINAPI D3DPERF_SetMarker(DWORD col, LPCWSTR wszName) {
    reinterpret_cast<decltype(&D3DPERF_SetMarker)>(GetProcAddress(real_d3d9, "D3DPERF_SetMarker"))(col, wszName);
}

extern "C" __declspec(dllexport) int WINAPI D3DPERF_SetOptions(DWORD dwOptions) {
    return reinterpret_cast<decltype(&D3DPERF_SetOptions)>(GetProcAddress(real_d3d9, "D3DPERF_SetOptions"))(dwOptions);
}

extern "C" __declspec(dllexport) void WINAPI D3DPERF_SetRegion(DWORD col, LPCWSTR wszName) {
    reinterpret_cast<decltype(&D3DPERF_SetRegion)>(GetProcAddress(real_d3d9, "D3DPERF_SetRegion"))(col, wszName);
}

extern "C" __declspec(dllexport) void DebugSetMute(void) {
    reinterpret_cast<decltype(&DebugSetMute)>(GetProcAddress(real_d3d9, "DebugSetMute"))();
}

extern "C" __declspec(dllexport) void Direct3DShaderValidatorCreate9(void) {
    reinterpret_cast<decltype(&Direct3DShaderValidatorCreate9)>(GetProcAddress(real_d3d9,
                                                                               "Direct3DShaderValidatorCreate9"))();
}

BOOL APIENTRY DllMain(HMODULE module, DWORD reason, LPVOID) {
    if (reason == DLL_PROCESS_ATTACH) {
        // Store the filename of this library for later.
        GetModuleFileName(module, module_path, MAX_PATH);

        // Try to find the real d3d9.dll library in the system directory.
        TCHAR system_dir[MAX_PATH];
        GetSystemDirectory(system_dir, MAX_PATH);

        // Load the real d3d9.dll, get the 'Direct3DCreate9' function, call it, and return the result.
        real_d3d9 = LoadLibrary(std::filesystem::path(system_dir).append("d3d9.dll").c_str());
    }

    return TRUE;
}
