#include <iostream>
#include <portaudio.h>

//Handle PortAudio Errors
static void checkErr(PaError err) {
	if (err != paNoError) {
		printf("PortAudio Error : %s", Pa_GetErrorText(err));
		exit(EXIT_FAILURE);
	}
}

int main() {
	PaError err;
	
	//Initialize PortAudio
	err = Pa_Initialize();
	checkErr(err);

	int numDevices = Pa_GetDeviceCount();
	std::cout << "\n\nNumber of Available Devices :: " << numDevices << "\n" << std::endl;

	const PaDeviceInfo* deviceInfo;
	for (int i = 0; i < numDevices; i++) {
		deviceInfo = Pa_GetDeviceInfo(i);
		std::cout << "Device " << i << "\n";
		std::cout << "\tname: " << deviceInfo->name << std::endl;
	}

	//Terminate PortAudio
	err = Pa_Terminate();
	checkErr(err);

	return 0;
}