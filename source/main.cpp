// TODO: cmake, install portaudio
#include <stdio.h>
#include <stdlib.h>
#include "portaudio.h"
#include "lame.h"

#define SAMPLE_RATE (44100)
#define FRAMES_PER_BUFFER (1024)
#define NUM_CHANNELS (2)

int main(int argc, char **argv) {
    PaStream *stream;
    PaError error;
    lame_global_flags *lame;
    int ret;

    // 初始化PortAudio和LAME库
    Pa_Initialize();
    lame = lame_init();
    lame_set_in_samplerate(lame, SAMPLE_RATE);
    lame_set_VBR(lame, vbr_default);

    // 打开录音设备
    error = Pa_OpenDefaultStream(&stream,
                                 NUM_CHANNELS,
                                 0,
                                 paFloat32,
                                 SAMPLE_RATE,
                                 FRAMES_PER_BUFFER,
                                 NULL,
                                 NULL);
    if (error != paNoError) {
        fprintf(stderr, "Unable to open audio device: %s\n", Pa_GetErrorText(error));
        return 1;
    }

    // 开始录音
    error = Pa_StartStream(stream);
    if (error != paNoError) {
        fprintf(stderr, "Unable to start audio stream: %s\n", Pa_GetErrorText(error));
        return 1;
    }

    // 创建输出文件
    FILE *outFile = fopen("output.mp3", "wb");
    if (!outFile) {
        fprintf(stderr, "Unable to create output file\n");
        return 1;
    }

    // 采集数据并编码成mp3格式
    float buffer[FRAMES_PER_BUFFER * NUM_CHANNELS];
    unsigned char mp3_buffer[FRAMES_PER_BUFFER * NUM_CHANNELS];
    int write_count;
    int read_count;
    while ((read_count = Pa_ReadStream(stream, buffer, FRAMES_PER_BUFFER)) > 0) {
        write_count = lame_encode_buffer_interleaved(lame, buffer, read_count, mp3_buffer, sizeof(mp3_buffer));
        if (write_count < 0) {
            fprintf(stderr, "Unable to encode audio: %s\n", lame_encode_strerror(write_count));
            break;
        }
        ret = fwrite(mp3_buffer, write_count, 1, outFile);
        if (ret < 1) {
            fprintf(stderr, "Unable to write data to output file\n");
            break;
        }
    }

    // 关闭输出文件和流
    fclose(outFile);
    Pa_StopStream(stream);
    Pa_CloseStream(stream);

    // 清理PortAudio和LAME库资源
    Pa_Terminate();
    lame_close(lame);

    return 0;
}
