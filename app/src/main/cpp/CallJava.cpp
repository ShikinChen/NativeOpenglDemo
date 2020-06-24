//
// Created by Shiki on 2020/4/30.
//

#include "CallJava.h"


CallJava::CallJava(JavaVM *javaVm, JNIEnv *jniEnv, jobject *obj) : javaVm(javaVm), jniEnv(jniEnv) {
    this->jobj = jniEnv->NewGlobalRef(*obj);

    jclass jclz = jniEnv->GetObjectClass(jobj);
    if (!jclz) {
        if (LOG_DEBUG) {
            LOGE("获取class失败");
        }
        return;
    }
    jmid_hashCode = jniEnv->GetMethodID(jclz, "hashCode", "()I");
}

CallJava::~CallJava() {
    javaVm = NULL;
    if (javaVm && jobj) {
        jniEnv->DeleteGlobalRef(jobj);
        javaVm = NULL;
        jniEnv = NULL;
        jobj = NULL;
    }
}

int CallJava::onCallHashCode(jobject *obj, ThreadType threadType) {
    int hashCode = -1;
    if (threadType == Main) {
        hashCode = jniEnv->CallIntMethod(*obj, jmid_hashCode);
    } else if (threadType == Child) {
        JNIEnv *jniEnv;
        javaVm->AttachCurrentThread(&jniEnv, NULL);
        if (!jniEnv) {
            if (LOG_DEBUG) {
                LOGE("获取子线程env失败");
            }
            return hashCode;
        }
        hashCode = jniEnv->CallIntMethod(*obj, jmid_hashCode);
        javaVm->DetachCurrentThread();
    }
    return hashCode;
}

