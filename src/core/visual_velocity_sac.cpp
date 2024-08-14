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

#include "core/visual_velocity_sac.h"
#include "opengv/sac/Ransac.hpp"
#include "spdlog/spdlog.h"

namespace {
bool IKALIBR_UNIQUE_NAME(_2_) = ns_ikalibr::_1_(__FILE__);
}

namespace ns_ikalibr {

int VisualVelocitySacProblem::getSampleSize() const { return 2; }

bool VisualVelocitySacProblem::computeModelCoefficients(
    const std::vector<int> &indices, VisualVelocitySacProblem::model_t &outModel) const {
    std::vector<std::tuple<Eigen::Vector2d, Eigen::Vector2d, double>> selected(indices.size());
    for (int i = 0; i < static_cast<int>(indices.size()); ++i) {
        selected.at(i) = dynamics.at(indices.at(i));
    }
    auto res =
        VisualVelocityEstimator::Create(selected, intri)->Estimate(timeByBr, spline, SO3_DnToBr);
    if (res) {
        outModel = *res;
        return true;
    } else {
        return false;
    }
}

void VisualVelocitySacProblem::getSelectedDistancesToModel(
    const VisualVelocitySacProblem::model_t &model,
    const std::vector<int> &indices,
    std::vector<double> &scores) const {
    scores.resize(indices.size());
    const double fx = intri->FocalX(), fy = intri->FocalY();
    const double cx = intri->PrincipalPoint()(0), cy = intri->PrincipalPoint()(1);

    for (int i = 0; i < static_cast<int>(indices.size()); ++i) {
        const auto &[pixel, vel, depth] = dynamics.at(i);
        const double up = pixel(0) - cx, vp = pixel(1) - cy;

        Eigen::Matrix<double, 2, 3> subAMat = Eigen::Matrix<double, 2, 3>::Zero();
        subAMat(0, 0) = -fx;
        subAMat(1, 1) = -fy;
        subAMat(0, 2) = up;
        subAMat(1, 2) = vp;

        Eigen::Matrix<double, 2, 3> subBMat = Eigen::Matrix<double, 2, 3>::Zero();
        subBMat(0, 0) = up * vp / fy;
        subBMat(0, 1) = -fx - up * up / fx;
        subBMat(0, 2) = fx * vp / fy;
        subBMat(1, 0) = fy + vp * vp / fy;
        subBMat(1, 1) = -up * vp / fx;
        subBMat(1, 2) = -fy * up / fx;

        Eigen::Vector3d ANG_VEL_BrToWInBr = spline.VelocityBody(timeByBr);
        Eigen::Vector3d ANG_VEL_DnToWInDn = SO3_DnToBr.inverse() * ANG_VEL_BrToWInBr;

        scores.at(i) = (1 / depth * subAMat * model + subBMat * ANG_VEL_DnToWInDn - vel).norm();
    }
}

void VisualVelocitySacProblem::optimizeModelCoefficients(
    const std::vector<int> &inliers,
    const VisualVelocitySacProblem::model_t &model,
    VisualVelocitySacProblem::model_t &optimized_model) {
    computeModelCoefficients(inliers, optimized_model);
}

std::optional<Eigen::Vector3d> VisualVelocitySacProblem::VisualVelocityEstimationRANSAC(
    const std::vector<std::tuple<Eigen::Vector2d, Eigen::Vector2d, double>> &dynamics,
    const ns_veta::PinholeIntrinsic::Ptr &intri,
    double timeByBr,
    const VisualVelocityEstimator::So3SplineType &spline,
    const Sophus::SO3d &SO3_DnToBr,
    double thd) {
    opengv::sac::Ransac<VisualVelocitySacProblem> ransac;
    std::shared_ptr<VisualVelocitySacProblem> probPtr(
        new VisualVelocitySacProblem(dynamics, intri, timeByBr, spline, SO3_DnToBr));
    ransac.sac_model_ = probPtr;
    ransac.threshold_ = Configor::Prior::CauchyLossForRGBDFactor * intri->Focal();
    ransac.max_iterations_ = 20;
    bool res = ransac.computeModel();
    if (res) {
        // spdlog::info("inlier rate: {}/{}", ransac.inliers_.size(), dynamics.size());
        Eigen::Vector3d vel;
        probPtr->optimizeModelCoefficients(ransac.inliers_, ransac.model_coefficients_, vel);

        // std::cout << " RANSAC: " << vel.transpose() << std::endl;
        // std::cout << "LS PROB: "
        //           << VisualVelocityEstimator::Create(dynamics, intri)
        //                  ->Estimate(timeByBr, spline, SO3_DnToBr)
        //                  ->transpose()
        //           << std::endl;
        return vel;
    } else {
        spdlog::warn("compute velocity using RANSAC failed, try to use all measurements to fit...");
        auto vvEstimator = VisualVelocityEstimator::Create(dynamics, intri);
        return vvEstimator->Estimate(timeByBr, spline, SO3_DnToBr);
    }
}
}  // namespace ns_ikalibr