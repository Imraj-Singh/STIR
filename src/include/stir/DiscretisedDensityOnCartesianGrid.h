//
//
#ifndef __stir_DiscretisedDensityOnCartesianGrid_H__
#define __stir_DiscretisedDensityOnCartesianGrid_H__
/*
    Copyright (C) 2000- 2007, Hammersmith Imanet Ltd
    Copyright (C) 2000 PARAPET partners
    Copyright (C) 2018, University College London
    This file is part of STIR.

    SPDX-License-Identifier: Apache-2.0 AND License-ref-PARAPET-license

    See STIR/LICENSE.txt for details
 */
/*!
  \file
  \ingroup densitydata

  \brief defines the stir::DiscretisedDensityOnCartesianGrid class

  \author Sanida Mustafovic
  \author Kris Thielemans
  \author (help from Alexey Zverovich)
  \author PARAPET project


*/
#include "stir/DiscretisedDensity.h"

START_NAMESPACE_STIR

/*!
  \ingroup densitydata
  \brief This abstract class is the basis for images on a Cartesian grid.

  The only new information compared to DiscretisedDensity is the grid_spacing.
  This is currently a BasicCoordinate<num_dimensions,float> object.
  It really should be CartesianCoordinate<num_dimensions,float> object, but
  we don't have such a class (as we can't provide names for the coordinates
  in the n-dimensional case.)
*/

template <int num_dimensions, typename elemT>
class DiscretisedDensityOnCartesianGrid : public DiscretisedDensity<num_dimensions, elemT>
{
private:
  typedef DiscretisedDensity<num_dimensions, elemT> base_type;

public:
  //! Construct an empty DiscretisedDensityOnCartesianGrid
  inline DiscretisedDensityOnCartesianGrid();

  //! Constructor given range, grid spacing and origin
  inline DiscretisedDensityOnCartesianGrid(const IndexRange<num_dimensions>& range,
                                           const CartesianCoordinate3D<float>& origin,
                                           const BasicCoordinate<num_dimensions, float>& grid_spacing);

  //! Constructor given exam_info, range, grid spacing and origin
  inline DiscretisedDensityOnCartesianGrid(const shared_ptr<const ExamInfo>& exam_info_sptr,
                                           const IndexRange<num_dimensions>& range,
                                           const CartesianCoordinate3D<float>& origin,
                                           const BasicCoordinate<num_dimensions, float>& grid_spacing);

  //! Return the grid_spacing
  inline const BasicCoordinate<num_dimensions, float>& get_grid_spacing() const;

  //! Set the grid_spacing
  inline void set_grid_spacing(const BasicCoordinate<num_dimensions, float>& grid_spacing_v);

protected:
  inline bool actual_has_same_characteristics(DiscretisedDensity<num_dimensions, elemT> const&,
                                              std::string& explanation) const override;

  //! Return the relative coordinates of the centre of the basis-function corresponding to \c indices.
  /*! The return value is relative to the origin.
      \see get_relative_coordinates_for_indices

      If \c num_dimensions is not 3, then the indices are assumed to correspond
      to the lowest dimensions.

      \todo cope with non-standard orientations
  */
  inline CartesianCoordinate3D<float>
  actual_get_relative_coordinates_for_indices(const BasicCoordinate<num_dimensions, float>& indices) const override;

  //! Return the indices of the basis-function closest to the given point.
  /*! The input argument should be relative to the origin.
      \see get_index_coordinates_for_relative_coordinates

      If \c num_dimensions is not 3, then the indices coordinates are assumed to correspond
      to the lowest dimensions.

      \todo cope with non-standard orientations
  */
  inline BasicCoordinate<num_dimensions, float>
  actual_get_index_coordinates_for_relative_coordinates(const CartesianCoordinate3D<float>& coords) const override;

private:
  BasicCoordinate<num_dimensions, float> grid_spacing;
};

END_NAMESPACE_STIR

#include "stir/DiscretisedDensityOnCartesianGrid.inl"
#endif
