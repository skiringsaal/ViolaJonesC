#include <stdio.h>
#include <stdlib.h>

typedef struct _XYPair{
    float x;
    int y;
} XYPair;

typedef struct _FeatureClassifierInfo{
    int idx;
    int err;
    float thr;
} FeatureClassifierInfo;

int cmp_xypairs( const void * a, const void * b )
{
    float A = ((XYPair*)a)->x;
    float B = ((XYPair*)b)->x;
    
   return A > B ? 1 : -1;
}

int cmp_fc_by_err( const void * a, const void * b )
{
    int A = ((FeatureClassifierInfo*)a)->err;
    int B = ((FeatureClassifierInfo*)b)->err;
    
   return A < B ? 1 : -1;
}


int main(){
    int num_ex = 6;    // Number of training examples
    int num_f = 4;    // Number of features per example
    int num_excv = 10;    //

    float x_train[6][4] = {
        {  1.10,  2.30, 10.82,  6.95 },
        {  1.68,  2.47,  9.97,  6.78 },
        {  2.01,  2.53, 10.57,  6.93 },
        {  1.36,  2.49, 10.13,  7.04 },
        {  1.77,  2.50, 10.24,  7.15 },
        {  1.90,  2.28, 10.53,  7.01 }
    };
    
    int y_train[6] = {
         1,
        -1,
         1,
        -1,
        -1,
         1
    };
    
        
    float x_cv[10][4] = {
        {  1.09,  2.27, 10.57,  6.99 },
        {  1.57,  2.50, 10.68,  7.00 },
        {  1.98,  2.54, 10.89,  6.93 },
        {  1.35,  2.31,  9.99,  6.81 },
        {  1.78,  2.49, 10.21,  7.04 },
        {  1.99,  2.29, 10.59,  7.01 },
        {  1.50,  2.37, 10.03,  6.85 },
        {  1.43,  2.45, 10.28,  6.96 },
        {  1.23,  2.51, 10.69,  6.86 },
        {  1.49,  2.43,  9.96,  7.03 }
    };
    
    int y_cv[10] = {
        -1,
         1,
         1,
         1,
        -1,
         1,
        -1,
         1,
        -1,
        -1
    };
    
    int i = 0, j = 0, r = 0, e = 0;
    
    #if 1
    printf( "\nTraining examples ( Total %d ) :\n", num_ex );
    printf( "   X1    X2    X3    X4     Y\n-------------------------------- \n" );
    for( i = 0; i < num_ex; i++ ){
        for( j = 0; j < num_f; j++ ){
            printf( "%5.2f ", x_train[i][j] );
        }
        printf( "   %+d\n", y_train[i] );
    }
    #endif
    
    #if 1
    printf( "\nCross-validation examples ( Total %d ) :\n", num_excv );
    printf( "   X1    X2    X3    X4     Y\n-------------------------------- \n" );
    for( i = 0; i < num_excv; i++ ){
        for( j = 0; j < num_f; j++ ){
            printf( "%5.2f ", x_cv[i][j] );
        }
        printf( "   %+d\n", y_cv[i] );
    }
    #endif


    FeatureClassifierInfo fcinfoarray[4];
    XYPair array_xypairs[6];
    int err = 0;
    int min_err = 0;
    int min_err_idx = 0;
    float thr = 0;
    
    for( j = 0; j < num_f; j++ ){
        // Create array of x,y pairs
        for( i = 0; i < num_ex; i++ ){
            array_xypairs[i].x = x_train[i][j];
            array_xypairs[i].y = y_train[i];
        }
        
        // debug print
        #if 0
        printf( "\nFeature %d :\n", j );

        printf( "Before sorting...\n" );
        for( i = 0; i < num_ex; i++ ){
            printf( "(%f,%d)\n", array_xypairs[i].x, array_xypairs[i].y );
        }
        #endif
        
        // Sort the Pair array
        qsort( array_xypairs, num_ex, sizeof( XYPair ), cmp_xypairs );
        
        // debug print
        #if 0
        printf( "After sorting...\n" );
        for( i = 0; i < num_ex; i++ ){
            printf( "(%f,%d)\n", array_xypairs[i].x, array_xypairs[i].y );
        }
        #endif
        
        // Find threshold with minimum error
        err = 0;
        for( i = 0; i < num_ex; i++ )
            if( array_xypairs[i].y < 0 ) err++;

        min_err = err;
        min_err_idx = -1; 
        thr = 0;
        
        for( i = 0; i < num_ex; i++ ){
            if( array_xypairs[i].y > 0 ) err++;
            else err--;
        
            if( err < min_err ){
                min_err = err;
                min_err_idx = i;
            }
        }
   
        // debug print 
        #if 0
        printf( "Minimum error = %d\n", min_err );
        #endif

        if( min_err_idx == -1 ) thr = ( float )( array_xypairs[0].x - 1 );
        else if( min_err_idx == 4 ) thr = ( float )( array_xypairs[0].x + 1 );
        else thr = (float)( array_xypairs[min_err_idx].x + array_xypairs[min_err_idx+1].x ) / 2;
        
        fcinfoarray[j].idx = j;
        fcinfoarray[j].thr = thr;
        fcinfoarray[j].err = min_err;
    }
    
    #if 0
    printf( "\nClassifiers info :\n" );
    for( j = 0; j < num_f; j++ ){
        printf( "idx = %d, threshold = %5.2f, error = %d/%d\n", fcinfoarray[j].idx, fcinfoarray[j].thr, fcinfoarray[j].err, num_ex );
    }
    #endif
    
    // Sort descending by error value
    /*
    qsort( fcinfoarray, num_f, sizeof( FeatureClassifierInfo ), cmp_fc_by_err );
    
    printf( "After sorting classifiers...\n" );
    for( j = 0; j < num_f; j++ ){
        printf( "idx = %d, threshold = %5.2f, error = %d/%d\n", fcinfoarray[j].idx, fcinfoarray[j].thr, fcinfoarray[j].err, num_ex );
    }
    */
    
    // Evaluate classifiers on cross-validation data
    int prediction = 0;
    float ex_wts[10];
    int cv_eval_err_mat[10][4];

    for( j = 0; j < num_f; j++ ) {
        for( i = 0; i < num_excv; i++ ){
            prediction = x_cv[i][j] > fcinfoarray[j].thr ? 1 : -1;
            cv_eval_err_mat[i][j] = prediction == y_cv[i] ? 0 : 1;
        }
    }

    #if 0
    printf( "\nErrors in cross-validation data:\n" );
    printf( "X1 X2 X3 X4\n-------------------------------- \n" );
    for( i = 0; i < num_excv; i++ ){
        for( j = 0; j < num_f; j++ ){
            printf( "%2d ", cv_eval_err_mat[i][j] );
        }
        printf( "\n" );
    }
    #endif

    
    // Initialize weights for the examples
    int num_positive = 0;
    int num_negative = 0;
    for( i = 0; i < num_excv; i++ ){
        if( y_cv[i] == 1 ){
            num_positive++;
        } else {
            num_negative++;
        }
    }
    
    for( i = 0; i < num_excv; i++ ){
        ex_wts[i] = y_cv[i] == 1 ? ( float )( 1.0f  / ( 2.0f * num_positive ) ) : ( float )( 1.0f  / ( 2.0f * num_negative ) );
    }
    
    #if 1
    printf( "\nInitial weights for examples:\n" );
    for( i = 0; i < num_excv; i++ ){
        printf( "%f\n", ex_wts[i] );
    }
    #endif
    
    return 0;

    float sum = 0;
    int fc_idx_array[4];
    float fc_err = 0;
    float min_fc_err = 0;
    int min_fc_err_idx = 0;
    float tmp = 0;

    // Initialize feature index array, this reduces every iteration
    for( j = 0; j < num_f; j++ ){
        fc_idx_array[j] = j;
    }

    // AdaBoost main loop
    for( r = num_f; r >= 0; r-- ){
        // normalize weights
        sum = 0;
        for( i = 0; i < num_excv; i++ ){
            sum += ex_wts[i];
        }

        for( i = 0; i < num_excv; i++ ){
            ex_wts[i] = ex_wts[i] / sum;
        }

        // For every 'remaining' feature classifier, calculate weighted error and calculate min error as well

        for( e = 0; e < r; e++ ){
            fc_err = 0;
            for( i = 0; i < num_excv; i++ ){
                fc_err += cv_eval_err_mat[i][fc_idx_array[e]] * ex_wts[i];
            }

            if( e == 0 ){
                min_fc_err = fc_err;
                min_fc_err_idx = e;
            }
            else if( fc_err < min_fc_err ){
                min_fc_err = fc_err;
                min_fc_err_idx = e;
            }
        }

        // find minimum error classifier's weight

        // change example weights based on the minimum error
        
        // Eliminate minimum error classifier for future rounds
        tmp = fc_idx_array[e];
        fc_idx_array[e] = fc_idx_array[r-1];
        fc_idx_array[r-1] = tmp;

    }

    return 0;
}
