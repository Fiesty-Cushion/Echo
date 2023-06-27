#include <iostream>
#include <portaudio.h>
#include <deepspeech.h>

#define SAMPLE_RATE 16000
#define FRAMES_PER_BUFFER 512

StreamingState* streamingState;

// Handle PortAudio Errors
static void checkErr(PaError err) {
	if (err != paNoError) {
		printf("PortAudio Error : %s", Pa_GetErrorText(err));
		exit(EXIT_FAILURE);
	}
}

// Audio Callback Function
static int paTestCallback(
	const void* inputBuffer, void* outputBuffer, unsigned long framesPerBuffer,
	const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags,
	void* userData
) {
	char* result;

	DS_FeedAudioContent(streamingState, static_cast<const short*>(inputBuffer), framesPerBuffer);
	
	// Translate speech to text
	result = DS_IntermediateDecode(streamingState);
	std::cout << "Text : " << result << "\r";
	std::cout.flush();

	return paContinue;
}

int main() {
	// -----------DeepSpeech Stuff------------ //
	ModelState* ctx;

	bool scorer = true;

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

	// Audio Stream that DeepSpeech Utilizes
	DS_CreateStream(ctx, &streamingState);

	// ------------PortAudio Stuff------------- //
	PaError err;

	//Initialize PortAudio
	err = Pa_Initialize();
	checkErr(err);

	int numDevices = Pa_GetDeviceCount();
	//std::cout << "\n\nNumber of Available Devices :: " << numDevices << "\n" << std::endl;

	if (numDevices < 0) {
		printf("Error getting device count.\n");
		exit(EXIT_FAILURE);
	}
	else if (numDevices == 0) {
		printf("There are no available audio devices on this device.\n");
		exit(EXIT_SUCCESS);
	}

	// Display info about available audio I/O Devices
	const PaDeviceInfo* deviceInfo;
	for (int i = 0; i < numDevices; i++) {
		deviceInfo = Pa_GetDeviceInfo(i);
		printf("Device %d:\n", i);
		printf(" name: %s\n", deviceInfo->name);
		printf(" maxInuptChannels: %d\n", deviceInfo->maxInputChannels);
		printf(" maxOutputChannels: %d\n", deviceInfo->maxOutputChannels);
		printf(" defaultSampleRate: %f\n", deviceInfo->defaultSampleRate);
	}
	/*
	int inputDevice = 1;
	int outputDevice = 3;

	PaStreamParameters inputParameters;
	PaStreamParameters outputParameters;

	memset(&inputParameters, 0, sizeof(inputParameters));
	inputParameters.channelCount = 2;
	inputParameters.device = inputDevice;
	inputParameters.hostApiSpecificStreamInfo = NULL;
	inputParameters.sampleFormat = paFloat32;
	inputParameters.suggestedLatency = Pa_GetDeviceInfo(inputDevice)->defaultLowInputLatency;

	memset(&outputParameters, 0, sizeof(outputParameters));
	outputParameters.channelCount = 2;
	outputParameters.device = outputDevice;
	outputParameters.hostApiSpecificStreamInfo = NULL;
	outputParameters.sampleFormat = paFloat32;
	outputParameters.suggestedLatency = Pa_GetDeviceInfo(outputDevice)->defaultLowInputLatency;
	*/
	PaStream* stream;

	/*
	err = Pa_OpenStream(
		&stream,
		&inputParameters,
		&outputParameters,
		SAMPLE_RATE,
		FRAMES_PER_BUFFER,
		paNoFlag,
		paTestCallback,
		NULL
	);
	checkErr(err);
	*/

	// Creates an Input Stream from default Input Device
	err = Pa_OpenDefaultStream(
		&stream,
		1,
		0,
		paInt16,
		SAMPLE_RATE,
		FRAMES_PER_BUFFER,
		paTestCallback,
		NULL
	);
	checkErr(err);

	// Starts listening to Input Stream and handles Callback
	err = Pa_StartStream(stream);
	checkErr(err);

	Pa_Sleep(60 * 1000);

	// Stops listening to Input Stream
	err = Pa_StopStream(stream);
	checkErr(err);

	// Terminate Input Stream
	err = Pa_CloseStream(stream);
	checkErr(err);


	//Terminate PortAudio
	err = Pa_Terminate();
	checkErr(err);

	// Memory Management
	DS_FreeStream(streamingState);
	DS_FreeModel(ctx);

	return 0;
}