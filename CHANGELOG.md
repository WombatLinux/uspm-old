# Changelog
All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

## Added
- Documentation
- `check_if_first_run()` checks if the current run is the first one by checking for
the main files.
- Checksums
- `uspm-chksum` can get the checksum of a file and also compare 2 files by checksum

## Changed
- Made uspp a shared library instead of static
- `uspm-mkpkg` is now ran outside of the package directory
- Split up the different software into their own folders to meet with uspm packaging plans

## Removed
- Unnecessary print statements

## [1.2.1] - 2020-08-3
## Changed
- Made it so both the clang and the GCC version are given.

## [1.2.0] - 2020-08-31
## Added
- The USPP library 
    - a library that is used for USPM and USPM-extended to work
    - replaces the source and header files that were a part of USPM

## Changed
- Moved all the excess source and header files to the new USPP library

## Removed
- The "check dependencies" function because its very useless

## [1.1.1] - 2020-02-23
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
