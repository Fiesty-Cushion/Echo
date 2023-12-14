# Uploading Readme...

# ECHO - Speech to Text


![Introductory logo to the premium version of the software](/technology.png)

> Creativity is Intelligence Having Fun.           -**Albert Einstein**


## Introduction

Echo is an innovative application designed to change the way we interact with audio content. Echo takes a big step forward in turning spoken words into written text. Using Language Models, Echo offers various features like creating song lyrics, subtitles, real-time speech-to-text, and more. Covering a wide range of functions, Echo aims to provide an all-in-one solution for turning audio into text. It's made to fit different needs and situations.


<!-- ## About Whisper -->


<!-- ## The Team

@Fiesty-Cushion 
@bishal0602
@th3-roGue-deV1l -->


## Applications

1. **Speech-to-Text Conversion** :
  Real-time audio is taken from the user and display to the interface in the form of text.
2. **Karaoke Generation** :
  An audio file is received from the user and outputted as a track with karaoke features. 
3. **Subtitles Generation** :
  Video file is inputted from the user and the hard-coded subtitles are displayed on the screen.


## Installation

**For Unix Platform**
```
mkdir build
cd build
cmake ..
make
./Echo
```

**For Windows Platform**
```
mkdir build
cd build
cmake -G "MinGW Makefiles" ..
mingw32-make
./Echo.exe
```

>[!Important]
>**To run this you have to download a Language Model [ggml-model-whisper-base.en.bin](https://ggml.ggerganov.com/ggml-model-whisper-base.en.bin) and place it under `Models/` folder on root directory.**
