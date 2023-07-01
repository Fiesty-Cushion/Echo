#include "whisper.h"
#include <iostream>

int main() {
    struct whisper_context* ctx = whisper_init_from_file("D:/Development/whisper/ggml-model-whisper-base.en.bin");
    std::cout << "Works?" << std::endl;
    whisper_free(ctx);
}
     