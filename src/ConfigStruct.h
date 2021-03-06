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
// std
#include <vector>

// boost
#include <boost/math/constants/constants.hpp>

// Eigen
#include <Eigen/Core>

// SpaceVecAlg
#include <SpaceVecAlg/SpaceVecAlg>

// RBDyn
#include <RBDyn/MultiBody.h>


// forward declaration
namespace sch
{
class S_Object;
}

namespace pg
{

struct FixedPositionContact
{
  FixedPositionContact() {}
  FixedPositionContact(int bId, const Eigen::Vector3d& t,
                       const sva::PTransformd& sf)
    : bodyId(bId)
    , target(t)
    , surfaceFrame(sf)
  {}

  int bodyId;
  Eigen::Vector3d target; ///< Position target in world coordinate.
  sva::PTransformd surfaceFrame; ///< Body surface frame in body coordinate.
};


struct FixedOrientationContact
{
  FixedOrientationContact() {}
  FixedOrientationContact(int bId, const Eigen::Matrix3d& t,
                          const sva::PTransformd& sf)
    : bodyId(bId)
    , target(t)
    , surfaceFrame(sf)
  {}

  int bodyId;
  Eigen::Matrix3d target; ///< Orientation target in world coordinate.
  sva::PTransformd surfaceFrame; ///< Body surface frame in body coordinate.
};


struct PlanarContact
{
  PlanarContact() {}
  PlanarContact(int bId,
                const sva::PTransformd& tf, std::vector<Eigen::Vector2d> tp,
                const sva::PTransformd& sf, std::vector<Eigen::Vector2d> sp)
    : bodyId(bId)
    , targetFrame(tf)
    , targetPoints(std::move(tp))
    , surfaceFrame(sf)
    , surfacePoints(std::move(sp))
  {}

  int bodyId;
  sva::PTransformd targetFrame; ///< Target frame in world coordinate.
  std::vector<Eigen::Vector2d> targetPoints; ///< Target surface points in surface coordinate.
  sva::PTransformd surfaceFrame; ///< Body surface frame in body coordinate.
  std::vector<Eigen::Vector2d> surfacePoints; ///< Body surface points in surface coordinate.
};


struct EllipseContact
{
  EllipseContact() {}
  EllipseContact(int bId, double rMin,
                 const sva::PTransformd& tf, std::vector<Eigen::Vector2d> tp,
                 const sva::PTransformd& sf, std::vector<Eigen::Vector2d> sp)
    : bodyId(bId)
    , radiusMin1(rMin)
    , radiusMin2(rMin)
    , targetFrame(tf)
    , targetPoints(std::move(tp))
    , surfaceFrame(sf)
    , surfacePoints(std::move(sp))
  {
    assert( rMin > 0 && "rMin can't be negative");
  }
  EllipseContact(int bId, double rMin1, double rMin2,
                 const sva::PTransformd& tf, std::vector<Eigen::Vector2d> tp,
                 const sva::PTransformd& sf, std::vector<Eigen::Vector2d> sp)
    : bodyId(bId)
    , radiusMin1(rMin1)
    , radiusMin2(rMin2)
    , targetFrame(tf)
    , targetPoints(std::move(tp))
    , surfaceFrame(sf)
    , surfacePoints(std::move(sp))
  {
    assert( (rMin1 > 0 || rMin2 > 0) && "rMin1 and rMin2 can't be both negative");
    if (rMin2 < 0 && rMin1 >= 0)
      radiusMin2 = rMin1;
    else if (rMin1 < 0 && rMin2 >= 0)
      radiusMin1 = rMin2;
  }

  int bodyId;
  double radiusMin1;
  double radiusMin2;
  sva::PTransformd targetFrame; ///< Target frame in world coordinate.
  std::vector<Eigen::Vector2d> targetPoints; ///< Target surface points in surface coordinate.
  sva::PTransformd surfaceFrame; ///< Body surface frame in body coordinate.
  std::vector<Eigen::Vector2d> surfacePoints; ///< Body surface points in surface coordinate.
};


struct GripperContact
{
  GripperContact() {}
  GripperContact(int bId,
                const sva::PTransformd& tf, std::vector<Eigen::Vector2d> tp,
                const sva::PTransformd& sf, std::vector<Eigen::Vector2d> sp)
    : bodyId(bId)
    , targetFrame(tf)
    , targetPoints(std::move(tp))
    , surfaceFrame(sf)
    , surfacePoints(std::move(sp))
  {}

  int bodyId;
  sva::PTransformd targetFrame; ///< Target frame in world coordinate.
  std::vector<Eigen::Vector2d> targetPoints; ///< Target surface points in surface coordinate.
  sva::PTransformd surfaceFrame; ///< Body surface frame in body coordinate.
  std::vector<Eigen::Vector2d> surfacePoints; ///< Body surface points in surface coordinate.
};


struct CylindricalContact
{
  CylindricalContact() {}
  CylindricalContact(int bId, double tR, double tW,
                     const sva::PTransformd& tf, const sva::PTransformd& sf)
    : bodyId(bId)
    , targetRadius(tR)
    , targetWidth(tW)
    , targetFrame(tf)
    , surfaceFrame(sf)
  {}

  int bodyId;
  double targetRadius;
  double targetWidth;
  sva::PTransformd targetFrame; ///< Target frame in world coordinate.
  sva::PTransformd surfaceFrame; ///< Body surface frame in body coordinate.
};


struct ForceContact
{
  ForceContact() {}
  ForceContact(int bId, std::vector<sva::PTransformd> p, double m)
    : bodyId(bId)
    , points(std::move(p))
    , mu(m)
  {}

  int bodyId;
  std::vector<sva::PTransformd> points;
  double mu;
};


struct EnvCollision
{
  EnvCollision() {}
  EnvCollision(int bId, sch::S_Object* bHull, const sva::PTransformd& bT,
               sch::S_Object* eHull,
               double md)
    : bodyId(bId)
    , bodyHull(bHull)
    , bodyT(bT)
    , envHull(eHull)
    , minDist(md)
  {}

  int bodyId;
  sch::S_Object* bodyHull;
  sva::PTransformd bodyT;
  sch::S_Object* envHull;
  double minDist;
};


struct SelfCollision
{
  SelfCollision() {}
  SelfCollision(int b1Id, sch::S_Object* b1Hull, const sva::PTransformd& b1T,
                int b2Id, sch::S_Object* b2Hull, const sva::PTransformd& b2T,
                double md)
    : body1Id(b1Id)
    , body1Hull(b1Hull)
    , body1T(b1T)
    , body2Id(b2Id)
    , body2Hull(b2Hull)
    , body2T(b2T)
    , minDist(md)
  {}

  int body1Id;
  sch::S_Object* body1Hull;
  sva::PTransformd body1T;
  int body2Id;
  sch::S_Object* body2Hull;
  sva::PTransformd body2T;
  double minDist;
};


struct CoMHalfSpace
{
  CoMHalfSpace() {}
  CoMHalfSpace(std::vector<Eigen::Vector3d> O,
               std::vector<Eigen::Vector3d> n)
    : origins(std::move(O))
    , normals(std::move(n))
  {}

  std::vector<Eigen::Vector3d> origins; ///< origin of the plane
  std::vector<Eigen::Vector3d> normals; ///< normal of the plane
};


struct BodyPositionTarget
{
  BodyPositionTarget() {}
  BodyPositionTarget(int bId, const Eigen::Vector3d& t, double s)
    : bodyId(bId)
    , target(t)
    , scale(s)
  {}

  int bodyId;
  Eigen::Vector3d target;
  double scale;
};


struct BodyOrientationTarget
{
  BodyOrientationTarget() {}
  BodyOrientationTarget(int bId, const Eigen::Matrix3d& t, double s)
    : bodyId(bId)
    , target(t)
    , scale(s)
  {}

  int bodyId;
  Eigen::Matrix3d target;
  double scale;
};


struct ForceContactMinimization
{
  ForceContactMinimization() {}
  ForceContactMinimization(int bId, double s)
    : bodyId(bId)
    , scale(s)
  {}

  int bodyId;
  double scale;
};


struct TorqueContactMinimization
{
  TorqueContactMinimization() {}
  TorqueContactMinimization(int bId, const Eigen::Vector3d& o,
                            const Eigen::Vector3d& a, double s)
    : bodyId(bId)
    , origin(o)
    , axis(a)
    , scale(s)
  {}

  int bodyId;
  Eigen::Vector3d origin, axis;
  double scale;
};


struct NormalForceTarget
{
  NormalForceTarget() {}
  NormalForceTarget(int bId, double t, double s)
    : bodyId(bId)
    , target(t)
    , scale(s)
  {}

  int bodyId;
  double target;
  double scale;
};


struct TangentialForceMinimization
{
  TangentialForceMinimization() {}
  TangentialForceMinimization(int bId, double s)
    : bodyId(bId)
    , scale(s)
  {}

  int bodyId;
  double scale;
};


struct EllipseResult
{
  int bodyIndex;  //Each ellipse is defined relatively to a Surface of a Body
  double x;     //x coord of the center
  double y;     //y coord of the center
  double theta; //Angle between the x-axis and the first axis of the ellipse
  double r1;    //First radius
  double r2;    //Second radius
  std::string print()
  {
    std::stringstream result;
    result << "ellipse = Ellipse((" << this->x << ", " << this->y << "), ";
    result << 2*this->r1 << ", " << 2*this->r2 << ", " << 180*this->theta/boost::math::constants::pi<double>() << ")\n";
    return result.str();
  }
};


struct RobotConfig
{
  RobotConfig()
    : postureScale(0.)
    , torqueScale(0.)
    , forceScale(0.)
    , ellipseCostScale(0.)
  {}

  RobotConfig(rbd::MultiBody multibody)
    : mb(std::move(multibody))
    , postureScale(0.)
    , torqueScale(0.)
    , forceScale(0.)
    , ellipseCostScale(0.)
  {}

  // robot
  rbd::MultiBody mb;

  // constraints
  std::vector<FixedPositionContact> fixedPosContacts;
  std::vector<FixedOrientationContact> fixedOriContacts;
  std::vector<PlanarContact> planarContacts;
  std::vector<EllipseContact> ellipseContacts;
  std::vector<GripperContact> gripperContacts;
  std::vector<CylindricalContact> cylindricalContacts;
  std::vector<ForceContact> forceContacts;
  std::vector<EnvCollision> envCollisions;
  std::vector<SelfCollision> selfCollisions;
  std::vector<CoMHalfSpace> comHalfSpaces;
  std::vector<std::vector<double>> ql, qu;
  std::vector<std::vector<double>> tl, tu;
  std::vector<std::vector<Eigen::VectorXd>> tlPoly, tuPoly;

  // costs
  double postureScale;
  double torqueScale;
  double forceScale;
  double ellipseCostScale;
  std::vector<BodyPositionTarget> bodyPosTargets;
  std::vector<BodyOrientationTarget> bodyOriTargets;
  std::vector<ForceContactMinimization> forceContactsMin;
  std::vector<TorqueContactMinimization> torqueContactsMin;
  std::vector<NormalForceTarget> normalForceTargets;
  std::vector<TangentialForceMinimization> tanForceMin;
};


struct BodyLink
{
  BodyLink()
    : bodyId(-1)
    , body1T(sva::PTransformd::Identity())
    , body2T(sva::PTransformd::Identity())
  {}

  BodyLink(int bId, const sva::PTransformd& b1T, const sva::PTransformd& b2T)
    : bodyId(bId)
    , body1T(b1T)
    , body2T(b2T)
  {}

  int bodyId;
  sva::PTransformd body1T;
  sva::PTransformd body2T;
};


struct RobotLink
{
  RobotLink()
    : robot1Index(-1)
    , robot2Index(-1)
    , linkedBodies()
  {}

  RobotLink(int r1Index, int r2Index, std::vector<BodyLink> lbodies)
    : robot1Index(r1Index)
    , robot2Index(r2Index)
    , linkedBodies(std::move(lbodies))
  {}

  int robot1Index;
  int robot2Index;
  std::vector<BodyLink> linkedBodies;
};


struct RunConfig
{
  RunConfig(){}
  RunConfig(std::vector<std::vector<double>> iQ,
            std::vector<sva::ForceVecd> iF,
            std::vector<std::vector<double>> tQ)
    : initQ(std::move(iQ))
    , initForces(std::move(iF))
    , targetQ(std::move(tQ))
  {}

  std::vector<std::vector<double>> initQ;
  std::vector<sva::ForceVecd> initForces;
  std::vector<std::vector<double>> targetQ;
};


struct IterateQuantities
{
  double obj, constr_viol;
};

} // namespace pg
