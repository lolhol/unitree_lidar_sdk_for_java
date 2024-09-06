#include <iostream>
#include <map>
#include <string>
#include <thread>

#include "jni.h"
#include "jni_utils.h"
#include "stdio.h"
#include "stdlib.h"
#include "unitree_lidar_sdk.h"

using namespace unitree_lidar_sdk;

class UnitreeLidar4JavaInternal {
    UnitreeLidarReader* lreader;
    int cloud_scan_num;
    const std::string& lidarDirPath;

   public:
    explicit UnitreeLidar4JavaInternal(const std::string& lidarPath) : lidarDirPath(lidarPath) {
        lreader = createUnitreeLidarReader();
        cloud_scan_num = 1;

        if (lreader->initialize(cloud_scan_num, lidarDirPath)) {
            printf("Unilidar initialization failed! Exit here!\n");
            exit(-1);
        } else {
            printf("Unilidar initialization succeed!\n");
        }
    }

    void setStateLidar(int state) {
        lreader->setLidarWorkingMode(formIntToWorkingModeEnum(state));
    }

    std::string checkLidarVersion() {
        if (lreader->runParse() == VERSION) {
            return lreader->getVersionOfFirmware().c_str();
        }

        return "";
    }

    std::string getSDKVersion() {
        return lreader->getVersionOfSDK().c_str();
    }

    unitree_lidar_sdk::MessageType parseMSGEnumRet() {
        return lreader->runParse();
    }

    unitree_lidar_sdk::PointCloudUnitree getPointCloud() {
        return lreader->getCloud();
    }

    unitree_lidar_sdk::IMUUnitree getIMUData() {
        return lreader->getIMU();
    }

    float getDirtyPercentage() {
        return lreader->getDirtyPercentage();
    }

    void setLEDState(const int state) {
        lreader->setLEDDisplayMode(fromIntToLEDDisplayEnum(state));
    }

    // util

    unitree_lidar_sdk::LidarWorkingMode formIntToWorkingModeEnum(const int state) {
        if (state == 0) {
            return unitree_lidar_sdk::LidarWorkingMode::STANDBY;
        } else if (state == 1) {
            return unitree_lidar_sdk::LidarWorkingMode::NORMAL;
        }

        return unitree_lidar_sdk::LidarWorkingMode::STANDBY;
    }

    /*
        FORWARD_SLOW = 2,       Function mode forward at slow. |
        FORWARD_FAST = 3,       Function mode forward at fast. |
        REVERSE_SLOW = 4,      / Function mode reverse at slow. |
        REVERSE_FAST = 5,      / Function mode reverse at fast. |
        TRIPLE_FLIP = 6,       / Function mode triple flip. |
        TRIPLE_BREATHING = 7,  Function mode triple breathing. |
        SIXSTAGE_BREATHING = 8 Function mode six-stage breathing. |
        */
    unitree_lidar_sdk::LEDDisplayMode fromIntToLEDDisplayEnum(const int state) {
        switch (state) {
            case 2:
                return unitree_lidar_sdk::LEDDisplayMode::FORWARD_SLOW;
            case 3:
                return unitree_lidar_sdk::LEDDisplayMode::FORWARD_FAST;
            case 4:
                return unitree_lidar_sdk::LEDDisplayMode::REVERSE_SLOW;
            case 5:
                return unitree_lidar_sdk::LEDDisplayMode::REVERSE_FAST;
            case 6:
                return unitree_lidar_sdk::LEDDisplayMode::TRIPLE_FLIP;
            case 7:
                return unitree_lidar_sdk::LEDDisplayMode::TRIPLE_BREATHING;
            case 8:
                return unitree_lidar_sdk::LEDDisplayMode::SIXSTAGE_BREATHING;
            default:
                return unitree_lidar_sdk::LEDDisplayMode::FORWARD_SLOW;
        }
    }
};

extern "C" {

JNIEXPORT void JNICALL
Java_brigero_UnitreeLidar4Java_init(JNIEnv* env,
                                    jobject thisobject, jstring lidarPath) {
    const std::string configDirStr = jstringToString(env, lidarPath);
    const auto unitree_lidar_module = new UnitreeLidar4JavaInternal(configDirStr);
    ptr_to_obj(env, thisobject, unitree_lidar_module);
}

JNIEXPORT void JNICALL
Java_brigero_UnitreeLidar4Java_setStateImpl(JNIEnv* env,
                                            jobject thisobject, jint state) {
    auto* unitreeLidarInternal = (UnitreeLidar4JavaInternal*)ptr_from_obj(env, thisobject);
    unitreeLidarInternal->setStateLidar(state);
}

JNIEXPORT jobject JNICALL
Java_brigero_UnitreeLidar4Java_getPointCloudObject(JNIEnv* env, jobject thisobject) {
    auto* unitreeLidarInternal = (UnitreeLidar4JavaInternal*)ptr_from_obj(env, thisobject);
    unitree_lidar_sdk::PointCloudUnitree pointCloud = unitreeLidarInternal->getPointCloud();

    jclass pointClass = env->FindClass("brigero/Point");
    jobjectArray pointArray = env->NewObjectArray(pointCloud.points.size(), pointClass, nullptr);

    for (int i = 0; i < pointCloud.points.size(); i++) {
        PointUnitree curPointCloud = pointCloud.points[i];
        jobject obj = createJavaPointObject(env, curPointCloud.x, curPointCloud.y, curPointCloud.z, curPointCloud.intensity, curPointCloud.time, curPointCloud.ring);
        env->SetObjectArrayElement(pointArray, i, obj);
    }

    return createJavaPointCloudObject(env, pointCloud.stamp, pointCloud.id, pointCloud.ringNum, pointArray);
}

JNIEXPORT jint JNICALL
Java_brigero_UnitreeLidar4Java_getCurMessageInt(JNIEnv* env,
                                                jobject thisobject) {
    auto* unitreeLidarInternal = (UnitreeLidar4JavaInternal*)ptr_from_obj(env, thisobject);
    unitree_lidar_sdk::MessageType enumRet = unitreeLidarInternal->parseMSGEnumRet();
    int c = static_cast<int>(enumRet);
    /*std::cout << c << " !!!!"
              << std::endl;*/
    return c;
}

JNIEXPORT jobject JNICALL
Java_brigero_UnitreeLidar4Java_getIMUData(JNIEnv* env, jobject thisobject) {
    auto* unitreeLidarInternal = (UnitreeLidar4JavaInternal*)ptr_from_obj(env, thisobject);
    unitree_lidar_sdk::IMUUnitree imuData = unitreeLidarInternal->getIMUData();

    jfloatArray quaternionArray = env->NewFloatArray(4);
    env->SetFloatArrayRegion(quaternionArray, 0, 4, imuData.quaternion);

    jfloatArray angularVelocityArray = env->NewFloatArray(3);
    env->SetFloatArrayRegion(angularVelocityArray, 0, 3, imuData.angular_velocity);

    jfloatArray linearAccelerationArray = env->NewFloatArray(3);
    env->SetFloatArrayRegion(linearAccelerationArray, 0, 3, imuData.linear_acceleration);

    return createJavaIMUObject(env, imuData.stamp, imuData.id, quaternionArray, angularVelocityArray, linearAccelerationArray);
}

JNIEXPORT void JNICALL
Java_brigero_UnitreeLidar4Java_setLidarLEDModeImpl(JNIEnv* env,
                                                   jobject thisobject, jint state) {
    auto* unitreeLidarInternal = (UnitreeLidar4JavaInternal*)ptr_from_obj(env, thisobject);
    unitreeLidarInternal->setLEDState(state);
}
};