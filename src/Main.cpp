#include "Dream.h"
#include "ArgumentParser.h"
#include <iostream>

int main(int argc, const char** argv) {
    std::cout << "Dream: Starting..." << std::endl;
    Dream::Dream dream;
    dream.loadFromArgumentParser(new Dream::ArgumentParser(argc,argv));
    return dream.runProject();
}
