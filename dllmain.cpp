#include <Windows.h>
#include <thread>

void FakeEntry(HMODULE hModule) {
  AllocConsole();
  FILE *file = nullptr;

  freopen_s(&file, "CONOUT$", "w", __acrt_iob_func(1));
  
  printf("Starting at %p",(void*)FakeEntry);

  while (!GetAsyncKeyState(VK_END)) {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }

  FreeConsole();
  fclose(file);
  FreeLibraryAndExitThread(hModule, 0);
}

int __stdcall DllMain(HMODULE hModule, DWORD ul_reason_for_call,
                      LPVOID lpReserved) {
  if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
    auto thread = CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)FakeEntry,
                               (HMODULE)hModule, 0, 0);

    if (thread)
      CloseHandle(thread);

    return TRUE;
  }

  return TRUE;
}
