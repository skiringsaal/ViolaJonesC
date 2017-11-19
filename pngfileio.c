#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <png.h>

int getRGBImageFromPNGFile( char* filename, uint8_t** img_array_ref, uint32_t* img_width, uint32_t* img_height ){ 
    /* open file */
    if( filename == NULL ){
        printf( "Bad file name.\n" );
        return -1;
    }

    printf( "Input is %s\n", filename );
    FILE* fp = NULL;
    fp = fopen( filename, "rb" );
    if( fp == NULL ){
        printf( "Could not open file.\n" );
        return -1;
    }

    /* read png file */    
    char header[8];    // 8 is the maximum size that can be checked
    int x, y;
    int width, height;
    png_byte color_type;
    png_byte bit_depth;
    png_structp png_ptr;
    png_infop info_ptr;
    int number_of_passes;
    png_bytep* row_pointers;
    
     
    /* open file and test for it being a png */
    fread( header, 1, 8, fp );
    if ( png_sig_cmp( header, 0, 8 ) ){
        printf( "File %s is not recognized as a PNG file", filename );
        return -1;
    }
    
    /* initialize stuff */
    png_ptr = png_create_read_struct( PNG_LIBPNG_VER_STRING, NULL, NULL, NULL );
    if( png_ptr == NULL ){
        printf( "png_create_read_struct failed\n" );
        return -1;
    }
    
    info_ptr = png_create_info_struct( png_ptr );
    if( info_ptr == NULL ){
        printf( "png_create_info_struct failed\n" );
        return -1;
    }
    
    if( setjmp( png_jmpbuf( png_ptr ) ) ){
        printf( "Error during init_io\n" );
        return -1;
    }
    
    png_init_io( png_ptr, fp );
    png_set_sig_bytes( png_ptr, 8 );
    
    png_read_info( png_ptr, info_ptr );
    
    width = png_get_image_width( png_ptr, info_ptr );
    height = png_get_image_height( png_ptr, info_ptr) ;
    color_type = png_get_color_type( png_ptr, info_ptr );
    bit_depth = png_get_bit_depth( png_ptr, info_ptr );
    
    number_of_passes = png_set_interlace_handling( png_ptr );
    png_read_update_info( png_ptr, info_ptr );

    if( color_type != PNG_COLOR_TYPE_RGB ){
        printf( "Color type is not RGB.\n" );
        return -1;
    }

    /* read file */
    if( setjmp( png_jmpbuf( png_ptr ) ) ){
        printf( "Error during read_image\n" );
        return -1;
    }
    
    row_pointers = ( png_bytep* )malloc( height * sizeof( png_bytep ) );
    for( y = 0; y < height; y++ ){
        row_pointers[y] = ( png_byte* )malloc( png_get_rowbytes( png_ptr, info_ptr ) );
    }
    
    png_read_image( png_ptr, row_pointers );

    fclose( fp );
    
    /* Create new uint8_t* buffer */
    uint8_t* img_buf = ( uint8_t* )malloc( width * 3 * height * sizeof( uint8_t ) );
    if( img_buf == NULL ){
        printf( "Malloc failure.\n" );
        return -1;
    }
    
    /* Copy pixels to buffer */ 
    uint32_t i = 0;
    for( y = 0; y < height; y++ ){
        png_byte* row = row_pointers[y];
        for( x = 0; x < width; x++ ){
            png_byte* ptr = &( row[ x * 3 ] );
            //printf("Copying ( %d, %d ) = rgb( %d, %d, %d )\n", x, y, ptr[0], ptr[1], ptr[2] );
            
            img_buf[i]     = ptr[0];
            img_buf[i + 1] = ptr[1];
            img_buf[i + 2] = ptr[2];
            
            i += 3;
        }
    }
    
    /* Assign to outputs */
    *img_array_ref = img_buf;
    *img_width = ( uint32_t )width;
    *img_height = ( uint32_t )height;
    //printf( "DEBUG: *img_array_ref = %s, img_width = %d, and img_height = %d\n", ( *img_array_ref == NULL ? "NULL" : "Not NULL" ), *img_width, *img_height );
       
    /* free memory */ 
    for( y = 0; y < height; y++ ){
        free( row_pointers[y] );
    }
    free( row_pointers );
    
    return 0;
}

/* ------------------------------------------------*/

int makePNGFileFromRGBImage( char* filename, uint8_t* img_bytes, uint32_t img_width, uint32_t img_height ){ 

    /* Check inputs */
    if( img_bytes == NULL || img_width == 0 || img_height == 0 ){
        printf( "Bad inputs.\n" );
        return -1;
    }

    /* create file */
    if( filename == NULL ){
        printf( "Bad file name.\n" );
        return -1;
    }

    printf( "Output is %s\n", filename );
    FILE* fp = NULL;
    fp = fopen( filename, "wb" );
    if( fp == NULL ){
        printf( "Could not open file for writing.\n" );
        return -1;
    }

    /* local variables */
    int y;
    png_structp png_ptr;
    png_infop info_ptr;
    png_bytep* row_pointers;

    /* Create row pointers from img buffer */
    row_pointers = ( png_bytep* )malloc( img_height * sizeof( png_bytep ) ); // Free at the end
    for( y = 0; y < img_height; y++ ){
        row_pointers[y] = ( png_byte* )( &img_bytes[ y * img_width * 3 ] );
    }

    /* initialize stuff */
    png_ptr = png_create_write_struct( PNG_LIBPNG_VER_STRING, NULL, NULL, NULL );

    if( png_ptr == NULL ){
        printf( "png_create_write_struct failed" );
        return -1;
    }

    info_ptr = png_create_info_struct( png_ptr );
    if( info_ptr == NULL ){
        printf( "png_create_info_struct failed" );
        return -1;
    }
    
    if( setjmp( png_jmpbuf( png_ptr ) ) ){
        printf( "Error during init_io" );
        return -1;
    }

    png_init_io( png_ptr, fp );

    /* write header */
    if( setjmp( png_jmpbuf( png_ptr ) ) ){
        printf( "Error during writing header" );
    }

    png_set_IHDR( png_ptr, info_ptr, ( png_uint_32 )img_width, ( png_uint_32 )img_height, 8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE );

    png_write_info( png_ptr, info_ptr );

    /* write bytes */
    if( setjmp( png_jmpbuf( png_ptr ) ) ){
        printf( "Error during writing bytes" );
        return -1;
    }

    png_write_image( png_ptr, row_pointers );

    /* end write */
    if( setjmp( png_jmpbuf( png_ptr ) ) ){
        printf( "Error during end of write" );
    }

    png_write_end( png_ptr, NULL );

    fclose( fp );

    /* free memory */
    free( row_pointers );

    return 0;
}