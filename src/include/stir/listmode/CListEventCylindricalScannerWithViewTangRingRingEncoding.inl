//
//
/*
    Copyright (C) 2003- 2011, Hammersmith Imanet Ltd
    Copyright (C) 2018, 2022, University College London
    This file is part of STIR.

    SPDX-License-Identifier: Apache-2.0

    See STIR/LICENSE.txt for details
*/
/*!
  \file
  \ingroup listmode
  \brief Implementation for stir::CListEventCylindricalScannerWithViewTangRingRingEncoding

  \author Kris Thielemans
  \author Elise Emond
  \author Nikos Efthimiou

*/

START_NAMESPACE_STIR

template <class Derived>
void
CListEventCylindricalScannerWithViewTangRingRingEncoding<Derived>::get_detection_position(DetectionPositionPair<>& det_pos) const
{
  det_pos.pos1().radial_coord() = 0;
  det_pos.pos2().radial_coord() = 0;
  int tangential_pos_num;
  int view_num;
  static_cast<const Derived*>(this)->get_data().get_sinogram_and_ring_coordinates(
      view_num, tangential_pos_num, det_pos.pos1().axial_coord(), det_pos.pos2().axial_coord());

  int det_num_1, det_num_2;
  this->get_uncompressed_proj_data_info_sptr()->get_det_num_pair_for_view_tangential_pos_num(
      det_num_1, det_num_2, view_num, tangential_pos_num);
  det_pos.pos1().tangential_coord() = det_num_1;
  det_pos.pos2().tangential_coord() = det_num_2;
  det_pos.timing_pos() = this->get_uncompressed_proj_data_info_sptr()->get_tof_bin(delta_time);
}

template <class Derived>
void
CListEventCylindricalScannerWithViewTangRingRingEncoding<Derived>::set_detection_position(const DetectionPositionPair<>& det_pos)
{
  int tangential_pos_num;
  int view_num;
  const bool swap_rings = this->get_uncompressed_proj_data_info_sptr()->get_view_tangential_pos_num_for_det_num_pair(
      view_num, tangential_pos_num, det_pos.pos1().tangential_coord(), det_pos.pos2().tangential_coord());

  if (swap_rings)
    {
      static_cast<Derived*>(this)->get_data().set_sinogram_and_ring_coordinates(
          view_num, tangential_pos_num, det_pos.pos1().axial_coord(), det_pos.pos2().axial_coord());
    }
  else
    {
      static_cast<Derived*>(this)->get_data().set_sinogram_and_ring_coordinates(
          view_num, tangential_pos_num, det_pos.pos2().axial_coord(), det_pos.pos1().axial_coord());
    }
  if (this->get_uncompressed_proj_data_info_sptr()->is_tof_data())
    error(
        "TODO: CListEventCylindricalScannerWithViewTangRingRingEncoding::set_detection_position needs to be implemented for TOF");
}

static void
sinogram_coordinates_to_bin(Bin& bin,
                            const int view_num,
                            const int tang_pos_num,
                            const int ring_a,
                            const int ring_b,
                            const ProjDataInfoCylindrical& proj_data_info)
{
  if (proj_data_info.get_segment_axial_pos_num_for_ring_pair(bin.segment_num(), bin.axial_pos_num(), ring_a, ring_b)
      == Succeeded::no)
    {
      bin.set_bin_value(-1);
      return;
    }
  bin.set_bin_value(1);
  bin.view_num() = view_num / proj_data_info.get_view_mashing_factor();
  bin.tangential_pos_num() = tang_pos_num;
}

template <class Derived>
void
CListEventCylindricalScannerWithViewTangRingRingEncoding<Derived>::get_bin(Bin& bin, const ProjDataInfo& proj_data_info) const
{
  assert(dynamic_cast<const ProjDataInfoCylindricalNoArcCorr*>(&proj_data_info) != 0);

  int tangential_pos_num;
  int view_num;
  unsigned int ring_a;
  unsigned int ring_b;
  static_cast<const Derived*>(this)->get_data().get_sinogram_and_ring_coordinates(view_num, tangential_pos_num, ring_a, ring_b);
  sinogram_coordinates_to_bin(
      bin, view_num, tangential_pos_num, ring_a, ring_b, static_cast<const ProjDataInfoCylindrical&>(proj_data_info));
  bin.timing_pos_num() = proj_data_info.get_tof_bin(delta_time);
}

template <class Derived>
bool
CListEventCylindricalScannerWithViewTangRingRingEncoding<Derived>::is_valid_template(const ProjDataInfo& proj_data_info) const
{
  if (dynamic_cast<ProjDataInfoCylindricalNoArcCorr const*>(&proj_data_info) != 0)
    return true;

  return false;
}

END_NAMESPACE_STIR
