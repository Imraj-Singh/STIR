//
//
/*
    Copyright (C) 2003- 2011, Hammersmith Imanet Ltd
    This file is part of STIR.

    SPDX-License-Identifier: Apache-2.0

    See STIR/LICENSE.txt for details
*/
/*!
  \file
  \ingroup listmode
  \brief Definition of class stir::CListEventCylindricalScannerWithViewTangRingRingEncoding

  \author Kris Thielemans

*/

#ifndef __stir_listmode_CListEventCylindricalScannerWithViewTangRingRingEncoding_H__
#define __stir_listmode_CListEventCylindricalScannerWithViewTangRingRingEncoding_H__

#include "stir/listmode/CListEventCylindricalScannerWithDiscreteDetectors.h"
#include "stir/DetectionPositionPair.h"

START_NAMESPACE_STIR

//! Helper class for listmode events when using 2d sinograms and ring-pairs is most efficient
/*! \ingroup listmode

  This class simplifies coding of a CListEventCylindricalScannerWithDiscreteDetectors
  class in case the coordinates are stored in the raw data as
  \c view_num, \c tangential_pos_num, \c ring_a and \c ring_b.

  The default implementations for get_detection_position() etc are somewhat inefficient in such
  case. This helper class provides faster implementations. For example usage,
  see ecat::ecat7::CListEventECAT966, but it's intended to be used as follows

  \code
  class myEventClass : public  CListEventCylindricalScannerWithDiscreteDetectors<myEventClass>
  {
    public:
      someType get_data();
    // etc
  };
  \endcode
  This implementation of this class relies on \c someType providing the following functions
  \code
  class someType
  {
  void get_sinogram_and_ring_coordinates(int& view, int& tangential_pos_num, unsigned int& ring_a, unsigned int& ring_b) const;

  void set_sinogram_and_ring_coordinates(
                        const int view_num, const int tangential_pos_num,
                        const int ring_a, const int ring_b);
  // etc
  };
  \endcode
*/
template <class Derived>
class CListEventCylindricalScannerWithViewTangRingRingEncoding : public CListEventCylindricalScannerWithDiscreteDetectors
{
public:
  CListEventCylindricalScannerWithViewTangRingRingEncoding(const shared_ptr<ProjDataInfo>& proj_data_info)
      : CListEventCylindricalScannerWithDiscreteDetectors(proj_data_info)
  {}

  //! This routine returns the corresponding detector pair
  inline void get_detection_position(DetectionPositionPair<>&) const;

  /*! This routine constructs a (prompt) coincidence event */
  inline void set_detection_position(const DetectionPositionPair<>&);

  //! warning only ProjDataInfoCylindricalNoArcCorr
  inline virtual void get_bin(Bin&, const ProjDataInfo&) const;

  //! This method checks if the template is valid for LmToProjData
  /*! Used before the actual processing of the data (see issue #61), before calling get_bin()
   *  Most scanners have listmode data that correspond to non arc-corrected data and
   *  this check avoids a crash when an unsupported template is used as input.
   */
  inline virtual bool is_valid_template(const ProjDataInfo&) const;
};

END_NAMESPACE_STIR

#include "stir/listmode/CListEventCylindricalScannerWithViewTangRingRingEncoding.inl"

#endif
