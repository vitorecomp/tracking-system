#include <iostream>

#include "../headers/backgroundSub.hpp"
#include "../headers/pointsFinder.hpp"
#include "../headers/pointsRelation.hpp"


using namespace std;

// CUDA kernel. Each thread takes care of one element of c
__global__ void subVector(int *a, int *b, int *d, int *N){
    int tid = threadIdx.x + blockIdx.x * blockDim.x;
    if (tid<*N) {
        d[tid] = a[tid] - b[tid];
    }
}

int main(int argc, char *argv[]){
    //vendo se o numero de argumento esta correto
    if(argc < 3){
        cout << "Erro nos parametros" << endl;
        cout << "Para utilizar:" << endl;
        cout << "./parallelTracking video" << endl;
    }

    Video video(argv[1]);
    Cuda cuda(video.imageSize());
    cuda.tansfer(video.ini_frame());
    //pegando o video enquanto tem quadros, a serem computados
    while(1){
        //se tem espaço em video
        if(cuda.hasMemory()){
            Video.hasframes();
            //movendo para a memoria de video

            //setando na lista de quadros a posic desta
        }
        //tem posic de memorias não computadas
        //assincrona

            //fazendo a subtração com o quadro anterior

            //pegando os pontos que se movimentotaram

            //fazendo a relação com os objetos anteriores

            //liberando imagens da memoria

        //puxando as relações da memoria de video
            //jogando em momoria

    //finalizado a contagem de tempo
    //imprimindo os resultados
    }
}
