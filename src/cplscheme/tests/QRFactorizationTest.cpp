#ifndef PRECICE_NO_MPI
#include <mpi.h>
#endif
#include <Eigen/Core>
#include "cplscheme/impl/BaseQNPostProcessing.hpp"
#include "cplscheme/impl/QRFactorization.hpp"
#include "testing/Testing.hpp"

BOOST_AUTO_TEST_SUITE(CplSchemeTests)

using namespace precice;
using namespace cplscheme;

void testQRequalsA(
    Eigen::MatrixXd &Q,
    Eigen::MatrixXd &R,
    Eigen::MatrixXd &A)
{
  Eigen::MatrixXd A_prime = Q * R;

  for (int i = 0; i < A.rows(); i++) {
    for (int j = 0; j < A.cols(); j++) {
      BOOST_TEST(testing::equals(A_prime(i, j), A(i, j)));
    }
  }
}

void testQTQequalsIdentity(Eigen::MatrixXd &Q)
{
  Eigen::MatrixXd QTQ = Q.transpose() * Q;

  // test if Q^TQ equals identity
  for (int i = 0; i < QTQ.rows(); i++) {
    for (int j = 0; j < QTQ.cols(); j++) {
      if (i == j) {
        BOOST_TEST(testing::equals(QTQ(i, j), 1.0, 1e-12));
      } else {
        BOOST_TEST(testing::equals(QTQ(i, j), 0.0, 1e-12));
      }
    }
  }
}

BOOST_AUTO_TEST_CASE(testQRFactorization)
{
  int             m = 6, n = 8;
  int             filter = impl::BaseQNPostProcessing::QR1FILTER;
  Eigen::MatrixXd A(n, m);

  // Set values according to Hilbert matrix.
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      double entry = 1.0 / static_cast<double>(i + j + 1);
      A(i, j)      = entry;
    }
  }

  // compute QR factorization of A via successive inserting of columns
  impl::QRFactorization qr_1(A, filter);

  // test if Q^TQ equals identity
  testQTQequalsIdentity(qr_1.matrixQ());
  // test if QR equals A
  testQRequalsA(qr_1.matrixQ(), qr_1.matrixR(), A);

  /**
   * *************** deleting/adding Columns ************************
   */

  // ----------- delete last column ---------------
  Eigen::MatrixXd A_prime1 = A;
  Eigen::VectorXd col6     = A.col(m - 1);
  A_prime1.conservativeResize(n, m - 1);
  qr_1.deleteColumn(m - 1);

  // test if Q^TQ equals identity
  testQTQequalsIdentity(qr_1.matrixQ());
  // test if QR equals A
  testQRequalsA(qr_1.matrixQ(), qr_1.matrixR(), A_prime1);

  // ----------- delete first column ---------------
  Eigen::MatrixXd A_prime2 = A_prime1;
  Eigen::VectorXd col1     = A.col(0);
  for (int i = 0; i < A_prime2.rows(); i++)
    for (int j = 0; j < A_prime2.cols() - 1; j++)
      A_prime2(i, j) = A_prime2(i, j + 1);
  A_prime2.conservativeResize(n, A_prime2.cols() - 1);
  qr_1.deleteColumn(0);

  // test if Q^TQ equals identity
  testQTQequalsIdentity(qr_1.matrixQ());
  // test if QR equals A
  testQRequalsA(qr_1.matrixQ(), qr_1.matrixR(), A_prime2);

  // ----------- add first column -----------------
  qr_1.insertColumn(0, col1);

  // test if Q^TQ equals identity
  testQTQequalsIdentity(qr_1.matrixQ());
  // test if QR equals A
  testQRequalsA(qr_1.matrixQ(), qr_1.matrixR(), A_prime1);

  // ----------- add last column -----------------
  qr_1.insertColumn(qr_1.cols(), col6); // ??
  // test if Q^TQ equals identity
  testQTQequalsIdentity(qr_1.matrixQ());
  // test if QR equals A
  testQRequalsA(qr_1.matrixQ(), qr_1.matrixR(), A);

  // ----------- delete middle column ---------------
  int             k        = 3;
  Eigen::MatrixXd A_prime3 = A;
  Eigen::VectorXd colk     = A.col(k);
  for (int i = 0; i < A_prime3.rows(); i++)
    for (int j = k; j < A_prime3.cols() - 1; j++)
      A_prime3(i, j) = A_prime3(i, j + 1);
  A_prime3.conservativeResize(n, A_prime3.cols() - 1);
  qr_1.deleteColumn(k);

  // test if Q^TQ equals identity
  testQTQequalsIdentity(qr_1.matrixQ());
  // test if QR equals A
  testQRequalsA(qr_1.matrixQ(), qr_1.matrixR(), A_prime3);

  // ----------- add middle column -----------------
  qr_1.insertColumn(k, colk);

  // test if Q^TQ equals identity
  testQTQequalsIdentity(qr_1.matrixQ());
  // test if QR equals A
  testQRequalsA(qr_1.matrixQ(), qr_1.matrixR(), A);

  // ------------ reset ----------------------------
  qr_1.reset();
  qr_1.reset(A, A.rows());
  testQTQequalsIdentity(qr_1.matrixQ());
  // test if QR equals A
  testQRequalsA(qr_1.matrixQ(), qr_1.matrixR(), A);

  Eigen::MatrixXd q = qr_1.matrixQ();
  Eigen::MatrixXd r = qr_1.matrixR();
  qr_1.reset();
  qr_1.reset(q, r, q.rows(), q.cols());
  testQTQequalsIdentity(qr_1.matrixQ());
  // test if QR equals A
  testQRequalsA(qr_1.matrixQ(), qr_1.matrixR(), A);
}

BOOST_AUTO_TEST_SUITE_END()
