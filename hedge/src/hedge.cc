
#include "hedge.hpp"

#include <vector>
#include <glm/glm.hpp>
#include <easylogging++.h>

namespace hedge {

#ifdef HEDGE_ENABLE_DEFAULT_KERNEL
class basic_kernel_t : public kernel_t {
  std::vector<glm::vec3> points;
  std::vector<vertex_t> vertices;
  std::vector<face_t> faces;
  std::vector<edge_t> edges;
public:
  basic_kernel_t() {
    points.emplace_back(glm::vec3(0.0f, 0.0f, 0.0f));
    vertices.emplace_back( vertex_t {} );
    faces.emplace_back( face_t {} );
    edges.emplace_back( edge_t {} );
  }

  edge_t* get(edge_index_t index) override {
    edge_t* edge = nullptr;
    if (index && index.offset < edges.size()) {
      edge = edges.data() + index.offset;
      if (edge->generation != index.generation) {
        edge = nullptr;
      }
    }
    return edge;
  }

  edge_index_t create(edge_t** edge) override {
    edge_index_t index(edges.size(), 0);
    edges.emplace_back( edge_t {} );
    (*edge) = edges.data() + index.offset;
    return index;
  }

  /**
     Removes an element by swapping with the end. Increments the element generation.
   */
  void remove(edge_index_t index) override {
    auto* edge_at_index = get(index);
    if (edge_at_index != nullptr) {
      auto edge_at_back = edges.back();
      *edge_at_index = edge_at_back;
      edge_at_index->generation++;
      edges.pop_back();
    }
  }

  size_t point_count() const override {
    return points.size();
  }

  size_t vertex_count() const override {
    return vertices.size();
  }

  size_t face_count() const override {
    return faces.size();
  }

  size_t edge_count() const override {
    return edges.size();
  }
};

mesh_t::mesh_t()
  : _kernel(new basic_kernel_t, [](kernel_t* k) { delete k; })
{}
#endif // HEDGE_ENABLE_BASIC_MESH

mesh_t::mesh_t(kernel_t::ptr_t&& kernel)
  : _kernel(std::move(kernel))
{}

kernel_t::ptr_t const& mesh_t::kernel() const {
  return _kernel;
}


} // namespace hedge
