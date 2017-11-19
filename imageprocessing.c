/* Image processing routines */
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

int createGrayscaleFromRGBImage( uint8_t* img_array, uint32_t img_width, uint32_t img_height, uint8_t** img_grayscale_arr_ref ){
	/* Create a grayscale image using the following weights on r,g,b values,
	    Y = 0.299R + 0.587G + 0.114B
	*/

	/* create an image array for holding grayscale values */
	uint8_t* img_grayscale_array = ( uint8_t* )malloc( img_height * img_width * sizeof( uint8_t ) );
	if( img_grayscale_array == NULL ){
		printf( "Malloc error!\n" );
		return -1;
	}

	int i = 0, j = 0;
	for( i = 0; i < img_height; i++ ){
		for( j = 0; j < img_width; j++ ){
			img_grayscale_array[img_width*i+j] = 0.299 * img_array[(img_width*i+j)*3+0] + 0.587 * img_array[(img_width*i+j)*3+1] + 0.114 * img_array[(img_width*i+j)*3+2];
		}
	}

	*img_grayscale_arr_ref = img_grayscale_array;
	return 0;
}

int mapGrayscaleToRGBImage( uint8_t* img_grayscale_array, uint32_t img_width, uint32_t img_height, uint8_t** img_grayrgb_arr_ref ){
	/* grayscale looking rgb image from a grayscale array, so you can use png writer on it */
	/* create an image array for holding grayscale values */
	uint8_t* img_grayrgb_array = ( uint8_t* )malloc( img_height * img_width * 3 * sizeof( uint8_t ) );
	if( img_grayrgb_array == NULL ){
		printf( "Malloc error!\n" );
		return -1;
	}

	int i = 0, j = 0;
	for( i = 0; i < img_height; i++ ){
		for( j = 0; j < img_width; j++ ){
			img_grayrgb_array[(img_width*i+j)*3+0] = img_grayscale_array[img_width*i+j];
			img_grayrgb_array[(img_width*i+j)*3+1] = img_grayscale_array[img_width*i+j];
			img_grayrgb_array[(img_width*i+j)*3+2] = img_grayscale_array[img_width*i+j];
		}
	}

	*img_grayrgb_arr_ref = img_grayrgb_array;
	return 0;
}