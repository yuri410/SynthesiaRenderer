#pragma once
#ifndef APOC3D_STREAM_H
#define APOC3D_STREAM_H

/**
 * -----------------------------------------------------------------------------
 * This source file is part of Apoc3D Engine
 * 
 * Copyright (c) 2009+ Tao Xin
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  if not, write to the Free Software Foundation, 
 * Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA, or go to
 * http://www.gnu.org/copyleft/gpl.txt.
 * 
 * -----------------------------------------------------------------------------
 */

#include "apoc3d/Collections/List.h"
#include "apoc3d/Collections/Queue.h"
#include <fstream>

using namespace Apoc3D::Collections;

namespace Apoc3D
{
	namespace IO
	{
		/** Defines represent reference points in streams for seeking. */
		enum struct SeekMode
		{
			Begin,
			Current,
			End
		};

		/** Provides a generic access of a sequence of bytes. */
		class APAPI Stream
		{
			RTTI_BASE;
		protected:
			Stream() { }
		public:
			virtual ~Stream() { }

			virtual bool IsReadEndianIndependent() const = 0;
			virtual bool IsWriteEndianIndependent() const = 0;

			virtual bool CanRead() const = 0;
			virtual bool CanWrite() const = 0;

			virtual int64 getLength() const = 0;

			virtual void setPosition(int64 offset) = 0;
			virtual int64 getPosition() = 0;

			bool isEndofStream() { return getPosition() >= getLength(); }

			virtual int64 Read(char* dest, int64 count) = 0;
			
			virtual void Write(const char* src, int64 count) = 0;

			void WriteByte(byte value) { Write(reinterpret_cast<const char*>(&value), 1); }


			virtual void Seek(int64 offset, SeekMode mode) = 0;

			virtual void Flush() = 0;

			int ReadByte()
			{
				char buffer;
				if (Read(&buffer, 1) == 0)
				{
					return -1;
				}
				return reinterpret_cast<const byte&>(buffer);
			}

		};

		/** Provides read-only access to a file as a stream */
		class APAPI FileStream : public Stream
		{
			RTTI_DERIVED(FileStream, Stream);
		public:
			static const int32 SequencialCountTrigger = 16;

			FileStream(const String& filename);
			virtual ~FileStream();

			virtual bool IsReadEndianIndependent() const override { return true; }
			virtual bool IsWriteEndianIndependent() const override { return true; }

			virtual bool CanRead() const override { return true; }
			virtual bool CanWrite() const override { return false; }

			virtual int64 getLength() const override { return m_length; }

			virtual void setPosition(int64 offset) override;
			virtual int64 getPosition() override;

			virtual int64 Read(char* dest, int64 count) override;
			virtual void Write(const char* src, int64 count) override;

			virtual void Seek(int64 offset, SeekMode mode) override;
			
			virtual void Flush() override { }

		private:
			void ClearReadBuffer();
			
			int64 ReadImpl(char* dest, int64 count);
			void SeekImpl(int64 offset, SeekMode mode);
			int64 GetPositionImpl();

			bool isBuffered() const { return m_sequentialCount >= SequencialCountTrigger; }
//#define USE_WIN32_FILE
#ifdef USE_WIN32_FILE// APOC3D_PLATFORM == APOC3D_PLATFORM_WINDOWS
			void* m_file;
#else
			std::ifstream m_in;
#endif

			int64 m_length;

			int32 m_sequentialCount = 0;
			int32 m_endofStream = false;
			FixedQueue<char, 4096> m_readBuffer;
		};

		/** Provides write-only access to a file as a stream */
		class APAPI FileOutStream : public Stream
		{
			RTTI_DERIVED(FileOutStream, Stream);
		public:
			FileOutStream(const String& filename, bool noTrunc = false);
			virtual ~FileOutStream();

			virtual bool IsReadEndianIndependent() const override { return true; }
			virtual bool IsWriteEndianIndependent() const override { return true; }

			virtual bool CanRead() const override { return false; }
			virtual bool CanWrite() const override { return true; }

			virtual int64 getLength() const override { return m_length; }

			virtual void setPosition(int64 offset) override;
			virtual int64 getPosition() override;

			virtual int64 Read(char* dest, int64 count) override;
			virtual void Write(const char* src, int64 count) override;

			virtual void Seek(int64 offset, SeekMode mode) override;
			
			virtual void Flush() override;

		private:
			std::ofstream m_out;
			int64 m_length = 0;
			char m_buffer[4096];
		};

		/** Provides access to a space in memory as a stream */
		class APAPI MemoryStream : public Stream
		{
			RTTI_DERIVED(MemoryStream, Stream);
		public:
			MemoryStream(char* data, int64 length)
				: m_data(data), m_length(length) { }

			virtual ~MemoryStream()
			{ }

			virtual bool IsReadEndianIndependent() const override { return false; }
			virtual bool IsWriteEndianIndependent() const override { return false; }

			virtual bool CanRead() const override { return true; }
			virtual bool CanWrite() const override { return true; }

			virtual int64 getLength() const override { return m_length; }
			virtual void setPosition(int64 offset) override { m_position = offset; }
			virtual int64 getPosition() override { return m_position; }

			virtual int64 Read(char* dest, int64 count) override;
			virtual void Write(const char* src, int64 count) override;
			
			virtual void Seek(int64 offset, SeekMode mode) override;
			
			virtual void Flush() override { }

			char* getDataPointer() const { return m_data; }

		private:
			NO_INLINE static void throwEndofStreamException();

			int64 m_length = 0;
			char* m_data = nullptr;
			int64 m_position = 0;
		};

		/** 
		 *  Provides access to a portion of another stream as a new stream
		 *  Close operation done on this stream will not affect the parent stream.
		 */
		class APAPI VirtualStream : public Stream
		{
			RTTI_DERIVED(VirtualStream, Stream);
		public:
			VirtualStream(Stream* strm);
			VirtualStream(Stream* strm, int64 baseOffset);
			VirtualStream(Stream* strm, int64 baseOffset, int64 length);
			VirtualStream(Stream* strm, int64 baseOffset, int64 length, bool releaseStream);
			~VirtualStream();

			VirtualStream(VirtualStream&& other);
			VirtualStream& operator=(VirtualStream&& other);

			VirtualStream(const VirtualStream&) = delete;
			VirtualStream& operator=(const VirtualStream&) = delete;

			virtual bool IsReadEndianIndependent() const override { return m_baseStream->IsReadEndianIndependent(); }
			virtual bool IsWriteEndianIndependent() const override { return m_baseStream->IsWriteEndianIndependent(); }

			virtual bool CanRead() const override { return m_baseStream->CanRead(); }
			virtual bool CanWrite() const override { return m_baseStream->CanWrite(); }

			virtual int64 getLength() const override;

			virtual void setPosition(int64 offset) override;
			virtual int64 getPosition() override;

			virtual int64 Read(char* dest, int64 count) override;
			virtual void Write(const char* src, int64 count) override;
			virtual void Seek(int64 offset, SeekMode mode) override;

			virtual void Flush() override { m_baseStream->Flush(); }

			Stream* getBaseStream() const { return m_baseStream; }
			bool isOutput() const { return m_isOutput; }

			int64 getBaseOffset() const { return m_baseOffset; }
			int64 getAbsolutePosition() const { return m_baseStream->getPosition(); }

		private:
			Stream* m_baseStream;
			int64 m_length;
			int64 m_baseOffset = 0;

			bool m_isOutput = false;
			bool m_releaseStream = false;
		};

		/** Provides access to a dynamic length of space in memory as a stream */
		class APAPI MemoryOutStream : public Stream
		{
			RTTI_DERIVED(MemoryOutStream, Stream);
		public:
			MemoryOutStream(int64 preserved)
				: m_data((int32)preserved) { }

			MemoryOutStream(MemoryOutStream&&);
			MemoryOutStream& operator=(MemoryOutStream&&);

			virtual ~MemoryOutStream()
			{ }

			virtual bool IsReadEndianIndependent() const override { return false; }
			virtual bool IsWriteEndianIndependent() const override { return false; }

			virtual bool CanRead() const override { return true; }
			virtual bool CanWrite() const override { return true; }

			virtual int64 getLength() const override { return m_length; }

			virtual void setPosition(int64 offset) override { m_position = offset; }
			virtual int64 getPosition() override { return m_position; }

			virtual int64 Read(char* dest, int64 count) override;
			virtual void Write(const char* src, int64 count) override;

			virtual void Seek(int64 offset, SeekMode mode) override;
			
			virtual void Flush() override { }

			const char* getDataPointer() const { return m_data.getElements(); }
			char* getDataPointer() { return m_data.getElements(); }

			char* AllocateArrayCopy() const { return m_data.AllocateArrayCopy(); }

		private:
			int64 m_length = 0;
			int64 m_position = 0;
			List<char> m_data;
		};

		class APAPI PipeOutStream : public MemoryOutStream
		{
			RTTI_DERIVED(PipeOutStream, MemoryOutStream);
		public:
			PipeOutStream(int64 preserved)
				: MemoryOutStream(preserved)
			{ }

			virtual ~PipeOutStream();

			void PopAll(char* dest);
		};


		/**
		 *  This reader can read Stream with an internal buffer to 
		 *  avoid frequent calls to Stream's read methods (which is expensive for various reasons)
		 */
		class APAPI BufferedStreamReader
		{
		public:
			static const int32 BufferSize = 4096;

			BufferedStreamReader(Stream* strm)
				: m_baseStream(strm)
			{
				assert(strm->CanRead()); 
			}
			~BufferedStreamReader() 
			{
			}

			bool IsReadEndianIndependent() const { return m_baseStream->IsReadEndianIndependent(); }
			int32 getLength() const { return static_cast<int32>(m_baseStream->getLength()); }

			int32 Read(char* dest, int32 count);
			bool ReadByte(char& result);
		
		private:
			void ClearBuffer();
			void ReadBuffer(char* dest, int32 count);
			void FillBuffer();

			int getBufferContentSize() const { return m_size; }


			Stream* m_baseStream;
			bool m_endofStream = false;

			char m_buffer[BufferSize];

			int32 m_head = 0;
			int32 m_tail = 0;
			int32 m_size = 0;
		};
	};
}

#endif