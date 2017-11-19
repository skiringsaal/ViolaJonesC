#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "violajones.h"

int main( int argc, char** argv ){
    int error = 0;
    // Parse png file, and everything else
    uint8_t* img_array = NULL;
    uint32_t img_width = 0;
    uint32_t img_height = 0;
    //uint32_t* face_windows_array = NULL;
    //uint32_t num_face_windows;
    
    if( argc != 3 ){
        printf( "Viola Jones face detector cli app. ver 0.\nUsage:\nvjfd.out <input png file name> <output png file name>\n" );
        return -1;
    }

    error = getRGBImageFromPNGFile( argv[1], &img_array, &img_width, &img_height );
    if( error != 0 ){
        printf( "Error in opening file!\n" );
        return -1;
    }
    
    /* Get face windows */
    /*
    error = detectFaces( img_array, img_height, img_width, &face_windows_array, &num_face_windows );
    if( error != 0 ){
        printf( "Error while detecting faces!\n" );
        return -1;
    }

    printf( "Number of windows = %d\n", num_face_windows );
    printf("Start row: %d\nStart column: %d\nrow span: %d\nColumn span: %d\n", face_windows_array[0], face_windows_array[1], face_windows_array[2], face_windows_array[3] );

    // draw windows on the image
    error = drawWindowsOnRGBImage( img_array, img_width, img_height, face_windows_array, num_face_windows );
    if( error != 0 ){
        printf( "Error while drawing windows!\n" );
        return -1;
    }
    printf("After drawing windows on rgb image:\n");
    printRGBPixel( img_array, img_width, img_height, 10, 10 );
    printRGBPixel( img_array, img_width, img_height, 59, 59 );
    */

    /* Create a grayscale image from input image */
    uint8_t* img_grayscale_array = NULL;
    error = createGrayscaleFromRGBImage( img_array, img_width, img_height, &img_grayscale_array );
    if( error != 0){
        printf( "Failed to create grayscale image!\n" );
        return -1;
    }

    /* Create an integral image from grayscale image */
    /*
    uint32_t* integral_img_array = NULL;
    error = createIntegralImageFromGrayscaleImage( img_grayscale_array, img_width, img_height, &integral_img_array );
    if( error != 0 ){
        printf( "Failed integral image creation!\n" );
        return -1;
    }
    */
    
    /* Test values of gray image vs integral image */
    /*
    int i = 0, j = 0;
    for( i = 0; i < 3; i++ ){
        for( j = 0; j < 3; j++ ){
            printU8Pixel( img_grayscale_array, img_width, img_height, i, j );
            putchar( ',' );
            printU32Pixel( integral_img_array, img_width, img_height, i, j );
            putchar( ' ' );
        }
        putchar( '\n' );
    }
    */
    
    /* Test 2recV feature values at given windows */
    /*
    uint32_t win1[4] = { 0, 0, 3, 2 };
    int32_t fval = 0;
    error = compute2RecV( integral_img_array, img_width, img_height, win1, &fval );
    if( error != 0 ){
        printf( "Error in computing feature value.\n" );
        return -1;
    }


    printf( "feature value for window: (%d,%d,%d,%d) is %d.\n", win1[0], win1[1], win1[2], win1[3], fval );
    */
    
    /* Create an rgb image array that looks grayscale */
    
    uint8_t* img_grayrgb_array = NULL;
    error = mapGrayscaleToRGBImage( img_grayscale_array, img_width, img_height, &img_grayrgb_array );
    if( error != 0 ){
        printf( "Failed grayrgb procedure.\n" );
        return -1;
    }
    

    /* Save original image as png */
    /*
    error = makePNGFileFromRGBImage( "png_out.png", img_array, img_width, img_height );
    if( error == -1 ){
        printf("Can't make PNG file.\n" );
        return -1;
    }
    */
    

    /* Save grayrgb image as png also */
    
    error = makePNGFileFromRGBImage( argv[2], img_grayrgb_array, img_width, img_height );
    if( error == -1 ){
        printf("Can't make PNG file.\n" );
        return -1;
    }
    

    printf( "Done!\n" );

    /* Free memory */
    free( img_array );
    //free( face_windows_array );
    free( img_grayscale_array );
    free( img_grayrgb_array );
    //free( integral_img_array );

    return 0;
}
