
#include "hedge.hpp"

#include <array>
#include <vector>

#include <easylogging++.h>


namespace hedge {

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
    TElement* element = get(index.offset);
    if (element != nullptr && element->generation != index.generation) {
      element = nullptr;
    }
    return element;
  }

  TElement* get(offset_t offset) const {
    TElement* element = nullptr;
    if (offset < collection.size()) {
      element = (TElement*)collection.data() + offset;
    }
    return element;
  }

  TElementIndex emplace(TElement&& element) {
    TElementIndex index(collection.size(), 0);
    collection.emplace_back(std::move(element));
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
  element_vector_t<vertex_t, vertex_index_t> vertices;
  element_vector_t<face_t, face_index_t>     faces;
  element_vector_t<edge_t, edge_index_t>     edges;
  std::vector<point_t>                       points;
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
  point_t* get(point_index_t index) override {
    point_t* result = nullptr;
    if (index.offset < points.size()) {
      result = points.data() + index.offset;
    }
    return result;
  }

  edge_index_t emplace(edge_t&& edge) override {
    return edges.emplace(std::move(edge));
  }
  face_index_t emplace(face_t&& face) override {
    return faces.emplace(std::move(face));
  }
  vertex_index_t emplace(vertex_t&& vertex) override {
    return vertices.emplace(std::move(vertex));
  }
  point_index_t emplace(point_t&& point) override {
    point_index_t index(points.size(), 0);
    points.emplace_back(std::move(point));
    return index;
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
    auto* point_at_index = get(index);
    auto point_at_back = points.back();
    (*point_at_index) = point_at_back;
    points.pop_back();
  }

  size_t point_count() const override {
    return points.size();
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
  : kernel(new basic_kernel_t, [](kernel_t* k) { delete k; })
{}

mesh_t::mesh_t(kernel_t::ptr_t&& _kernel)
  : kernel(std::move(kernel))
{}

size_t mesh_t::point_count() const {
  return kernel->point_count();
}
size_t mesh_t::vertex_count() const {
  return kernel->vertex_count() - 1;
}
size_t mesh_t::edge_count() const {
  return kernel->edge_count() - 1;
}
size_t mesh_t::face_count() const {
  return kernel->face_count() - 1;
}


} // namespace hedge
