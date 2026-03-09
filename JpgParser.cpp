#include "JpgParser.h"
#include "Log.h"
unsigned char *raw_image = NULL;

static void my_init_source(j_decompress_ptr cinfo) {}

static boolean my_fill_input_buffer(j_decompress_ptr cinfo) {
	my_source_mgr* src = (my_source_mgr*)cinfo->src;
	// No more data.  Probably an incomplete image;  just output EOI.
	src->pub.next_input_byte = EOI_BUFFER;
	src->pub.bytes_in_buffer = 1;
	return TRUE;
}

static void my_skip_input_data(j_decompress_ptr cinfo, long num_bytes) {
	my_source_mgr* src = (my_source_mgr*)cinfo->src;
	if (src->pub.bytes_in_buffer < num_bytes) {
		// Skipping over all of remaining data;  output EOI.
		src->pub.next_input_byte = EOI_BUFFER;
		src->pub.bytes_in_buffer = 1;
	}
	else {
		// Skipping over only some of the remaining data.
		src->pub.next_input_byte += num_bytes;
		src->pub.bytes_in_buffer -= num_bytes;
	}
}

static void my_term_source(j_decompress_ptr cinfo) {}

static void my_set_source_mgr(j_decompress_ptr cinfo, const char* data, size_t len) {
	my_source_mgr* src;
	if (cinfo->src == 0) { // if this is first time;  allocate memory
		cinfo->src = (struct jpeg_source_mgr *)(*cinfo->mem->alloc_small)
			((j_common_ptr)cinfo, JPOOL_PERMANENT, sizeof(my_source_mgr));
	}
	src = (my_source_mgr*)cinfo->src;
	src->pub.init_source = my_init_source;
	src->pub.fill_input_buffer = my_fill_input_buffer;
	src->pub.skip_input_data = my_skip_input_data;
	src->pub.resync_to_restart = jpeg_resync_to_restart; // default
	src->pub.term_source = my_term_source;
	// fill the buffers
	src->data = (const JOCTET *)data;
	src->len = len;
	src->pub.bytes_in_buffer = len;
	src->pub.next_input_byte = src->data;

	Log(L"Jpeg data stored in: %p \n", cinfo);
}

//Replace ijl15's DecodeFromJPEGBuffer
BOOL DecodeFromLibJpeg(
	char* lpJpgBuffer,
	unsigned int dwJpgBufferSize,
	unsigned char** lppRgbBuffer, //supposed to be char array but it works anyway.
	unsigned int* lpdwWidth,
	unsigned int* lpdwHeight,
	unsigned int* lpdwNumberOfChannels)
{
	struct jpeg_decompress_struct cinfo;
	struct jpeg_error_mgr jerr;

	JSAMPROW row_pointer[1];
	unsigned long location = 0;
	int i = 0;
	// Setup decompression structure
	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_decompress(&cinfo);
	my_set_source_mgr(&cinfo, lpJpgBuffer, dwJpgBufferSize);

	// read info from header.
	int r = jpeg_read_header(&cinfo, TRUE);

	if (r != 1)
	{
		Log(L"It's not a JPEG file\n", r);
		return false;
	}

	Log(L"JPEG Header = %d\n", r);
	jpeg_start_decompress(&cinfo);

	// ..<snip>..

	//width = cinfo.output_width;
	//height = cinfo.output_height;

	Log(L"components = %d\n", cinfo.num_components);

	raw_image = (unsigned char*) malloc(cinfo.output_width*cinfo.output_height*cinfo.num_components);

	row_pointer[0] = (unsigned char *)malloc(cinfo.output_width*cinfo.num_components);

	while (cinfo.output_scanline < cinfo.image_height)
	{
		jpeg_read_scanlines(&cinfo, row_pointer, 1);
		for (i = 0; i<cinfo.image_width*cinfo.num_components; i++)
			raw_image[location++] = row_pointer[0][i];
	}
	*lpdwWidth = cinfo.output_width;
	*lpdwHeight = cinfo.output_height;
	*lpdwNumberOfChannels = cinfo.num_components;
	*lppRgbBuffer = raw_image; //ijl15
	Log(L"Success!\n");
	free(row_pointer[0]);
	// .. decompress the JPEG here as usual
	// ..<snip>..
	jpeg_destroy_decompress(&cinfo);
	free(&cinfo);
	return true;
}