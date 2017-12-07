
#pragma once

#include <cstdint>
#include <memory>

namespace hedge {

struct edge_t;
struct face_t;
struct vertex_t;
class  mesh_t;

// This is not so awesome, but I don't want people to require glm
using vec3_t = std::array<float, 3>;
using vec4_t = std::array<float, 4>;

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
    return offset != 0;
  }
};

// Discriminated types to assist in API design and reduce the potential for errors
// that can arise from using generic index types like plain integers and so on.
struct edge_index_t : public index_t<index_type_t::edge> { using index_t::index_t; };
struct face_index_t : public index_t<index_type_t::face> { using index_t::index_t; };
struct vertex_index_t : public index_t<index_type_t::vertex> { using index_t::index_t; };
struct point_index_t : public index_t<index_type_t::point> { using index_t::index_t; };


////////////////////////////////////////////////////////////////////////////////
// Our principle element structures.

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

////////////////////////////////////////////////////////////////////////////////
// Function sets proxy the mesh and elements and provide an easy access api

/**
   We have this simple templated base class which allows functions that request
   an alement to return an "empty" proxy. An alternative to this would be to
   use std::optional but it seems like this would be sufficient for now.
 */
template<typename TIndexType>
class element_fn_t {
  mesh_t* _mesh;
  TIndexType _index;
public:
  explicit element_fn_t(mesh_t* mesh, TIndexType index)
    : _mesh(mesh), _index(index)
  {}
  explicit operator bool() const noexcept {
    return _mesh != nullptr && (bool)_index;
  }
};

class edge_fn_t : public element_fn_t<edge_index_t> {
public:
  using element_fn_t::element_fn_t;
};

class face_fn_t : public element_fn_t<face_index_t> {
public:
  using element_fn_t::element_fn_t;
};

class vertex_fn_t : public element_fn_t<vertex_index_t> {
public:
  using element_fn_t::element_fn_t;
};

////////////////////////////////////////////////////////////////////////////////


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

  size_t edge_count() const;
  size_t face_count() const;
  size_t vertex_count() const;
  size_t point_count() const;

  edge_fn_t   get(edge_index_t   index) const;
  face_fn_t   get(face_index_t   index) const;
  vertex_fn_t get(vertex_index_t index) const;
  vec3_t      get(point_index_t  index) const;
};

} // namespace hedge
