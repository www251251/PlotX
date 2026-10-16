#include "NodeTree.hpp"
#include "plotx/PlotX.hpp"
#include "plotx/core/PlotRegistry.hpp"
#include <cstddef>
#include <queue>

namespace plotx {


NodeTree::Node::Node(int x, int z) : x(x), z(z) {}
NodeTree::Node::Node(NodePair p) : x(p.first), z(p.second) {}
NodeTree::Node::Node(int x, int z, double distance) : x(x), z(z), distance(distance) {}

NodeTree::Node::operator std::pair<int, int>() const { return {x, z}; }

bool NodeTree::contains(NodePair const& p) const { return nodes.contains(p); }
bool NodeTree::contains(int x, int z) const { return contains({x, z}); }
void NodeTree::insert(NodePair const& p) { nodes.insert(p); }
void NodeTree::insert(int x, int z) { insert({x, z}); }
void NodeTree::remove(NodePair const& p) { nodes.erase(p); }
void NodeTree::remove(int x, int z) { remove({x, z}); }
void NodeTree::clear() { nodes.clear(); }

std::optional<NodeTree::Node> NodeTree::findNearestUnmarkedNodeFromRoot(NodePair root) {
    static std::vector<NodePair> directions = {
        {1,  0 },
        {-1, 0 },
        {0,  1 },
        {0,  -1}
    };

    // 优先队列初始化
    std::priority_queue<Node> pq;
    pq.push(Node{root});

    // 记录已访问的节点
    std::unordered_set<NodePair, PairHash> visited;
    visited.emplace(root);

    while (!pq.empty()) {
        Node current = pq.top();
        pq.pop();

        // 检查是否未被标记
        if (nodes.find({current.x, current.z}) == nodes.end()
            && PlotX::getInstance().getPlotRegistry()->hasPlot(current.x, current.z)) {
            return current;
        }

        // 将邻近的节点加入优先队列
        for (const auto& dir : directions) {
            int newX = current.x + dir.first;
            int newZ = current.z + dir.second;
            if (visited.find({newX, newZ}) == visited.end()) {
                double dist = std::sqrt(newX * newX + newZ * newZ);
                pq.push(Node{newX, newZ, dist});
                visited.emplace(newX, newZ);
            }
        }
    }
    return std::nullopt;
}

} // namespace plotx