# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

### Added

- New dependency, libtar

### Changed

- Use libtar instead of external commands for extraction of tar files.
- `uspm-mkpkg` tool now uses libtar instead of commands for creation of tar files.
- `uspm-extended` now includes an update checking system.
- `parser.h` has been properly named to `uspm.h` to reflect its use as a header for core uspm fuctions
- string comparing has been changed to substring comparison

### [1.1.1] - 2020-02-23

### Fixed

- Fixed uspm-extended where commands weren't doing what they were supposed to do

## [1.1.0] - 2020-02-20

### Added

- Travis CI build configuration
- Manpage
- Ability to install/uninstall/check more than one package
- `uspm-mkpkg` tool that makes package creation easier
- Build status to README
- Extended commands
- Added proper version checking for dependencies

### Changed

- Changed the default configuration's mirror site.
- Changed `CMakeLists.txt` file to include `uspm-mkpkg`
- Changed location of `uspm` C source and header files
- Changed package creation instructions within the README
- Dependency resolution now is seperate from normal install functions.

### Fixed

- Fixed bug where default mirror links to a nonexistent server

## [1.0.1] - 2020-02-04

### Added

- Option to use CMake instead of Makefile

### Changed

- Removed "C Edition" from README
- Added install instructions for CMake

### Fixed

- Fixed bug where if config file doesn't exist, it breaks.

## [1.0.0] - 2020-02-03

### Added

- Install, Uninstall, Check dependencies commands
