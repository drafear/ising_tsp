#include "mylib.h"
#include "problem.h"
#include "ising_solver.h"
#include "lib/cmdline.h"

#include <iostream>
#include <fstream>
#include <tuple>

using namespace std;

void run(const cmdline::parser& parser) {
  const string& input_file_path = parser.rest()[0];
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
  solver.init(IsingSolver::InitMode::Random, parser.get<double>("cool"), parser.get<double>("update-ratio"));
  bool is_detail = parser.exist("detail");
  int step = 0, zeroStep = 0;
  while (zeroStep < 10) {
    if (step > 0) solver.step();
    cout << "[Step " << step << "]" << endl;
    cout << "energy: " << solver.getCurrentEnergy() << endl;
    if (is_detail) cout << "spin: " << solver.getCurrentSpin() << endl;
    cout << "flip: " << solver.getActiveNodeCount() << " / " << solver.size() << endl;
    prob.getAnswerFromSpin(solver.getCurrentSpin()).output(cout, is_detail);
    cout << endl;
    if (solver.getActiveNodeCount() == 0) ++zeroStep;
    ++step;
  }
  cout << "[Answer]" << endl;
  cout << "energy: " << solver.getOptimalEnergy() << endl;
  if (is_detail) cout << "spin: " << solver.getOptimalSpin() << endl;
  Answer ans = prob.getAnswerFromSpin(solver.getOptimalSpin());
  ans.output(cout, true);
  cout << "is_answer: " << boolalpha << ans.verify() << endl;
}
cmdline::parser get_command_line_parser() {
  cmdline::parser parser;
  parser.add<double>("cool", 'c', "coefficient of cooling", false, 0.999);
  parser.add<double>("update-ratio", 'u', "the ratio of nodes to update in 1 step", false, 0.3);
  parser.add("detail", 'd', "print log in detail");
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
  run(parser);
}
