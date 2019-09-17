// Copyright 2004-present Facebook. All Rights Reserved.

#pragma once

#include <jsi/jsi.h>

#if ANDROID
#include <jni.h>
#endif

using namespace facebook;

#if ANDROID
extern "C" {
    JNIEXPORT void JNICALL
    Java_org_libsodium_jni_SodiumJNI_sodium_1install(JNIEnv* env, jobject thiz, jlong runtimePtr);
}
#endif

namespace sodium {

  /*
    * Exposes Test to JavaScript realm.
    */
  class SodiumBinding : public jsi::HostObject {
  public:
    /*
      * Installs Sodium into JavaSctipt runtime.
      * Thread synchronization must be enforced externally.
      */
    static void install(
                        jsi::Runtime &runtime,
                        std::shared_ptr<SodiumBinding> testBinding);

    // SodiumBinding(std::unique_ptr<Test> test);

    /*
      * `jsi::HostObject` specific overloads.
      */
    jsi::Value get(jsi::Runtime &runtime, const jsi::PropNameID &name) override;

  private:
    // std::unique_ptr<Test> test_;
  };

} // namespace sodium
