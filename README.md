Ultra Simple Package Manager (C Edition)
========================================

A a simple package manager that allows the user to install and uninstall packages and their dependencies.

## How it works

All packages come in `.uspm` files, which are just tarballs. Within the tarball there must be two included files. 

`PACKAGECODE` is the install/uninstall script, it contains all the code necessary to install that package

`PACKAGEDATA` is the JSON data that will get added by the package manager to the master list.

## Installation
```bash
git clone https://github.com/afroraydude/uspm.git`
cd uspm
make uspm
make install
```

## How to create a package for USPM

1) Compile the package from source
2) Place all compiled files in a folder
3) Make a folder with the name of the package and place the previous folder in that
4) Inside the package folder, create two files: `PACKAGECODE` and `PACAKGEDATA`
5) In PACKAGECODE, include all the code necessary to migrate the compiled files into the system so that they are usable, put it in a function that can be called by `PACKAGECODE install`
6) In PACKAGECODE, include all the code necessary to allow for an uninstallation of all files, in a function that can be called by `PACKAGECODE uninstall`
7) In PACKAGEDATA, include the version and dependencies (ex: `{"version":"1.0.0", "dependencies":{"test":"1.0.0","test2":"1.0.0"}}`)
8) Archive the whole directory into a tarball and make sure the extension is `.uspm`
9) Done!