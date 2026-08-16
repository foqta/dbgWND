#include <windows.h>
#include <iostream>
#include <string>
#include <winuser.h>
#include <psapi.h>


int main() {
	// = wchar_t -> wide character 2 bytes

	bool ejecutando = true;

	while (ejecutando) {
		
		HWND ventanaActiva = GetForegroundWindow();
		int longVentana = GetWindowTextLengthW(ventanaActiva);
		wchar_t* buffer = new wchar_t[512]; // para cada new, un delete
		int tituloVentana = GetWindowTextW(ventanaActiva, buffer, longVentana + 1);
		DWORD pid = 0;
		DWORD PID = GetWindowThreadProcessId(ventanaActiva, &pid);
		HANDLE proceso = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);
		PROCESS_MEMORY_COUNTERS_EX infoMemoria;
		GetProcessMemoryInfo(proceso, reinterpret_cast<PROCESS_MEMORY_COUNTERS*>(&infoMemoria), sizeof(infoMemoria));

		std::cout << "RAM: " << infoMemoria.WorkingSetSize / 1024 / 1024 << " MB" << std::endl;
		std::wcout << buffer <<std::endl;
		std::cout << pid << std::endl;
		delete[] buffer;
		Sleep(1000);
		system("cls");

	}



}
