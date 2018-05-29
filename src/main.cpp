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
  int step = 0, zeroStep = 0;
  while (zeroStep < 10) {
    if (step > 0) solver.step();
    cout << "[Step " << step << "]" << endl;
    cout << "energy: " << solver.getCurrentEnergy() << endl;
    cout << "spin: " << solver.getCurrentSpin() << endl;
    cout << "flip: " << solver.getActiveNodeCount() << " / " << solver.size() << endl;
    prob.getAnswerFromSpin(solver.getCurrentSpin()).output(cout);
    cout << endl;
    if (solver.getActiveNodeCount() == 0) ++zeroStep;
    ++step;
  }
  cout << "[Answer]" << endl;
  cout << "energy: " << solver.getOptimalEnergy() << endl;
  cout << "spin: " << solver.getOptimalSpin() << endl;
  prob.getAnswerFromSpin(solver.getOptimalSpin()).output(cout);
}
