
#include "hedge.hpp"

#include <vector>
#include <glm/glm.hpp>
#include <easylogging++.h>

namespace hedge {

////////////////////////////////////////////////////////////////////////////////

static const edge_index_t default_edge_index;
static const face_index_t default_face_index;
static const vertex_index_t default_vertex_index;
static const point_index_t default_point_index;

// index_t::index_t() noexcept
//   : offset(0)
//   , generation(0)
// {}

// index_t::index_t(uint32_t _offset, uint32_t _generation) noexcept
//   : offset(_offset)
//   , generation(_generation)
// {}

// bool index_t::operator !=(const index_t& b) const {
//   return offset != b.offset || generation != b.generation;
// }

// bool index_t::operator ==(const index_t& b) const {
//   return offset == b.offset && generation == b.generation;
// }

// index_t::operator bool() const noexcept {
//   return offset != 0;
// }

////////////////////////////////////////////////////////////////////////////////

#ifdef HEDGE_ENABLE_DEFAULT_KERNEL
#include "default_kernel.cc"
#endif // HEDGE_ENABLE_BASIC_MESH

mesh_t::mesh_t()
  : kernel(new mesh_kernel_t, [](mesh_kernel_t* k) { delete k; })
{}

size_t mesh_t::vertex_count() const {
  return kernel->vertex_count();
}

size_t mesh_t::edge_count() const {
  return kernel->edge_count();
}

size_t mesh_t::face_count() const {
  return kernel->face_count();
}

size_t mesh_t::point_count() const {
  return kernel->point_count();
}


} // namespace hedge
