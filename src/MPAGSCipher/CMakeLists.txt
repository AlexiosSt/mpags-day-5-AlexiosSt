# - Build sub-script for the MPAGSCipher library

# - Declare the build of the static MPAGSCipher library
add_library(MPAGSCipher STATIC
  Alphabet.hpp
  Cipher.hpp
  CipherFactory.hpp
  CipherFactory.cpp
  CaesarCipher.hpp
  CaesarCipher.cpp
  CipherMode.hpp
  CipherType.hpp
  PlayfairCipher.hpp
  PlayfairCipher.cpp
  VigenereCipher.hpp
  VigenereCipher.cpp
  ProcessCommandLine.hpp
  ProcessCommandLine.cpp
  TransformChar.hpp
  TransformChar.cpp
  )

target_include_directories(MPAGSCipher
  PUBLIC ${CMAKE_CURRENT_LIST_DIR}
  )
target_compile_features(MPAGSCipher
  PUBLIC cxx_std_17
  )