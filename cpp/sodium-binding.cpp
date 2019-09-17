#include "sodium-binding.h"

#include "sodium.h"
#include <jsi/JSIDynamic.h>

#if ANDROID
extern "C"
{
  JNIEXPORT void JNICALL
  Java_org_libsodium_jni_SodiumJNI_sodium_1install(JNIEnv* env, jobject thiz, jlong runtimePtr)
  {
      auto sodiumBinding = std::make_shared<sodium::SodiumBinding>();
      jsi::Runtime* runtime = (jsi::Runtime*)runtimePtr;

      sodium::SodiumBinding::install(*runtime, sodiumBinding);
  }
}
#endif

namespace sodium {

  static jsi::Object getModule(
                                jsi::Runtime &runtime,
                                const std::string &moduleName) {
    auto batchedBridge =
    runtime.global().getPropertyAsObject(runtime, "__fbBatchedBridge");
    auto getCallableModule =
    batchedBridge.getPropertyAsFunction(runtime, "getCallableModule");
    auto module = getCallableModule
    .callWithThis(
                  runtime,
                  batchedBridge,
                  {jsi::String::createFromUtf8(runtime, moduleName)})
    .asObject(runtime);
    return module;
  }

  void SodiumBinding::install(
                                  jsi::Runtime &runtime,
                                  std::shared_ptr<SodiumBinding> sodiumBinding) {
    auto moduleName = "sodium";
    auto object = jsi::Object::createFromHostObject(runtime, sodiumBinding);
    runtime.global().setProperty(runtime, moduleName, std::move(object));
  }

  jsi::Value SodiumBinding::get(
                                    jsi::Runtime &runtime,
                                    const jsi::PropNameID &name) {
    auto methodName = name.utf8(runtime);

    if (methodName == "sodium_version_string") {
      return jsi::Function::createFromHostFunction(runtime, name, 0, [](
                                                                              jsi::Runtime &runtime,
                                                                              const jsi::Value &thisValue,
                                                                              const jsi::Value *arguments,
                                                                              size_t count) -> jsi::Value {
        return jsi::String::createFromUtf8(runtime, sodium_version_string());
      });
    }

    else if (methodName == "randombytes_random") {
      return jsi::Function::createFromHostFunction(runtime, name, 0, [](
                                                                              jsi::Runtime &runtime,
                                                                              const jsi::Value &thisValue,
                                                                              const jsi::Value *arguments,
                                                                              size_t count) -> jsi::Value {
        return 0;//randombytes_random();
      });
    }

    else if (methodName == "randombytes_buf") {
      return jsi::Function::createFromHostFunction(runtime, name, 0, [](
                                                                              jsi::Runtime &runtime,
                                                                              const jsi::Value &thisValue,
                                                                              const jsi::Value *arguments,
                                                                              size_t count) -> jsi::Value {

        auto arg0 = &arguments[0];
        int size = int (arg0->getNumber());
        unsigned char buf[size];

        randombytes_buf(&buf, size);

        return jsi::ArrayBuffer(buf);
      });
    }

    return jsi::Value::undefined();
  }

} // namespace sodium
