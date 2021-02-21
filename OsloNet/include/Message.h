#pragma once
#include <Types.h>
#include <MemoryStream.h>

namespace Oslo
{
	namespace net
	{
		struct Message
		{
			Message(size_t i_size) : body(i_size){};
			Message(char* i_buffer, uint32 i_byteCount) : body(i_buffer, i_byteCount){};
			Message() : body(){};

			template<typename T>
			void Write(T data)
			{
				body.Write<T>(data);
			}

			template<typename T>
			void Read(T& data)
			{
				body.Read<T>(data);
			}

			memory::MemoryStream body;
		};
	}
}