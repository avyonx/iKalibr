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

#ifndef IKALIBR_VISUAL_GRAVITY_H
#define IKALIBR_VISUAL_GRAVITY_H

#include "opencv2/core.hpp"
#include "ctraj/core/spline_bundle.h"
#include "config/configor.h"
#include "veta/veta.h"
#include "veta/camera/pinhole_brown.h"
#include "util/cloud_define.hpp"

_3_

namespace ns_ikalibr {
    struct CalibParamManager;
    using CalibParamManagerPtr = std::shared_ptr<CalibParamManager>;
    struct CameraFrame;
    using CameraFramePtr = std::shared_ptr<CameraFrame>;

    class VisualGravityDrawer {
    public:
        using Ptr = std::shared_ptr<VisualGravityDrawer>;
        using SplineBundleType = ns_ctraj::SplineBundle<Configor::Prior::SplineOrder>;

    private:
        std::string _topic;
        ns_veta::Veta::Ptr _veta;
        SplineBundleType::Ptr _splines;
        CalibParamManagerPtr _parMagr;

        ns_veta::PinholeIntrinsic::Ptr _intri;

        Sophus::SE3d SE3_CmToBr;
        double TO_CmToBr;

    public:
        VisualGravityDrawer(std::string topic, ns_veta::Veta::Ptr veta, SplineBundleType::Ptr splines,
                            CalibParamManagerPtr parMagr);

        static Ptr Create(const std::string &topic, const ns_veta::Veta::Ptr &veta,
                          const SplineBundleType::Ptr &splines,
                          const CalibParamManagerPtr &parMagr);

        cv::Mat CreateGravityImg(const CameraFramePtr &frame, float scale = 0.1f);
    };
}


#endif //IKALIBR_VISUAL_GRAVITY_H