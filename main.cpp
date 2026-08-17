#include <windows.h>
#include <iostream>
#include <string>
#include <winuser.h>
#include <psapi.h>
#include <processthreadsapi.h>

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
		HMODULE arrayModulos[1024]; // reservas 1024 espacios de un array 0-1023 creo, osea un margen que le damos
		DWORD bytesNeces; // variable donde almacena la funcion los bytes que realmente ha usado
		EnumProcessModules(proceso, arrayModulos, sizeof(arrayModulos), &bytesNeces); //in, out, in, out
		MODULEINFO info;
		GetModuleInformation(proceso, arrayModulos[0], &info, sizeof(info));
		
		uintptr_t dirBase = reinterpret_cast<uintptr_t>(info.lpBaseOfDll); // reinterpret_cast<TIPO_AL_QUE_QUIERO_CONVERTIR>(LO_QUE_QUIERO_CONVERTIR)
		wchar_t bufferRutaArchivo[MAX_PATH]; // MAX_PATH justo pensado para rutas de archivo (260 espacios¿?)
		// ruta de exe
		GetModuleFileNameEx(proceso, NULL, bufferRutaArchivo, sizeof(bufferRutaArchivo) / sizeof(bufferRutaArchivo[0]));
		// tiempo ejecutado
		FILETIME horacreacionproceso, horasalidaproceso, kerneltiempoproceso, usertiempoproceso;

		GetProcessTimes(proceso, &horacreacionproceso, &horasalidaproceso, &kerneltiempoproceso, &usertiempoproceso);
		std::cout << "=================================================" << std::endl;
		std::cout << "Nombre de ventana: "; std::wcout << buffer << std::endl;
		std::cout << "PID:" << pid << std::endl;
		std::cout << "=================================================" << std::endl;
		std::cout << "Direccion Base: 0x" << std::hex << dirBase << std::dec << std::endl; // imprimes texto normal, luego cambias el modo de impresion a hexadecimal, imprimes el valor de dirBase en hex y luego cambias otra vez a decimal
		std::cout << "RAM: " << infoMemoria.WorkingSetSize / 1024 / 1024 << " MB" << std::endl;
		std::cout << "Ruta ejecutable: ";
		std::wcout << bufferRutaArchivo << std::endl;

		
		
		delete[] buffer;
		CloseHandle(proceso);
		Sleep(1000);
		system("cls");

	}



}
