#include <algorithm>
#include <iostream>
#include <limits>
#include <numeric>
#include <stdexcept>
#include <vector>

// BGL include
#include <boost/graph/adjacency_list.hpp>

// BGL flow include *NEW*
#include <boost/graph/push_relabel_max_flow.hpp>

// Graph Type with nested interior edge properties for flow algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS>
    traits;
typedef boost::adjacency_list<
    boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
                    boost::property<boost::edge_residual_capacity_t, long,
                                    boost::property<boost::edge_reverse_t,
                                                    traits::edge_descriptor>>>>
    graph;

typedef traits::vertex_descriptor vertex_desc;
typedef traits::edge_descriptor edge_desc;

// Custom edge adder class, highly recommended
class edge_adder {
  graph &G;

 public:
  explicit edge_adder(graph &G) : G(G) {}

  void add_edge(int from, int to, long capacity) {
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    const auto e = boost::add_edge(from, to, G).first;
    const auto rev_e = boost::add_edge(to, from, G).first;
    c_map[e] = capacity;
    c_map[rev_e] = 0;  // reverse edge has no capacity!
    r_map[e] = rev_e;
    r_map[rev_e] = e;
  }
};

#include <boost/graph/graphviz.hpp>
using namespace std;
void testcase() {
  int n, m, k, l;
  cin >> n >> m >> k >> l;
  graph G(2 * n + 2);  // double graph

  int sink = boost::num_vertices(G) - 1;
  int source = sink - 1;
  edge_adder adder(G);

  // Add edge between policestation to sink
  for (int i = 0; i < k; i++) {
    int pStation;
    cin >> pStation;
    adder.add_edge(source, pStation, 1);
    adder.add_edge(n + pStation, sink, 1);
  }

  for (int i = 0; i < l; i++) {
    int photoLoc;
    cin >> photoLoc;
    adder.add_edge(photoLoc, n + photoLoc, 1);
  }
  // Add other intersections with cap 1
  for (int i = 0; i < m; i++) {
    int x, y;
    cin >> x >> y;
    adder.add_edge(x, y, INT_MAX);
    adder.add_edge(n + x, n + y, 1);
  }
  long flow = boost::push_relabel_max_flow(G, source, sink);
  std::cout << flow << endl;
}
int main(int argc, char const *argv[]) {
  ios_base::sync_with_stdio(false);
  int t = 0;
  cin >> t;
  for (int i = 0; i < t; i++) {
    testcase();
  }
}
