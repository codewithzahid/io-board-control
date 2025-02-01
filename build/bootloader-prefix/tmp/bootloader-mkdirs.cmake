# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "E:/Espressif/v5.2.1/esp-idf/components/bootloader/subproject"
  "G:/Github/io-board-control/build/bootloader"
  "G:/Github/io-board-control/build/bootloader-prefix"
  "G:/Github/io-board-control/build/bootloader-prefix/tmp"
  "G:/Github/io-board-control/build/bootloader-prefix/src/bootloader-stamp"
  "G:/Github/io-board-control/build/bootloader-prefix/src"
  "G:/Github/io-board-control/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "G:/Github/io-board-control/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "G:/Github/io-board-control/build/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()
