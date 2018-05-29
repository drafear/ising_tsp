#include "mylib.h"
#include "ising_solver.h"
#include <vector>
#include <random>
#include <cassert>

using namespace std;

Edge::Edge(int to, Weight weight) : to(to), weight(weight) {}

IsingSolver::IsingSolver(const Graph& J, const vector<Weight>& h) : J(J), h(h) {}

void IsingSolver::init(const IsingSolver::InitMode mode, const int seed) {
  rnd.seed(seed);
  // rnd = random_device();
  switch (mode) {
    case Negative:
      current_spin.assign(size(), -1);
      break;
    case Positive:
      current_spin.assign(size(), 1);
      break;
    case Random:
      current_spin.resize(size());
      for (auto&& s : current_spin) {
        s = rnd() % 2 ? 1 : -1;
      }
      break;
  }
  optimal_spin = current_spin;
}
void IsingSolver::init(const IsingSolver::InitMode mode) {
  random_device rd;
  init(mode, rd());
}
void IsingSolver::step() {
  if (current_spin.size() != size()) {
    throw new runtime_error("call init method first");
  }
}
size_t IsingSolver::size() const {
  return h.size();
}
Weight IsingSolver::calcEnergyCoe(const vector<int>& spin, const int node_id) const {
  Weight res = h[node_id];
  for (auto&& e : J[node_id]) {
    res += spin[e.to] * e.weight;
  }
  return res;
}
Weight IsingSolver::calcEnergy(const vector<int>& spin) const {
  assert(spin.size() == size());
  Weight res{};
  rep(i, size()) res += spin[i] * calcEnergyCoe(spin, i);
  return res;
}
Weight IsingSolver::getCurrentEnergy() const {
  return calcEnergy(current_spin);
}
Weight IsingSolver::getOptimalEnergy() const {
  return calcEnergy(optimal_spin);
}
const std::vector<int>& IsingSolver::getCurrentSpin() const {
  return current_spin;
}
const std::vector<int>& IsingSolver::getOptimalSpin() const {
  return optimal_spin;
}
