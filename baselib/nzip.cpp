#include "nzip.h"
#include <zlib.h>
baselib::c_array crypt::compress_packet(baselib::c_array& message)
{
    std::vector<unsigned char> compressedData;

    z_stream stream;
    stream.zalloc = Z_NULL;
    stream.zfree = Z_NULL;
    stream.opaque = Z_NULL;
    deflateInit(&stream, Z_BEST_COMPRESSION);

    // Compress the input data
    stream.avail_in = message.get_buffer_length();
    stream.next_in = const_cast<unsigned char*>(message.get_buffer());

    // Temporary buffer to store compressed data
    unsigned char tempBuffer[1024];
    do {
        stream.avail_out = sizeof(tempBuffer);
        stream.next_out = tempBuffer;
        deflate(&stream, Z_FINISH);
        compressedData.insert(compressedData.end(), tempBuffer, tempBuffer + sizeof(tempBuffer) - stream.avail_out);
    } while (stream.avail_out == 0);

    // Clean up zlib resources
    deflateEnd(&stream);

    baselib::c_array arr;
    arr.init((std::int8_t*)compressedData.data(), compressedData.size());
    return arr;
}

BYTE* crypt::compress_data(BYTE* message, size_t inSize, size_t& outSize)
{
    // Calculate the maximum possible compressed length
    uLong destination_length = compressBound(inSize);

    // Allocate memory for the compressed data
    byte* destination_data = new byte[destination_length];

    // Compress the message
    compress2(destination_data, (uLong*)&outSize, message, inSize, Z_DEFAULT_COMPRESSION);

    return destination_data;
}

baselib::c_array crypt::decompress_packet(baselib::c_array& message)
{
    std::vector<unsigned char> decompressedData;

    // Set up zlib stream for decompression
    z_stream stream;
    stream.zalloc = Z_NULL;
    stream.zfree = Z_NULL;
    stream.opaque = Z_NULL;
    inflateInit(&stream);

    // Decompress the input data
    stream.avail_in = message.get_buffer_length();
    stream.next_in = const_cast<unsigned char*>(message.get_buffer());

    // Temporary buffer to store decompressed data
    unsigned char tempBuffer[1024];
    do {
        stream.avail_out = sizeof(tempBuffer);
        stream.next_out = tempBuffer;
        inflate(&stream, Z_NO_FLUSH);
        decompressedData.insert(decompressedData.end(), tempBuffer, tempBuffer + sizeof(tempBuffer) - stream.avail_out);
    } while (stream.avail_out == 0);

    // Clean up zlib resources
    inflateEnd(&stream);

    baselib::c_array arr;
    arr.init((std::int8_t*)decompressedData.data(), decompressedData.size());
    return arr;
}