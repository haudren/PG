// This file is part of PG.
//
// PG is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// PG is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with PG.  If not, see <http://www.gnu.org/licenses/>.

#pragma once

// include
// roboptim
#include <roboptim/core/differentiable-function.hh>

// RBDyn
#include <RBDyn/Jacobian.h>

namespace pg
{
class PGData;

class FixedPositionContactConstr : public roboptim::DifferentiableSparseFunction
{
public:
  typedef typename parent_t::argument_t argument_t;

public:
  FixedPositionContactConstr(PGData* pgdata, const std::string & bodyName,
      const Eigen::Vector3d& target,
      const sva::PTransformd& surfaceFrame);
  ~FixedPositionContactConstr();


  void impl_compute(result_ref res, const_argument_ref x) const;
  void impl_jacobian(jacobian_ref jac, const_argument_ref x) const;
  void impl_gradient(gradient_ref /* gradient */,
      const_argument_ref /* x */, size_type /* functionId */) const
  {
    throw std::runtime_error("NEVER GO HERE");
  }

private:
  PGData* pgdata_;

  int bodyIndex_;
  Eigen::Vector3d target_;
  sva::PTransformd surfaceFrame_;
  mutable rbd::Jacobian jac_;
};




class FixedOrientationContactConstr : public roboptim::DifferentiableSparseFunction
{
public:
  typedef typename parent_t::argument_t argument_t;

public:
  FixedOrientationContactConstr(PGData* pgdata, const std::string & bodyName,
      const Eigen::Matrix3d& target,
      const sva::PTransformd& surfaceFrame);
  ~FixedOrientationContactConstr();


  void impl_compute(result_ref res, const_argument_ref x) const;
  void impl_jacobian(jacobian_ref jac, const_argument_ref x) const;
  void impl_gradient(gradient_ref /* gradient */,
      const_argument_ref /* x */, size_type /* functionId */) const
  {
    throw std::runtime_error("NEVER GO HERE");
  }

private:
  template<typename Derived1, typename Derived2, typename Derived3>
  void dotDerivative(const Eigen::MatrixBase<Derived1>& posRow,
                     const Eigen::MatrixBase<Derived2>& targetRow,
                     Eigen::MatrixBase<Derived3> const & jac) const;

private:
  PGData* pgdata_;

  int bodyIndex_;
  sva::Matrix3d target_;
  sva::PTransformd surfaceFrame_;

  mutable rbd::Jacobian jac_;
  mutable Eigen::MatrixXd dotCacheSum_;
};

} // namespace pg
