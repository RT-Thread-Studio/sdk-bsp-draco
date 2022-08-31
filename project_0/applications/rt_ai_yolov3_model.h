#ifndef __RT_AI_YOLOV3_MODEL_H
#define __RT_AI_YOLOV3_MODEL_H

/* model info ... */

// model name
#define RT_AI_YOLOV3_MODEL_NAME			"yolov3"

#define RT_AI_YOLOV3_WORK_BUFFER_BYTES	(403200)

#define AI_YOLOV3_DATA_WEIGHTS_SIZE		(1493120) //unused

#define RT_AI_YOLOV3_BUFFER_ALIGNMENT		(4)

#define RT_AI_YOLOV3_IN_NUM				(1)

#define RT_AI_YOLOV3_IN_1_SIZE			(1 * 3 * 240 * 320)
#define RT_AI_YOLOV3_IN_1_SIZE_BYTES		((1 * 3 * 240 * 320) * 4)
#define RT_AI_YOLOV3_IN_SIZE_BYTES		{	\
	((1 * 3 * 240 * 320) * 4) ,	\
}

#define RT_AI_YOLOV3_IN_TOTAL_SIZE_BYTES	((1 * 3 * 240 * 320) * 4)


#define RT_AI_YOLOV3_OUT_NUM				(1)

#define RT_AI_YOLOV3_OUT_1_SIZE			(1 * 18 * 7 * 7)
#define RT_AI_YOLOV3_OUT_1_SIZE_BYTES		((1 * 18 * 7 * 7) * 4)
#define RT_AI_YOLOV3_OUT_SIZE_BYTES		{	\
	((1 * 18 * 7 * 7) * 4) ,	\
}

#define RT_AI_YOLOV3_OUT_TOTAL_SIZE_BYTES	((1 * 18 * 7 * 7) * 4)



#define RT_AI_YOLOV3_TOTAL_BUFFER_SIZE		//unused

#endif	//end
