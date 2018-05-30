#include "mylib.h"
#include "problem.h"
#include "ising_solver.h"
#include "lib/cmdline.h"

#include <iostream>
#include <fstream>
#include <tuple>

using namespace std;

void run(const string& input_file_path, double cool_coe, double update_ratio) {
  ifstream ifs(input_file_path);
  if (ifs.fail()) {
    cerr << "can't open the file: " << input_file_path << endl;
    exit(1);
  }
  Problem prob = Problem::fromIstream(ifs);
  auto Jh = prob.getJhForIsing();
  Graph J;
  vector<Weight> h;
  tie(J, h) = Jh;
  IsingSolver solver(J, h);
  solver.init(IsingSolver::InitMode::Random, cool_coe, update_ratio);
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
cmdline::parser get_command_line_parser() {
  cmdline::parser parser;
  parser.add<double>("cool", 'c', "coefficient of cooling", false, 0.999);
  parser.add<double>("update-ratio", 'u', "the ratio of nodes to update in 1 step", false, 0.3);
  parser.footer("filename");
  return parser;
}
int main(int argc, char *argv[]) {
  auto parser = get_command_line_parser();
  parser.parse_check(argc, argv);
  if (parser.rest().size() < 1) {
    cerr << parser.usage();
    exit(1);
  }
  run(parser.rest()[0], parser.get<double>("cool"), parser.get<double>("update-ratio"));
}
