#ifndef __arm__
#include <stdio.h>

#define int16_t     short
#define uint16_t    unsigned short
#define int32_t     int
#define uint32_t    unsigned int

void _pad_matrix(float* input, float* output, uint16_t input_size, uint16_t filter_size);
#endif

int main(void) {
    float test_matrix[6][6] = {{1,4,7,6,8,4}, {4,3,2,9,0,1}, {9,4,5,0,4,7}, {4,2,3,9,5,8}, {1,0,9,3,5,7}, {9,3,4,5,7,4}};
    print_matrix(&test_matrix, 6);
    #ifndef __arm__
    printf("\n");
    #endif
    /* Padded matrix with dimensions x_len+2, y_len+2 */
    static float padded_matrix[8][8];
    pad_matrix(&test_matrix, &padded_matrix, 6u, 5u);
    #ifndef __arm__
    print_matrix(&padded_matrix, 10u);
    #endif
}

#ifndef __arm__
void print_matrix(float* matrix, uint16_t matrix_size) {
    /*
     * Prints out a square matrix. 
     * matrix: address of matrix (just pass &matrix)
     * matrix_size: number of rows/columns
     */
    for(int i=0; i<matrix_size; i++) {
        for(int j=0; j<matrix_size; j++) {
            printf("%f ", *(matrix + i*matrix_size + j));
        }
        printf("\n");
     }
}
#endif

void pad_matrix(float* input, float* output, uint16_t input_size, uint16_t filter_size) {
    /* Padding Corners: Format (output + row# + col#) */
    uint16_t padding_size = filter_size >> 1;
    uint16_t output_size = input_size + (padding_size << 1);
    for(int16_t x=0; x<output_size; x++) {
        for(int16_t y=0; y<output_size; y++) {
            /* Copying matrix */
            if((x>=padding_size && y>=padding_size) && (x<output_size-padding_size && y<output_size-padding_size)) {
                *(output + y*output_size + x) = *(input + (y-padding_size)*input_size + (x-padding_size));
            /* Padding top */
            } else if(y<padding_size && x>=padding_size && x<output_size-padding_size) {
                *(output + y*output_size + x) = *(input + (x-padding_size));
            /* Padding bottom */
            } else if(y>=output_size-padding_size && x>=padding_size && x<output_size-padding_size) {
                *(output + y*output_size + x) = *(input + input_size*(input_size-1) + (x-padding_size));
            /* Padding left */
            } else if(x<padding_size && y>=padding_size && y<output_size-padding_size) {
                *(output + y*output_size + x) = *(input + (y-padding_size)*input_size);
            /* Padding right */
            } else if(x>=output_size-padding_size && y>=padding_size && y<output_size-padding_size) {
                *(output + y*output_size + x) = *(input + (y-padding_size)*input_size + (input_size-1));
            /* Top left corner */
            } else if(y<padding_size && x<padding_size) {
                *(output + y*output_size + x) = *input;
            /* Top right corner */
            } else if(y<padding_size && x>=output_size-padding_size) {
                *(output + y*output_size + x) = *(input + 0*input_size + (input_size-1));
            /* bottom left corner */
            } else if(x<padding_size && y>=output_size-padding_size) {
                *(output + y*output_size + x) = *(input + input_size*(input_size-1) + 0);
            /* Bottom right corner */
            } else if(x>=output_size-padding_size && y>=output_size-padding_size) {
                *(output + y*output_size + x) = *(input + input_size*input_size - 1);
            } 
            #ifndef __arm__
            else {
                printf("\n\nError in pad_matrix\n\n");
            }
            #endif
        }
    }
}