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

// include
// std
#include <tuple>

// boost
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Diff test
#include <boost/test/unit_test.hpp>
#include <boost/math/constants/constants.hpp>

// roboptim
#include <roboptim/core/finite-difference-gradient.hh>

// PG
#include "ConfigStruct.h"
#include "PGData.h"
#include "FixedContactConstr.h"
#include "PlanarSurfaceConstr.h"
#include "StaticStabilityConstr.h"

// Arm
#include "Z12Arm.h"

const Eigen::Vector3d gravity(0., 9.81, 0.);


template <typename T>
double checkGradient(
 const roboptim::GenericDifferentiableFunction<T>& function,
 const typename roboptim::GenericDifferentiableFunction<T>::vector_t& x)
{
  Eigen::MatrixXd jac(std::get<0>(function.jacobianSize()),
                      std::get<1>(function.jacobianSize()));
  Eigen::MatrixXd jacF(std::get<0>(function.jacobianSize()),
                       std::get<1>(function.jacobianSize()));

  roboptim::GenericFiniteDifferenceGradient<T> fdfunction(function);
  function.jacobian(jac, x);
  fdfunction.jacobian(jacF, x);

  return (jac - jacF).norm();
}


template <typename T>
double checkForceGradient(
 const roboptim::GenericDifferentiableFunction<T>& function,
 const typename roboptim::GenericDifferentiableFunction<T>::vector_t& x,
    const pg::PGData& pgdata)
{
  auto rows = std::get<0>(function.jacobianSize());
  auto cols = std::get<1>(function.jacobianSize());
  Eigen::MatrixXd jac(rows, cols);
  Eigen::MatrixXd jacF(rows, cols);

  roboptim::GenericFiniteDifferenceGradient<T> fdfunction(function);
  function.jacobian(jac, x);
  fdfunction.jacobian(jacF, x);

  int deb = pgdata.forceParamsBegin();
  return (jac.block(0, deb, rows, cols - deb) -
          jacF.block(0, deb, rows, cols - deb)).norm();
}


BOOST_AUTO_TEST_CASE(FixedContactPosTest)
{
  rbd::MultiBody mb;
  rbd::MultiBodyConfig mbc;
  std::tie(mb, mbc) = makeZ12Arm();

  pg::PGData pgdata(mb, gravity);

  Eigen::Vector3d target(2., 0., 0.);
  sva::PTransformd surface(sva::PTransformd::Identity());

  pg::FixedPositionContactConstr fpc(&pgdata, 12, target, surface);

  for(int i = 0; i < 100; ++i)
  {
    Eigen::VectorXd x(Eigen::VectorXd::Random(mb.nrDof()));
    BOOST_CHECK_SMALL(checkGradient(fpc, x), 1e-4);
  }
}

BOOST_AUTO_TEST_CASE(FixedContactOriTest)
{
  namespace cst = boost::math::constants;

  rbd::MultiBody mb;
  rbd::MultiBodyConfig mbc;
  std::tie(mb, mbc) = makeZ12Arm();

  pg::PGData pgdata(mb, gravity);

  Eigen::Matrix3d oriTarget(sva::RotZ(-cst::pi<double>()));
  sva::PTransformd surface(sva::PTransformd::Identity());

  pg::FixedOrientationContactConstr foc(&pgdata, 12, oriTarget, surface);

  for(int i = 0; i < 100; ++i)
  {
    Eigen::VectorXd x(Eigen::VectorXd::Random(mb.nrDof()));
    BOOST_CHECK_SMALL(checkGradient(foc, x), 1e-4);
  }
}

BOOST_AUTO_TEST_CASE(PlanarPositionContactTest)
{
  namespace cst = boost::math::constants;

  rbd::MultiBody mb;
  rbd::MultiBodyConfig mbc;
  std::tie(mb, mbc) = makeZ12Arm();

  pg::PGData pgdata(mb, gravity);

  sva::PTransformd target(Eigen::Vector3d(0., 1., 0.));
  sva::PTransformd surface(sva::PTransformd::Identity());

  pg::PlanarPositionContactConstr ppp(&pgdata, 12, target, surface);

  for(int i = 0; i < 100; ++i)
  {
    Eigen::VectorXd x(Eigen::VectorXd::Random(mb.nrDof()));
    BOOST_CHECK_SMALL(checkGradient(ppp, x), 1e-4);
  }
}

BOOST_AUTO_TEST_CASE(PlanarOrientationContactTest)
{
  namespace cst = boost::math::constants;

  rbd::MultiBody mb;
  rbd::MultiBodyConfig mbc;
  std::tie(mb, mbc) = makeZ12Arm();

  pg::PGData pgdata(mb, gravity);

  Eigen::Matrix3d oriTarget(sva::RotZ(-cst::pi<double>()));
  sva::PTransformd surface(sva::PTransformd::Identity());

  pg::PlanarOrientationContactConstr pop(&pgdata, 12, oriTarget, surface, 1);

  for(int i = 0; i < 100; ++i)
  {
    Eigen::VectorXd x(Eigen::VectorXd::Random(mb.nrDof()));
    BOOST_CHECK_SMALL(checkGradient(pop, x), 1e-4);
  }
}

BOOST_AUTO_TEST_CASE(PlanarInclusionTest)
{
  namespace cst = boost::math::constants;

  rbd::MultiBody mb;
  rbd::MultiBodyConfig mbc;
  std::tie(mb, mbc) = makeZ12Arm();

  pg::PGData pgdata(mb, gravity);

  sva::PTransformd targetSurface(sva::RotZ(-cst::pi<double>()), Eigen::Vector3d(0., 1., 0.));
  sva::PTransformd bodySurface(sva::RotZ(-cst::pi<double>()/2.), Eigen::Vector3d(0., 1., 0.));
  std::vector<Eigen::Vector2d> targetPoints = {{1., 1.}, {-0., 1.}, {-0., -1.}, {1., -1.}};
  std::vector<Eigen::Vector2d> surfPoints = {{0.1, 0.1}, {-0.1, 0.1}, {-0.1, -0.1}, {0.1, -0.1}};

  pg::PlanarInclusionConstr pi(&pgdata, 12, targetSurface, targetPoints,
                               bodySurface, surfPoints);

  for(int i = 0; i < 100; ++i)
  {
    Eigen::VectorXd x(Eigen::VectorXd::Random(mb.nrDof()));
    BOOST_CHECK_SMALL(checkGradient(pi, x), 1e-4);
  }
}

BOOST_AUTO_TEST_CASE(StaticStabilityTest)
{
  using namespace Eigen;
  namespace cst = boost::math::constants;

  rbd::MultiBody mb;
  rbd::MultiBodyConfig mbc;
  std::tie(mb, mbc) = makeZ12Arm();

  pg::PGData pgdata(mb, gravity);

  sva::PTransformd bodySurface(sva::RotZ(-cst::pi<double>()/2.), Eigen::Vector3d(0., 1., 0.));
  std::vector<Vector2d> surfPoints = {{0.1, 0.1}, {-0.1, 0.1}, {-0.1, -0.1}, {0.1, -0.1}};
  std::vector<sva::PTransformd> points(surfPoints.size());
  for(std::size_t i = 0; i < points.size(); ++i)
  {
    points[i] = sva::PTransformd(Vector3d(surfPoints[i][0], surfPoints[i][1], 0.))*bodySurface;
  }
  pgdata.forces({pg::ForceContact{12, points, 0.7}, pg::ForceContact{0, points, 0.7}});

  pg::StaticStabilityConstr ss(&pgdata);

  for(int i = 0; i < 100; ++i)
  {
    Eigen::VectorXd x(Eigen::VectorXd::Random(pgdata.pbSize()));
    BOOST_CHECK_SMALL(checkGradient(ss, x), 1e-4);
  }
}
