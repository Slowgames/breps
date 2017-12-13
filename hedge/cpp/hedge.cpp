
#include "hedge.hpp"

#include <array>
#include <vector>

#include <easylogging++.h>


namespace hedge {

/**
   Rather than create a bunch of preprocessor macros to prevent copypasta I decided
   to create a simple templated wrapper over std::vector which implements the
   requirements for element storage.
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
    if (element != nullptr) {
      if (element->generation != index.generation) {
        LOG(WARNING) << "Generation mismatch for element: " << index.offset << ", " << index.generation;
        LOG(DEBUG) << "Offset: " << index.offset
                   << ", Generation " << index.generation << " != " << element->generation;
        element = nullptr;
      }
    }
    return element;
  }

  TElement* get(offset_t offset) const {
    TElement* element = nullptr;
    if (offset < collection.size()) {
      element = (TElement*)collection.data() + offset;
    }
    else {
      LOG(ERROR) << "Offset requested exceeded element current storage size: "
                 << offset << " > " << collection.size();
    }
    return element;
  }

  TElementIndex emplace(TElement&& element) {
    TElementIndex index(collection.size(), element.generation);
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

///////////////////////////////////////////////////////////////////////////////////////

class basic_kernel_t : public kernel_t {
  element_vector_t<vertex_t, vertex_index_t> vertices;
  element_vector_t<face_t, face_index_t>     faces;
  element_vector_t<edge_t, edge_index_t>     edges;
  element_vector_t<point_t, point_index_t>   points;

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
    return points.get(index);
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
    return points.emplace(std::move(point));
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

  void resolve(edge_index_t* index, edge_t** edge) const override {
    *edge = edges.get(index->offset);
    index->generation = (*edge)->generation;
  }
  void resolve(face_index_t* index, face_t** face) const override {
    *face = faces.get(index->offset);
    index->generation = (*face)->generation;
  }
  void resolve(point_index_t* index, point_t** point) const override {
    *point = points.get(index->offset);
    index->generation = (*point)->generation;
  }
  void resolve(vertex_index_t* index, vertex_t** vert) const override {
    *vert = vertices.get(index->offset);
    index->generation = (*vert)->generation;
  }
};

// basic_kernel_t
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////

mesh_t::mesh_t()
  : kernel(new basic_kernel_t, [](kernel_t* k) { delete k; })
{}

mesh_t::mesh_t(kernel_t::ptr_t&& _kernel)
  : kernel(std::move(kernel))
{}

size_t mesh_t::point_count() const {
  return kernel->point_count() - 1;
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

edge_fn_t mesh_t::edge(edge_index_t index) const {
  return edge_fn_t(kernel.get(), index);
}

face_fn_t mesh_t::face(face_index_t index) const {
  return face_fn_t(kernel.get(), index);
}

vertex_fn_t mesh_t::vertex(vertex_index_t index) const {
  return vertex_fn_t(kernel.get(), index);
}

point_t* mesh_t::point(offset_t offset) const {
  point_index_t index(offset);
  point_t* p;
  kernel->resolve(&index, &p);
  return p;
}
point_t* mesh_t::point(point_index_t pindex) const {
  return kernel->get(pindex);
}
point_t* mesh_t::point(vertex_index_t vindex) const {
  auto* vert = kernel->get(vindex);
  return point(vert->point_index);
}

std::pair<point_t*, point_t*> mesh_t::points(edge_index_t eindex) const {
  auto e = edge(eindex);
  auto* p0 = e.vertex().point();
  auto* p1 = e.next().vertex().point();
  return std::make_pair(p0, p1);
}

// mesh_t
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////

element_t::element_t()
  : generation(0)
  , flags(0)
{}

point_t::point_t(float x, float y, float z)
  : element_t()
  , position(x, y, z)
{}

point_t::point_t()
  : element_t()
  , position(0.f)
{}

///////////////////////////////////////////////////////////////////////////////

#define FN_GETTER(A, B)   \
  auto *elem = element(); \
  if (elem != nullptr)    \
  {                       \
    return A;             \
  }                       \
  else                    \
  {                       \
    return B;             \
  }

#define MAKE_VERT_FN(INDEX) \
  FN_GETTER(vertex_fn_t(_kernel, INDEX), vertex_fn_t(_kernel, vertex_index_t()))

#define MAKE_FACE_FN(INDEX) \
  FN_GETTER(face_fn_t(_kernel, INDEX), face_fn_t(_kernel, face_index_t()))

#define MAKE_EDGE_FN(INDEX) \
  FN_GETTER(edge_fn_t(_kernel, INDEX), edge_fn_t(_kernel, edge_index_t()))

///////////////////////////////////////////////////////////////////////////////

vertex_fn_t edge_fn_t::vertex() {
  MAKE_VERT_FN(elem->vertex_index)
}

face_fn_t edge_fn_t::face() {
  MAKE_FACE_FN(elem->face_index)
}

edge_fn_t edge_fn_t::next() {
  MAKE_EDGE_FN(elem->next_index)
}

edge_fn_t edge_fn_t::prev() {
  MAKE_EDGE_FN(elem->prev_index)
}

edge_fn_t edge_fn_t::adjacent() {
  MAKE_EDGE_FN(elem->adjacent_index)
}

bool edge_fn_t::is_boundary() const {
  auto* edge = element();
  if (edge != nullptr) {
    if (edge->adjacent_index) {
      return false;
    }
  }
  return true;
}

// edge_fn_t
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////

edge_fn_t vertex_fn_t::edge() {
  MAKE_EDGE_FN(elem->edge_index)
}

point_t* vertex_fn_t::point() {
  auto* vert = element();
  return _kernel->get(vert->point_index);
}

// vertex_fn_t
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////

edge_fn_t face_fn_t::edge() {
  MAKE_EDGE_FN(elem->edge_index)
}

// face_fn_t
///////////////////////////////////////////////////////////////////////////////

} // namespace hedge
