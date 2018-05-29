#include "mylib.h"
#include "problem.h"
#include "ising_solver.h"
#include <cassert>
#include <complex>

using namespace std;

pair<Graph, vector<Weight>> ConvertTo01(const Graph& J, const vector<Weight>& h) {
  assert(J.size() == h.size());
  const int V = h.size();
  vector<Weight> nh(V, 0);
  rep(i, V) nh[i] += 2 * h[i];
  rep(i, V) each(e, J[i]) {
    nh[i] -= e.weight;
    nh[e.to] -= e.weight;
  }
  return {J, move(nh)};
}

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
std::pair<Graph, vector<Weight>> Problem::getJhForIsing() const {
  const int n = size();
  Graph J(n * n);
  vector<Weight> h(n * n, 0);
  const Weight B = 1;
  rep(i, n) rep(j, n) rep(step, n) {
    Weight dist = abs(points[i] - points[j]) * Base * B;
    J[NODE(step, i)].emplace_back(
      NODE((step+1)%n, j), dist
    );
  }
  const Weight A = 15;
  rep(i, n) rep(j, n) rep(k, n) {
    Weight cost = Base * A;
    J[NODE(i, j)].emplace_back(
      NODE(i, k), cost
    );
    J[NODE(i, j)].emplace_back(
      NODE(k, j), cost
    );
  }
  rep(i, h.size()) h[i] += 4 * Base * A;
  return ConvertTo01(J, h);
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

Answer::Answer(const Problem& prob, const vector<int>& order) : prob(prob), order(order) {}
void Answer::output(ostream& os) const {
  os << "order: ";
  double sum = 0;
  if (order.size() > 0) {
    rep(i, order.size()+1) {
      if (i) os << " -> ";
      assert(order[i % order.size()] >= 0);
      assert(order[i % order.size()] < int(prob.size()));
      auto point = prob.points[order[i % order.size()]];
      os << "(" << point.real() << ", " << point.imag() << ")";
    }
    os << endl;
    rep(i, order.size()) {
      sum += abs(prob.points[order[(i+1) % order.size()]] - prob.points[order[i]]);
    }
  }
  os << "total distance: " << sum << endl;
}
