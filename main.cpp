#include <windows.h>
#include <iostream>
#include <string>
#include <winuser.h>
#include <psapi.h>
#include <processthreadsapi.h>


/*
typedef union _ULARGE_INTEGER {
	struct {
		DWORD LowPart;
		DWORD HighPart;
	} DUMMYSTRUCTNAME;
	struct {
		DWORD LowPart;
		DWORD HighPart;
	} u;
	ULONGLONG QuadPart;
} ULARGE_INTEGER;


typedef struct _FILETIME {
	DWORD dwLowDateTime;
	DWORD dwHighDateTime;
} FILETIME, *PFILETIME, *LPFILETIME;
*/


ULONGLONG fileTime_a_QuadWord(FILETIME * hora) { // el puntero de un filetime
	ULARGE_INTEGER uli; //uli tiene dos partes, una high y otra low las dos de 32 bits (igual que filetime) 
	//ularge_integer es una union no una struct, por eso tiene quadpart tienes la manera de ver low y high en 64 bits  
	uli.LowPart = hora->dwLowDateTime;
	uli.HighPart = hora->dwHighDateTime; // => (*hora).dwHighDateTime
	return uli.QuadPart;


}

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
			FILETIME tiempoactual;
			GetSystemTimeAsFileTime(&tiempoactual); // hora actual

			ULONGLONG inicio = fileTime_a_QuadWord(&horacreacionproceso);
			ULONGLONG ahora = fileTime_a_QuadWord(&tiempoactual);
			ULONGLONG inicioKernel = fileTime_a_QuadWord(&kerneltiempoproceso);
			ULONGLONG tiempopasado = ahora - inicio; // ticks - un tick = 100 nanoseg. -> seg


			
			DWORD prioridad = GetPriorityClass(proceso);

		
		double segundos = static_cast<double>(tiempopasado) / 10000000.0; // (1 segundo = 10,000,000 de intervalos)
		double horas = static_cast<double>(segundos) / 3600;
		double segundosKernel = static_cast<double>(inicioKernel) / 10000000.0;
		std::cout << "====[ DATOS PROGRAMA ]=======================" << std::endl;
		std::cout << "[1] Nombre de ventana: "; std::wcout << buffer << std::endl;
		std::cout << "[2] PID:" << pid << std::endl;
		std::cout << "====[ INFORMACION PROGRAMA ]=================" << std::endl;
		std::cout << "Direccion Base: 0x" << std::hex << dirBase << std::dec << std::endl; // imprimes texto normal, luego cambias el modo de impresion a hexadecimal, imprimes el valor de dirBase en hex y luego cambias otra vez a decimal
		std::cout << "RAM: " << infoMemoria.WorkingSetSize / 1024 / 1024 << " MB" << std::endl;
		std::cout << "Ruta ejecutable: "; std::wcout << bufferRutaArchivo << std::endl;
		std::cout << "Tiempo ejecucion: " << segundos << " seg. | " << horas << " horas [*] Tiempo Kernel: " << segundosKernel <<std::endl;
		std::cout << "Prioridad: 0x";
		switch (prioridad) {
		case 32:
			std::cout << std::hex << prioridad << std::dec << " | Prioridad Normal | ";
			break;
		case 256:
			std::cout << std::hex << prioridad << std::dec << " | Prioridad en Tiempo Real | ";
			break;
		case 64:
			std::cout << std::hex << prioridad << std::dec << " | En reposo | ";
			break;
		case 128: 
			std::cout << std::hex << prioridad << std::dec << " | Prioridad Alta | ";
			break;
		case 16384:
			std::cout << std::hex << prioridad << std::dec << " | Prioridad por debajo de lo Normal | ";
			break;
		case 32768:
			std::cout << std::hex << prioridad << std::dec << " | Prioridad por encima de lo normal | ";
			break;
		default:
			std::cout << std::hex << prioridad << std::dec << " | Desconocida | ";
			break;





		}
		std::cout << std::endl;


		

		
		
		delete[] buffer;
		CloseHandle(proceso);
		Sleep(1000);
		system("cls");

	}



}
