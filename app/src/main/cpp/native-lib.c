//
// Created by Administrator on 2017/12/6.
//
#include "com_yixitongda_myapplication_Fileutils.h"
#include <android/log.h>
#include <assert.h>
#include <malloc.h>

//int __android_log_print(int prio, const char* tag, const char* fmt, ...)
#define TAG "Chen_JNI"  //这个是C中的宏  他代表的意思是我们的TAG  代表的意思是 Chen_JNI
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,TAG,__VA_ARGS__)

//为了方便从源文件中拷贝一个宏定义
#define NELEM(x) (int) (sizeof(x)/sizeof((x)[0]))


/**
 *这是用来测试的 我们需要新开一个项目 然后调用这里的So库
 * @param na
 * @param b
 * @return
 */
//int addTest(int na,int b){
//    LOGI("这是第三方的so库调用测试 %d %d",na,b);
//    return na+b;
//}
//获取文件大小
long get_file_size(const char *path) {
    FILE *fp = fopen(path, "rb");//打开一个文件 文件必须存在
    fseek(fp, 0, SEEK_END);
    long ret=ftell(fp);
    fclose(fp);
    return ret;
}

/*
 * Class:     com_yixitongda_myapplication_Fileutils
 * Method:    diff
 * Signature: (Ljava/lang/String;Ljava/lang/String;I)V
 */
JNIEXPORT void JNICALL native_diff
        (JNIEnv *env, jclass clazz, jstring path, jstring pattenr_Path, jint file_num) {
    LOGI("JNI BAGIN  动态注册");
    LOGI("jni native diff");
    const char *path_Str = (*env)->GetStringUTFChars(env, path, NULL);
    const char *pattern_Path_str = (*env)->GetStringUTFChars(env, pattenr_Path, NULL);

//申请二维字符数据,存放子文件名
    char **pathches = (char **) malloc(sizeof(char *) * file_num);

    int i = 0;
    int j = 0;
    for (; i < file_num; i++) {
        //每个文件名申请地址
        pathches[i] = (char *) malloc(sizeof(char *) * 100);
        //需要分割的文件名android_video.mkv

        //每一个文件的名称android_video_n.mkv
        sprintf(pathches[i], pattern_Path_str, i);//格式化文件名
        LOGI("patch path: %s", pathches[i]);

    }


    int fileSize = get_file_size(path_Str);

    printf("%d",fileSize);

    LOGI("fileSize:%d",fileSize);

    FILE *fpr = fopen(path_Str, "rb");

    /**
     * 1.判断文件大小  能够被file_num整除
     * 2.如果能整除 评分
     * 3.不能整除  就分file_num-1
     */

    if (fileSize % file_num == 0) {
        int part = fileSize / file_num;
        LOGI("能够被整除");
        for (i = 0; i < file_num; i++) {
            FILE *fpw = fopen(pathches[i], "wb");//文件已经存在 则删除  只运行
            for (j = 0; j < part; j++) {
                fputc(fgetc(fpr), fpw);
            }
            fclose(fpw);
        }
    } else {
        LOGI("不能够被整除");
        //其中很重要的一点  我们的fgetc()  每一次只会从我们的文件中拿出一个字节的数据
        int part = fileSize / (file_num - 1);

        for (i = 0; i < file_num; i++) {
            FILE *fpw = fopen(pathches[i], "wb");//文件已经存在 则删除  只运行
            for (j = 0; j < part; j++) {
                fputc(fgetc(fpr), fpw);
            }
            fclose(fpw);
        }

        FILE *fpw = fopen(pathches[file_num - 1], "wb");

        for (i = 0; i < fileSize % (file_num - 1); ++i) {
            fputc(fgetc(fpr), fpw);
        }

        fclose(fpr);

    }

    (*env)->ReleaseStringUTFChars(env, path, path_Str);
    (*env)->ReleaseStringUTFChars(env, pattenr_Path, pattern_Path_str);

};


static const JNINativeMethod gMethods[] = {

        "diff", "(Ljava/lang/String;Ljava/lang/String;I)V", (void *) native_diff
};

static int registerNative(JNIEnv *engv) {
    LOGI("registerNatives begin");
    jclass jclass1;
    jclass1 = (*engv)->FindClass(engv, "com/yixitongda/myapplication/Fileutils");

    if (jclass1 == NULL) {
        LOGI("class is null");
        return JNI_FALSE;
    }

    if ((*engv)->RegisterNatives(engv, jclass1, gMethods, NELEM(gMethods)) < 0) {
        LOGI("RegisterNatives error");
        return JNI_FALSE;
    }

}


JNIEXPORT jint JNI_OnLoad(JavaVM *vm, void *reserved) {
    LOGI("jni onLoad begin");
    return JNI_TRUE;

    JNIEnv *env = NULL;
    jint result = -1;

    if ((*vm)->GetEnv(vm, (void **) &env, JNI_VERSION_1_4) != JNI_OK) {
        LOGI("Error:GetEnv failed\n");
        return -1;
    }

    assert(env != NULL);

    registerNative(env);

    return JNI_VERSION_1_4;


}



//经过测试  动态注册成功  下面是动态注册的顺序
//D/chen debug: diff:
//01-04 03:07:40.188 15798-15798/? I/Chen_JNI: jni onLoad begin
//01-04 03:07:40.188 15798-15798/? I/Chen_JNI: registerNatives begin
//01-04 03:07:40.188 15798-15798/? I/Chen_JNI: JNI BAGIN  动态注册

/*
 * Class:     com_yixitongda_myapplication_Fileutils
 * Method:    diff
 * Signature: (Ljava/lang/String;Ljava/lang/String;I)V
 */
//JNIEXPORT void JNICALL Java_com_yixitongda_myapplication_Fileutils_diff
//        (JNIEnv *env, jclass clazz, jstring path , jstring pattenr_Path, jint file_num){
//    LOGI("JNI BAGIN");
//};