#include "Dream.h"
#include "ArgumentParser.h"

int main(int argc, const char** argv) {
    Dream::Dream dream;
    dream.loadFromArgumentParser(new Dream::ArgumentParser(argc,argv));
    return dream.runProject();
}
