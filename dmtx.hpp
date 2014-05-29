#ifndef DMTX_HPP_INCLUDED
#define DMTX_HPP_INCLUDED

#include <memory>
#include <cstdint>
#include "dmtx.h"

#   if (defined(WINDOWS) || defined(WIN32)) && !defined(DMTX_STATIC)
#       ifndef DMTX_DECL
#           ifdef DMTX_BUILD_DLL
            /* We are building this library */
#           define DMTX_DECL __declspec(dllexport)
#           else
            /* We are using this library */
#           define DMTX_DECL __declspec(dllimport)
#           endif
#       endif
#   else
#       ifndef DMTX_DECL
#       define DMTX_DECL 
#       endif
#   endif

namespace dmtx
{
    struct pack
    {
        enum type
        {
            /* Custom format */
            custom            = 100,
            /* 1 bpp */
            k1             = 200,
            /* 8 bpp grayscale */
            k8             = 300,
            /* 16 bpp formats */
            rgb16          = 400,
            rgbx16,
            xrgb16,
            bgr16,
            bgrx16,
            xbgr16,
            ycbcr16,
            /* 24 bpp formats */
            rgb24          = 500,
            bgr24,
            ycbcr24,
            /* 32 bpp formats */
            rgbx32         = 600,
            xrgb32,
            bgrx32,
            xbgr32,
            cmyk32
        };
    };

    struct flip
    {
        enum type
        {
            none               = 0x00,
            x                  = 0x01 << 0,
            y                  = 0x01 << 1
        };
    };

    namespace detail
    {
        template<class T, DmtxPassFail f(T **)>
        struct handle
        {
            struct deleter
            {
                void operator()(T *ptr) const
                {
                    f(&ptr);
                }
            };

            typedef std::unique_ptr<T, deleter> type;
        };
    }

    struct image
    {
        image(std::uint8_t* pxl, int width, int height, int pack)
            : _pimpl(dmtxImageCreate(pxl, width, height, pack))
        {}

        void set_channel(int channelStart, int bitsPerChannel) const
        {
            dmtxImageSetChannel(native_handle(), channelStart, bitsPerChannel);
        }

        DmtxImage* native_handle() const
        {
            return _pimpl.get();
        }

    private:

        detail::handle<DmtxImage, dmtxImageDestroy>::type _pimpl;
    };

    typedef detail::handle<DmtxRegion, dmtxRegionDestroy>::type region_ptr;

    typedef detail::handle<DmtxMessage, dmtxMessageDestroy>::type message_ptr;

    struct decoder
    {
        explicit decoder(image const& img, int scale = 1)
            : _pimpl(dmtxDecodeCreate(img.native_handle(), scale))
        {}

        region_ptr next_region() const
        {
            return region_ptr(dmtxRegionFindNext(native_handle(), nullptr));
        }

        message_ptr decode(region_ptr const& reg, int fix = DmtxUndefined) const
        {
            return message_ptr(dmtxDecodeMatrixRegion(native_handle(), reg.get(), fix));
        }

        DmtxDecode* native_handle() const
        {
            return _pimpl.get();
        }

    private:

        detail::handle<DmtxDecode, dmtxDecodeDestroy>::type _pimpl;
    };
}

#endif
