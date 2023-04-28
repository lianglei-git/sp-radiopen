#include "Mp3Encoder.h"
#include <iostream>
int Mp3Encoder::Init(const char *pcmFilePath, const char *mp3FilePath, int sampleRate,
                     int channels, int bitRate)
{
    int ref = -1;
    pcmFile = fopen(pcmFilePath, "rb");
    std::cout<<pcmFile<<std::endl;
    if (!pcmFile)
    {
        return -1;
    }
    mp3File = fopen(mp3FilePath, "wb");
    if (!mp3File)
    {
        return -1;
    }

    // 初始化lame
    lameClient = lame_init();
    lame_set_in_samplerate(lameClient, sampleRate);
    lame_set_out_samplerate(lameClient, sampleRate);
    lame_set_num_channels(lameClient, channels);
    lame_set_brate(lameClient, bitRate / 1000);
    lame_init_params(lameClient);

    return 0;
}

void Mp3Encoder::Encode()
{
     int bufferSize = 1024 * 256;
    short *buffer = new short[bufferSize / 2];
    short *leftChannelBuffer = new short[bufferSize / 4];//左声道
    short *rightChannelBuffer = new short[bufferSize / 4];//右声道
    unsigned char *mp3_buffer = new unsigned char[bufferSize];
    size_t readBufferSize = 0;
    while ((readBufferSize = fread(buffer, 2, bufferSize / 2, pcmFile)) > 0) {
        for (int i = 0; i < readBufferSize; i++) {
            if (i % 2 == 0) {
                leftChannelBuffer[i / 2] = buffer[i];
            } else {
                rightChannelBuffer[i / 2] = buffer[i];
            }
        }
        size_t writeSize = lame_encode_buffer(
                lameClient,
                (short int *) leftChannelBuffer,
                (short int *) rightChannelBuffer,
                (int) (readBufferSize / 2),
                mp3_buffer,
                bufferSize);
        fwrite(mp3_buffer, 1, writeSize, mp3File);
    }

    delete[] buffer;
    delete[] leftChannelBuffer;
    delete[] rightChannelBuffer;
    delete[] mp3_buffer;
}

void Mp3Encoder::Destroy(){
    if(pcmFile){
        fclose(pcmFile);
    }
}