
#include "hedge.hpp"

#include <vector>
#include <glm/glm.hpp>
#include <easylogging++.h>

namespace hedge {

#ifdef HEDGE_ENABLE_DEFAULT_KERNEL
#include "default_kernel.cc"
#endif // HEDGE_ENABLE_BASIC_MESH

mesh_t::mesh_t()
  : kernel(new mesh_kernel_t, [](mesh_kernel_t* k) { delete k; })
{}

size_t mesh_t::edge_count() const {
  return kernel->edge_count();
}

size_t mesh_t::face_count() const {
  return kernel->face_count();
}

size_t mesh_t::vertex_count() const {
  return kernel->vertex_count();
}

size_t mesh_t::point_count() const {
  return kernel->point_count();
}


} // namespace hedge
