# 🎮 Project64 Dedicated Server — AQZ Netplay

Servidor dedicado ligero para jugar **Nintendo 64 online** con baja latencia usando el plugin **AQZ Netplay** en Linux. Permite a múltiples jugadores conectarse a una sala persistente sin necesidad de emulador en el servidor.

> El plugin AQZ Netplay sincroniza los inputs de los controles entre jugadores conectados a la red. Funciona como una capa sobre un input plugin real — el plugin de netplay **no lee controles por sí mismo**, sino que se apoya en otro input plugin (como N-Rage o DirectInput) para leer el hardware.

---

## 📋 Tabla de Contenidos

- [Guía de Instalación (Ubuntu/Debian)](#%EF%B8%8F-guía-de-instalación-ubuntudebian)
- [Manual para Jugadores (Cliente)](#%EF%B8%8F-manual-para-jugadores-cliente)
- [Comandos de Chat](#-comandos-de-chat)
- [Advertencias Críticas](#%EF%B8%8F-advertencias-críticas)
- [Créditos](#-créditos)

---

## 🛠️ Guía de Instalación (Ubuntu/Debian)

Instrucciones para el **administrador del servidor**.

### Paso 1 — Instalar dependencias

```bash
sudo apt update
sudo apt install git build-essential make g++ -y
```

### Paso 2 — Clonar y compilar

```bash
# 1. Clonar el repositorio (--recursive para incluir submódulos)
git clone --recursive https://github.com/GSaucedoA/NetplayInputPlugin.git

# 2. Acceder al código fuente (estructura de doble carpeta)
cd NetplayInputPlugin/NetplayInputPlugin

# 3. Compilar
make
```

El binario se genera en el directorio `build/gcc/`.

### Paso 3 — Ejecutar el servidor

**Modo prueba** (se detiene al cerrar la terminal):

```bash
./build/gcc/netplay_server
```

**Modo persistente** (sigue corriendo en segundo plano):

```bash
nohup ./build/gcc/netplay_server > servidor.log 2>&1 &
```

> Para ver los logs en tiempo real: `tail -f servidor.log`

### Paso 4 — Configuración de red

El servidor utiliza el **puerto 6400 (TCP)** por defecto. Asegúrate de que esté abierto en el firewall:

```bash
# UFW (Ubuntu)
sudo ufw allow 6400/tcp

# iptables
sudo iptables -A INPUT -p tcp --dport 6400 -j ACCEPT
```

Si el servidor está detrás de un router, es necesario configurar **port forwarding** del puerto TCP `6400` hacia la IP local de la máquina.

---

## 🕹️ Manual para Jugadores (Cliente)

### Requisitos

- **Project64** (versión 2.x o superior).
- **Plugin AQZ Netplay** instalado y configurado como **Input Plugin** en Project64.
- Un **input plugin real** (como N-Rage o DirectInput) configurado dentro de AQZ Netplay para la lectura del control/teclado.

### Conexión

1. Abre **Project64** y carga la ROM que se va a jugar.
2. En la ventana de chat del plugin AQZ Netplay, usa el comando de conexión:
   ```
   /join <IP_DEL_SERVIDOR>:6400
   ```
   > Pide la IP al administrador del servidor.
3. Asigna tu control con `/map` e inicia la partida con `/start`.

> **Tip:** La ventana del plugin incluye una barra de botones (Host, Host UPnP, Start, Sync Saves, Room Check, Mode, Golf, AutoLag) que ejecutan los comandos principales sin necesidad de escribirlos.

---

## 💬 Comandos de Chat

| Comando | Descripción |
|---|---|
| `/join <host>:6400[/room]` | Unirse al servidor. Opcionalmente se puede especificar una sala (ej. `/join x.x.x.x:6400/sala1`). |
| `/host [port]` | Crear un servidor privado en el puerto indicado (requiere port forwarding en el router). |
| `/hostupnp [port]` | Crear un servidor con mapeo automático de puertos via UPnP (no requiere port forwarding manual). |
| `/name <Nick>` | Cambiar tu nombre de jugador. Se recuerda entre sesiones. |
| `/map <local> <netplay>` | Asignar control local a un slot de jugador. Ej: `/map 1 2` → tu control 1 es el Jugador 2. |
| `/start` | Iniciar la partida (todos los jugadores deben estar listos). |
| `/lag <ms>` | Establecer lag manual en milisegundos. |
| `/autolag` | Activar ajuste de lag automático. |
| `/golf` | Modo turnos — cero lag sin importar la conexión. Se activa automáticamente en Mario Golf. |
| `/favorite <host>` | Guardar un servidor en la lista de favoritos. |
| `/unfavorite <host>` | Quitar un servidor de la lista de favoritos. |
| `/savesync [nombre]` | Sincronizar archivos de guardado con otro jugador (o todos si no se especifica nombre). |
| `/roomcheck` | Verificar que los saves de todos los jugadores coincidan. |
| `/mode` | Alternar entre modo CLIENT (lag compensado) y HOST (input authority del host). |
| `/hia_rate <N>` | Establecer la frecuencia del modo HOST en Hz (por defecto 60). |

---

## ⚠️ Advertencias Críticas

1. **MISMA ROM:** Todos los jugadores deben tener exactamente la **misma ROM** (mismo archivo, mismo hash). Cualquier diferencia causará desyncs.
2. **MISMO SAVE FILE:** Todos deben tener el **mismo archivo de guardado** en la carpeta `Save` de Project64. Si un jugador tiene un save diferente o no lo tiene, la emulación se desincronizará.
3. **Conexión por cable:** Se recomienda **Ethernet (cable)** en lugar de Wi-Fi. Como indica la documentación oficial: *"If anyone is using Wi-Fi, the game will probably stutter a lot."*

---

## 📝 Créditos

- **Autor original del plugin:** [CoderTimZ](https://github.com/CoderTimZ) — Creador de [AQZ Netplay Input Plugin](https://www.play64.com/netplay-plugin/).
- **Mantenimiento de este fork:** [GSaucedoA](https://github.com/GSaucedoA)
