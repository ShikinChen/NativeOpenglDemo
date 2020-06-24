//
// Created by Shiki on 2020/4/30.
//

#ifndef NATIVEOPENGLDEMO_CALLJAVA_H
#define NATIVEOPENGLDEMO_CALLJAVA_H
enum ThreadType {
    Main,
    Child
};

#include <jni.h>
#include "AndroidLog.h"

class CallJava {
public:
    JavaVM *javaVm = NULL;
    JNIEnv *jniEnv = NULL;
    jobject jobj;


    CallJava(JavaVM *javaVm, JNIEnv *jniEnv, jobject *obj);

    virtual  ~CallJava();

    int onCallHashCode(jobject *obj, ThreadType threadType = Main);


private:
    jmethodID jmid_hashCode;
};


#endif //NATIVEOPENGLDEMO_CALLJAVA_H
