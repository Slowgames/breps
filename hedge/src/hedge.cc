
#include "hedge.hpp"

#include <array>
#include <vector>

#ifdef HEDGE_ENABLE_DEFAULT_KERNEL
#include <glm/glm.hpp>
#include <easylogging++.h>
#endif // HEDGE_ENABLE_DEFAULT_KERNEL


namespace hedge {

#ifdef HEDGE_ENABLE_DEFAULT_KERNEL

/**
   Rather than create a bunch of preprocessor macros to prevent copypasta I decided
   to create a simple templated wrapper over std::vector.
 */
template<typename TElement, typename TElementIndex>
class element_vector_t {
  std::vector<TElement> collection;
public:
  element_vector_t() {
    collection.emplace_back( TElement {} );
  }

  void reserve(size_t elements) {
    collection.reserve(elements);
  }

  size_t count() const {
    return collection.size();
  }

  TElement* get(TElementIndex index) const {
    TElement* element = nullptr;
    if (index && index.offset < collection.size()) {
      element = (TElement*)collection.data() + index.offset;
      if (element->generation == index.generation) {
        element = nullptr;
      }
    }
    return element;
  }

  TElementIndex create(TElement** element) {
    TElementIndex index(collection.size(), 0);
    collection.emplace_back(TElement {});
    (*element) = collection.data() + index.offset;
    return index;
  }

  /**
     Removes an element by swapping with the end. Increments the element generation.
   */
  void remove(TElementIndex index) { // TODO: Return status to indicate whether it was removed?
    auto* element_at_index = get(index);
    if (element_at_index != nullptr) {
      if (index.offset == 1) {
        collection.pop_back();
      }
      else {
        auto element_at_back = collection.back();
        *element_at_index = element_at_back;
        element_at_index->generation++;
        collection.pop_back();
      }
    }
  }
};

class basic_kernel_t : public kernel_t {
  element_vector_t<vec3_t, point_index_t>    points;
  element_vector_t<vertex_t, vertex_index_t> vertices;
  element_vector_t<face_t, face_index_t>     faces;
  element_vector_t<edge_t, edge_index_t>     edges;
public:
  edge_t* get(edge_index_t index) override {
    return edges.get(index);
  }
  face_t* get(face_index_t index) override {
    return faces.get(index);
  }
  vertex_t* get(vertex_index_t index) override {
    return vertices.get(index);
  }
  vec3_t* get(point_index_t index) override {
    return points.get(index);
  }

  edge_index_t create(edge_t** edge) override {
    return edges.create(edge);
  }
  face_index_t create(face_t** face) override {
    return faces.create(face);
  }
  vertex_index_t create(vertex_t** vertex) override {
    return vertices.create(vertex);
  }
  point_index_t create(vec3_t** point) override {
    return points.create(point);
  }

  void remove(edge_index_t index) override {
    return edges.remove(index);
  }
  void remove(face_index_t index) override {
    return faces.remove(index);
  }
  void remove(vertex_index_t index) override {
    return vertices.remove(index);
  }
  void remove(point_index_t index) override {
    return points.remove(index);
  }

  size_t point_count() const override {
    return points.count();
  }

  size_t vertex_count() const override {
    return vertices.count();
  }

  size_t face_count() const override {
    return faces.count();
  }

  size_t edge_count() const override {
    return edges.count();
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
