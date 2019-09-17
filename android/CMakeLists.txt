cmake_minimum_required(VERSION 3.4.1)

set(ARCH_DIR ${ANDROID_ARCH_NAME})

if( ARCH_DIR STREQUAL "arm" )
  set( ARCH_DIR "armv7-a" )
elseif( ARCH_DIR STREQUAL "arm64" )
  set( ARCH_DIR "armv8-a" )
elseif( ARCH_DIR STREQUAL "x86" )
  set( ARCH_DIR "i686" )
elseif( ARCH_DIR STREQUAL "x86_64" )
  set( ARCH_DIR "westmere" )
elseif( ARCH_DIR STREQUAL "mips" )
  set( ARCH_DIR "mips32" )
elseif( ARCH_DIR STREQUAL "mips64" )
  set( ARCH_DIR "mips64r6" )
endif()

execute_process(COMMAND sh -c "(`pwd`/scripts/ios-install-third-party.sh)"
                WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/../../node_modules/react-native
)

# Needed to locate double-conversion src correctly for folly includes
execute_process (COMMAND ln "-s" "src" "${PROJECT_SOURCE_DIR}/../../react-native/third-party/double-conversion-1.1.6/double-conversion")

add_definitions(
  -DFOLLY_USE_LIBCPP=1
  -DFOLLY_NO_CONFIG=1
  -DFOLLY_HAVE_MEMRCHR=1
)

include_directories(
  ${PROJECT_SOURCE_DIR}/../libsodium/libsodium-android-${ARCH_DIR}/include/
  ${PROJECT_SOURCE_DIR}/../../react-native/React
  ${PROJECT_SOURCE_DIR}/../../react-native/React/Base
  ${PROJECT_SOURCE_DIR}/../../react-native/ReactCommon
  ${PROJECT_SOURCE_DIR}/../../react-native/third-party/folly-2018.10.22.00
  ${PROJECT_SOURCE_DIR}/../../react-native/third-party/double-conversion-1.1.6
  ${PROJECT_SOURCE_DIR}/../../react-native/third-party/boost_1_63_0
  ${PROJECT_SOURCE_DIR}/../../react-native/third-party/glog-0.3.5/src
)

add_library(sodium SHARED IMPORTED)

set_target_properties( sodium PROPERTIES IMPORTED_LOCATION ${PROJECT_SOURCE_DIR}/../libsodium/libsodium-android-${ARCH_DIR}/lib/libsodium.so )
file(COPY ${PROJECT_SOURCE_DIR}/../libsodium/libsodium-android-${ARCH_DIR}/lib/libsodium.so DESTINATION "${PROJECT_SOURCE_DIR}/lib/${ANDROID_ABI}")

add_library(
  sodium-jni
  SHARED
  ${PROJECT_SOURCE_DIR}/../../react-native/ReactCommon/jsi/jsi.cpp
  src/main/cpp/sodium-jni.c
  ../cpp/sodium-binding.cpp
  )

target_link_libraries(sodium-jni sodium)
