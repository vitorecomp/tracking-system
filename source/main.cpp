#include <iostream>
#include <stdlib.h>
#include <pthread.h>

//files
#include "./header/IOMananger.hpp"
#include "./header/ManangerCore.hpp"

//defines

//namespaces
using namespace std;


//main
int main(int argc, char *argv[]) {
    if(argc < 2){
        cout << "Por favor, entrar com os argumentos corretamente." << endl;
        cout << "./tracking_system configFile.json" << endl;
        cout << endl;
        cout << "configFile.json sendo um arquivo que contenha as configurações necessarias" << endl;
        cout << "para executar o programa" << endl;
        exit(1);
    }

    //gera as configuracoes
    io::configs.openFile(argv[1]);
    io::configs.setConfigs();

    //cria core genetico
    ManangerCore mananger_core;

    //cria populacoes
    mananger_core.startTrackelets();

    //executa o brain genetico
    mananger_core.run();

    //sumariza informações
    mananger_core.buildOutputEnv();
    mananger_core.generateInformation();

    return 0;
}
