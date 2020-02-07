// This file is distributed under the MIT license.
// See the LICENSE file for details.

#include <vkt/linalg.hpp>
#include <vkt/Voxel.hpp>

#include <vkt/System.h>
#include <vkt/Voxel.h>

#include "VoxelMapping.hpp"

//-------------------------------------------------------------------------------------------------
// C++ API
//

namespace vkt
{
    Error MapVoxel(
            uint8_t* dst,
            float value,
            uint16_t bytesPerVoxel,
            float mappingLo,
            float mappingHi
            )
    {
        MapVoxelImpl(dst, value, bytesPerVoxel, mappingLo, mappingHi);

        return NoError;
    }

    Error UnmapVoxel(
            float& value,
            uint8_t const* src,
            uint16_t bytesPerVoxel,
            float mappingLo,
            float mappingHi
            )
    {
        UnmapVoxelImpl(value, src, bytesPerVoxel, mappingLo, mappingHi);

        return NoError;
    }

} // vkt

//-------------------------------------------------------------------------------------------------
// C API
//

vktError vktMapVoxel(
        uint8_t* dst,
        float value,
        uint16_t bytesPerVoxel,
        float mappingLo,
        float mappingHi
        )
{
    return (vktError)vkt::MapVoxel(dst, value, bytesPerVoxel, mappingLo, mappingHi);
}

vktError vktUnmapVoxel(
        float* value,
        uint8_t const* src,
        uint16_t bytesPerVoxel,
        float mappingLo,
        float mappingHi
        )
{
    return (vktError)vkt::UnmapVoxel(*value, src, bytesPerVoxel, mappingLo, mappingHi);
}
