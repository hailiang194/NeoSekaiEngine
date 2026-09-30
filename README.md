# NeoSekaiEngine
Neo Sekai Game engine - my 2D game engine built base on what I know about game development. This game engine can run on Windows, Linux and Web platform

[![GithubAction](https://github.com/hailiang194/NeoSekaiEngine/actions/workflows/build_test.yaml/badge.svg?branch=main)](https://github.com/hailiang194/NeoSekaiEngine/actions/workflows/build_test.yaml/badge.svg?branch=main)

## Libraries
* raylib 6.0
* freetype 2.13.3
* GoogleTest 1.12.1 (For testing)

**Note:** See [THIRD_PARTY_NOTICES.md](THIRD_PARTY_NOTICES.md) for third party notices

## Requirements
* git
* [CMake](https://cmake.org/download/) 3.25 or newer
* C++17 or newer
* [Emscripten SDK](https://emscripten.org/docs/getting_started/downloads.html) (for Web platform) — or **Docker**, to build the Web platform through the pinned `emscripten/emsdk` image with no SDK installed
* **Node.js** and **npm** (only when building the Web UI, `BUILD_WEB_UI=ON`)
* **MSVC** or **Makefile**
* Doxygen and Graphviz

## Installation
### Clone this project
``` bash
git clone https://github.com/hailiang194/NeoSekaiEngine.git <project_name>
```

### Install SekaiEngine

#### Linux user
If you use **Debian** or **Redhat**, you need to run the pre-installer script in scripts

First, you need to give the script the permission to execute
``` bash
sudo chmod +x scripts/pre-installer/linux/<os>/preinstaller.sh
```
Then run that script
``` bash
./scripts/pre-installer/linux/<os>/preinstaller.sh
```
Which ```<os>``` is **```debian```** if you use **Debian** distro and **```redhat```** if you use **Fedora-based**
## Generate project
### Setup project

Download ```scripts/tool/tool.py```

Run the script
```sh
python3 tool.py [-h] [--engine_versions] [-y] [-v] [-p PATH] project_name engine_version
```
```
positional arguments:
  project_name          Name of project
  engine_version        set version of engine

options:
  -h, --help            show this help message and exit
  --engine_versions     Show all engine versions
  -y, --yes             Yes to all for every prompt
  -v, --verbose         Show verbose
  -p PATH, --path PATH  Project path
```

**Note**: You may need ```sudo``` for setup script in Linux. 

### Desktop platform
Create ```build``` folder and go to this folder 
``` bash
mkdir build
cd build
```
run CMake
``` bash
cmake ..
```
After that, It's going to generate a project for you
### Web platform
At the root project folder run
``` bash
emcmake cmake -S . -B build -DPLATFORM=Web -DBUILD_SHARED_LIBS=0
```
Add `-DBUILD_WEB_UI=ON` to also build and stage the Vue web UI (requires npm on the host):
``` bash
emcmake cmake -S . -B build -DPLATFORM=Web -DBUILD_SHARED_LIBS=0 -DBUILD_WEB_UI=ON
```

Each Web target is emitted as an **ES6 module** — `<Game>.js`, `<Game>.wasm` and the preloaded `<Game>.data` — into its own output folder `build/out/<Game>/` (no `.html` shell is generated). With `BUILD_WEB_UI=ON`, the generic UI from `engine/web-ui/` is built (vite) and merged into that same folder as `index.html` + assets. Desktop/Windows builds are unaffected and still land directly in `build/out/`.

**No Emscripten SDK installed?** The Docker wrapper does the exact same configure + build inside the pinned `emscripten/emsdk` image:
``` bash
./scripts/build-web.sh
```
## Build project
### Desktop platform
#### **MSVC:**
If you set ```MSBuild.exe``` as a PATH, Run ```MSBuild.exe``` the ```*.snl``` file in build folder.
```
MSBuild <Project-name>.sln
```
If you open by Visual Studio, keep going

#### **Makefile**

Go to build folder and run
```
make
```
### Web platform
Go to build folder and build the game target
``` bash
cmake --build build --target <Game>
```
## Start our game
### Desktop platformm
#### **MSVC**
If you build project by ```MSBuild.exe```, your execution file is in
```
<Project root folder>\build\out\<Configure Mode>\<Game-project-name>.exe
```
and if you use Visual Studio, just click **Run**
#### **Makefile**
Your execution file in in ```<Project root folder>\build\out``` folder
### Web platform
Go to the game's output folder
``` bash
cd build/out/<Game>
```
Run a static server in that folder (do **not** open `index.html` via `file://`, the module fetch needs http)
```
python -m http.server <port>
```
Go to the browser and access to
```
localhost:<port>/
```

## Logging

The engine ships a built-in logger (no extra setup) that prints to the console
by default on every platform:

```cpp
#include "SekaiEngine/Log.h"

SEKAI_INFO("Loading level %d", level);
SEKAI_WARNING("Texture %s not found", name);
SEKAI_ERROR("failed to connect: %s", error);
```

Levels are `SEKAI_TRACE`, `SEKAI_DEBUG`, `SEKAI_INFO`, `SEKAI_WARNING`,
`SEKAI_ERROR` and `SEKAI_FATAL`. The minimum level that reaches any sink is
`Info`; change it with `SekaiEngine::Log::SetLevel(...)`.

### Desktop: optional log file

Request a rotating log file from code or via the environment:

```cpp
SekaiEngine::Log::SetFile("sekai/log.txt");
```
or start the game with `SEKAI_LOG_FILE=path/to/sekai.log`. No file is ever
created unless one of these is used. The file rolls over when it grows (max ~1 MB,
5 kept).

### Web: browser console

On Web builds all messages go to the browser devtools console, mapped to
`console.log` / `console.warn` / `console.error` by severity. `SetFile()` is a
no-op on Web.

### Desktop: crash log

On desktop, the most recent log messages are captured in memory and, on a
fatal fault (segfault, aborted assert, trap, ...), a `crash-<pid>.log` file is
written to the working directory with the last lines intact — independent of
the file-sink setting.

### raylib

raylib's own internal logs are routed through this logger and respect the same
level threshold, so you will not see them printed twice.