#include <jni.h>
#include <string.h>
#include <android/log.h>
#include <cstdlib>
#include <iostream>
#include <cstring>
#define pass "00000"

extern "C" {
    jboolean checkLib(
    JNIEnv* env,
    jobject thiz
    ){
        
        jclass MainClass = env->GetObjectClass(thiz);
        
        jmethodID getPM = env->GetMethodID(MainClass, "getPackageManager", "()Landroid/content/pm/PackageManager;");
        jobject packageManager = env->CallObjectMethod(thiz, getPM);
        
        jmethodID getPN = env->GetMethodID(MainClass, "getPackageName", "()Ljava/lang/String;");
        jstring packageName = (jstring) env->CallObjectMethod(thiz, getPN);
        
        jclass pmClass = env->FindClass("android/content/pm/PackageManager");
        jmethodID appInfoMethod= env->GetMethodID(pmClass, "getApplicationInfo", "(Ljava/lang/String;I)Landroid/content/pm/ApplicationInfo;");
        jobject appInfo = env->CallObjectMethod(packageManager, appInfoMethod,packageName, 0);
        
        jclass appInfoClass = env->FindClass("android/content/pm/ApplicationInfo");
        jfieldID sourceDirField = env->GetFieldID(appInfoClass, "sourceDir", "Ljava/lang/String;");
        jstring apkPath = (jstring) env->GetObjectField(appInfo, sourceDirField);
        
        const char* oPath = env->GetStringUTFChars(apkPath, nullptr);
        const char* str1 = "base.apk";
        const char* str2;
        jclass buildClass = env->FindClass("android/os/Build");
        jfieldID abisField = env->GetStaticFieldID(buildClass, "SUPPORTED_ABIS", "[Ljava/lang/String;");
        jobjectArray abisArray = (jobjectArray) env->GetStaticObjectField(buildClass, abisField);
        
        jstring arch = (jstring) env->GetObjectArrayElement(abisArray, 0);
        const char* archStr = env->GetStringUTFChars(arch, nullptr);
        
        if(strstr(archStr, "arm64") != nullptr ){
            str2 = "lib/arm64/";
        } else {
            str2 = "lib/arm/";
        }
        
        const char* found = strstr(oPath, str1);
        if (found != nullptr) {
            
        size_t newLength = strlen(oPath) - strlen(str1) + strlen(str2);

        // Allocate memory for the new string
        char* newText = new char[newLength + 1];

        // Copy the part before the found string
        strncpy(newText, oPath, found - oPath);

        // Copy the replacement text
        strcpy(newText + (found - oPath), str2);

        // Copy the part after the found string
        strcpy(newText + (found - oPath) + strlen(str2), found + strlen(str1));
        
        jclass fileClass = env->FindClass("java/io/File");
        jmethodID fileConst = env->GetMethodID(fileClass, "<init>", "(Ljava/lang/String;)V");
        jobject file = env->NewObject(fileClass, fileConst, env->NewStringUTF(newText));
        
        jmethodID exitsMethod = env->GetMethodID(fileClass, "exists", "()Z");
        jboolean exits = env->CallBooleanMethod(file, exitsMethod);

        // Delete allocated memory
        delete[] newText;

        jmethodID listFileMethod = env->GetMethodID(fileClass, "listFiles","()[Ljava/io/File;");
        jobjectArray fileArray = (jobjectArray) env->CallObjectMethod(file, listFileMethod);
        jint fileCount = env->GetArrayLength(fileArray);

        if(fileCount == 1){
            return true;
        } else {
            return false;
        }
        
        }
        }
        
    
    jstring getSignature(
    JNIEnv* env,
    jobject thiz
    ){
        jclass contextTemp = env->FindClass("android/content/Context");
        jclass contextClass = (jclass)env->NewGlobalRef(contextTemp);
        
        jmethodID getPackageManager = env->GetMethodID(contextClass, "getPackageManager", "()Landroid/content/pm/PackageManager;");
        jobject pm = (jobject) env->CallObjectMethod(thiz, getPackageManager);
        
        jmethodID getPackageName = env->GetMethodID(contextClass, "getPackageName", "()Ljava/lang/String;");
        jstring packageName = (jstring)env->CallObjectMethod(thiz, getPackageName);
        
        jclass pmClass = env->FindClass("android/content/pm/PackageManager");
        
        jfieldID sigID = env->GetStaticFieldID(pmClass, "GET_SIGNATURES", "I");
        jint sig = env->GetStaticIntField(pmClass, sigID);
        
        jclass pmnClass = (jclass) env->NewGlobalRef(pmClass);
        jmethodID getPackageInfo = env->GetMethodID(pmnClass, "getPackageInfo", "(Ljava/lang/String;I)Landroid/content/pm/PackageInfo;");
        
        jobject info = (jobject) env->CallObjectMethod(pm, getPackageInfo, packageName, sig);
        jclass piClass = env->FindClass("android/content/pm/PackageInfo");
        
        jfieldID signatures = env->GetFieldID(piClass, "signatures", "[Landroid/content/pm/Signature;");
        
        jobjectArray rows = reinterpret_cast<jobjectArray>( env->GetObjectField(info,signatures));
        
        jclass signatureClass = env->FindClass("android/content/pm/Signature");
        
        jclass mdClass = env->FindClass("java/security/MessageDigest");
        jclass fnMDClass = (jclass) env->NewGlobalRef(mdClass);
        
        jmethodID getInstance = env->GetStaticMethodID(fnMDClass, "getInstance", "(Ljava/lang/String;)Ljava/security/MessageDigest;");
        jmethodID update = env->GetMethodID(fnMDClass, "update", "([B)V");
        
        jmethodID toByteArray = env->GetMethodID(signatureClass, "toByteArray", "()[B");
        
        
        jclass base64Class = env->FindClass("android/util/Base64");
        jclass fnBase64Class = (jclass)env->NewGlobalRef(base64Class);
        
        jmethodID encodeToString = env->GetStaticMethodID(fnBase64Class, "encodeToString", "([BI)Ljava/lang/String;");
        
        jmethodID digest = env->GetMethodID(fnMDClass, "digest", "()[B");
        
        jobject md = env->CallStaticObjectMethod(fnMDClass, getInstance, env->NewStringUTF("SHA"));
        
        jobject signature = env->GetObjectArrayElement(rows, 0);
        
        env->CallVoidMethod(md, update, (jbyteArray)env->CallObjectMethod(signature, toByteArray));
        
        return (jstring) env->CallStaticObjectMethod(fnBase64Class, encodeToString, env->CallObjectMethod(md, digest), (jint)0);
        
    }
    
    JNIEXPORT void JNICALL Java_com_test_cpp_MainActivity_checkSignature
    (
    JNIEnv* env, 
    jobject thiz
    ){
        
        
        std::string p1 = "sVX0q9GKeEBUXs6z/";
        std::string p2 = "8ZaOwnzy0g=";
        
        const char* c_p1 = p1.c_str();
        const char* c_p2 = p2.c_str();
        
        size_t len1 = strlen(c_p1);
    size_t len2 = strlen(c_p2);
    size_t totalLen = len1 + len2;
    
    
        
        char* originalSig = new char[totalLen + 1];
        
        strcpy(originalSig, c_p1);
        strcat(originalSig, c_p2);
        
        jstring curSig = getSignature(env, thiz);
        
        //__android_log_print(ANDROID_LOG_INFO, "APP SIGNATURE", "%s", env->GetStringUTFChars(curSig, nullptr));
        
        const char* currentSig = env->GetStringUTFChars(curSig, NULL);
        int res = strncmp(originalSig, currentSig, 28);
        
        delete[] originalSig;
        
        jclass myclaz = env->GetObjectClass(thiz);
            jfieldID tvID = env->GetFieldID(myclaz, "tv", "Landroid/widget/TextView;");
            jobject tvObject = env->GetObjectField(thiz, tvID);
            
            jclass tvClass = env->FindClass("android/widget/TextView");
            jmethodID setText = env->GetMethodID(tvClass, "setText", "(Ljava/lang/CharSequence;)V");
        
        //finishAffinity for exit
        jclass mClass = env->GetObjectClass(thiz);
        jmethodID finishAffinity = env->GetMethodID(mClass, "finishAffinity", "()V");
        
        if(checkLib(env, thiz)){
        if (res != 0) {
            //signature killed
          //   env->CallVoidMethod(thiz, finishAffinity);
        } else{
            //here all verification is success
            env->CallVoidMethod(tvObject, setText, env->NewStringUTF("Verification Success!"));

        }
        } else {
        //signature killer lib is added
        //  env->CallVoidMethod(thiz, finishAffinity);
    }
    }
}