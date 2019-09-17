
package org.libsodium.jni;

public class Sodium extends  SodiumJNI {

  static {
    System.loadLibrary("sodium-jni");
  }

  public final static void loadLibrary() {
    sodium_init();
  }

  public final static void installBinding(long jsContextNativePointer) {
    sodium_install(jsContextNativePointer);
  }

}
