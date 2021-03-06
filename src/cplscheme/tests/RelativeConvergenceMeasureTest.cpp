#include "../impl/RelativeConvergenceMeasure.hpp"
#include "testing/Testing.hpp"

BOOST_AUTO_TEST_SUITE(CplSchemeTests)

BOOST_AUTO_TEST_CASE(RelativeConvergenceMeasureTest)
{
  using Eigen::Vector3d;
  double                                               convergenceLimit = 0.1; // 10%
  precice::cplscheme::impl::RelativeConvergenceMeasure measure(convergenceLimit);

  // Create data sets for old state of data and new state of data
  Vector3d oldValues0(1, 1, 1);
  Vector3d oldValues1(2, 2, 2);
  Vector3d oldValues2(2.9, 2.9, 2.9);
  Vector3d newValues(3, 3, 3);
  Vector3d designSpec(0, 0, 0);

  measure.measure(oldValues0, newValues, designSpec);
  BOOST_TEST(not measure.isConvergence());

  measure.measure(oldValues1, newValues, designSpec);
  BOOST_TEST(not measure.isConvergence());

  measure.measure(oldValues2, newValues, designSpec);
  BOOST_TEST(measure.isConvergence());
}

BOOST_AUTO_TEST_SUITE_END()
