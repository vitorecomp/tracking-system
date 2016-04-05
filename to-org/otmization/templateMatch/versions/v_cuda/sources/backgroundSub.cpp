#include "../headers/backgroundSub.hpp"


Background::Background(Mat, Mat);
void Background::alloc2Image(){
    cudaMalloc(&d_a, bytes);
    cudaMalloc(&d_b, bytes);
}

void Background::allocImage(){
    cudaMalloc(&d_a, bytes);
}

void Background::tranferImage(){
    cudaMemcpy( d_a, h_a, bytes, cudaMemcpyHostToDevice);
    cudaMemcpy( d_b, h_b, bytes, cudaMemcpyHostToDevice);
}

void Background::runkernellSub(){
    vecSub<<<gridSize, blockSize>>>(d_a, d_b, d_c, n);
}

void Background::getDiffentsPoints(){
    imageReduce<<<gridSize, blockSize>>>(d_a, d_b, d_c, n);

}

Background::~Background(){
    // Release device memory
    cudaFree(d_a);
    cudaFree(d_b);
    cudaFree(d_c);
}
