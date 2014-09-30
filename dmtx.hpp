#ifndef DMTX_HPP_INCLUDED
#define DMTX_HPP_INCLUDED

#include <string>
#include <memory>
#include <cstdint>
#include "dmtx.h"

namespace dmtx
{
    struct pack
    {
        enum type
        {
            /* Custom format */
            custom              = 100,
            /* 1 bpp */
            k1                  = 200,
            /* 8 bpp grayscale */
            k8                  = 300,
            /* 16 bpp formats */
            rgb16               = 400,
            rgbx16,
            xrgb16,
            bgr16,
            bgrx16,
            xbgr16,
            ycbcr16,
            /* 24 bpp formats */
            rgb24               = 500,
            bgr24,
            ycbcr24,
            /* 32 bpp formats */
            rgbx32              = 600,
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
        image(void const* pxl, int width, int height, int pack)
          : _pimpl(dmtxImageCreate(static_cast<std::uint8_t*>(const_cast<void*>(pxl)), width, height, pack))
        {}
        
        explicit image(DmtxImage* ptr)
          : _pimpl(ptr)
        {}

        void set_channel(int channelStart, int bitsPerChannel) const
        {
            dmtxImageSetChannel(native_handle(), channelStart, bitsPerChannel);
        }

        void flip(flip::type val) const
        {
            dmtxImageSetProp(native_handle(), DmtxPropImageFlip, val);
        }

        flip::type flip() const
        {
            dmtxImageGetProp(native_handle(), DmtxPropImageFlip);
        }

        DmtxImage* native_handle() const
        {
            return _pimpl.get();
        }
        
        void swap(image& other)
        {
            _pimpl.swap(other._pimpl);
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
        
        message_ptr operator()(int fix = DmtxUndefined) const
        {
            if (auto reg = next_region())
                return decode(reg, fix);
            return nullptr;
        }

        DmtxDecode* native_handle() const
        {
            return _pimpl.get();
        }
        
        void swap(decoder& other)
        {
            _pimpl.swap(other._pimpl);
        }

    private:

        detail::handle<DmtxDecode, dmtxDecodeDestroy>::type _pimpl;
    };
    
    struct encoder
    {
        encoder()
          : _pimpl(dmtxEncodeCreate())
        {}
        
        image operator()(void const* msg, int n) const
        {
            auto pimpl = native_handle();
            dmtxEncodeDataMatrix(pimpl, n
              , static_cast<std::uint8_t*>(const_cast<void*>(msg)));
            auto img = pimpl->image;
            pimpl->image = nullptr;
            return image(img);
        }
        
        image operator()(std::string const& msg) const
        {
            return operator()(
                reinterpret_cast<std::uint8_t const*>(msg.data())
              , msg.size());
        }
        
        DmtxEncode* native_handle() const
        {
            return _pimpl.get();
        }
        
        void swap(encoder& other)
        {
            _pimpl.swap(other._pimpl);
        }
        
    private:

        detail::handle<DmtxEncode, dmtxEncodeDestroy>::type _pimpl;
    };
}

#endif
