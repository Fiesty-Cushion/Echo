#include "CallbackFactory.h"

StreamCallback CallbackFactory::Generate(Modes mode)
{
	switch (mode)
	{
	case Modes::RealTime:
		return RealTimeCallback;
		break;
	case Modes::Subtitles:
		return SubtitlesCallback;
		break;
	case Modes::Lyrics:
		return LyricsCallback;
		break;
	default:
		break;
	}
}
int CallbackFactory::RealTimeCallback(const void* inputBuffer, void* outputBuffer, unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void* userData)
{
	if (stopRequested)
		return paComplete;
	const float* input = static_cast<const float*>(inputBuffer);

	for (int i = 0; i < framesPerBuffer; i++)
	{
		pcm32_buffer.insert(pcm32_buffer.begin() + static_cast<std::ptrdiff_t>(count) * PA_FRAMES_PER_BUFFER, 1, input[i]);
	}
	count++;
	if (pcm32_buffer.size() >= PA_SAMPLE_RATE * 2)
	{
		std::unique_lock<std::mutex> lock(bufferMutex);
		pcm32_bufferQueue.push(pcm32_buffer);
		count = 0;
		pcm32_buffer.clear();
		bufferCV.notify_one();
	}

	return paContinue;
}

int CallbackFactory::SubtitlesCallback(const void* inputBuffer, void* outputBuffer, unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void* userData)
{
	// TODO: Implement this
	return 0;
}

int CallbackFactory::LyricsCallback(const void* inputBuffer, void* outputBuffer, unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void* userData)
{
	// TODO: Implement this
	return 0;
}