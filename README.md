# dbgWND

> ⚠️ Esto está a medio hacer. Lo estoy usando para aprender C++ y la API de Windows desde cero, así que el código cambia según voy entendiendo cosas nuevas — no esperes nada pulido todavía.

Un programa de consola que mira qué ventana tienes activa en cada momento y te dice cosas de ella: título, PID y cuánta memoria está usando el proceso. Todo con la API de Windows a pelo, sin librerías externas de por medio.

## Qué hace

Cada segundo el programa:
1. Mira qué ventana tienes en primer plano (`GetForegroundWindow`)
2. Saca su título
3. Saca el PID del proceso que la ha creado
4. Abre ese proceso y le pregunta cuánta memoria está usando (working set y memoria privada)
5. Saca la dirección base de su módulo principal y la muestra en hexadecimal

## Requisitos

- Windows, porque es Win32 puro y no tiene nada de multiplataforma
- Un compilador que entienda `<windows.h>` y `<psapi.h>` (MSVC o MinGW me sirven)
- Con MinGW hay que enlazar `-luser32 -lpsapi` a mano

## Compilar

```bash
g++ main.cpp -o dbgWND.exe -luser32 -lpsapi
```

## Uso

```bash
./dbgWND.exe
```

Déjalo corriendo y ve cambiando de ventana — cada segundo actualiza los datos con la que tengas activa en ese momento.

## Cosas que he ido aprendiendo con esto

- Qué son los punteros y las direcciones de memoria (`&`, `*`) de verdad, más allá de la teoría
- Por qué hay que liberar a mano lo que reservas con `new[]` (y cómo se nota cuando no lo haces)
- La diferencia entre `wchar_t` y `char`, y por qué Windows va todo en UTF-16 por dentro
- Un patrón que se repite todo el rato en la Win32 API: le pasas a la función una caja vacía por dirección y ella la rellena
- `reinterpret_cast` para cuando una función pide una struct básica pero tú le pasas su versión extendida
- Que "memoria usada por un proceso" no es un solo número — el Administrador de Tareas y `GetProcessMemoryInfo` no siempre cuentan lo mismo

## Lo que falta

- [ ] Sacar la dirección base del módulo principal (`EnumProcessModules` + `GetModuleInformation`)
- [ ] Un menú interactivo en terminal — de momento aparcado, caerá en la v2 sin tirar de librerías tipo FTXUI
- [ ] El overlay de verdad: borde sobre la ventana + card con la info encima (ImGui + DirectX 11)
