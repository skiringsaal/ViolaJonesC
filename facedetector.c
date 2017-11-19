
// input  : - rgb image array, in rgb 888 format,
//          - number of rows in the image, same as height
//            - number of columns in the image, same as width
// output : face windows array, and number of such windows in the array
//          - Information about a window is in this format: {start_row, start_col, num_rows, num_cols} - that's one window
//          - Several of these windows one after the other would form the face_windows_array, and the number of such windows is placed in num_face_windows.
//          - The face_windows_array must be of size 4 * num_face_windows

#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>

int detectFaces( uint8_t* image_array, uint32_t im_rows, uint32_t im_cols, uint32_t** face_windows_array_ref, uint32_t* num_face_windows ){
    /* Placeholder function : returns arbitrarily chosen windows for now
       TODO: Replace by an actual face detection algorithm
    */

    /* do input checks */
    if( image_array == NULL || im_rows == 0 || im_cols == 0 ){
        printf( "Invalid inputs!\n" );
        return -1;
    }

    uint32_t nfw = 2;
    // create windows array
    uint32_t* ptr = ( uint32_t* )malloc( nfw * 4 * sizeof( uint32_t ) );
    if( ptr == NULL ){
        printf( "Failed malloc!\n" );
        return -1;
    }

    /* fill the array with 1 face window */
    ptr[0] = 10;
    ptr[1] = 10;
    ptr[2] = 50;
    ptr[3] = 50;

    /* fill second array with face window numbers */
    ptr[4] = 70;
    ptr[5] = 70;
    ptr[6] = 100;
    ptr[7] = 100;

    *face_windows_array_ref = ptr;    // callers responsibility to clean up before exiting.
    *num_face_windows = nfw;
    return 0;
}


/* Viola Jones algorithm related routines */

int createIntegralImageFromGrayscaleImage( uint8_t* img_array, uint32_t img_width, uint32_t img_height, uint32_t** integral_img_arr_ref ){
    /*
    - Takes in a grayscale image array and produces an 'integral image' as described in Viola Jones paper.
    - A value at (i,j) = sum of all image pixels above and to the left of this pixel, including pixel (i,j).
    - Algorithm:
        - First calculate row sum values, rs(i,j) = sum of pixels in row i upto and including pixel (i,j).
        - Computing rs first makes computing integral image values much much easier and faster.
    */


    /* allocate arrays rs and ii */
    uint32_t* ii = NULL;    /* integral image */
    uint32_t* rs = NULL;    /* row sum array */

    ii = ( uint32_t* )malloc( img_width * img_height * sizeof( uint32_t ) );
    if( ii == NULL ){
        printf( "Malloc failed!\n" );
        return -1;
    }

    rs = ( uint32_t* )malloc( img_width * img_height * sizeof( uint32_t ) );
    if( rs == NULL ){
        printf( "Malloc failed!\n" );
        return -1;
    }

    /* calculate rs values */
    int i = 0, j = 0;
    for( i = 0; i < img_height; i++ ){
        rs[img_width*i+0] = ( uint32_t )img_array[img_width*i+0];
        for( j = 1; j < img_width; j++ ){
            rs[img_width*i+j] = rs[img_width*i+(j-1)] + ( uint32_t )img_array[img_width*i+j];
        }
    }

    /* calculate ii values */
    for( j = 0; j < img_width; j++ ){
        ii[img_width*0+j] = rs[img_width*0+j];  /* First row of ii is identical to that of rs */
    }

    for( i = 1; i < img_height; i++ ){
        ii[img_width*i+0] = ii[img_width*(i-1)+0] + rs[img_width*i+0];    /* First column of ii, each item is sum of ii above + current rs */
    }
    
    for( i = 1; i < img_height; i++ ){
        for( j = 1; j < img_width; j++ ){
            ii[img_width*i+j] = ii[img_width*(i-1)+j] + rs[img_width*i+j];    /* ii at any point equals ii above + rs at the point */
        }
    }

    *integral_img_arr_ref = ii;

    free( rs );
    return 0;
}


/* 
Compute 2-rectangle, vertical split feature over a given window in an image
- Information about a window is in this format: {start_row, start_col, num_rows, num_cols}
- 2RecV means find the value of 2 rectangle, vertical split feature.
- The whole window rectangle is split vertically into two rectangles, as shown below
-----------
| 1  |  2 |
-----------
- The sums of pixels within each rectangle are found and their difference (rect 1 sum - rect 2 sum) is returned as output.
*/
 
int compute2RecV( uint32_t* integral_img_array, uint32_t img_width, uint32_t img_height, uint32_t* window, int32_t* feature_value_ref ){
    if( integral_img_array == NULL || img_width == 0 || img_height == 0 || window == NULL ){
        printf( "Input error!\n" );
        return -1;
    }

    int32_t sr = ( int32_t )window[0];    // start row.
    int32_t sc = ( int32_t )window[1];    // start col
    int32_t nr = ( int32_t )window[2];    // height in rows
    int32_t nc = ( int32_t )window[3];    // width in columns

    /* if window is not within image bounds, return error */
    if( sr < 0 || sc < 0 || sr+nr > img_height || sc+nc > img_width ){
        printf( "Window dimensions exceed integral image dimensions!\n" );
        return -1;
    }

    /* if window width or height is 0, return error */
    if( nr == 0 || nc == 0 ){
        printf( "Window size is 0!\n" );
        return -1;
    }

    if( nc % 2 != 0 ){
        printf( "Error: Window width is not a multiple of 2.\n" );
        return -1;
    }

    int32_t pxA = 0;    // Value at point A, top left of the window.
    int32_t pxB = 0;    // Value at point B, top middle of the window.
    int32_t pxC = 0;    // Value at point C, top right of the window.
    int32_t pxD = 0;    // Value at point D, bottom left of the window.
    int32_t pxE = 0;    // Value at point E, bottom middle of the window.
    int32_t pxF = 0;    // Value at point F, bottom right of the window.
    
    uint32_t* ii = integral_img_array;

    if( (sr-1) >= 0 && (sc-1) >= 0 ){
        printf( "sr = %d, sc = %d\n", sr, sc );
        pxA = ( int32_t )( ii[img_width*(sr-1)+(sc-1)] );
    }

    if( (sr-1) >= 0 && (sc+(nc/2)-1) >= 0 ){
        pxB = (int32_t)ii[img_width*(sr-1)+(sc+(nc/2)-1)];
    }
    
    if( (sr-1) >= 0 && (sc+nc-1) >= 0 ){
        pxC = (int32_t)ii[img_width*(sr-1)+(sc+nc-1)];
    }

    if( (sr+nr-1) >= 0 && (sc-1) >= 0 ){
        pxD = (int32_t)ii[img_width*(sr+nr-1)+(sc-1)];
    }

    if( (sr+nr-1) >= 0 && (sc+(nc/2)-1) >= 0 ){
        pxE = (int32_t)ii[img_width*(sr+nr-1)+(sc+(nc/2)-1)];
    }
    
    if( (sr+nr-1) >= 0 && (sc+nc-1) >= 0 ){
        pxF = (int32_t)ii[img_width*(sr+nr-1)+(sc+nc-1)];
    }

    /* find difference of sums rect 1 - rect 2. */
    int32_t dif = ( 2*pxE + pxA + pxC ) - ( 2*pxB + pxD + pxF );
    
    /* return this value */
    *feature_value_ref = dif;

    return 0;
}
