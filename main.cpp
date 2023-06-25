#include <iostream>
#include <fstream>
#include <deepspeech.h>
#include <portaudio.h>
#include <vector>

int check() {
    ModelState* ctx;
    bool scorer = true;
    char* speech;

    int status = DS_CreateModel("D:/Projects/C++/Echo/Models/deepspeech-0.9.3-models.pbmm", &ctx);
    if (status != 0) {
        char* error = DS_ErrorCodeToErrorMessage(status);
        fprintf(stderr, "Could not create model: %s\n", error);
        free(error);
        return 1;
    }

    if (scorer) {
        status = DS_EnableExternalScorer(ctx, "D:/Projects/C++/Echo/Models/deepspeech-0.9.3-models.scorer");
        if (status != 0) {
            fprintf(stderr, "Could not enable external scorer.\n");
            return 1;
        }
    }

    DS_SetModelBeamWidth(ctx, 10000);

    std::ifstream file("D:/Projects/C++/Echo/Audio/audio/8455-210777-0068.wav", std::ios::binary | std::ios::ate);
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<char> buffer(size);
    if (file.read(buffer.data(), size))
    {
        /* worked! */
        const short* myBuffer = reinterpret_cast<const short*>(buffer.data());
        speech = DS_SpeechToText(ctx, myBuffer, size);
        std::cout << speech << std::endl;
    }

    //DS_FreeString(speech);
    DS_FreeModel(ctx);

    std::cout << "PortAudio Version: " << Pa_GetVersion() << std::endl;

    return 0;
}