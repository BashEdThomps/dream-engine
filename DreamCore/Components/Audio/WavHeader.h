#pragma once

#include <cstdint>

namespace octronic::dream
{
    typedef struct
    {
        // RIFF Chunk Descriptor
        uint8_t  RIFF[4];// RIFF Header Magic header
        uint32_t ChunkSize; // RIFF Chunk Size
        uint8_t  WAVE[4]; // WAVE Header
        // "fmt" sub-chunk
        uint8_t  fmt[4]; // FMT header
        // Size of the fmt chunk
        uint32_t Subchunk1Size;
        // Audio format
        // 1   = PCM
        // 6   = mulaw
        // 7   = alaw
        // 257 = IBM Mu-Law
        // 258 = IBM A-Law
        // 259 = ADPCM
        uint16_t AudioFormat;
        // Number of channels
        // 1 = Mono
        // 2 = Stereo
        uint16_t NumOfChannels;
        // Sampling Frequency in Hz
        uint32_t SamplesPerSecond;
        // Bytes Per Second
        uint32_t BytesPerSecond;
        // Block Alignment
        // 2 = 16-bit mono
        // 4 = 16-bit stereo
        uint16_t BlockAlign;
        // Number of bits per sample
        uint16_t BitsPerSample;
        // "data" sub-chunk
        // "data"  string
        uint8_t  Subchunk2ID[4];
        // Sampled data length
        uint32_t Subchunk2Size;

    } WavHeader ;

}
