/* FaceDetector header file
- Currently contains function declarations from all modules.
- May split into several modules if number of functions increase, or it simply makes sense to do so.
*/

#ifndef __VIOLA_JONES_H__
#define __VIOLA_JONES_H__

extern int detectFaces( uint8_t* image_array, uint32_t im_rows, uint32_t im_cols, uint32_t** face_windows_array, uint32_t* num_face_windows );
extern int getRGBImageFromPNGFile( char* filename, uint8_t** img_array_ref, uint32_t* img_width, uint32_t* img_height );
extern int makePNGFileFromRGBImage( char* filename, uint8_t* img_bytes, uint32_t img_width, uint32_t img_height );
extern int drawWindowsOnRGBImage( uint8_t* img_array, uint32_t img_width, uint32_t img_height, uint32_t* face_windows_array, uint32_t num_face_windows );
extern void printRGBPixel( uint8_t* img_array, uint32_t img_width, uint32_t img_height, uint32_t i, uint32_t j );
extern int createGrayscaleFromRGBImage( uint8_t* img_array, uint32_t img_width, uint32_t img_height, uint8_t** img_grayscale_arr_ref );
extern int mapGrayscaleToRGBImage( uint8_t* img_grayscale_array, uint32_t img_width, uint32_t img_height, uint8_t** img_grayrgb_arr_ref );
extern int createIntegralImageFromGrayscaleImage( uint8_t* img_array, uint32_t img_width, uint32_t img_height, uint32_t** integral_img_arr_ref );
extern void printU32Pixel( uint32_t* img_u32_array, uint32_t img_width, uint32_t img_height, uint32_t i, uint32_t j );
extern void printU8Pixel( uint8_t* img_u8_array, uint32_t img_width, uint32_t img_height, uint32_t i, uint32_t j );
extern int compute2RecV( uint32_t* integral_img_array, uint32_t img_width, uint32_t img_height, uint32_t* window, int32_t* feature_value_ref );

#endif /* __VIOLA_JONES_H__ */