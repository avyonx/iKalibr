// iKalibr: Unified Targetless Spatiotemporal Calibration Framework
// Copyright 2024, the School of Geodesy and Geomatics (SGG), Wuhan University, China
// https://github.com/Unsigned-Long/iKalibr.git
//
// Author: Shuolong Chen (shlchen@whu.edu.cn)
// GitHub: https://github.com/Unsigned-Long
//  ORCID: 0000-0002-5283-9057
//
// Purpose: See .h/.hpp file.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// * Redistributions of source code must retain the above copyright notice,
//   this list of conditions and the following disclaimer.
// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
// * The names of its contributors can not be
//   used to endorse or promote products derived from this software without
//   specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

#ifndef HASTE_DATA_IO_H
#define HASTE_DATA_IO_H

#include "util/utils.h"
#include "util/cereal_archive_helper.hpp"

namespace {
bool IKALIBR_UNIQUE_NAME(_2_) = ns_ikalibr::_1_(__FILE__);
}

namespace ns_veta {
struct PinholeIntrinsic;
using PinholeIntrinsicPtr = std::shared_ptr<PinholeIntrinsic>;
}  // namespace ns_veta

namespace ns_ikalibr {
struct EventArray;
using EventArrayPtr = std::shared_ptr<EventArray>;

struct EventsInfo {
public:
    struct SubBatch {
        int index;
        double start_time;
        double end_time;
        std::size_t event_count;

        SubBatch(int index, double start_time, double end_time, std::size_t event_count);

        SubBatch() = default;

        template <class Archive>
        void serialize(Archive &ar) {
            ar(CEREAL_NVP(index), CEREAL_NVP(start_time), CEREAL_NVP(end_time),
               CEREAL_NVP(event_count));
        }
    };

public:
    std::string topic;
    std::string root_path;
    double raw_start_time{};
    std::vector<SubBatch> batches;

public:
    EventsInfo(std::string topic,
               std::string root_path,
               double raw_start_time,
               const std::vector<SubBatch> &batches);

    EventsInfo() = default;

    void SaveToDisk(const std::string &filename,
                    CerealArchiveType::Enum archiveType = CerealArchiveType::Enum::YAML) const;

    static EventsInfo LoadFromDisk(
        const std::string &filename,
        CerealArchiveType::Enum archiveType = CerealArchiveType::Enum::YAML);

public:
    template <class Archive>
    void serialize(Archive &ar) {
        ar(CEREAL_NVP(topic), CEREAL_NVP(root_path), CEREAL_NVP(raw_start_time),
           CEREAL_NVP(batches));
    }
};

struct HASTEDataIO {
public:
    static std::pair<std::string, EventsInfo::SubBatch> SaveRawEventData(
        const std::vector<EventArrayPtr>::const_iterator &fromIter,
        const std::vector<EventArrayPtr>::const_iterator &toIter,
        const ns_veta::PinholeIntrinsicPtr &intri,
        const std::vector<Eigen::Vector2d> &seeds,
        double seedsTime,
        const std::string &subWS,
        int batchIdx = 0);

    static void SaveEventsInfo(const EventsInfo &info, const std::string &ws);
};
}  // namespace ns_ikalibr

#endif  // HASTE_DATA_IO_H
