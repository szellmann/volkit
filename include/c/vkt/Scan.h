// This file is distributed under the MIT license.
// See the LICENSE file for details.

#pragma once

#include <stdint.h>

#include "common.h"
#include "forward.h"
#include "linalg.h"

#ifdef __cplusplus
extern "C"
{
#endif

VKTAPI vktError vktScanSV(vktStructuredVolume dst,
                          vktStructuredVolume src);

VKTAPI vktError vktScanRangeSV(vktStructuredVolume dst,
                               vktStructuredVolume src,
                               int32_t firstX,
                               int32_t firstY,
                               int32_t firstZ,
                               int32_t lastX,
                               int32_t lastY,
                               int32_t lastZ,
                               int32_t dstOffsetX,
                               int32_t dstOffsetY,
                               int32_t dstOffsetZ);

#ifdef __cplusplus
}
#endif
