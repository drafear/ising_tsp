#include "mylib.h"
#include "problem.h"
#include "ising_solver.h"

#include <iostream>
#include <tuple>

int main() {
  using namespace std;
  Problem prob = Problem::fromIstream(cin);
  auto Jh = prob.getJhForIsing();
  Graph J;
  vector<Weight> h;
  tie(J, h) = Jh;
  IsingSolver solver(J, h);
  solver.init(IsingSolver::InitMode::Random);
  const int Steps = 1000;
  rep(t, Steps+1) {
    if (t > 0) solver.step();
    cout << "[Step " << t << "]" << endl;
    cout << "energy: " << solver.getCurrentEnergy() << endl;
    cout << "spin: " << solver.getCurrentSpin() << endl;
    cout << "active: " << solver.getActiveNodeCount() << " / " << solver.size() << endl;
    prob.getAnswerFromSpin(solver.getCurrentSpin()).output(cout);
    cout << endl;
  }
  cout << "[Answer]" << endl;
  cout << "energy: " << solver.getOptimalEnergy() << endl;
  cout << "spin: " << solver.getOptimalSpin() << endl;
  prob.getAnswerFromSpin(solver.getOptimalSpin()).output(cout);
}
