#pragma once
#include "base_includes.h"
#include "c_scalarlength.h"







namespace baselib {
    class c_array {
    public:
        // Constructor
        inline c_array() {}

        // Copy constructor
        inline c_array(const c_array& obj) {
            init((int8_t*)obj.buffer.data(), obj.buffer_length);
        }

        // Parameterized constructor
        inline c_array(int8_t* in_buffer, std::uint64_t length) {
            init(in_buffer, length);
        }

        // Initialize the buffer with the provided data and length
        void init(int8_t* in_buffer, std::uint64_t length) {
            buffer_length = length;
            buffer.insert(buffer.end(), in_buffer, in_buffer + length);
        }

    public:
        // Write an object to the buffer
        template <class obj_type>
        void write(obj_type obj) {
            auto obj_size = sizeof(obj_type);
            int8_t* bytes = new int8_t[obj_size];
            memcpy(bytes, &obj, obj_size);
            buffer.insert(buffer.end(), bytes, bytes + obj_size);
            delete[] bytes;
            bytes = nullptr;
            write_offset += obj_size;
            buffer_length += obj_size;
        }

        // Read an object from the buffer
        template <class obj_type>
        bool read(obj_type& obj) {
            auto obj_size = sizeof(obj_type);
            if (obj_size + read_offset > buffer_length)
                return false;
            auto raw_buffer = buffer.data();
            obj = *(obj_type*)(raw_buffer + read_offset);
            read_offset += obj_size;
            return true;
        }

        // Read a string from the buffer
        bool read_string(std::string& obj) {
            bool result = false;
            uint16_t string_size = 0;
            if (read<uint16_t>(string_size)) {
                bool failed = false;
                for (int i = 0; i < string_size; i++) {
                    char c = 0;
                    if (!read<char>(c)) {
                        failed = true;
                        break;
                    }
                    obj += c;
                }
                result = !failed;
            }
            return result;
        }

        // Write a string to the buffer
        void write_string(std::string obj) {
            auto size = obj.length();
            write<uint16_t>(size);
            write((uint8_t*)obj.c_str(), size);
        }

        // Read bytes from the buffer
        bool read(uint8_t* out_buffer, size_t size) {
            size_t read_size = size;
            if (read_size == -1)
                read_size = buffer_length - read_offset;
            if (read_size > buffer_length || !read_size)
                return false;
            auto raw_buffer = buffer.data();
            memcpy(out_buffer, (raw_buffer + read_offset), read_size);
            read_offset += read_size;
            return true;
        }

        // Write bytes to the buffer
        void write(uint8_t* write_buffer, size_t size) {
            buffer.insert(buffer.end(), write_buffer, write_buffer + size);
            write_offset += size;
            buffer_length += size;
        }

        // Read a scalar length from the buffer
        bool read_scalar(c_scalarlength& obj) {
            bool result = false;
            if (read<uint8_t>(obj.scalar_index)) {
                switch (obj.scalar_index) {
                case 1: {
                    uint8_t byte_length_scalar = 0;
                    result = read<uint8_t>(byte_length_scalar);
                    obj.scalar_length = byte_length_scalar;
                    break;
                }
                case 2: {
                    uint16_t byte_length_scalar = 0;
                    result = read<uint16_t>(byte_length_scalar);
                    obj.scalar_length = byte_length_scalar;
                    break;
                }
                case 4: {
                    uint32_t byte_length_scalar = 0;
                    result = read<uint32_t>(byte_length_scalar);
                    obj.scalar_length = byte_length_scalar;
                    break;
                }
                case 8: {
                    uint64_t byte_length_scalar = 0;
                    result = read<uint64_t>(byte_length_scalar);
                    obj.scalar_length = byte_length_scalar;
                    break;
                }
                }
            }
            return result;
        }

        // Write a scalar length to the buffer
        void write_scalar(c_scalarlength obj) {
            write_scalar(obj.scalar_index, obj.scalar_length);
        }

        void write_scalar(uint64_t length) {
            write_scalar(c_scalarlength(length));
        }

        void write_scalar(uint8_t index, uint64_t length) {
            write<uint8_t>(index);
            switch (index) {
            case 1: {
                write<uint8_t>(length);
                break;
            }
            case 2: {
                write<uint16_t>(length);
                break;
            }
            case 4: {
                write<uint32_t>(length);
                break;
            }
            case 8: {
                write<uint64_t>(length);
                break;
            }
            }
        }

        // Remove bytes from the beginning of the buffer
        void remove_bytes(uint64_t x) {
            buffer.erase(buffer.begin(), buffer.begin() + x);
            buffer_length -= x;
            write_offset = buffer_length;
            read_offset = 0;
        }

    public:
        // Getters for offsets and buffer length
        uint64_t get_read_offset() { return read_offset; }
        uint64_t get_write_offset() { return write_offset; }
        uint64_t get_buffer_length() { return buffer_length; }

        // Getters for buffer data
        uint8_t* get_buffer() { return buffer.data(); }
        BYTE* get_bytes() { return (BYTE*)buffer.data(); }

        // Get the number of bytes left to be read
        uint64_t bytes_left() { return get_buffer_length() - get_read_offset(); }

        // Reset the buffer and offsets
        void reset() {
            buffer.clear();
            read_offset = 0;
            write_offset = 0;
            buffer_length = 0;
        }

    private:
        std::vector<uint8_t> buffer;
        uint64_t read_offset = 0;
        uint64_t write_offset = 0;
        uint64_t buffer_length = 0;
    };
}