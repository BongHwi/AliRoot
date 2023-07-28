//**************************************************************************\
//* This file is property of and copyright by the ALICE Project            *\
//* ALICE Experiment at CERN, All rights reserved.                         *\
//*                                                                        *\
//* Primary Authors: Matthias Richter <Matthias.Richter@ift.uib.no>        *\
//*                  for The ALICE HLT Project.                            *\
//*                                                                        *\
//* Permission to use, copy, modify and distribute this software and its   *\
//* documentation strictly for non-commercial purposes is hereby granted   *\
//* without fee, provided that the above copyright notice appears in all   *\
//* copies and that both the copyright notice and this permission notice   *\
//* appear in the supporting documentation. The authors make no claims     *\
//* about the suitability of this software for any purpose. It is          *\
//* provided "as is" without express or implied warranty.                  *\
//**************************************************************************

/// \file GPUTRDInterfaceO2Track.h
/// \author Ole Schmidt

#ifndef GPUTRDINTERFACEO2TRACK_H
#define GPUTRDINTERFACEO2TRACK_H

// This is the interface for the GPUTRDTrack based on the O2 track type
#include "GPUCommonDef.h"
namespace GPUCA_NAMESPACE
{
namespace gpu
{
template <typename T>
class trackInterface;
class GPUTPCGMMergedTrack;
namespace gputpcgmmergertypes
{
struct GPUTPCOuterParam;
} // namespace gputpcgmmergertypes
} // namespace gpu
} // namespace GPUCA_NAMESPACE

#include "ReconstructionDataFormats/Track.h"
#include "ReconstructionDataFormats/TrackTPCITS.h"
#include "DataFormatsTPC/TrackTPC.h"
#include "ReconstructionDataFormats/GlobalTrackID.h"
#include "ReconstructionDataFormats/TrackLTIntegral.h"

namespace GPUCA_NAMESPACE
{
namespace gpu
{

template <>
class trackInterface<o2::track::TrackParCov> : public o2::track::TrackParCov
{
 public:
  GPUdDefault() trackInterface<o2::track::TrackParCov>() = default;
  trackInterface<o2::track::TrackParCov>(const o2::track::TrackParCov& param) = delete;
  GPUd() trackInterface<o2::track::TrackParCov>(const o2::dataformats::TrackTPCITS& trkItsTpc) : o2::track::TrackParCov(trkItsTpc.getParamOut()) {}
  GPUd() trackInterface<o2::track::TrackParCov>(const o2::tpc::TrackTPC& trkTpc) : o2::track::TrackParCov(trkTpc.getParamOut()) {}

  GPUd() void set(float x, float alpha, const float* param, const float* cov)
  {
    setX(x);
    setAlpha(alpha);
    for (int i = 0; i < 5; i++) {
      setParam(param[i], i);
    }
    for (int i = 0; i < 15; i++) {
      setCov(cov[i], i);
    }
  }
  GPUd() trackInterface<o2::track::TrackParCov>(const GPUTPCGMMergedTrack& trk);
  GPUd() trackInterface<o2::track::TrackParCov>(const gputpcgmmergertypes::GPUTPCOuterParam& param);
  GPUd() void updateCovZ2(float addZerror) { updateCov(addZerror, o2::track::CovLabels::kSigZ2); }
  GPUd() o2::track::TrackLTIntegral& getLTIntegralOut() { return mLTOut; }
  GPUd() const o2::track::TrackLTIntegral& getLTIntegralOut() const { return mLTOut; }
  GPUd() o2::track::TrackParCov& getOuterParam() { return mParamOut; }
  GPUd() const o2::track::TrackParCov& getOuterParam() const { return mParamOut; }

  GPUdi() const float* getPar() const { return getParams(); }

  GPUdi() bool CheckNumericalQuality() const { return true; }

  typedef o2::track::TrackParCov baseClass;

 private:
  o2::track::TrackLTIntegral mLTOut;
  o2::track::TrackParCov mParamOut;

  ClassDefNV(trackInterface, 1);
};

} // namespace gpu
} // namespace GPUCA_NAMESPACE

#endif