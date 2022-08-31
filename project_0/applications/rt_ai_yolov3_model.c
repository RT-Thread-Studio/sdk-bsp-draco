#include <rt_ai.h>
#include <backend_k210_kpu.h>
#include <rt_ai_yolov3_model.h>
#include <kpu.h>

extern unsigned char yolov3_kmodel[];

/* based on k210 */
#define RT_AI_YOLOV3_INFO    {       \
    RT_AI_YOLOV3_IN_NUM,             \
    RT_AI_YOLOV3_OUT_NUM,            \
    RT_AI_YOLOV3_IN_SIZE_BYTES,      \
    RT_AI_YOLOV3_OUT_SIZE_BYTES,     \
    RT_AI_YOLOV3_WORK_BUFFER_BYTES,  \
    ALLOC_INPUT_BUFFER_FLAG                 \
}

#define RT_AI_YOLOV3_HANDLE  {         \
    .info   =     RT_AI_YOLOV3_INFO    \
}

#define RT_K210_AI_YOLOV3   {   \
    .parent         = RT_AI_YOLOV3_HANDLE,   \
    .model          = yolov3_kmodel, \
    .dmac           = DMAC_CHANNEL5,        \
}

static struct k210_kpu rt_k210_ai_yolov3 = RT_K210_AI_YOLOV3;

static int rt_k210_ai_yolov3_init(){
    rt_ai_register(RT_AI_T(&rt_k210_ai_yolov3),RT_AI_YOLOV3_MODEL_NAME,0,backend_k210_kpu,&rt_k210_ai_yolov3);
    return 0;
}

INIT_APP_EXPORT(rt_k210_ai_yolov3_init);
