
#pragma once

#include <cstdint>
#include <memory>

namespace hedge {

// An opaque type that hides the implementation details of how items are
// stored and created and destroyed. Ideally to allow us to use the most
// appropriate methods for the platform/runtime.
// A default implementation using standard c++14 types is available when
// you define HEDGE_ENABLE_BASIC_MESH.
struct mesh_kernel_t;
using mesh_kernel_ptr_t = std::unique_ptr<mesh_kernel_t, void(*)(mesh_kernel_t*)>;

/**
   Using a strong index type instead of a bare pointer or generic integer index
   allows you to potentially re-use cells (if the kernel implements support for
   it) and can be more easily or directly validated.
 */
enum class index_type_t : uint16_t {
  vertex, edge, face, point, attribute, unsupported
};
template<index_type_t TIndexType = index_type_t::unsupported>
struct index_t {
  //const index_type_t index_type;
  const uint32_t generation;
  const uint32_t offset;

  explicit index_t() noexcept
    : generation(0)
    , offset(0)
  {}

  explicit index_t(uint32_t _g, uint32_t _o) noexcept
    : generation(_g)
    , offset(_o)
  {}

  bool operator !=(const index_t& other) const {
    return !(*this == other);
  }
  bool operator ==(const index_t& other) const {
    return offset == other.offset && generation == other.generation;
  }

  explicit operator bool() const noexcept {
    return index_type != index_type_t::unsupported && offset != 0;
  }
};

// Discriminated types to assist in API design
struct edge_index_t : public index_t<index_type_t::edge> { using index_t::index_t; };
struct face_index_t : public index_t<index_type_t::face> { using index_t::index_t; };
struct vertex_index_t : public index_t<index_type_t::vertex> { using index_t::index_t; };
struct point_index_t : public index_t<index_type_t::point> { using index_t::index_t; };


struct edge_t {
  vertex_index_t vertex_index;
  face_index_t face_index;
  edge_index_t next_index;
  edge_index_t prev_index;
  edge_index_t adjacent_index;
};

struct face_t {
  edge_index_t edge_index;
};

struct vertex_t {
  edge_index_t edge_index;
};


/**
   Mesh can do a great deal of work on it's own as long as the kernel implements
   a couple of principle functions related to data storage.
   - add_vertex, remove_vertex, get_vertex
   - add_edge, remove_edge, get_edge
   - add_face, remove_face, get_face
   - add_attribute, remove_attribute, get_attribute
 */
class mesh_t {
  mesh_kernel_ptr_t kernel;
public:
  mesh_t();

  size_t point_count() const;
  size_t vertex_count() const;
  size_t edge_count() const;
  size_t face_count() const;
};

} // namespace hedge
