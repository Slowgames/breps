/**
   A mesh kernel needs to provide the fundamental data operations and ideally nothing else.
 */


struct mesh_kernel_t {
  std::vector<glm::vec3> points;
  std::vector<vertex_t> vertices;
  std::vector<face_t> faces;
  std::vector<edge_t> edges;

  mesh_kernel_t() {
    points.emplace_back(glm::vec3(0.0f, 0.0f, 0.0f));
    vertices.emplace_back(vertex_t { default_edge_index });
    faces.emplace_back(face_t { default_edge_index });
    edges.emplace_back(
      edge_t {
        default_vertex_index,
          default_face_index,
          default_edge_index,
          default_edge_index,
          default_edge_index
          });
  }

  size_t point_count() const {
    return points.size();
  }

  size_t vertex_count() const {
    return vertices.size();
  }

  size_t face_count() const {
    return faces.size();
  }

  size_t edge_count() const {
    return edges.size();
  }
};
