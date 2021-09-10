//
// Copyright (c) 2011 Christopher Baker <https://christopherbaker.net>
// Copyright (c) 2011 Jacques Fortier <https://github.com/jacquesf/COBS-Consistent-Overhead-Byte-Stuffing>
//
// SPDX-License-Identifier: MIT
//


#pragma once


#include "Arduino.h"

#include <vector>
#include <cstring>


/// \brief A Consistent Overhead Byte Stuffing (COBS) Encoder.
///
/// Consistent Overhead Byte Stuffing (COBS) is an encoding that removes all 0
/// bytes from arbitrary binary data. The encoded data consists only of bytes
/// with values from 0x01 to 0xFF. This is useful for preparing data for
/// transmission over a serial link (RS-232 or RS-485 for example), as the 0
/// byte can be used to unambiguously indicate packet boundaries. COBS also has
/// the advantage of adding very little overhead (at least 1 byte, plus up to an
/// additional byte per 254 bytes of data). For messages smaller than 254 bytes,
/// the overhead is constant.
///
/// \sa http://conferences.sigcomm.org/sigcomm/1997/papers/p062.pdf
/// \sa http://en.wikipedia.org/wiki/Consistent_Overhead_Byte_Stuffing
/// \sa https://github.com/jacquesf/COBS-Consistent-Overhead-Byte-Stuffing
/// \sa http://www.jacquesf.com/2011/03/consistent-overhead-byte-stuffing

//Modifed to use vectors instead of raw buffers

class COBS
{
public:
    static constexpr uint8_t marker = 0x00;

    static size_t encode(const std::vector<uint8_t> &buffer, std::vector<uint8_t> &encodedBuffer)
    {
        //NOTE THIS METHOD DOESNT WRITE END PACKET MARKER
        size_t size = buffer.size();
        size_t encode_offset = encodedBuffer.size();
        encodedBuffer.resize(encode_offset+getEncodedBufferSize(size));

        size_t read_index  = 0;
        size_t write_index = 1; // initialized at 1 so we can add the first code
        size_t code_index  = 0; // index of the last code / last apperance of zero?
        uint8_t code       = 1; // next code / distance to next zero

        while (read_index < size)
        {
            if (buffer.at(read_index) == 0)
            {
                encodedBuffer.at(encode_offset+code_index) = code;
                code = 1;
                code_index = write_index++;
                read_index++;
            }
            else
            {
                encodedBuffer.at(encode_offset+write_index++) = buffer.at(read_index++);
                code++;

                if (code == 0xFF) // end of block (each block is 256 in len)
                {
                    encodedBuffer.at(encode_offset+code_index) = code;
                    code = 1;
                    code_index = write_index++;
                }
            }
        }

        encodedBuffer.at(encode_offset+code_index) = code;
        
        return write_index; 
    }

    static size_t decode(const std::vector<uint8_t> &encodedBuffer,std::vector<uint8_t> &decodedBuffer)
    {   
        
        size_t size = encodedBuffer.size();
        decodedBuffer.resize(size);


        if (size == 0)
            return 0;

        size_t read_index  = 0;
        size_t write_index = 0;
        uint8_t code       = 0;
        uint8_t i          = 0;

        while (read_index < size)
        {
            code = encodedBuffer.at(read_index);

            if (read_index + code > size && code != 1)
            {
                return 0;
            }

            read_index++;

            for (i = 1; i < code; i++)
            {
                decodedBuffer.at(write_index++) = encodedBuffer.at(read_index++);
            }

            if (code != 0xFF && read_index != size)
            {
                decodedBuffer.at(write_index++) = 0x00;
            }
        }

        return write_index;
    }

    /// \brief Get the maximum encoded buffer size for an unencoded buffer size.
    /// \param unencodedBufferSize The size of the buffer to be encoded.
    /// \returns the maximum size of the required encoded buffer.
    static size_t getEncodedBufferSize(size_t unencodedBufferSize)
    {
        return unencodedBufferSize + unencodedBufferSize / 254 + 1;
    }

};