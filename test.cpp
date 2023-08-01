/*******************************************************************************************
*
*   raylib MPEG video player
*
*   We have two options to decode video & audio using pl_mpeg.h library:
*
*   1) Use plm_decode() and just hand over the delta time since the last call.
*      It will decode everything needed and call your callbacks (specified through
*      plm_set_{video|audio}_decode_callback()) any number of times.
*
*   2) Use plm_decode_video() and plm_decode_audio() to decode exactly one
*      frame of video or audio data at a time. How you handle the synchronization of
*      both streams is up to you.
*
*   This example uses option 2) and handles synchonization manually.
*
*   This example has been created using raylib 3.0 (www.raylib.com)
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   Copyright (c) 2020 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib.h"

#include <stdlib.h>     // Required for: NULL

#define PL_MPEG_IMPLEMENTATION
#include "pl_mpeg.h"

int plm_get_total_video_frames(const char *filename)
{
    int total_frames = 0;
    
    plm_t *plm = plm_create_with_filename(filename);

    if (plm != NULL)
    {
        plm_frame_t *frame = plm_skip_video_frame(plm);
        while (frame != NULL)
        {
            total_frames++;
            frame = plm_skip_video_frame(plm);
        }

        plm_destroy(plm);
    }

    return total_frames;
}

int plm_get_total_audio_frames(const char *filename)
{
    int total_frames = 0;
    
    plm_t *plm = plm_create_with_filename(filename);

    if (plm != NULL)
    {
        plm_samples_t *sample = plm_decode_audio(plm);
        while (sample != NULL)
        {
            total_frames++;
            sample = plm_decode_audio(plm);
        }

        plm_destroy(plm);
    }

    return total_frames;
}

int main(void)
{
    // Initialization
    //---------------------------------------------------------
    const int screenWidth = 960;
    const int screenHeight = 540;

    InitWindow(screenWidth, screenHeight, "raylib MPEG video player");
    
    InitAudioDevice();
    
    plm_t *plm = NULL;
	plm_frame_t *frame = NULL;
    plm_samples_t *sample = NULL;
    double framerate = 0.0;
    int samplerate = 0;
    
    Image imFrame = { 0 };
    Texture texture = { 0 };
    AudioStream stream = { 0 };

    double baseTime = 0.0;
    double timeExcess = 0.0;
    bool pause = false;
    
    int totalVideoFrames = 0;
    int currentVideoFrame = 0;
    
    // NOTE: Every audio frame contains 1152 samples
    int totalAudioFrames = 0;
    int currentAudioFrame = 0;
    
    Rectangle timeBar = { 0 };

    //SetTargetFPS(60);     // Let the app run at full speed!
    //----------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //-----------------------------------------------------
        if (IsFileDropped())
        {
            int dropsCount = 0;
            char **droppedFiles = GetDroppedFiles(&dropsCount);
            
            if ((dropsCount == 1) && IsFileExtension(droppedFiles[0], ".mpg"))
            {
                if (plm != NULL)
                {
                    // If a video file is already loaded, 
                    // unload and reset everything
                    plm_destroy(plm);
                    frame = NULL;
                    sample = NULL;
                    framerate = 0.0;
                    samplerate = 0;
                    
                    UnloadImage(imFrame);
                    UnloadTexture(texture);
                    CloseAudioStream(stream);
                    baseTime = 0.0;
                    timeExcess = 0.0;
                    pause = false;
                }
                
                totalVideoFrames = plm_get_total_video_frames(droppedFiles[0]);
                printf("Video total frames: %i\n", totalVideoFrames);
                
                totalAudioFrames = plm_get_total_audio_frames(droppedFiles[0]);
                printf("Audio total samples: %i\n", totalAudioFrames);
                
                plm = plm_create_with_filename(droppedFiles[0]);

                if (plm != NULL)        // MPEG file loaded successfully!
                {
                    plm_set_loop(plm, false);   // No loop by default

                    framerate = plm_get_framerate(plm);
                    samplerate = plm_get_samplerate(plm);

                    TraceLog(LOG_INFO, "[%s] Loaded succesfully. Framerate: %f - Samplerate: %i", droppedFiles[0], (float)framerate, samplerate);
                    
                    // Init video settings
                    int videoWidth = plm_get_width(plm);
                    int videoHeight = plm_get_height(plm);
                    
                    SetWindowSize(videoWidth, videoHeight);
                    
                    timeBar = (Rectangle){ 0, GetScreenHeight() - 10, GetScreenWidth(), 10 };

                    imFrame.width = videoWidth;
                    imFrame.height = videoHeight;
                    imFrame.format = UNCOMPRESSED_R8G8B8;
                    imFrame.mipmaps = 1;
                    imFrame.data = (unsigned char *)malloc(imFrame.width*imFrame.height*3);
                    
                    texture = LoadTextureFromImage(imFrame);
                    
                    // Init audio settings (if track available)
                    if (plm_get_num_audio_streams(plm) > 0)
                    {
                        plm_set_audio_enabled(plm, true, 0);

                        // Init audio stream (sample rate: 44100, sample size: 32bit, channels: 2-stereo)
                        // WARNING: InitAudioDevice() inits internal double buffering system to (internal default size)*2,
                        // but every audio sample is PLM_AUDIO_SAMPLES_PER_FRAME, always 1152 samples...
                        // Two solutions:
                        // 1. Just change raudio internal AUDIO_BUFFER_SIZE to match PLM_AUDIO_SAMPLES_PER_FRAME (1152)
                        // 2. Keep internal raudio AUDIO_BUFFER_SIZE (4096) and fill it with multiple plm audio samples,
                        //    main issue is that (4096/1152 = 3.555) no round numbers, so, some samples should be divided
                        //    into the double buffering system... not trivial to do...
                        SetAudioStreamBufferSizeDefault(1152);  // Solution 1.
                        stream = InitAudioStream(samplerate, 32, 2);

                        PlayAudioStream(stream);        // Start processing stream buffer (no data loaded currently)

                        // Adjust the audio lead time according to the audio_spec buffer size
                        //plm_set_audio_lead_time(plm, (double)PLM_AUDIO_SAMPLES_PER_FRAME/(double)samplerate);
                    }
                }
            }
            
            ClearDroppedFiles();
        }
        
        if (IsKeyPressed(KEY_SPACE)) pause = !pause;
        
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(GetMousePosition(), timeBar))
        {
            int barPositionX = GetMouseX();
            
            // Get equivalent audio frame
            currentAudioFrame = (barPositionX*totalAudioFrames)/GetScreenWidth();
            
            // Get equivalent video frame (use audio frame to sync)
            currentVideoFrame = (int)((float)currentAudioFrame*(float)totalVideoFrames/(float)totalAudioFrames);
            
            // Reset video/audio and move to required frame/sample
            if (plm != NULL)
            {
                plm_rewind(plm);
                frame = NULL;
                sample = NULL;
                baseTime = 0.0;
                timeExcess = 0.0;
            }
            
            for (int i = 0; i < currentAudioFrame; i++) sample = plm_decode_audio(plm); //plm_skip_audio_frame(plm)
            for (int i = 0; i < currentVideoFrame; i++) frame = plm_skip_video_frame(plm);
        }
        
        if (IsKeyPressed(KEY_R))
        {
            if (plm != NULL) plm_destroy(plm);
            
            plm = NULL;
            frame = NULL;
            sample = NULL;
            baseTime = 0.0;
            timeExcess = 0.0;
        }

        if ((plm != NULL) && !pause)
        {
            // Video should run at 'framerate' fps => One new frame every 1/framerate
            double time = (GetTime() - baseTime);
            
            if (time >= (1.0/framerate))
            {
                timeExcess += (time - 0.040);
                baseTime = GetTime();
                
                // Decode video frame
                frame = plm_decode_video(plm);  // Get frame as 3 planes: Y, Cr, Cb
                currentVideoFrame++;
                
                if (timeExcess >= 0.040)
                {
                    // Discard previous frame and load new one
                    frame = plm_decode_video(plm);
                    currentVideoFrame++;
                    timeExcess = 0;
                }
                
                if (frame != NULL)      // We got a frame!
                {
                    plm_frame_to_rgb(frame, imFrame.data);  // Convert (Y, Cr, Cb) to RGB on the CPU (slow)
                    UpdateTexture(texture, imFrame.data);   // Update texture with new data for drawing
                }
            }

            // Refill audio stream if required
            while (IsAudioStreamProcessed(stream))
            {
                // Decode audio sample
                sample = plm_decode_audio(plm);
                currentAudioFrame++;
                
                if (sample != NULL)     // We got a sample!
                {
                    // Copy sample to audio stream
                    UpdateAudioStream(stream, sample->interleaved, PLM_AUDIO_SAMPLES_PER_FRAME*2);
                }
            }
            
            if (currentVideoFrame >= totalVideoFrames) 
            {
                //plm_rewind(plm);  // Rewind to the beginning (loop)
                if (plm != NULL) plm_destroy(plm);
                plm = NULL;
                frame = NULL;
                sample = NULL;
                baseTime = 0.0;
                timeExcess = 0.0;
            }
        }
        //-----------------------------------------------------

        // Draw
        //-----------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);

            if (plm != NULL) 
            {                
                DrawTexture(texture, GetScreenWidth()/2 - texture.width/2, GetScreenHeight()/2 - texture.height/2, WHITE);
                
                DrawText(TextFormat("CURRENT VIDEO FRAME: %i", currentVideoFrame), 10, 10, 10, LIGHTGRAY);
                DrawText(TextFormat("CURRENT AUDIO FRAME: %i", currentAudioFrame), 10, 30, 10, LIGHTGRAY);
                
                DrawRectangleRec(timeBar, GRAY);
                DrawRectangle(0, GetScreenHeight() - 10, (GetScreenWidth()*currentVideoFrame)/totalVideoFrames, 10, BLUE);
                if (CheckCollisionPointRec(GetMousePosition(), timeBar)) DrawRectangleLinesEx(timeBar, 1, DARKBLUE);
                
                //if (GuiButton((Rectangle){ 10, 10, 40, 40 }, pause? "#132#" : "#131#")) pause = !pause;   // Requires raygui.h
                
                if (pause)
                {
                    DrawRectangle(GetScreenWidth()/2 - 40, GetScreenHeight()/2 - 40, 20, 80, RAYWHITE);
                    DrawRectangle(GetScreenWidth()/2 + 10, GetScreenHeight()/2 - 40, 20, 80, RAYWHITE);
                }
            }
            else 
            {
                DrawText("MPEG Video Player", 320, 180, 30, LIGHTGRAY);
                DrawText("Drag and drop your MPEG file", 310, 240, 20, LIGHTGRAY);
            }

        EndDrawing();
        //-----------------------------------------------------
    }

    // De-Initialization
    //---------------------------------------------------------
    UnloadImage(imFrame);
    UnloadTexture(texture);
    
    CloseAudioStream(stream);
    CloseAudioDevice();

    if (plm != NULL) plm_destroy(plm);
    
    CloseWindow();        // Close window and OpenGL context
    //----------------------------------------------------------

    return 0;
}
