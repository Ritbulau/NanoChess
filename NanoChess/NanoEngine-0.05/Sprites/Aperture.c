#ifdef __has_include
    #if __has_include("lvgl.h")
        #ifndef LV_LVGL_H_INCLUDE_SIMPLE
            #define LV_LVGL_H_INCLUDE_SIMPLE
        #endif
    #endif
#endif

#if defined(LV_LVGL_H_INCLUDE_SIMPLE)
    #include "lvgl.h"
#else
    #include "lvgl/lvgl.h"
#endif


#ifndef LV_ATTRIBUTE_MEM_ALIGN
#define LV_ATTRIBUTE_MEM_ALIGN
#endif

#ifndef LV_ATTRIBUTE_IMG_APERTURE
#define LV_ATTRIBUTE_IMG_APERTURE
#endif

const LV_ATTRIBUTE_MEM_ALIGN LV_ATTRIBUTE_LARGE_CONST LV_ATTRIBUTE_IMG_APERTURE uint8_t Aperture_map[] = {
  0xff, 0xff, 0xff, 0xff, 	/*Color of index 0*/
  0x00, 0x00, 0x00, 0xff, 	/*Color of index 1*/

  0x00, 0x00, 0x00, 
  0x03, 0xc0, 0x00, 
  0x0b, 0xd8, 0x00, 
  0x19, 0xdc, 0x00, 
  0x3c, 0xde, 0x00, 
  0x3e, 0x58, 0x00, 
  0x00, 0x13, 0x00, 
  0x7c, 0x07, 0x00, 
  0x78, 0x0f, 0x00, 
  0x70, 0x1f, 0x00, 
  0x64, 0x00, 0x00, 
  0x0d, 0x3e, 0x00, 
  0x3d, 0x9e, 0x00, 
  0x1d, 0xcc, 0x00, 
  0x0d, 0xe8, 0x00, 
  0x01, 0xe0, 0x00, 
  0x00, 0x00, 0x00, 
};

const lv_img_dsc_t Aperture = {
  .header.cf = LV_IMG_CF_INDEXED_1BIT,
  .header.always_zero = 0,
  .header.reserved = 0,
  .header.w = 17,
  .header.h = 17,
  .data_size = 59,
  .data = Aperture_map,
};
