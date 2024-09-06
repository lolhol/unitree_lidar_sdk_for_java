/*
 * jni_utils.h Utilities for Java Native Interface code
 *
 * Copyright (C) 2014 Simon D. Levy
 *
 * This code is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This code is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this code.  If not, see <http:#www.gnu.org/licenses/>.
 */

#include <jni.h>
#include <stdio.h>
#include <stdlib.h>

#include <string>

static jfieldID get_fid(JNIEnv *env, jobject object, const char *fieldname, const char *fieldsig) {
    jclass cls = env->GetObjectClass(object);
    return env->GetFieldID(cls, fieldname, fieldsig);
}

static jfieldID get_this_fid(JNIEnv *env, jobject thisobject) {
    return get_fid(env, thisobject, "native_ptr", "J");
}

static void *ptr_from_obj(JNIEnv *env, jobject thisobject) {
    return (void *)env->GetLongField(thisobject, get_this_fid(env, thisobject));
}

static void ptr_to_obj(JNIEnv *env, jobject thisobject, void *ptr) {
    env->SetLongField(thisobject, get_this_fid(env, thisobject), (long)ptr);
}

static double get_double_field(JNIEnv *env, jobject object, const char *fieldname) {
    return env->GetDoubleField(object, get_fid(env, object, fieldname, "D"));
}

static std::string jstringToString(JNIEnv *env, jstring javaString) {
    const char *charArray = env->GetStringUTFChars(javaString, nullptr);
    std::string cppString(charArray);
    env->ReleaseStringUTFChars(javaString, charArray);
    return cppString;
}

static bool convertJBooleanToBool(jboolean jboolValue) {
    return (jboolValue == JNI_TRUE);
}

static double convertJDoubleToDouble(jdouble jdoubleValue) {
    return static_cast<double>(jdoubleValue);
}

static jobject createJavaPointObject(JNIEnv *env, float x, float y, float z, float intensity, float time, int ring) {
    jclass pointClass = env->FindClass("brigero/Point");
    jmethodID constructor = env->GetMethodID(pointClass, "<init>", "(FFFFFI)V");
    return env->NewObject(pointClass, constructor, x, y, z, intensity, time, ring);
}

static jobject createJavaPointCloudObject(JNIEnv *env, double stamp, int id, int validPointsNum, jobjectArray pointArray) {
    jclass pointCloudClass = env->FindClass("brigero/PointCloud");
    jmethodID constructor = env->GetMethodID(pointCloudClass, "<init>", "(DII[Lbrigero/Point;)V");
    return env->NewObject(pointCloudClass, constructor, stamp, id, validPointsNum, pointArray);
}

static jobject createJavaIMUObject(JNIEnv *env, double stamp,
                                   uint32_t id,
                                   jfloatArray quaternion,
                                   jfloatArray angular_velocity,
                                   jfloatArray linear_acceleration) {
    jclass imuClass = env->FindClass("brigero/IMUUnitree");
    jmethodID constructor = env->GetMethodID(imuClass, "<init>", "(DI[F[F[F)V");

    return env->NewObject(imuClass, constructor, stamp, id, quaternion, angular_velocity, linear_acceleration);
}
