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

#ifndef IKALIBR_CONFIGOR_H
#define IKALIBR_CONFIGOR_H

#include "util/utils.h"
#include "util/enum_cast.hpp"
#include "util/cereal_archive_helper.hpp"

namespace {
bool IKALIBR_UNIQUE_NAME(_2_) = ns_ikalibr::_1_(__FILE__);
}

namespace ns_ikalibr {
// myenumGenor OutputOption ParamInEachIter BSplines LiDARMaps VisualMaps RadarMaps HessianMat
// VisualLiDARCovisibility VisualKinematics ColorizedLiDARMap AlignedInertialMes VisualReprojErrors
// RadarDopplerErrors
enum class OutputOption : std::uint32_t {
    /**
     * @brief options
     */
    NONE = 1 << 0,
    ParamInEachIter = 1 << 1,
    BSplines = 1 << 2,
    LiDARMaps = 1 << 3,
    VisualMaps = 1 << 4,
    RadarMaps = 1 << 5,
    HessianMat = 1 << 6,
    VisualLiDARCovisibility = 1 << 7,
    VisualKinematics = 1 << 8,
    ColorizedLiDARMap = 1 << 9,
    AlignedInertialMes = 1 << 10,
    VisualReprojErrors = 1 << 11,
    RadarDopplerErrors = 1 << 12,
    ALL = ParamInEachIter | BSplines | LiDARMaps | VisualMaps | RadarMaps | HessianMat |
          VisualLiDARCovisibility | VisualKinematics | ColorizedLiDARMap | AlignedInertialMes |
          VisualReprojErrors | RadarDopplerErrors
};

struct Configor {
public:
    using Ptr = std::shared_ptr<Configor>;

public:
    static struct DataStream {
        struct IMUConfig {
        public:
            std::string Type;
            std::string Intrinsics;
            double AcceWeight;
            double GyroWeight;

            IMUConfig()
                : Type(),
                  Intrinsics(),
                  AcceWeight(),
                  GyroWeight(){};

        public:
            template <class Archive>
            void serialize(Archive &ar) {
                ar(CEREAL_NVP(Type), CEREAL_NVP(Intrinsics), CEREAL_NVP(AcceWeight),
                   CEREAL_NVP(GyroWeight));
            }
        };

        struct RadarConfig {
        public:
            std::string Type;
            double Weight;

            RadarConfig()
                : Type(),
                  Weight(){};

        public:
            template <class Archive>
            void serialize(Archive &ar) {
                ar(CEREAL_NVP(Type), CEREAL_NVP(Weight));
            }
        };

        struct LiDARConfig {
        public:
            std::string Type;
            double Weight;

            LiDARConfig()
                : Type(),
                  Weight(){};

        public:
            template <class Archive>
            void serialize(Archive &ar) {
                ar(CEREAL_NVP(Type), CEREAL_NVP(Weight));
            }
        };

        struct CameraConfig {
        public:
            std::string Type;
            std::string Intrinsics;
            double Weight;
            int TrackLengthMin;

            CameraConfig()
                : Type(),
                  Intrinsics(),
                  Weight(),
                  TrackLengthMin(){};

        public:
            template <class Archive>
            void serialize(Archive &ar) {
                ar(CEREAL_NVP(Type), CEREAL_NVP(Intrinsics), CEREAL_NVP(Weight),
                   CEREAL_NVP(TrackLengthMin));
            }
        };

        struct RGBDConfig {
        public:
            std::string Type;
            std::string Intrinsics;
            std::string DepthTopic;
            double DepthFactor;
            double Weight;

            RGBDConfig()
                : Type(),
                  Intrinsics(),
                  DepthTopic(),
                  DepthFactor(),
                  Weight(){};

        public:
            template <class Archive>
            void serialize(Archive &ar) {
                ar(CEREAL_NVP(Type), CEREAL_NVP(Intrinsics), CEREAL_NVP(DepthTopic),
                   CEREAL_NVP(DepthFactor), CEREAL_NVP(Weight));
            }
        };

        static std::optional<std::string> CreateImageStoreFolder(const std::string &camTopic);

        static std::optional<std::string> CreateSfMWorkspace(const std::string &camTopic);

        static std::string GetImageStoreInfoFile(const std::string &camTopic);

        static std::map<std::string, IMUConfig> IMUTopics;
        static std::map<std::string, RadarConfig> RadarTopics;
        static std::map<std::string, LiDARConfig> LiDARTopics;
        static std::map<std::string, CameraConfig> CameraTopics;
        static std::map<std::string, RGBDConfig> RGBDTopics;

        static std::string ReferIMU;

        static std::string BagPath;
        static double BeginTime;
        static double Duration;

        static std::string OutputPath;
        const static std::string PkgPath;
        const static std::string DebugPath;

    public:
        template <class Archive>
        void serialize(Archive &ar) {
            ar(CEREAL_NVP(IMUTopics), CEREAL_NVP(RadarTopics), CEREAL_NVP(LiDARTopics),
               CEREAL_NVP(CameraTopics), CEREAL_NVP(RGBDTopics), CEREAL_NVP(ReferIMU),
               CEREAL_NVP(BagPath), CEREAL_NVP(BeginTime), CEREAL_NVP(Duration),
               CEREAL_NVP(OutputPath));
        }
    } dataStream;

    static struct Prior {
        static std::string SpatTempPrioriPath;
        static double GravityNorm;
        static constexpr int SplineOrder = 4;
        static bool OptTemporalParams;
        static double TimeOffsetPadding;
        static double ReadoutTimePadding;

        static struct KnotTimeDist {
            static double SO3Spline;
            static double ScaleSpline;

        public:
            template <class Archive>
            void serialize(Archive &ar) {
                ar(CEREAL_NVP(SO3Spline), CEREAL_NVP(ScaleSpline));
            }
        } knotTimeDist;

        static struct NDTLiDAROdometer {
            static double Resolution;
            static double KeyFrameDownSample;

        public:
            template <class Archive>
            void serialize(Archive &ar) {
                ar(CEREAL_NVP(Resolution), CEREAL_NVP(KeyFrameDownSample));
            }
        } ndtLiDAROdometer;

        static struct LiDARDataAssociate {
            static double MapDownSample;
            static double PointToSurfelMax;
            static double PlanarityMin;

            static constexpr double QueryDepthMin = 1;
            static constexpr double QueryDepthMax = 2;
            static constexpr double SurfelPointMin = 100;
            //   0,   1,   2,   3,   4, ...
            // 0.1, 0.2, 0.4, 0.8, 1.6, ...
            static constexpr double MapResolution = 0.1;
            static constexpr std::uint8_t MapDepthLevels = 16;

            static constexpr double PointToSurfelCountInScan = 200;

        public:
            template <class Archive>
            void serialize(Archive &ar) {
                ar(CEREAL_NVP(MapDownSample), CEREAL_NVP(PointToSurfelMax),
                   CEREAL_NVP(PlanarityMin));
            }
        } lidarDataAssociate;

        static double CauchyLossForRadarFactor;
        static double CauchyLossForLiDARFactor;
        static double CauchyLossForCameraFactor;
        static double CauchyLossForRGBDFactor;

    public:
        template <class Archive>
        void serialize(Archive &ar) {
            ar(CEREAL_NVP(SpatTempPrioriPath), CEREAL_NVP(GravityNorm),
               CEREAL_NVP(OptTemporalParams), CEREAL_NVP(TimeOffsetPadding),
               CEREAL_NVP(ReadoutTimePadding), cereal::make_nvp("KnotTimeDist", knotTimeDist),
               cereal::make_nvp("NDTLiDAROdometer", ndtLiDAROdometer),
               cereal::make_nvp("LiDARDataAssociate", lidarDataAssociate),
               CEREAL_NVP(CauchyLossForRadarFactor), CEREAL_NVP(CauchyLossForLiDARFactor),
               CEREAL_NVP(CauchyLossForCameraFactor), CEREAL_NVP(CauchyLossForRGBDFactor));
        }
    } prior;

    static struct Preference {
        static bool UseCudaInSolving;
        static OutputOption Outputs;
        static std::set<std::string> OutputsStr;
        // str for file configuration, and enum for internal use
        static std::string OutputDataFormatStr;
        static CerealArchiveType::Enum OutputDataFormat;
        const static std::map<CerealArchiveType::Enum, std::string> FileExtension;
        static int ThreadsToUse;

        const static std::string SO3_SPLINE, SCALE_SPLINE;

        // in visualizator
        static double SplineScaleInViewer;
        static double CoordSScaleInViewer;

        static int AvailableThreads();

    public:
        template <class Archive>
        void serialize(Archive &ar) {
            ar(CEREAL_NVP(UseCudaInSolving), cereal::make_nvp("Outputs", OutputsStr),
               cereal::make_nvp("OutputDataFormat", OutputDataFormatStr), CEREAL_NVP(ThreadsToUse),
               CEREAL_NVP(SplineScaleInViewer), CEREAL_NVP(CoordSScaleInViewer));
        }
    } preference;

public:
    Configor();

    static Ptr Create();

    // load configure information from file
    static bool LoadConfigure(const std::string &filename,
                              CerealArchiveType::Enum archiveType = CerealArchiveType::Enum::YAML);

    // save configure information to file
    bool SaveConfigure(const std::string &filename,
                       CerealArchiveType::Enum archiveType = CerealArchiveType::Enum::YAML);

    // print the main fields
    static void PrintMainFields();

    static std::string GetFormatExtension();

    [[nodiscard]] static bool IsLiDARIntegrated();

    [[nodiscard]] static bool IsCameraIntegrated();

    [[nodiscard]] static bool IsRadarIntegrated();

    [[nodiscard]] static bool IsRGBDIntegrated();

protected:
    // check the input configure
    static void CheckConfigure();

public:
    template <class Archive>
    void serialize(Archive &ar) {
        ar(cereal::make_nvp("DataStream", dataStream), cereal::make_nvp("Prior", prior),
           cereal::make_nvp("Preference", preference));
    }
};
}  // namespace ns_ikalibr

#endif  // IKALIBR_CONFIGOR_H
