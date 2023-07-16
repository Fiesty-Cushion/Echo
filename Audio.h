#pragma once
#include <stdio.h>
#include <iostream>
#include "CallbackFactory.h"


class Audio
{
private:
	PaError error;
	PaStream* stream = nullptr;
	void ExitIfError();
public:
	Audio();
	void StartStream(Modes mode);
	~Audio();
};

