# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

if(EXISTS "F:/OpenGL/assimp_build/test/gtest/src/gtest-stamp/gtest-gitclone-lastrun.txt" AND EXISTS "F:/OpenGL/assimp_build/test/gtest/src/gtest-stamp/gtest-gitinfo.txt" AND
  "F:/OpenGL/assimp_build/test/gtest/src/gtest-stamp/gtest-gitclone-lastrun.txt" IS_NEWER_THAN "F:/OpenGL/assimp_build/test/gtest/src/gtest-stamp/gtest-gitinfo.txt")
  message(STATUS
    "Avoiding repeated git clone, stamp file is up to date: "
    "'F:/OpenGL/assimp_build/test/gtest/src/gtest-stamp/gtest-gitclone-lastrun.txt'"
  )
  return()
endif()

execute_process(
  COMMAND ${CMAKE_COMMAND} -E rm -rf "F:/OpenGL/assimp_build/test/gtest/src/gtest"
  RESULT_VARIABLE error_code
)
if(error_code)
  message(FATAL_ERROR "Failed to remove directory: 'F:/OpenGL/assimp_build/test/gtest/src/gtest'")
endif()

# try the clone 3 times in case there is an odd git clone issue
set(error_code 1)
set(number_of_tries 0)
while(error_code AND number_of_tries LESS 3)
  execute_process(
    COMMAND "E:/Program Files/Git/cmd/git.exe" 
            clone --no-checkout --config "advice.detachedHead=false" "https://github.com/google/googletest.git" "gtest"
    WORKING_DIRECTORY "F:/OpenGL/assimp_build/test/gtest/src"
    RESULT_VARIABLE error_code
  )
  math(EXPR number_of_tries "${number_of_tries} + 1")
endwhile()
if(number_of_tries GREATER 1)
  message(STATUS "Had to git clone more than once: ${number_of_tries} times.")
endif()
if(error_code)
  message(FATAL_ERROR "Failed to clone repository: 'https://github.com/google/googletest.git'")
endif()

execute_process(
  COMMAND "E:/Program Files/Git/cmd/git.exe" 
          checkout "master" --
  WORKING_DIRECTORY "F:/OpenGL/assimp_build/test/gtest/src/gtest"
  RESULT_VARIABLE error_code
)
if(error_code)
  message(FATAL_ERROR "Failed to checkout tag: 'master'")
endif()

set(init_submodules TRUE)
if(init_submodules)
  execute_process(
    COMMAND "E:/Program Files/Git/cmd/git.exe" 
            submodule update --recursive --init 
    WORKING_DIRECTORY "F:/OpenGL/assimp_build/test/gtest/src/gtest"
    RESULT_VARIABLE error_code
  )
endif()
if(error_code)
  message(FATAL_ERROR "Failed to update submodules in: 'F:/OpenGL/assimp_build/test/gtest/src/gtest'")
endif()

# Complete success, update the script-last-run stamp file:
#
execute_process(
  COMMAND ${CMAKE_COMMAND} -E copy "F:/OpenGL/assimp_build/test/gtest/src/gtest-stamp/gtest-gitinfo.txt" "F:/OpenGL/assimp_build/test/gtest/src/gtest-stamp/gtest-gitclone-lastrun.txt"
  RESULT_VARIABLE error_code
)
if(error_code)
  message(FATAL_ERROR "Failed to copy script-last-run stamp file: 'F:/OpenGL/assimp_build/test/gtest/src/gtest-stamp/gtest-gitclone-lastrun.txt'")
endif()
