# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/mnt/c/Users/Victoriya/Desktop/Study/good/Note/build/libs/cryptopp-cmake/cryptopp-cmake/cryptopp"
  "/mnt/c/Users/Victoriya/Desktop/Study/good/Note/build/_deps/cryptopp-build"
  "/mnt/c/Users/Victoriya/Desktop/Study/good/Note/build/_deps/cryptopp-subbuild/cryptopp-populate-prefix"
  "/mnt/c/Users/Victoriya/Desktop/Study/good/Note/build/_deps/cryptopp-subbuild/cryptopp-populate-prefix/tmp"
  "/mnt/c/Users/Victoriya/Desktop/Study/good/Note/build/_deps/cryptopp-subbuild/cryptopp-populate-prefix/src/cryptopp-populate-stamp"
  "/mnt/c/Users/Victoriya/Desktop/Study/good/Note/build/_deps/cryptopp-subbuild/cryptopp-populate-prefix/src"
  "/mnt/c/Users/Victoriya/Desktop/Study/good/Note/build/_deps/cryptopp-subbuild/cryptopp-populate-prefix/src/cryptopp-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/mnt/c/Users/Victoriya/Desktop/Study/good/Note/build/_deps/cryptopp-subbuild/cryptopp-populate-prefix/src/cryptopp-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/mnt/c/Users/Victoriya/Desktop/Study/good/Note/build/_deps/cryptopp-subbuild/cryptopp-populate-prefix/src/cryptopp-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
