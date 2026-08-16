# dbgWND

> ⚠️ Proyecto en desarrollo. Es un proyecto personal para aprender C++ y la API de Windows a bajo nivel — el código y las funcionalidades van cambiando según voy aprendiendo, no es una herramienta terminada ni pulida.

Programa de consola en C++ (Win32 API) que analiza la ventana activa del escritorio: título, PID y uso de memoria del proceso. Sin librerías externas.

## Qué hace

Cada segundo, el programa:
1. Detecta cuál es la ventana activa (`GetForegroundWindow`)
2. Muestra su título
3. Obtiene el PID del proceso dueño de esa ventana
4. Abre el proceso y consulta su consumo de memoria (working set y memoria privada)

## Requisitos

- Windows (usa la API Win32, no es multiplataforma)
- Compilador con soporte de `<windows.h>` y `<psapi.h>` (MSVC o MinGW)
- Si compilas con MinGW: enlazar `-luser32 -lpsapi`

## Compilar

```bash
g++ main.cpp -o dbgWND.exe -luser32 -lpsapi
```

## Uso

```bash
./dbgWND.exe
```

Cambia de ventana activa mientras el programa corre y verás cómo actualiza los datos cada segundo.

## Aprendizajes / conceptos tocados

- Punteros, direcciones de memoria (`&`, `*`) y por qué existen
- Reserva y liberación manual de memoria (`new[]` / `delete[]`) y fugas de memoria
- `wchar_t` vs `char`, y por qué Windows usa UTF-16 internamente
- El patrón "función que rellena una struct/buffer que tú le pasas por dirección", repetido en toda la Win32 API
- `reinterpret_cast` para compatibilidad entre structs extendidas (`_EX`) y sus versiones básicas
- Diferencia entre distintos contadores de memoria de un proceso (working set vs memoria privada) y por qué no siempre coinciden con el Administrador de Tareas

## Roadmap

- [ ] Dirección base del módulo principal del proceso (`EnumProcessModules` + `GetModuleInformation`)
- [ ] Menú interactivo en terminal (TUI) — de momento aparcado, se hará como v2 sin librerías externas tipo FTXUI
- [ ] Overlay visual con borde sobre la ventana + card de info (ImGui + DirectX 11)
