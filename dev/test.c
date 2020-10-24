#include <float.h> //may be an issue on STM32

#define MINUS_1(x) ((x) & ~0x1))

#ifndef __arm__
#include <stdio.h>
#define int16_t     short
#define uint16_t    unsigned short
#define int32_t     int
#define uint32_t    unsigned int
/*
 * Prints out a square matrix. 
 * matrix: address of matrix (just pass &matrix)
 * matrix_size: number of rows/columns
 */
void  _print_matrix(double* matrix, uint16_t matrix_size);
#endif

void _pad_matrix(double* input, double* output, uint16_t input_size, uint16_t filter_size, uint16_t input_depth);

/* input: pass 2d array with &input. 
 * output: 2d array with correct dimensions. e.g. if input is 8x8, and pool_size is 2, output should be 4x4
 * Ensure the input matrix is even
 * Only works for pools that are square
 */
void max_pool(double* input, double* output, uint16_t input_size, uint16_t pool_size, uint16_t input_depth);

/*
 * Assumes the matrices are both square matrices of the same size
 */ 
// inline
void _matrix_multiplication(double* input1, double* input2, double* output, uint16_t input_size, uint16_t input_num_cols, uint16_t input_x_offset);

/*
 * Pass a padded matrix for input
 */ 
void _convolution(double* input, double* filter, double* output, uint16_t input_size, uint16_t filter_size, uint16_t input_depth);

int main(void) {
    enum {matrix_size = 6};
    double test_matrix[matrix_size][matrix_size] = {{1,4,7,6,8,4}, {4,3,2,9,0,1}, {9,4,5,0,4,7}, {4,2,3,9,5,8}, {1,0,9,3,5,7}, {9,3,4,5,7,4}};
    #ifndef __arm__
    _print_matrix(&test_matrix, 6);
    printf("\n");
    #endif
    /* Padded matrix with dimensions x_len+2, y_len+2 */
    const uint16_t padding_amount = 2;
    const uint16_t padded_matrix_size = matrix_size + padding_amount;
    double padded_matrix[padded_matrix_size][padded_matrix_size];

    _pad_matrix(&test_matrix, &padded_matrix, matrix_size, padding_amount, 1);

    #ifndef __arm__
    _print_matrix(&padded_matrix, padded_matrix_size);
    #endif

    double pool_matrix[4][4];
    const uint16_t pool_size = 2;
    const uint16_t pool_matrix_size = padded_matrix_size / pool_size;
    max_pool(&padded_matrix, &pool_matrix, padded_matrix_size, pool_size, 1);
    #ifndef __arm__
    printf("\n");   
    _print_matrix(&pool_matrix, pool_matrix_size);
    #endif

    double matrix1[2][2] = {{1,2}, {8,4}};
    double matrix2[2][2] = {{5,6}, {4,5}};
    double output_matrix[2][2];
    _matrix_multiplication(&matrix1, &matrix2, &output_matrix, 2u, 0, 0);
    #ifndef __arm__
    printf("\n");
    _print_matrix(&output_matrix, 2);
    #endif

    enum {filter_size = 3};
    double filter[filter_size][filter_size] = {{0.5, 0.78, 1.32},{1.1,2.11, 1.43},{0.39, 0.13, 0.89}};
    static double conv_matrix[matrix_size][matrix_size];
    _convolution(&padded_matrix, &filter, &conv_matrix, padded_matrix_size, filter_size, 1);
    #ifndef __arm__
    printf("\n");
    _print_matrix(&conv_matrix, 6);
    #endif
}

#ifndef __arm__
void _print_matrix(double* matrix, uint16_t matrix_size) {
    for(int i=0; i<matrix_size; i++) {
        for(int j=0; j<matrix_size; j++) {
            printf("%f ", *(matrix + i*matrix_size + j));
        }
        printf("\n");
     }
}
#endif

void _pad_matrix(double* input, double* output, uint16_t input_size, uint16_t filter_size, uint16_t input_depth) {
    /* Padding Corners: Format (output + row# + col#) */
    uint16_t padding_size = filter_size >> 1;
    uint16_t output_size = input_size + (padding_size << 1);
    uint16_t x_coeff, y_coeff;
    for(int16_t z=0; z<input_depth; z++) {
        for(int16_t x=0; x<output_size; x++) {
            for(int16_t y=0; y<output_size; y++) {
                y_coeff =   y < padding_size                ?   0               : 
                            y >= output_size-padding_size   ?   input_size - 1  :
                                                                y - padding_size;
                x_coeff =   x < padding_size                ?   0               : 
                            x >= output_size-padding_size   ?   input_size - 1  :
                                                                x - padding_size;

                *(output + y*output_size + x + z*output_size*output_size) = *(input + y_coeff*input_size + x_coeff + z*input_size*input_size);


                // /* Copying matrix */
                // if((x>=padding_size && y>=padding_size) && (x<output_size-padding_size && y<output_size-padding_size)) {
                //     *(output + y*output_size + x + z*output_size*output_size) = *(input + (y-padding_size)*input_size + (x-padding_size) + z*input_size*input_size);
                // /* Padding top */
                // } else if(y<padding_size && x>=padding_size && x<output_size-padding_size) {
                //     *(output + y*output_size + x) = *(input + (x-padding_size));
                // /* Padding bottom */
                // } else if(y>=output_size-padding_size && x>=padding_size && x<output_size-padding_size) {
                //     *(output + y*output_size + x) = *(input + (input_size-1)*input_size + (x-padding_size));
                // /* Padding left */
                // } else if(x<padding_size && y>=padding_size && y<output_size-padding_size) {
                //     *(output + y*output_size + x) = *(input + (y-padding_size)*input_size);
                // /* Padding right */
                // } else if(x>=output_size-padding_size && y>=padding_size && y<output_size-padding_size) {
                //     *(output + y*output_size + x) = *(input + (y-padding_size)*input_size + (input_size-1));
                // /* Top left corner */
                // } else if(y<padding_size && x<padding_size) {
                //     *(output + y*output_size + x) = *input;
                // /* Top right corner */
                // } else if(y<padding_size && x>=output_size-padding_size) {
                //     *(output + y*output_size + x) = *(input + 0*input_size + (input_size-1));
                // /* bottom left corner */
                // } else if(x<padding_size && y>=output_size-padding_size) {
                //     *(output + y*output_size + x) = *(input + input_size*(input_size-1) + 0);
                // /* Bottom right corner */
                // } else if(x>=output_size-padding_size && y>=output_size-padding_size) {
                //     *(output + y*output_size + x) = *(input + input_size*input_size - 1);
                // } 
                // #ifndef __arm__
                // else {
                //     printf("\n\nError in pad_matrix\n\n");
                // }
                // #endif
            }
        }
    }
}

// the i>>1 only makes this work for 2x2 pools right now, change to i/pool_size pool_size != 2
void max_pool(double* input, double* output, uint16_t input_size, uint16_t pool_size, uint16_t input_depth) {
    uint16_t x, y, i;
    double max;
    for(y=0; y<input_size; y+=pool_size){ 
        for(x=0; x<input_size; x+=pool_size) {
            //get rid of this earlier and store directly in output
            max = -DBL_MAX;
            for(i=0; i<pool_size*pool_size; i++) {
                if(*(input + (y+(i>>1))*input_size + x + i%pool_size) > max) {
                    max = *(input + (y+(i>>1))*input_size + x + i%pool_size);
                }
            }
            *(output + (y>>1)*(input_size>>1) + (x>>1)) = max;
        }
    }
}

// inline
/*
 * Input and filter must be square matrices of the same size. 
 * Only for use in _convolution, not general purpose
 * 
 * Arguments:
 *  input1: pointer to 
 *  input_num_cols: if calling in _convolution, pass the number of columns in the original input (ie larger matrix) not just the convolved region
 *  input_x_offset: if calling in _convolution, pass the x-coordinate of the top-left corner of the matrix
 */ 
void _matrix_multiplication(double* input1, double* input2, double* output, uint16_t input_size, uint16_t input_num_cols, uint16_t input_x_offset) {
    uint16_t i, j, k;
    double sum;
    /* if passing from convolution, the actual number of columns != input_size, so adding y*input_size to the address won't work*/
    uint16_t input1_size = input_num_cols ? input_num_cols : input_size;
    for(i=0; i<input_size; i++) {
        for(j=0; j<input_size; j++) {
            sum = 0;
            for(k=0; k<input_size; k++)
                sum += (*(input1 + i*input1_size + k + input_x_offset))*(*(input2 + k*input_size + j));
            *(output + i*input1_size + j + input_x_offset) = sum;
        }
    }
}

//actually correlation, only 2D
void _convolution(double* input, double* filter, double* output, uint16_t input_size, uint16_t filter_size, uint16_t input_depth) {
    uint16_t x,y,i;
    uint16_t output_size = input_size - (filter_size & ~0x1);
    double sum;
    for(x=0; x<input_size-filter_size+1; x++) {
        for(y=0; y<input_size-filter_size+1; y++) {
            sum = 0;
            for(i=0; i<filter_size*filter_size; i++) {
                sum += *(input + (y+(i/filter_size))*input_size + x + i%filter_size)*(*(filter + i));
            }
            *(output + y*output_size + x) = sum;
        }
    }
}