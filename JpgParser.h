#pragma once
#include <Windows.h>
#include <iostream>
#include <fstream>
#include <jpeglib.h>
#include <vector>

const static JOCTET EOI_BUFFER[1] = { JPEG_EOI };
struct my_source_mgr {
	struct jpeg_source_mgr pub;
	const JOCTET *data;
	size_t       len;
};

static void my_init_source(j_decompress_ptr cinfo);

static boolean my_fill_input_buffer(j_decompress_ptr cinfo);

static void my_skip_input_data(j_decompress_ptr cinfo, long num_bytes);

static void my_term_source(j_decompress_ptr cinfo);

static void my_set_source_mgr(j_decompress_ptr cinfo, const char* data, size_t len);

BOOL DecodeFromLibJpeg(
	char* lpJpgBuffer,
	unsigned int dwJpgBufferSize,
	unsigned char** lppRgbBuffer,
	unsigned int* lpdwWidth,
	unsigned int* lpdwHeight,
	unsigned int* lpdwNumberOfChannels);