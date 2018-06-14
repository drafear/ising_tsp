#include "mylib.h"
#include "problem.h"
#include "ising_solver.h"
#include "cost_function.h"
#include <cassert>
#include <complex>

using namespace std;

const Weight Base = 1e3;

#define NODE(step, v) (n * (step) + (v))

vector<complex<double>> points;
Problem::Problem(vector<complex<double>>&& points) : points(points) {}
size_t Problem::size() const {
  return points.size();
}
Problem Problem::fromIstream(std::istream& is) {
  int n; is >> n;
  vector<complex<double>> points;
  points.reserve(n);
  rep(i, n) {
    double x, y; is >> x >> y;
    points.emplace_back(x, y);
  }
  return Problem(move(points));
}
CostFunction Problem::getCostFunction() const {
  const int n = size();
  Graph J1(n * n), J2(n * n);
  vector<Weight> h1(n * n, 0), h2(n * n, 0);
  rep(i, n) rep(j, n) rep(step, n) {
    Weight dist = abs(points[i] - points[j]) * Base;
    J1[NODE(step, i)].emplace_back(
      NODE((step+1)%n, j), dist
    );
  }
  rep(i, n) rep(j, n) rep(k, n) {
    Weight cost = Base;
    J2[NODE(i, j)].emplace_back(
      NODE(i, k), cost
    );
    J2[NODE(i, j)].emplace_back(
      NODE(k, j), cost
    );
  }
  rep(i, h2.size()) h2[i] -= 4 * Base;
  // return CostFunction(J2, h2).to01();
  return CostFunction(J1, h1, J2, h2, 1, 1e5).to01();
}
Answer Problem::getAnswerFromSpin(const vector<int>& spin) const {
  const int n = size();
  vector<int> order;
  rep(step, n) rep(v, n) {
    assert(NODE(step, v) >= 0);
    assert(NODE(step, v) < int(spin.size()));
    if (spin[NODE(step, v)] > 0) {
      order.push_back(v);
      break;
    }
  }
  return Answer(*this, move(order));
}
bool Answer::verify() const {
  if (order.size() != prob.size()) return false;
  rep(i, order.size()) rep(j, i) {
    if (order[i] == order[j]) {
      return false;
    }
  }
  return true;
}
Answer::Answer(const Problem& prob, const vector<int>& order) : prob(prob), order(order) {}
void Answer::output(ostream& os, bool is_detail) const {
  if (is_detail) os << "order: ";
  double sum = 0;
  if (order.size() > 0) {
    rep(i, order.size()+1) {
      if (i && is_detail) os << " -> ";
      assert(order[i % order.size()] >= 0);
      assert(order[i % order.size()] < int(prob.size()));
      auto point = prob.points[order[i % order.size()]];
      if (is_detail) os << "(" << point.real() << ", " << point.imag() << ")";
    }
    rep(i, order.size()) {
      sum += abs(prob.points[order[(i+1) % order.size()]] - prob.points[order[i]]);
    }
  }
  if (is_detail) os << endl;
  os << "total distance: " << sum << endl;
}
