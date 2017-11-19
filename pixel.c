#include <stdio.h>
#include <stdint.h>

void printRGBPixel( uint8_t* img_array, uint32_t img_width, uint32_t img_height, uint32_t i, uint32_t j ){
	printf( "Pixel (%d,%d) = rgb(%d,%d,%d)\n", i, j, img_array[(img_width*i+j)*3+0], img_array[(img_width*i+j)*3+1], img_array[(img_width*i+j)*3+2] );
}

void printU32Pixel( uint32_t* img_u32_array, uint32_t img_width, uint32_t img_height, uint32_t i, uint32_t j ){
	printf( "%d", img_u32_array[img_width*i+j] );
}

void printU8Pixel( uint8_t* img_u8_array, uint32_t img_width, uint32_t img_height, uint32_t i, uint32_t j ){
	printf( "%d", img_u8_array[img_width*i+j] );
}