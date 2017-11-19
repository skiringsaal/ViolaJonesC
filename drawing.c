#include <stdint.h>

int drawWindowsOnRGBImage( uint8_t* img_array, uint32_t img_width, uint32_t img_height, uint32_t* face_windows_array, uint32_t num_face_windows ){
    /* TODO: Do checks on input, and make sure windows are within image bounds */
    /* draw windows on the image */
    int n = 0, i = 0, j = 0;
    for( n = 0; n < num_face_windows; n++ ){
        uint32_t win_start_row = face_windows_array[n*4+0];
        uint32_t win_start_col = face_windows_array[n*4+1];
        uint32_t win_num_row = face_windows_array[n*4+2];
        uint32_t win_num_col = face_windows_array[n*4+3];
        
        /* Draw top edge */
        for( j = win_start_col; j < win_start_col + win_num_col; j++ ){
            img_array[ ( win_start_row * img_width + j ) * 3 + 0 ] = 0;
            img_array[ ( win_start_row * img_width + j ) * 3 + 1 ] = 255;
            img_array[ ( win_start_row * img_width + j ) * 3 + 2 ] = 0;
        }
    
        /* Draw bottom edge */
        for( j = win_start_col; j < win_start_col + win_num_col; j++ ){
            img_array[ ( ( win_start_row + win_num_row - 1 ) * img_width + j ) * 3 + 0 ] = 0;
            img_array[ ( ( win_start_row + win_num_row - 1 ) * img_width + j ) * 3 + 1 ] = 255;
            img_array[ ( ( win_start_row + win_num_row - 1 ) * img_width + j ) * 3 + 2 ] = 0;
        }
        
        /* Draw left edge */
        for( i = win_start_row; i < win_start_row + win_num_row; i++ ){
            img_array[ ( i * img_width + win_start_col ) * 3 + 0 ] = 0;
            img_array[ ( i * img_width + win_start_col ) * 3 + 1 ] = 255;
            img_array[ ( i * img_width + win_start_col ) * 3 + 2 ] = 0;
        }
        
        /* Draw right edge */
        for( i = win_start_row; i < win_start_row + win_num_row; i++ ){
            img_array[ ( i * img_width + ( win_start_col + win_num_col - 1 ) ) * 3 + 0 ] = 0;
            img_array[ ( i * img_width + ( win_start_col + win_num_col - 1 ) ) * 3 + 1 ] = 255;
            img_array[ ( i * img_width + ( win_start_col + win_num_col - 1 ) ) * 3 + 2 ] = 0;
        }    
    }
    

    return 0;
}