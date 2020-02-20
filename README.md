Ultra Simple Package Manager
============================

Build Status: [![Build Status](https://travis-ci.com/afroraydude/uspm.svg)](https://travis-ci.com/afroraydude/uspm)

Ultra Simple Package Manager (USPM) is a suite of executables that overall allow for the management of packages.

Here is a list of the current tools:
- uspm: The main application that allows for the installation and uninstallation of software packages and their 
dependencies.
- uspm-mkpkg: This optional application is useful for those who want to design USPM packages
- uspm-extended: This optional application adds more commands. This was made because of the original developer's rule that uspm can only 
have 3 commands: install, uninstall, and check dependencies.

## How it works

All packages come in `.uspm` files, which are just tarballs. Within the tarball there must be two included files. 

`PACKAGECODE` is the install/uninstall script, it contains all the code necessary to install that package

`PACKAGEDATA` is the JSON data that will get added by the package manager to the master list.

## Building and Installing
This package requires **at least** `cJSON` ([found here](https://github.com/DaveGamble/cJSON)) and libcurl

There are 2 ways to install:

1) Through Make (this requires GCC) [***not recommended***]
```bash
git clone https://github.com/afroraydude/uspm.git
cd uspm
make uspm
make install
```

2) Through CMake (clang or gcc)
```bash
git clone https://github.com/afroraydude/uspm.git
mkdir build
cd build
cmake ..
make
make install
```

## How to create a package for USPM

### The easy way
1) Compile the package from source
2) Place all compiled files in a folder
3) Make a folder with the name of the package and place the previous folder in that
4) Enter the directory
5) Run `uspm-mkpkg` and follow instructions
6) Exit the directory, you should now have a `.uspm` package file.

### The hard way
1) Compile the package from source
2) Place all compiled files in a folder
3) Make a folder with the name of the package and place the previous folder in that
4) Inside the package folder, create two files: `PACKAGECODE` and `PACAKGEDATA`
5) In PACKAGECODE, include all the code necessary to migrate the compiled files into the system so that they are usable, put it in a function that can be called by `PACKAGECODE install` (**NOTE: This must be a shell script**)
6) In PACKAGECODE, include all the code necessary to allow for an uninstallation of all files, in a function that can be called by `PACKAGECODE uninstall`
7) In PACKAGEDATA, include the version and dependencies (ex: `{"version":"1.0.0", "dependencies":{"test":"1.0.0","test2":"1.0.0"}}`)
8) Archive the whole directory into a tarball and make sure the extension is `.uspm`
9) Done!

Example `PACKAGEDATA` file
```json
{
  "version": "1.0.0",
  "dependencies": {"uspm": "1.0.0"}
}
```

Example `PACKAGECODE` file
```sh
cd package

# Install 
if [ $1 == 'install' ]
then
  cp -R files/* /usr/

# Uninstall
elif [ $1 == 'uninstall' ]
then
  rm /usr/local/bin/example
fi```
