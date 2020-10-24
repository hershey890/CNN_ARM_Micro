/*
 * cnn.c
 *
 *  Created on: Oct 19, 2020
 *      Author: Hersh
 */

#include "cnn.h"

//void conv2d(float input[], float filter[], float output[], uint16_t input_size, uint16_t filter_size) {
//    /*
//     * Performs 2D convolution between input and filter.
//     * Assumes input_size > filter_size, and filter_size is odd
//     * Assumes matrix1 and matrix2 are square matrices
//     * Assumes "same" padding to ensure the output is the same size as the input.
//     */
//
//    /*
//    1 2 3 4  1 1 1
//    4 5 6 5  1 1 1
//    7 8 9 7  1 1 1
//    3 5 6 3
//    */
//    // may be better to just make this whole part a function call in parent function before conv2d is even called
//    /* Create temporarily padded matrix first, keep as a function in parent function later and pass by reference */
//    uint16_t padding_input_size = input_size + filter_size - filter_size % 2;
//    // static should ensure it is 0 initialized, but check, maybe change later when doing pass-by-refernce
//    static float padded_input[padded_input_size][padded_input_size];
//    for(uint16_t x1=0; x<input_size; x1++) {
//        for (uint16_t y1=0; y<input_size; y1++) {
//            /* If at the edge */
//            //change to an amount not just 1 value
//            if ((!x1 || !x2) || ((x1==input_size-1) || (y1==input_size-1))) {
//                padded_input[x1][y1] == something for same
//            }
//        }
//    }
//
//    static float padded_input[input_size+2][input_size+2];
//    padded_input[0][0] = input[0][0];
//    padded_input[0][input_size] = input[0][input_size-1];
//    padded_input[input_size][0] = input[input_size-1][0];
//    padded_input[input_size][input_size] = input[input_size-1][input_size-1];
//
//    for(uint16_t x1=0; x<input_size; x1++) {
//        padded_input[x1+1][0] = input[x1][0];
//        padded_input[input_size][x1+1] = input[input_size-1][x1];
//        padded_input[x1+1][input_size] = input[x1][input_size-1];
//        padded_input[0][x1+1] = input[x1][0];
//    }
//
//    for(uint16_t x1=0; x<input_size; x1++) {
//        for (uint16_t y1=0; y<input_size; y1++) {
//            padded_input[x1+1][y1+1] = input[x1][y1];
//        }
//    }
//
//
//
//
//
//
//    for(uint16_t x1=0; x<input_size; x1++) {
//        for (uint16_t y1=0; y<input_size; y1++) {
//            output_matrx[x1][y1] =
//            for(uint16_t x2=0; x2<filter_size; x2++) {
//                for(uint16_t y2=0; y2<filter_size; y2++) {
//
//                }
//            }
//        }
//    }
//}
