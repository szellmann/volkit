// This file is distributed under the MIT license.
// See the LICENSE file for details.

#include <cassert>
#include <cstring>

#include <vkt/ExecutionPolicy.hpp>
#include <vkt/LookupTable.hpp>
#include <vkt/Memory.hpp>

#include <vkt/LookupTable.h>

#include "ColorFormatInfo.hpp"
#include "LookupTable_impl.hpp"

//-------------------------------------------------------------------------------------------------
// C++ API
//

namespace vkt
{
    LookupTable::LookupTable()
        : ManagedBuffer(0)
        , dims_{0, 0, 0}
        , format_(ColorFormat::Unspecified)
    {
    }

    LookupTable::LookupTable(
            int32_t dimX,
            int32_t dimY,
            int32_t dimZ,
            ColorFormat format
            )
        : ManagedBuffer(dimX * size_t(dimY) * dimZ * ColorFormatInfoTable[(int)format].size)
        , dims_{dimX, dimY, dimZ}
        , format_(format)
    {
    }

    void LookupTable::setDims(int32_t dimX, int32_t dimY, int32_t dimZ)
    {
        setDims({ dimX, dimY, dimZ });
    }

    void LookupTable::getDims(int32_t& dimX, int32_t& dimY, int32_t& dimZ)
    {
        dimX = dims_.x;
        dimY = dims_.y;
        dimZ = dims_.z;
    }

    void LookupTable::setDims(Vec3i dims)
    {
        dims_ = dims;
        std::size_t newSize = getSizeInBytes();

        resize(newSize);
    }

    Vec3i LookupTable::getDims() const
    {
        return dims_;
    }

    void LookupTable::setColorFormat(ColorFormat format)
    {
        format_ = format;
        std::size_t newSize = getSizeInBytes();

        resize(newSize);
    }

    ColorFormat LookupTable::getColorFormat() const
    {
        return format_;
    }

    void LookupTable::setData(uint8_t* data)
    {
        migrate();

        ExecutionPolicy ep = GetThreadExecutionPolicy();

        CopyKind ck = ep.device == ExecutionPolicy::Device::GPU
                        ? CopyKind::DeviceToDevice
                        : CopyKind::HostToHost;

        Memcpy(ManagedBuffer::data_, data, getSizeInBytes(), ck);
    }

    void LookupTable::setData(
            uint8_t* data,
            int32_t dimX,
            int32_t dimY,
            int32_t dimZ,
            ColorFormat sourceFormat
            )
    {
        migrate();

        ExecutionPolicy ep = GetThreadExecutionPolicy();

        CopyKind ck = ep.device == ExecutionPolicy::Device::GPU
                        ? CopyKind::DeviceToDevice
                        : CopyKind::HostToHost;

        // TODO: interpolate between different formats
        assert(sourceFormat == format_);

        unsigned bpv = ColorFormatInfoTable[(int)format_].size;

        for (int iz = 0; iz < dims_.z; ++iz)
        {
            for (int iy = 0; iy < dims_.y; ++iy)
            {
                for (int ix = 0; ix < dims_.x; ++ix)
                {
                    // TODO: linear
                    int32_t x = ix / (float)dims_.x * dimX;
                    int32_t y = iy / (float)dims_.y * dimY;
                    int32_t z = iz / (float)dims_.z * dimZ;

                    int32_t xlo = x - .5f;

                    size_t srcIndex = (z * dimX * size_t(dimY) + y * dimX + x) * bpv;
                    size_t dstIndex = (iz * dims_.x * size_t(dims_.y) + iy * dims_.x + ix) * bpv;

                    Memcpy(ManagedBuffer::data_ + dstIndex, data + srcIndex, bpv, ck);
                }
            }
        }
    }

    uint8_t* LookupTable::getData()
    {
        migrate();

        return ManagedBuffer::data_;
    }

    std::size_t LookupTable::getSizeInBytes() const
    {
        return dims_.x * size_t(dims_.y) * dims_.z * ColorFormatInfoTable[(int)format_].size;
    }


} // vkt

//-------------------------------------------------------------------------------------------------
// C API
//

void vktLookupTableCreate(
        vktLookupTable* lut,
        int32_t dimX,
        int32_t dimY,
        int32_t dimZ,
        vktColorFormat format
        )
{
    assert(lut != nullptr);

    *lut = new vktLookupTable_impl(dimX, dimY, dimZ, (vkt::ColorFormat)format);
}

void vktLookupTableDestroy(vktLookupTable lut)
{
    delete lut;
}

void vktLookupTableSetDims3i(
        vktLookupTable lut,
        int32_t dimX,
        int32_t dimY,
        int32_t dimZ
        )
{
    lut->lut.setDims(dimX, dimY, dimZ);
}

void vktLookupTableGetDims3i(
        vktLookupTable lut,
        int32_t* dimX,
        int32_t* dimY,
        int32_t* dimZ
        )
{
    lut->lut.getDims(*dimX, *dimY, *dimZ);
}

void vktLookupTableSetDims3iv(vktLookupTable lut, vktVec3i_t dims)
{
    lut->lut.setDims(dims.x, dims.y, dims.z);
}

vktVec3i_t vktLookupTableGetDims3iv(vktLookupTable lut)
{
    vkt::Vec3i dims = lut->lut.getDims();

    return { dims.x, dims.y, dims.z };
}

void vktLookupTableSetColorFormat(vktLookupTable lut, vktColorFormat format)
{
    lut->lut.setColorFormat((vkt::ColorFormat)format);
}

vktColorFormat vktLookupTableGetColorFormat(vktLookupTable lut)
{
    return (vktColorFormat)lut->lut.getColorFormat();
}

void vktLookupTableSetData(vktLookupTable lut, uint8_t* data)
{
    return lut->lut.setData(data);
}

uint8_t* vktLookupTableGetData(vktLookupTable lut)
{
    return lut->lut.getData();
}

size_t vktLookupTableGetSizeInBytes(vktLookupTable lut)
{
    return lut->lut.getSizeInBytes();
}

vktResourceHandle vktLookupTableGetResourceHandle(vktLookupTable lut)
{
    return (vktResourceHandle)lut->lut.getResourceHandle();
}

void vktLookupTableMigrate(vktLookupTable lut)
{
    lut->lut.migrate();
}
