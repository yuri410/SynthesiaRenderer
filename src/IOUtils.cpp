
#include "IOUtils.h"
#include <libpng/png.h>

namespace SR
{
	static void png_data_writer(png_structp png_ptr, png_bytep data, png_size_t length)
	{
		FileOutStream* strm = (FileOutStream*)png_get_io_ptr(png_ptr);
		strm->Write((const char*)data, length);
	}
	static void png_flusher(png_structp png_ptr) { }

	struct PngSaveContextImpl
	{ 
		png_structp png_ptr;
		png_infop info_ptr;
	};

	PngSaveContext* BeginStreamPng(int32 w, int32 h, FileOutStream& strm)
	{
		PngSaveContextImpl* c = (PngSaveContextImpl*)malloc(sizeof(PngSaveContextImpl));

		c->png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
		assert(c->png_ptr);

		c->info_ptr = png_create_info_struct(c->png_ptr);
		assert(c->info_ptr);

		png_init_io(c->png_ptr, NULL);

		png_set_write_fn(c->png_ptr, &strm, png_data_writer, png_flusher);

		png_set_IHDR(c->png_ptr, c->info_ptr, w, h, 8, PNG_COLOR_TYPE_RGBA, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
		png_write_info(c->png_ptr, c->info_ptr);

		return (PngSaveContext*)c;
	}

	void StreamInPng(PngSaveContext* ctx, RenderTarget* rt, int32 startY, int32 height, bool removeAlpha)
	{
		PngSaveContextImpl* c = (PngSaveContextImpl*)ctx;

		Rectangle lockArea;
		lockArea.X = 0;
		lockArea.Y = startY;
		lockArea.Width = rt->getWidth();
		lockArea.Height = height;

		DataRectangle dr = rt->Lock(LOCK_ReadOnly, lockArea);
		uint32* row = new uint32[dr.getWidth()];
		for (int32 i = 0; i < dr.getHeight(); i++)
		{
			memcpy(row, (char*)dr.getDataPointer() + i*dr.getPitch(), dr.getWidth() * sizeof(uint32));

			// swap r & b
			for (int32 j = 0; j < dr.getWidth(); j++)
			{
				byte* b = (byte*)(row + j);
				byte* r = (byte*)(row + j) + 2;

				byte t = *r;
				*r = *b;
				*b = t;

				if (removeAlpha)
				{
					byte& a = *((byte*)(row + j) + 3);
					a = 0xff;
				}
			}

			png_write_row(c->png_ptr, (png_bytep)row);
		}
		rt->Unlock();

		delete[] row; row = nullptr;
	}

	void EndStreamPng(PngSaveContext* ctx)
	{
		PngSaveContextImpl* c = (PngSaveContextImpl*)ctx;

		png_write_end(c->png_ptr, NULL);

		png_free_data(c->png_ptr, c->info_ptr, PNG_FREE_ALL, -1);
		png_destroy_write_struct(&c->png_ptr, NULL);

		free(c);
	}


	void SavePng(RenderTarget* rt, FileOutStream& strm, bool removeAlpha)
	{
		png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
		assert(png_ptr);

		png_infop info_ptr = png_create_info_struct(png_ptr);
		assert(info_ptr);

		png_init_io(png_ptr, NULL);

		png_set_write_fn(png_ptr, &strm, png_data_writer, png_flusher);

		png_set_IHDR(png_ptr, info_ptr, rt->getWidth(), rt->getHeight(), 8, PNG_COLOR_TYPE_RGBA, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
		png_write_info(png_ptr, info_ptr);


		DataRectangle dr = rt->Lock(LOCK_ReadOnly);
		uint32* row = new uint32[dr.getWidth()];
		for (int32 i = 0; i < dr.getHeight(); i++)
		{
			memcpy(row, (char*)dr.getDataPointer() + i*dr.getPitch(), dr.getWidth() * sizeof(uint32));

			// swap r & b
			for (int32 j = 0; j < dr.getWidth(); j++)
			{
				byte* b = (byte*)(row + j);
				byte* r = (byte*)(row + j) + 2;

				byte t = *r;
				*r = *b;
				*b = t;

				if (removeAlpha)
				{
					byte& a = *((byte*)(row + j) + 3);
					a = 0xff;
				}
			}

			png_write_row(png_ptr, (png_bytep)row);
		}
		rt->Unlock();

		delete[] row; row = nullptr;

		png_write_end(png_ptr, NULL);

		png_free_data(png_ptr, info_ptr, PNG_FREE_ALL, -1);
		png_destroy_write_struct(&png_ptr, NULL);
	}


	static void png_data_reader(png_structp png_ptr, png_bytep data, png_size_t length)
	{
		Stream* strm = (Stream*)png_get_io_ptr(png_ptr);
		strm->Read((char*)data, length);
	}

	Texture* LoadPngTexture(RenderDevice* device, const ResourceLocation& rl)
	{
		Stream* strm = rl.GetReadStream();

		png_byte header[8];
		strm->Read((char*)header, 8);

		if (png_sig_cmp(header, 0, 8))
		{
			return nullptr;
		}

		png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
		assert(png_ptr);

		png_infop info_ptr = png_create_info_struct(png_ptr);
		assert(info_ptr);

		//png_infop end_info = png_create_info_struct(png_ptr);
		//assert(end_info);

		png_init_io(png_ptr, NULL);

		png_set_read_fn(png_ptr, strm, png_data_reader);

		png_set_sig_bytes(png_ptr, 8);
		png_read_info(png_ptr, info_ptr);

		int bit_depth, color_type;
		png_uint_32 width, height;

		png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type,
			NULL, NULL, NULL);

		//png_read_update_info(png_ptr, info_ptr);

		int rowbytes = png_get_rowbytes(png_ptr, info_ptr);

		ObjectFactory* factory = device->getObjectFactory();
		Texture* result = nullptr;
		if (color_type == PNG_COLOR_TYPE_RGBA || color_type == PNG_COLOR_TYPE_RGB)
		{
			png_bytep rowData = (png_bytep)malloc(rowbytes);

			if (color_type == PNG_COLOR_TYPE_RGBA)
				assert(rowbytes == width * 4);
			else if (color_type == PNG_COLOR_TYPE_RGB)
				assert(rowbytes == width * 3);

			int32 bytepp = color_type == PNG_COLOR_TYPE_RGBA ? 4 : 3;

			result = factory->CreateTexture(width, height, 1, TU_Static, FMT_A8R8G8B8);

			auto dr = result->Lock(0, LOCK_None);

			for (uint32 i = 0; i < height; i++)
			{
				png_read_row(png_ptr, rowData, nullptr);

				if (color_type == PNG_COLOR_TYPE_RGBA)
				{
					// swap r & b
					for (uint32 j = 0; j < width; j++)
					{
						byte* b = rowData + j * bytepp;
						byte* r = rowData + j * bytepp + 2;

						byte t = *r;
						*r = *b;
						*b = t;
					}

					memcpy((char*)dr.getDataPointer() + i*dr.getPitch(), rowData, dr.getWidth() * sizeof(uint32));
				}
				else
				{
					byte* rowDst = (byte*)dr.getDataPointer() + i*dr.getPitch();
					for (uint32 j = 0; j < width; j++)
					{
						// bgr => A8R8G8B8(bgra)
						*rowDst++ = *rowData++;
						*rowDst++ = *rowData++;
						*rowDst++ = *rowData++;
						*rowDst++ = 0xff;
					}
				}
			}
			result->Unlock(0);

			free(rowData);
		}

		png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
		delete strm;
		return result;
	}

}