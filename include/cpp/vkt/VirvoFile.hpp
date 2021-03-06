// This file is distributed under the MIT license.
// See the LICENSE file for details.

#pragma once

#include <cstddef>
#include <cstdint>
#include <memory>

#include "common.hpp"
#include "linalg.hpp"

namespace vkt
{
    class VirvoFile : public DataSource
    {
    public:
        VirvoFile(char const* fileName);
       ~VirvoFile();

        virtual std::size_t read(char* buf, std::size_t len);
        virtual std::size_t write(char const* buf, std::size_t len);
        virtual bool seek(std::size_t pos);
        virtual bool flush();
        virtual bool good() const;

        void setDims(Vec3i dims);

        Vec3i getDims();

        void setBytesPerVoxel(uint16_t bpv);

        uint16_t getBytesPerVoxel();

    private:
        struct Impl;
        std::unique_ptr<Impl> impl_;

    };

} // vkt
