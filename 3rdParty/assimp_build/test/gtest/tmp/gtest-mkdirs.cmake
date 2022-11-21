# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "F:/OpenGL/assimp_build/test/gtest/src/gtest"
  "F:/OpenGL/assimp_build/test/gtest/src/gtest-build"
  "F:/OpenGL/assimp_build/test/gtest"
  "F:/OpenGL/assimp_build/test/gtest/tmp"
  "F:/OpenGL/assimp_build/test/gtest/src/gtest-stamp"
  "F:/OpenGL/assimp_build/test/gtest/src"
  "F:/OpenGL/assimp_build/test/gtest/src/gtest-stamp"
)

set(configSubDirs Debug;Release;MinSizeRel;RelWithDebInfo)
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "F:/OpenGL/assimp_build/test/gtest/src/gtest-stamp/${subDir}")
endforeach()
