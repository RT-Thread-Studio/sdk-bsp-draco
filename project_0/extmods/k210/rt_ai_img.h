#ifndef __RT_AI_IMAGE__
#define __RT_AI_IAMGE__

#include <stdlib.h>
#include <imlib.h>
/**
 * @brief bilinera interpolation resize. now support PIXFORMAT_GRAYSCALE/PIXFORMAT_RGB565/PIXFORMAT_RGB888_CHW/PIXFORMAT_RGB888_HWC
 * 
 * @param ori origin image
 * @param res result image
 */
void resize_bilinera_interpolation(image_t *ori, image_t *res);

/**
 * @brief crop ori img to res img by offset. now support PIXFORMAT_GRAYSCALE/PIXFORMAT_RGB565/PIXFORMAT_RGB888_CHW/PIXFORMAT_RGB888_HWC
 * @param ori origin image
 * @param res result image
 * @param offset_height height offset with start to crop
 * @param offset_width width offset with start to crop
 */
void crop(image_t *ori, image_t *res, uint32_t offset_height, uint32_t offset_width);

/**
 * @brief covert ori img to grayscale to res. now support PIXFORMAT_GRAYSCALE/PIXFORMAT_RGB565/PIXFORMAT_RGB888_CHW/PIXFORMAT_RGB888_HWC
 * 
 * @param ori origin image
 * @param res result image
 */
void to_gray(image_t *ori, image_t *res);

/**
 * @brief covert to ori img to rgb565 to res. now support PIXFORMAT_GRAYSCALE/PIXFORMAT_RGB565/PIXFORMAT_RGB888_CHW/PIXFORMAT_RGB888_HWC
 * 
 * @param ori origin image
 * @param res result image
 */
void to_rgb565(image_t *ori, image_t *res);

/**
 * @brief Set the pixel object. now support PIXFORMAT_GRAYSCALE/PIXFORMAT_RGB565/PIXFORMAT_BINARY
 * 
 * @param img the image be set
 * @param x x
 * @param y y
 * @param p pixel
 */
void set_pixel(image_t *img, int x, int y, int p);

/**
 * @brief draw line. now support PIXFORMAT_GRAYSCALE/PIXFORMAT_RGB565/PIXFORMAT_BINARY
 * 
 * @param img image_t.
 * @param x0 start x.
 * @param y0 start y.
 * @param x1 end x.
 * @param y1 end y.
 * @param c color.
 * @param thickness thickness.
 */
void draw_line(image_t *img, int x0, int y0, int x1, int y1, int c, int thickness);

/**
 * @brief draw rectangle. now support PIXFORMAT_GRAYSCALE/PIXFORMAT_RGB565/PIXFORMAT_BINARY.
 * 
 * @param img image_t
 * @param rx right-up x.
 * @param ry right-up y.
 * @param rw width.
 * @param rh height.
 * @param c color.
 * @param thickness thickness. 
 * @param fill fill the rectangle.
 */
void draw_rectangle(image_t *img, int rx, int ry, int rw, int rh, int c, int thickness, bool fill);

/**
 * @brief draw circle. now support PIXFORMAT_GRAYSCALE/PIXFORMAT_RGB565/PIXFORMAT_BINARY.
 * 
 * @param img image_t.
 * @param cx x of the centre of the circle.
 * @param cy y of the centre of a circle
 * @param r radius
 * @param c color
 * @param thickness thickness
 * @param fill fill the circle.
 */
void draw_circle(image_t *img, int cx, int cy, int r, int c, int thickness, bool fill);

/**
 * @brief draw string. now support PIXFORMAT_GRAYSCALE/PIXFORMAT_RGB565/PIXFORMAT_BINARY.
 * 
 * @param img image_t
 * @param x_off x
 * @param y_off y
 * @param str draw string
 * @param c color
 * @param scale string scale, eg: 1.0, 2.0....
 * @param x_spacing ?????????????????????????????????????????????????????????????????????????????????x??????????????????????????????
 * @param y_spacing ?????????????????????????????????????????????????????????????????????????????????y???????????????????????????
 * @param mono_space ??????True??????????????????????????????????????????????????????????????????????????????False???????????????????????????????????????????????????????????????
 * @param char_rotation 
 * @param char_hmirror ?????????True????????????????????????????????????????????????
 * @param char_vflip ?????????True????????????????????????????????????????????????
 * @param string_rotation ?????????0???90???180???270????????????????????????
 * @param string_hmirror ?????????True??????????????????????????????
 * @param string_hflip ?????????True??????????????????????????????
 */
void draw_string(image_t *img, int x_off, int y_off, const char *str, int c, float scale, int x_spacing, int y_spacing, bool mono_space,
                       int char_rotation, bool char_hmirror, bool char_vflip, int string_rotation, bool string_hmirror, bool string_hflip);

/**
 * @brief ??????????????????
 * 
 * @param img image_t
 * @param x_rotation ?????????x???????????????????????????????????????????????????????????????????????????
 * @param y_rotation ????????????????????????y??????????????????????????????????????????????????????
 * @param z_rotation ?????????z???????????????????????????????????????????????????????????????????????????????????????
 * @param x_translation ?????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????
 * @param y_translation ????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????
 * @param zoom ?????????????????????????????????????????????1.0???
 * @param fov ?????? 3D ????????????????????????????????? 2D->3D ???????????????????????????????????? ?????????????????? 0 ?????????????????????????????????????????????????????? ?????????????????? 180 ???????????????????????????????????? ???????????????????????????????????????????????????????????? 2D->3D ???????????????
 * @param corners ????????? (x,y) ???????????????????????????????????? 4 ????????????????????????????????? ???????????????????????? (0, 0)???????????????????????? (image_width-1, 0)??? ????????????????????? (image_width-1, image_height-1)???????????????????????? (0, image_height -1). ???????????????????????????????????? 3D ????????? ???????????????????????? rotation_corr ????????????????????????????????????????????????
 */
void rotation_corr(image_t *img, float x_rotation, float y_rotation, float z_rotation,
                         float x_translation, float y_translation,
                         float zoom, float fov, float *corners);

#endif //__RT_AI_IMAGE__
