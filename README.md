# Offline-Web-Doc

---
##### A offline locally document server, it have only single file after compiled.
---

## Purpose

Make a webpage-based offline document, there are only one file for could be easily to publish.

## Compile tools
Use `Clang` + `lld` by Default, if you are not using that, you can modify `CMakeLists.txt` according to your case.

## How it works
1. Generate byte array for web resources, as a C++ Header file(named to `datas.hpp`).
2. Include the header file containing data in server code
3. Do that compile jobs.
4. After executed executable file, it will randomized select a HTTP service port and listen on it, and it will open the web with default browser(Only available on Windows).

## Usage
### Compile the generate tool for web resources
```bash
clang ./genH.cpp -Wall -std=c++20 -o'genH.exe'
```

### Prepare web resources
Put the web folder to `dist` folder in source folder, `dist` folder will be consider root folder for web, like `dist/a.js` equals `http://ip:port/a.js`.

### Generate `datas.hpp` file
Just execute `genH.exe`

### Compile doc server
```bash
cd build; # you have to create the folder if there aren't the folder
cmake -DCMAKE_BUILD_TYPE=MINSIZEREL ..
make
```

## Working with VuePress
I wrote a PowerShell script for this(`go.ps1`),it can automatically to compile VuePress project and then generate header file,and then compile doc server,finally compress executable file via UPX, the executable file will be put into project folder(`main.exe`).
Command line arguments:
|Flag|Description|Default|
|:---:|:---:|:---:|
|`-vpProjPath`| VuePress project path | `./vuepress` |
|`-vpProjName`| VuePress site name | `doc` |

## Remarks
- May need to adjust the stack size of program in `CMakeLists` because all of resources will be combined to executable file, it would be occured stack overflow if stack size not enough.
- I added icon for executable file, you can remove it from `CMakeLists`, just don't linking `ico.o` file.

# Credits

- [cpp-httplib](https://github.com/yhirose/cpp-httplib)