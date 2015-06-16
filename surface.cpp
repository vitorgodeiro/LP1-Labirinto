#include "surface.h"
#include <stdio.h>
#include <string.h>

#define DEBUG
#include "debug.h"


Surface::Surface(const std::string &file) : pixels_(NULL)
{
    FILE* f;
    unsigned char data[18] = {0};
    int w, h, bpp, size;
    unsigned char* tmp;

    TRACE(("TGA: try open '%s'\n", file.c_str()));

    f = fopen( file.c_str(), "rb" );
    //TRACE(("TGA: %s\n", strerror(errno)));

    if (!f)
        return;


    if ( fread(data, 1, sizeof(data), f) != sizeof(data) ||
         memcmp(data, "\x00\x00\x02\x00\x00\x00\x00\x00\x00\x00\x00\x00", 12)
                  != 0)
    {
        fclose(f);
        return;
    }


    w   = data[13] * 256 + data[12];
    h   = data[15] * 256 + data[14];
    bpp = data[16];

    if ( w <= 0 || h <=0 || (bpp != 24 && bpp != 32) )
    {
        fclose(f);
        return;
    }

    size = w * h * (bpp/8);
    tmp = new unsigned char[ size ];

    if ( (int)fread(tmp, 1, size, f) != size )
    {
        fclose(f);
        delete[] tmp;
        return;
    }

    fclose(f);

    pixels_ = new unsigned char[ h * w * 4 ];

    if (bpp == 24)
    { // converte BGR para RGBA
        int i, j=0;

        for (i=0; i<size; i+=3)
        {
            pixels_[j+0] = tmp[i+2];
            pixels_[j+1] = tmp[i+1];
            pixels_[j+2] = tmp[i+0];
            pixels_[j+3] = 255;
            j+=4;
        }
    }
    else
    { // converte BGRA para RGBA
        int i, j=0;

        for (i=0; i<size; i+=4)
        {
            pixels_[j+0] = tmp[i+2];
            pixels_[j+1] = tmp[i+1];
            pixels_[j+2] = tmp[i+0];
            pixels_[j+3] = tmp[i+3];
            j+=4;
        }
    }

    delete[] tmp;

    TRACE(("TGA: %d, %d\n", w, h));
    rect_.w = w;
    rect_.h = h;
}



Surface::~Surface()
{
    delete pixels_;
}

