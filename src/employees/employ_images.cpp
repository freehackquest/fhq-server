#include <employ_images.h>
#include <fallen.h>
#define PNG_DEBUG 3
#include <png.h>

// Helpful information:
// http://zarb.org/~gc/html/libpng.html
// http://www.libpng.org/pub/png/libpng.html
// https://stackoverflow.com/questions/22907625/how-to-resize-an-image-using-libpng

REGISTRY_WJSCPP_EMPLOY(EmployImages)

// ---------------------------------------------------------------------

EmployImages::EmployImages()
    : WsjcppEmployBase(EmployImages::name(), {}) {
    TAG = EmployImages::name();
}

// ---------------------------------------------------------------------

bool EmployImages::init() {
    WsjcppLog::info(TAG, "Start init images");
    return true;
}

// ---------------------------------------------------------------------

bool EmployImages::deinit() {
    // TODO
    return true;
}

// ---------------------------------------------------------------------

struct avrgClr {
    avrgClr(int x1, int x2, int y1, int y2, bool bAlpha) : x1(x1), x2(x2), y1(y1), y2(y2), bAlpha(bAlpha) {
        // nothing
    }
    int x1, x2, y1, y2;
    bool bAlpha;
    png_bytep *row_pointers;
    png_byte* ptrResult;
};

void avarage_color(avrgClr &a) {
    long nColorRed = 0;
    long nColorGreen = 0;
    long nColorBlue = 0;
    long nColorAlpha = 0;
    long nCount = 0;

    for (int y=a.y1; y< a.y2; y++) {
        png_byte* row = a.row_pointers[y];
        for (int x=a.x1; x<a.x2; x++) {
            // row_pointers
            png_byte* ptr = &(row[x*(a.bAlpha ? 4 : 3)]);
            if (a.bAlpha) {
                nColorRed += ptr[0];
                nColorGreen += ptr[1];
                nColorBlue += ptr[2];
                nColorAlpha += ptr[3];
            } else {
                nColorRed += ptr[0];
                nColorGreen += ptr[1];
                nColorBlue += ptr[2];
            }
            nCount++;
        }
    }
    if (nCount == 0) {
        WsjcppLog::info("avarage_color", "nCount == 0");
        a.ptrResult[0] = 0;
        a.ptrResult[1] = 0;
        a.ptrResult[2] = 0;
        a.ptrResult[3] = 255;
    } else {
        a.ptrResult[0] = nColorRed/nCount;
        a.ptrResult[1] = nColorGreen/nCount;
        a.ptrResult[2] = nColorBlue/nCount;
        if (a.bAlpha) {
            a.ptrResult[3] = nColorAlpha/nCount;
        } else {
            a.ptrResult[3] = 255;
        }
    }
}

bool EmployImages::doThumbnailImagePng(const std::string &sourceImageFile, const std::string &targetImageFile, int width_resize, int height_resize) {
    // TODO keep proportional (will be got from web)

    WsjcppLog::info(TAG, "doThumbnailImagePng");
    
    int x, y;
    int width, height;
    // int width_resize = 100, height_resize = 100;
    png_byte color_type;
    png_byte bit_depth;

    png_structp png_ptr;
    png_infop info_ptr;
    int number_of_passes;
    png_bytep * row_pointers;
    int PNG_BYTES_TO_CHECK = 8;
    
    // read file
    {
        char header[8];    // 8 is the maximum size that can be checked

        /* open file and test for it being a png */
        FILE *fp = fopen(sourceImageFile.c_str(), "rb");
        if (!fp) {
            WsjcppLog::err(TAG, "[read_png_file] File " + sourceImageFile + " could not be opened for reading");
            return false;
        }
        fread(header, 1, 8, fp);
        
        // todo check png file
        // png_const_bytep *png_header = (png_const_bytep *)header;
        /*bool is_png = !png_sig_cmp(header, (png_size_t)0, (png_size_t)PNG_BYTES_TO_CHECK);
        if (!is_png) {
            WsjcppLog::err(TAG, "[read_png_file] File " + sourceFilepath + " is not recognized as a PNG file");
            return false;
        }*/

        /* initialize stuff */
        png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

        if (!png_ptr) {
            WsjcppLog::err(TAG, "[read_png_file] png_create_read_struct failed");
            return false;
        }

        info_ptr = png_create_info_struct(png_ptr);
        if (!info_ptr) {
            WsjcppLog::err(TAG, "[read_png_file] png_create_info_struct failed");
            return false;
        }

        if (setjmp(png_jmpbuf(png_ptr))) {
            WsjcppLog::err(TAG, "[read_png_file] Error during init_io");
            return false;
        }

        png_init_io(png_ptr, fp);
        png_set_sig_bytes(png_ptr, 8);

        png_read_info(png_ptr, info_ptr);

        width = png_get_image_width(png_ptr, info_ptr);
        height = png_get_image_height(png_ptr, info_ptr);
        color_type = png_get_color_type(png_ptr, info_ptr);
        bit_depth = png_get_bit_depth(png_ptr, info_ptr);
        // std::cout << "bit_depth (sizeof = " << sizeof(bit_depth) << ") : [" << int(bit_depth) << "]" << std::endl;

        number_of_passes = png_set_interlace_handling(png_ptr);
        png_read_update_info(png_ptr, info_ptr);

        /* read file */
        if (setjmp(png_jmpbuf(png_ptr))) {
            WsjcppLog::err(TAG, "[read_png_file] Error during read_image");
            return false;
        }
        row_pointers = (png_bytep*) malloc(sizeof(png_bytep) * height);
        for (y=0; y<height; y++) {
            row_pointers[y] = (png_byte*) malloc(png_get_rowbytes(png_ptr,info_ptr));
        }
        png_read_image(png_ptr, row_pointers);
        fclose(fp);
    }
    
    png_bytep * row_pointers_resize;
    
    // scale image to height_scale x width_scale
    {
        int nType = png_get_color_type(png_ptr, info_ptr);
        int bAlpha = false;
        
        
        if (nType != PNG_COLOR_TYPE_RGB && nType != PNG_COLOR_TYPE_RGBA) {
            WsjcppLog::err(TAG, "[process_file] input file is not PNG_COLOR_TYPE_RGB or PNG_COLOR_TYPE_RGBA (lacks the alpha channel)");
            return false;
        }

        if (nType == PNG_COLOR_TYPE_RGBA) {
            bAlpha = true;
        }
        
        // new size 
        row_pointers_resize = (png_bytep*) malloc(sizeof(png_bytep) * height_resize);
        for (y=0; y<height_resize; y++) {
            row_pointers_resize[y] = (png_byte*) malloc(sizeof(png_bytep) * width_resize);
        }
        
        // std::cout << "height: " << height << std::endl;
        // std::cout << "height_resize: " << height_resize << std::endl;
        float height_scale = float(height)/float(height_resize);
        // std::cout << "height_scale: " << height_scale << std::endl;
        
        // std::cout << "width: " << width << std::endl;
        // std::cout << "width_resize: " << width_resize << std::endl;
        float width_scale = float(width)/float(width_resize);
        // std::cout << "width_scale: " << width_scale << std::endl;
        
        for (y=0; y<height_resize; y++) {
            float y1, y2;
            y1 = height_scale*y;
            y2 = y1 + height_scale;
            y2 = y2 > height ? height : y2;
            
            png_byte* row_resize = row_pointers_resize[y];
            for (x=0; x<width_resize; x++) {
                float x1,x2;
                x1 = width_scale*x;
                x2 = x1 + width_scale;
                x2 = x2 > width ? width : x2;
                
                // row_pointers
                avrgClr a(x1,x2,y1,y2,bAlpha);
                a.row_pointers = row_pointers;
                a.ptrResult = &(row_resize[x*4]);

                avarage_color(a);
            }
        }
    }
    
    // write file
    {
        /* create file */
        FILE *fp = fopen(targetImageFile.c_str(), "wb");
        if (!fp) {
            WsjcppLog::err(TAG, "[write_png_file] File " + targetImageFile + " could not be opened for writing");
            return false;
        }


        /* initialize stuff */
        png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

        if (!png_ptr) {
            WsjcppLog::err(TAG, "[write_png_file] png_create_write_struct failed");
            return false;
        }

        info_ptr = png_create_info_struct(png_ptr);
        if (!info_ptr) {
            WsjcppLog::err(TAG, "[write_png_file] png_create_info_struct failed");
            return false;
        }

        if (setjmp(png_jmpbuf(png_ptr))) {
            WsjcppLog::err(TAG, "[write_png_file] Error during init_io");
            return false;
        }

        png_init_io(png_ptr, fp);


        /* write header */
        if (setjmp(png_jmpbuf(png_ptr))) {
            WsjcppLog::err(TAG, "[write_png_file] Error during writing header");
            return false;
        }

        png_set_IHDR(png_ptr, info_ptr, width_resize, height_resize,
                     8, PNG_COLOR_TYPE_RGBA, PNG_INTERLACE_NONE,
                     PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

        png_write_info(png_ptr, info_ptr);


        /* write bytes */
        if (setjmp(png_jmpbuf(png_ptr))) {
            WsjcppLog::err(TAG, "[write_png_file] Error during writing bytes");
            return false;
        }

        png_write_image(png_ptr, row_pointers_resize);


        /* end write */
        if (setjmp(png_jmpbuf(png_ptr))) {
            WsjcppLog::err(TAG, "[write_png_file] Error during end of write");
            return false;
        }

        png_write_end(png_ptr, NULL);

        /* cleanup heap allocation */
        for (y=0; y<height; y++) {
            free(row_pointers[y]);
        }
        free(row_pointers);

        for (y=0; y<height_resize; y++) {
            free(row_pointers_resize[y]);
        }
        free(row_pointers_resize);
        fclose(fp);
    }
    return true;
}

// ---------------------------------------------------------------------
