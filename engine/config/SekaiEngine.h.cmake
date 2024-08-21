/**
 * @file SekaiEngine.h
 * @author Luong The Hai (hailuongthe2000@gmail.com)
 * @brief The header which includes all the needed headers for game engine
 * @version @PROJECT_VERSION_MAJOR@.@PROJECT_VERSION_MINOR@
 * @date @RELEASE_DATE@
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef _SEKAI_ENGINE_SEKAI_ENGINE_H_
#define _SEKAI_ENGINE_SEKAI_ENGINE_H_

@INCLUDE_PART@
#include "version.h"

/** 
 * \mainpage NeoSekaiEngine
 * \section intro_sec Introduction
 * Neo Sekai Game engine - my 2D game engine built base on what I know about game development. 
 * This game engine can run on Windows, Linux and Web platform
 * \section lib_sec Libraries
 * * raylib 4.2.0
 * \section req_sec Requirements
 * * git
 * * [CMake](https://cmake.org/download/)
 * * [Emscripten SDK](https://emscripten.org/docs/getting_started/downloads.html) (for Web platform)
 * * **MSVC** or **Makefile**
 * * Doxygen and Graphviz
 * \section install_sec Installation
 * \subsection install_sec_clone_subsec Clone this project
 * ``` bash
 * git clone https://github.com/hailiang194/NeoSekaiEngine.git <project_name>
 * ```
 * \subsection install_sec_install_subsec Install SekaiEngine
 * \subsubsection install_sec_install_subsec_linux Linux user
 * If you use **Debian** or **Redhat**, you need to run the pre-installer script in scripts
 * First, you need to give the script the permission to execute
 * ``` bash
 * sudo chmod +x scripts/pre-installer/linux/<os>/preinstaller.sh
 * ```
 * Then run that script
 * ``` bash
 * ./scripts/pre-installer/linux/<os>/preinstaller.sh
 * ```
 * Which ```<os>``` is **debian** if you use **Debian** distro and **redhat** if you use **Fedora-based**
 * \section generate_sec Generate project
 * \subsection generate_sec_setup_subsec Setup project
 * Download ```scripts/tool/tool.py```
 * 
 * Run the script
 * ```sh
 * python3 tool.py [-h] [--engine_versions] [-y] [-v] [-p PATH] project_name engine_version
 * ```
 * ```
 * positional arguments:
 *  project_name          Name of project
 *  engine_version        set version of engine
 * 
 * options:
 *  -h, --help            show this help message and exit
 *  --engine_versions     Show all engine versions
 *  -y, --yes             Yes to all for every prompt
 *  -v, --verbose         Show verbose
 *  -p PATH, --path PATH  Project path
 * ```
 *
 * **Note**: You may need ```sudo``` for setup script in Linux. 
 * \subsection generate_sec_desktop_subsec Desktop platform
 * Create ```build``` folder and go to this folder 
 * ``` bash
 * mkdir build
 * cd build
 * ```
 * run CMake
 * ``` bash
 * cmake ..
 * ```
 * After that, It's going to generate a project for you
 * \subsection genarate_sec_web_subsec Web platform
 * At the root project folder run
 * ``` bash
 * emcmake cmake -S . -B build -DPLATFORM=Web -DBUILD_SHARED_LIBS=0
 * ```
 * \section build_sec Build project
 * \subsection build_sec_desktop_subsec Desktop platform
 * \subsubsection build_sec_desktop_subsec_msvc MSVC
 * If you set ```MSBuild.exe``` as a PATH, Run ```MSBuild.exe``` the ```*.snl``` file in build folder.
 * ```
 * MSBuild <Project-name>.sln
 * ```
 * If you open by Visual Studio, keep going
 * \subsubsection build_sec_desktop_makefile Makefile
 * Go to build folder and run
 * ```
 * make
 * ```
 * \subsection build_sec_web Web platform
 * Go to build folder
 * ``` bash
 * cd build
 * ```
 * Run makefile
 * ``` bash
 * make PLATFORM=PLATFORM_WEB -B
 * ```
 * \section start_sec Start our game
 * \subsection start_sec_desktop_subsec Desktop platform
 * \subsubsection start_sec_desktop_subsec_msvc MSVC
 * If you build project by ```MSBuild.exe```, your execution file is in
 * ```
 * <Project root folder>\build\out\<Configure Mode>\<Game-project-name>.exe
 * ```
 * and if you use Visual Studio, just click **Run**
 * \subsubsection start_sec_desktop_subsec_makefile Makefile
 * Your execution file in in ```<Project root folder>\build\out``` folder
 * \subsection start_sec_web_subsec Web platform
 * Go to ```<Project root folder>\build\bin``` folder
 * Run python server
 * ```
 * python -m http.server <port>
 * ```
 * Go to the browser and access to
 * ```
 * localhost:<port>/<Game-project-name>.html
 * ```
 */





#endif //!_SEKAI_ENGINE_SEKAI_ENGINE_H_