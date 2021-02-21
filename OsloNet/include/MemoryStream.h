#pragma once
#include <stdlib.h>
#include <type_traits>
#include <algorithm>

#include <ByteSwap.h>

namespace Oslo
{
	namespace net
	{
		namespace memory
		{
			constexpr unsigned int DEFAULT_SIZE_STREAM = 1460;

			class MemoryStream
			{
			public:

				MemoryStream(uint32 i_size = DEFAULT_SIZE_STREAM) : m_buffer(nullptr), m_capacity(0), m_writeHead(0), m_readHead(0)
				{
					ReallocBuffer(i_size);
				}

				MemoryStream(char* i_buffer, uint32 i_byteCount, uint32 i_size = DEFAULT_SIZE_STREAM) : m_buffer(nullptr), m_capacity(0), m_writeHead(0), m_readHead(0)
				{
					ReallocBuffer(i_size);
					memcpy(m_buffer, i_buffer, i_byteCount);
					m_writeHead = i_byteCount;
				}

				~MemoryStream()
				{
					free(m_buffer);
				}

				char* GetBufferPtr() const {return m_buffer;}
				uint32 GetCapacity() const {return m_capacity;}
				uint32 GetSize() const {return m_writeHead;}

				void Reset() {m_readHead = 0;}
			
				void Write(const void* i_data, size_t i_byteCount)
				{
					const uint32 resultHead = m_writeHead + static_cast<uint32>(i_byteCount);
					if (resultHead > m_capacity)
					{
						ReallocBuffer(max(m_capacity * 2, resultHead));
					}

					std::memcpy(m_buffer + m_writeHead, i_data, i_byteCount);
					m_writeHead = resultHead;
				}
			
				void Read(void* i_data, size_t i_byteCount)
				{
					uint32 resultHead = m_readHead + static_cast<uint32>(i_byteCount);
					if (resultHead > m_writeHead)
					{
						printf("Exceeded memory while reading buffer!");
					}

					std::memcpy(i_data, m_buffer + m_readHead, i_byteCount);
					m_readHead = resultHead;
				}
		
				template<typename T>
				void Write(T i_data)
				{
					static_assert(std::is_arithmetic<T>::value || std::is_enum<T>::value, "Generic Write only supports primitive data types");
					// LittleEndian, since we're running on windows
					T swappedData = ByteSwap(i_data);
					Write(&swappedData, sizeof(swappedData));
				}

				template<typename T>
				void Read(T& i_data)
				{
					static_assert(std::is_arithmetic<T>::value || std::is_enum<T>::value, "Generic Read only supports primitive data types");

					T unswappedData;
					Read(&unswappedData, sizeof(unswappedData));
					i_data = ByteSwap(unswappedData);
				}

			private:

				void ReallocBuffer(uint32 i_length)
				{
					m_buffer = static_cast<char*>(std::realloc(m_buffer, i_length));
					m_capacity = i_length;
				}

			private:
				char* m_buffer;
				uint32 m_capacity;
				uint32 m_writeHead;
				mutable uint32 m_readHead;
			};
		}
	}
}