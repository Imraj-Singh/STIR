//
//
/*!
  \file
  \ingroup recon_buildblock

  \brief Declaration of class BinNormalisationSPECT

  \author Kris Thielemans
  \author Daniel Deidda
*/
/*
    Copyright (C) 2019-2021, UCL
    Copyright (C) 2019-2021, NPL
    See STIR/LICENSE.txt for details
*/

#ifndef __stir_recon_buildblock_BinNormalisationSPECT_H__
#define __stir_recon_buildblock_BinNormalisationSPECT_H__

#include "stir/recon_buildblock/BinNormalisation.h"
#include "stir/recon_buildblock/BinNormalisationWithCalibration.h"
#include "stir/RegisteredParsingObject.h"
#include "stir/RelatedViewgrams.h"
#include "stir/decay_correction_factor.h"
#include "stir/Succeeded.h"

#include <string>

START_NAMESPACE_STIR

class BinNormalisationSPECT
    : public RegisteredParsingObject<BinNormalisationSPECT, BinNormalisation, BinNormalisationWithCalibration>
{
private:
  using base_type = BinNormalisationWithCalibration;

public:
  //! Name which will be used when parsing a BinNormalisation object
  static const char* const registered_name;

  BinNormalisationSPECT();

  BinNormalisationSPECT(const std::string& filename);

  void read_norm_data(const std::string& filename);
  Succeeded set_up(const shared_ptr<const ExamInfo>& exam_info_sptr, const shared_ptr<const ProjDataInfo>&) override;
  void set_num_views(int num_views) const { this->num_views = num_views; }

  void set_uniformity(Array<3, float>& uniformity) { this->down_sampled_uniformity = uniformity; }

  bool use_dead_time() const;
  bool use_detector_efficiencies() const;
  double get_half_life() const;
  bool use_uniformity_factors() const;
  bool use_decay_correction_factors() const;
  bool use_COR_factors() const;

  float get_dead_time_efficiency(const DetectionPosition<>& det_pos, const double start_time, const double end_time) const;

  // import all apply/undo methods from base-class (we'll override some below)
  using base_type::apply;
  using base_type::undo;

  void apply(RelatedViewgrams<float>& viewgrams) const override;

  void undo(RelatedViewgrams<float>& viewgrams) const override;

  float get_uncalibrated_bin_efficiency(const Bin& bin) const override;

  void read_linearity_table(Array<3, float>& linearity) const;
  void read_uniformity_table(Array<3, float>& uniformity) const;
  void read_cor_table(Array<3, float>& cor) const;

  void resample_uniformity( // Array<3,float>& down_sampled_uniformity,
      Array<3, float> uniformity,
      const int max_ax,
      int zoom) const;

protected:
  // parsing stuff
  void set_defaults() override;
  void initialise_keymap() override;
  bool post_processing() override;

  int max_tang;
  shared_ptr<ProjData> norm_proj_data_info_ptr;
  mutable Array<1, float> normalisation_spect;
  Array<3, float> uniformity;
  Array<3, float> cor;
  mutable float map[1048576];
  mutable Array<3, float> down_sampled_uniformity;
  mutable RelatedViewgrams<float> NCOR_viewgrams;
  std::string uniformity_filename, folder_prefix, projdata_filename;
  float bin_efficiency;
  float measured_calibration_factor;

  bool _use_detector_efficiencies;
  bool _use_decay_correction;
  bool _use_uniformity_factors;
  bool _use_dead_time;
  bool _use_cor_factors;
  double half_life, view_time_interval;
  int num_detector_heads;
  mutable int num_views;
  mutable bool resampled;
};

END_NAMESPACE_STIR

#endif
