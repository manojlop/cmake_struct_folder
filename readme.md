# CMake Project Setup Guide

This README provides a comprehensive guide to setting up and using CMake for C++ projects. It covers basic and advanced configurations to help you organize, build, and manage your C++ codebase effectively.

## Table of Contents

- [Introduction](#introduction)
- [Prerequisites](#prerequisites)
- [Basic Project Structure](#basic-project-structure)
- [Basic CMake Configuration](#basic-cmake-configuration)
- [Creating C++ Files](#creating-c-files)
- [VS Code Configuration](#vs-code-configuration)
- [Building and Running](#building-and-running)
- [Advanced CMake Configuration](#advanced-cmake-configuration)
- [Common CMake Commands](#common-cmake-commands)
- [Best Practices](#best-practices)
- [Troubleshooting](#troubleshooting)

## Introduction

CMake is a powerful cross-platform build system generator that simplifies the process of building, testing, and packaging C++ projects. This guide will help you understand how to set up and use CMake for your projects, from basic to more advanced configurations.

## Prerequisites

Before getting started, ensure you have the following tools installed:

### Installing necessary tools

For Linux (Debian/Ubuntu):

```bash
sudo apt install gcc # Or clang
sudo apt install cmake
```

For macOS (using Homebrew):

```bash
brew install gcc
brew install cmake
```

For Windows:
- Install Visual Studio with C++ development tools, or
- Install MinGW/MSYS2, and add CMake

Verify your installation:

```bash
g++ --version  # or clang++ --version
cmake --version
```

## Basic Project Structure

A well-organized CMake project typically follows this structure:

```
my_project/
├── CMakeLists.txt       # Main CMake configuration file
├── include/             # Header files (.h, .hpp)
│   └── my_lib.h
├── src/                 # Source files (.cpp, .cc)
│   ├── main.cpp
│   └── my_lib.cpp
├── test/                # Test files (optional)
├── lib/                 # Libraries (optional)
├── build/               # Build directory (created during build)
└── .vscode/             # VS Code configuration (if using VS Code)
    ├── settings.json
    └── launch.json
```

This structure separates interfaces (headers) from implementations (source files), making your project easier to navigate and maintain.

## Basic CMake Configuration

Here's a simple `CMakeLists.txt` file to get started:

```cmake
# Minimum required version of CMake
cmake_minimum_required(VERSION 3.10)

# Project name and version
project(MyProject VERSION 1.0)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Creates target 
# Either executable add_executable() or library (add_library())
add_executable(MyProject src/main.cpp src/my_lib.cpp)

# Add include directories
target_include_directories(MyProject PUBLIC include)
```

This configuration:
1. Sets the minimum CMake version requirement
2. Names your project and sets its version
3. Specifies the C++ standard to use
4. Creates an executable target from your source files
5. Tells the compiler where to find your header files

## Creating C++ Files

Let's create some basic C++ files to demonstrate the CMake build process:

### Header file (include/my_lib.h)

```cpp
// include/my_lib.h
#ifndef MY_LIB_H
#define MY_LIB_H

// A simple function declaration
int add(int a, int b);

#endif // MY_LIB_H
```

### Implementation file (src/my_lib.cpp)

```cpp
// src/my_lib.cpp
#include "my_lib.h"

// Implementation of our function
int add(int a, int b) {
    return a + b;
}
```

### Main file (src/main.cpp)

```cpp
// src/main.cpp
#include <iostream>
#include "my_lib.h"

int main() {
    int result = add(5, 3);
    std::cout << "The result is: " << result << std::endl;
    return 0;
}
```

## VS Code Configuration

If you're using Visual Studio Code, configure it for CMake with these files:

### .vscode/settings.json

```json
{
    "cmake.configureOnOpen": true,
    "cmake.buildDirectory": "${workspaceFolder}/build"
}
```

### .vscode/launch.json

```json
{
    "version": "0.2.0",
    "configurations": [
        {
            "name": "C++ Debug",
            "type": "cppdbg",
            "request": "launch",
            "program": "${command:cmake.launchTargetPath}",
            "args": [],
            "stopAtEntry": false,
            "cwd": "${workspaceFolder}",
            "environment": [],
            "externalConsole": false,
            "MIMode": "gdb",
            "setupCommands": [
                {
                    "description": "Enable pretty-printing for gdb",
                    "text": "-enable-pretty-printing",
                    "ignoreFailures": true
                }
            ]
        }
    ]
}
```

These configurations:
1. Tell VS Code to configure CMake when you open the project
2. Set the build directory to `build/` in your project folder
3. Configure debugging to use the CMake target

## Building and Running

### Using VS Code

1. Open VS Code and open the folder containing your project
2. Click on the CMake icon in the sidebar
3. Configure the project by clicking "Configure" in the CMake Tools extension
4. Select your compiler when prompted
5. Build the project by clicking "Build" in the CMake Tools extension
6. Run the project by clicking "Run"/"Launch" in the CMake Tools extension

### Using Command Line

```bash
# Create a build directory
mkdir -p build && cd build

# Configure the project
cmake ..

# Build the project
cmake --build .

# Run the executable
./bin/MyProject  # or ./MyProject depending on your configuration
```

## Advanced CMake Configuration

For more complex projects, here's a more comprehensive `CMakeLists.txt` file:

```cmake
# Minimum required version of CMake
cmake_minimum_required(VERSION 3.10)

# Project name and version
project(MyProject VERSION 1.0)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Set output directories for better organization
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Find all .cpp files in the src directory
file(GLOB_RECURSE SOURCE_FILES "src/*.cpp")

# Check if lib directory exists and has source files
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/lib")
    file(GLOB_RECURSE LIB_SOURCE_FILES "lib/*.cpp")
    if(LIB_SOURCE_FILES)
        # Create a library from the lib sources
        add_library(MyLibrary STATIC ${LIB_SOURCE_FILES})
        target_include_directories(MyLibrary PUBLIC include)
        
        # Apply the same compile options to the library
        target_compile_options(MyLibrary PRIVATE 
            -Wall
            -Wextra
            -pedantic
        )
        
        # Define preprocessor macros for the library
        target_compile_definitions(MyLibrary PRIVATE 
            __FLAG
        )
    endif()
endif()

# Add the executable with all source files
add_executable(MyProject ${SOURCE_FILES})

# Add include directories
target_include_directories(MyProject PUBLIC include)

# Link with library if it exists
if(TARGET MyLibrary)
    target_link_libraries(MyProject PRIVATE MyLibrary)
endif()

# Add compile options to a specific target
target_compile_options(MyProject PRIVATE 
    -Wall               # Enable all warnings
    -Wextra             # Enable extra warnings
    -pedantic           # Ensure compliance with language standards
)

# Define preprocessor macros
target_compile_definitions(MyProject PRIVATE 
    EXAMPLE_DEFINE
)

# Apply the -save-temps flag globally as in your original file
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -save-temps -masm=intel")

# Add optimization level options
# # Optimization Levels
# # No optimization - easiest to understand
# set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")
# # Moderate optimization
# set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2")
# # Aggressive optimization - hardest to understand
# set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")

# Add test support if test directory exists
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/test")
    file(GLOB_RECURSE TEST_SOURCE_FILES "test/*.cpp")
    if(TEST_SOURCE_FILES)
        enable_testing()
        add_executable(MyTests ${TEST_SOURCE_FILES})
        target_include_directories(MyTests PRIVATE include)
        
        # Apply the same compile options to tests
        target_compile_options(MyTests PRIVATE 
            -Wall
            -Wextra
            -pedantic
        )
        
        # Define the same preprocessor macros for tests
        target_compile_definitions(MyTests PRIVATE 
            EXAMPLE_DEFINE
            TESTING
        )
        
        # Link with library if it exists
        if(TARGET MyLibrary)
            target_link_libraries(MyTests PRIVATE MyLibrary)
        endif()
        
        # Add each test as a CTest test
        add_test(NAME AllTests COMMAND MyTests)
    endif()
endif()

# Add spike (experimental) executables support
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/spike")
    file(GLOB SPIKE_SOURCE_DIRS "spike/*")
    foreach(SPIKE_DIR ${SPIKE_SOURCE_DIRS})
        if(IS_DIRECTORY ${SPIKE_DIR})
            get_filename_component(SPIKE_NAME ${SPIKE_DIR} NAME)
            file(GLOB SPIKE_SOURCES "${SPIKE_DIR}/*.cpp")
            if(SPIKE_SOURCES)
                add_executable(spike_${SPIKE_NAME} ${SPIKE_SOURCES})
                target_include_directories(spike_${SPIKE_NAME} PRIVATE include)
                
                # Apply the same compile options to spike executables
                target_compile_options(spike_${SPIKE_NAME} PRIVATE 
                    -Wall
                    -Wextra
                    -pedantic
                )
                
                # Define the same preprocessor macros for spike executables
                target_compile_definitions(spike_${SPIKE_NAME} PRIVATE 
                    EXAMPLE_DEFINE
                    EXPERIMENTAL
                )
                
                # Link with library if it exists
                if(TARGET MyLibrary)
                    target_link_libraries(spike_${SPIKE_NAME} PRIVATE MyLibrary)
                endif()
            endif()
        endif()
    endforeach()
endif()

# Add an option to easily enable/disable features from command line with cmake -DENABLE_FEATURE
option(ENABLE_FEATURE "Enable a specific feature" OFF)
if(ENABLE_FEATURE)
    target_compile_definitions(MyProject PRIVATE FEATURE_ENABLED)
    if(TARGET MyLibrary)
        target_compile_definitions(MyLibrary PRIVATE FEATURE_ENABLED)
    endif()
endif()
```

This advanced configuration provides:

1. Organized output directories for binaries and libraries
2. Automatic discovery of source files using `file(GLOB_RECURSE ...)`
3. Conditional creation of libraries from source files in the `lib/` directory
4. Compiler warnings and standard compliance checks
5. Preprocessor macro definitions
6. Testing support with CTest
7. Support for experimental code in a `spike/` directory
8. Command-line options to enable/disable features

## Common CMake Commands

Here are some frequently used CMake commands and their purposes:

- `cmake_minimum_required(VERSION x.y)`: Sets the minimum required CMake version
- `project(Name VERSION x.y)`: Sets the project name and version
- `add_executable(target_name source1.cpp source2.cpp)`: Creates an executable target
- `add_library(library_name STATIC|SHARED|MODULE source1.cpp source2.cpp)`: Creates a library
- `target_include_directories(target_name PUBLIC|PRIVATE|INTERFACE dir1 dir2)`: Adds include directories
- `target_link_libraries(target_name PUBLIC|PRIVATE|INTERFACE lib1 lib2)`: Links libraries to a target
- `target_compile_options(target_name PUBLIC|PRIVATE|INTERFACE options)`: Adds compiler options
- `target_compile_definitions(target_name PUBLIC|PRIVATE|INTERFACE definitions)`: Adds preprocessor definitions
- `set(VARIABLE value)`: Sets a variable
- `option(OPTION_NAME "Description" ON|OFF)`: Adds a boolean option
- `file(GLOB|GLOB_RECURSE var_name pattern1 pattern2)`: Finds files matching patterns
- `enable_testing()`: Enables testing
- `add_test(NAME test_name COMMAND command_to_run)`: Adds a test

## Best Practices

1. **Avoid `file(GLOB)` in production code**: While convenient, it doesn't automatically pick up new files. Explicitly list source files for better control.

2. **Use `target_*` commands instead of global properties**: Target-specific commands provide better encapsulation and prevent unexpected side effects.

3. **Properly scope include directories and libraries**:
   - `PRIVATE`: Used only by the target and not propagated to dependents
   - `PUBLIC`: Used by the target and propagated to dependents
   - `INTERFACE`: Not used by the target but propagated to dependents

4. **Use out-of-source builds**: Always build in a separate directory (e.g., `build/`) to keep your source tree clean.

5. **Set reasonable defaults**: Ensure your CMake configuration works out-of-the-box with sensible defaults.

6. **Document your CMake files**: Add comments explaining non-obvious choices or configurations.

7. **Use modern CMake practices**: Prefer target-based approaches rather than setting global variables.

## Troubleshooting

### Common Issues and Solutions

1. **"No rule to make target..."**
   - Check that all source files listed in `add_executable()` or `add_library()` exist
   - Verify file paths are correct (CMake is case-sensitive)

2. **"Cannot find -l..."**
   - A required library is missing. Install it or check the path.

3. **Include directory problems**
   - Ensure include directories are correctly specified with `target_include_directories()`
   - Check include path syntax in your code

4. **Linker errors**
   - Ensure all required libraries are linked with `target_link_libraries()`
   - Check for circular dependencies

5. **CMake cache problems**
   - Delete the `CMakeCache.txt` file and `build/` directory and reconfigure

For more help, consult the [official CMake documentation](https://cmake.org/documentation/).

## Using Python Scripts in Your CMake Project

In addition to CMake, you can use Python scripts to automate tasks such as cleaning the build directory, configuring the project, building, and running executables. Below are some Python scripts that can be integrated into your CMake workflow.

1. Clean Build Directory (clean_build.py)

This script will clean the build directory by removing it entirely, which can be useful for fresh builds.

```python
#!/usr/bin/env python3

import subprocess

subprocess.Popen(['rm', '-rf', 'build'])
```

2.  Build Project with Custom Options (build_project.py)

This script configures and builds the project using CMake, with options to define compile definitions, enable quick compilation, or enable debug mode.

```python
#!/usr/bin/env python3

import argparse
import subprocess
import os

def run_command(command):
    """Helper function to run a shell command."""
    print(f"Running: {' '.join(command)}")
    result = subprocess.run(command, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    if result.returncode != 0:
        print(f"Error: {result.stderr.decode()}")
        exit(result.returncode)

def main():
    parser = argparse.ArgumentParser(description="Compile and build project using CMake.")
    parser.add_argument("-d", "--define", help="Set compile definitions for MyProject", default=None)
    parser.add_argument("-q", "--quick", help="Enable quick compilation mode", action="store_true")
    parser.add_argument("-t", "--target", help="Target filename for quick compilation (used with -q)", default=None)
    parser.add_argument("-dbg", "--debug", help="Enable debug mode with debug flags", action="store_true")
    
    args = parser.parse_args()
    build_dir = "build"
    os.makedirs(build_dir, exist_ok=True)
    
    cmake_command = ["cmake", "-B", build_dir, "-S", "."]
    if args.define:
        cmake_command.append(f"-DEXAMPLE_DEFINE={args.define}")
    
    if args.quick:
        if not args.target:
            print("Error: Quick mode (-q) requires a target filename (-t).")
            exit(1)
        cmake_command.append("-DQ=ON")
        cmake_command.append(f"-DQUICK={args.target}")
    
    if args.debug:
        cmake_command.append("-DCMAKE_BUILD_TYPE=Debug")
    
    # Run CMake configuration
    run_command(cmake_command)
    
    # Build the project
    build_command = ["cmake", "--build", build_dir]
    run_command(build_command)
    
    print("Build complete!")
    
if __name__ == "__main__":
    main()
```

2. Run the Executable or Quick Spike (run_project.py)

This script runs the compiled project or a quick "spike" (experimental executable), depending on the argument provided.

```python
#!/usr/bin/env python3

import argparse
import subprocess
import os

def run_command(command):
    """Helper function to run a shell command."""
    print(f"Running: {' '.join(command)}")
    result = subprocess.run(command)
    if result.returncode != 0:
        print(f"Error: Command failed with exit code {result.returncode}")
        exit(result.returncode)

def main():
    parser = argparse.ArgumentParser(description="Run the compiled project or a quick spike.")
    parser.add_argument("-t", "--target", help="Specify the target executable to run", default="MyProject")
    parser.add_argument("-q", "--quick", help="Run quick mode (spike)", action="store_true")
    
    args = parser.parse_args()
    build_dir = "build/bin"
    
    if args.quick:
        executable = os.path.join(build_dir, "spike")
    else:
        executable = os.path.join(build_dir, args.target if args.target else "MyProject")
    
    if not os.path.exists(executable):
        print(f"Error: Executable {executable} not found. Did you compile it?")
        exit(1)
    
    # Run the executable
    run_command([executable])
    
if __name__ == "__main__":
    main()

```

### How to Use the Python Scripts

1. Clean the Build Directory: To clean the build directory, run:
```bash
python clean_build.py
```
This will remove the build/ directory and all its contents.

2. Build the Project: To configure and build the project, use:
```bash
python build_project.py --define "MY_DEFINE" --quick --target "my_target" --debug
```

>You can pass various options:
>
>   * -d, --define: Define custom CMake variables.\
>   * -q, --quick: Enable quick compilation mode.\
>   * -t, --target: Specify the target for quick compilation.\
>   * -dbg, --debug: Enable the debug build type.\

3. Run the Executable: To run the compiled executable (or a quick spike), use:
```bash
python run_project.py --target "MyProject" --quick

    --target: Specify the name of the executable you want to run.
    --quick: If you want to run a quick spike, use this flag.
```
&copy; Manojlo Pekovic @ manojlopekovic0@gmail.com