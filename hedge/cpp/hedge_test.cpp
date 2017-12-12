
#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#include "hedge.hpp"


TEST_CASE( "An index can be created and assigned a value from indexes of the same type.", "[index_types]") {
  hedge::edge_index_t def;
  hedge::edge_index_t e1(1, 1);
  hedge::edge_index_t e2 = e1;

  REQUIRE(def.offset == 0);
  REQUIRE(def.generation == 0);

  REQUIRE(e1.offset == e2.offset);
  REQUIRE(e1.generation == e2.generation);
}

TEST_CASE( "An index can be compared with other indexes of the same type.", "[index_types]") {
  hedge::edge_index_t def;
  hedge::edge_index_t e1(1, 1);
  hedge::edge_index_t e2 = e1;

  REQUIRE(def != e1);
  REQUIRE(e1 == e2);
}

TEST_CASE( "Edges can be created and updated.", "[edges]") {
  hedge::edge_t edge;
  hedge::vertex_index_t vert(3, 0);
  edge.vertex_index = vert;
  REQUIRE(edge.vertex_index == vert);
}

TEST_CASE("Edge function sets can be given null input when needed and can be checked.", "[function_sets]") {
  hedge::edge_fn_t bad(nullptr, hedge::edge_index_t());
  REQUIRE_FALSE(bad);
}

TEST_CASE("Vertex function sets can be given null input when needed and can be checked.", "[function_sets]") {
  hedge::vertex_fn_t bad(nullptr, hedge::vertex_index_t());
  REQUIRE_FALSE(bad);
}

TEST_CASE("Face function sets can be given null input when needed and can be checked.", "[function_sets]") {
  hedge::face_fn_t bad(nullptr, hedge::face_index_t());
  REQUIRE_FALSE(bad);
}

TEST_CASE( "The default basic_mesh_t is of the expected number of elements", "[mesh]" ) {
  hedge::mesh_t mesh;

  REQUIRE(mesh.kernel->point_count() == 1);
  REQUIRE(mesh.kernel->vertex_count() == 1);
  REQUIRE(mesh.kernel->face_count() == 1);
  REQUIRE(mesh.kernel->edge_count() == 1);

  // The mesh should report the count as the user of the api would expect
  REQUIRE(mesh.point_count() == 0);
  REQUIRE(mesh.vertex_count() == 0);
  REQUIRE(mesh.face_count() == 0);
  REQUIRE(mesh.edge_count() == 0);
}

SCENARIO( "Essential kernel operations allow you to create a triangle.", "[kernel_operations]" ) {

  GIVEN("An empty mesh.") {
    hedge::mesh_t mesh;
    REQUIRE(mesh.point_count() == 0);

    WHEN("We add three points to the mesh") {
      mesh.kernel->emplace(hedge::point_t(0.f, 0.f, 0.f));
      auto pi1 = mesh.kernel->emplace(hedge::point_t(1.f, 0.f, 0.f));
      mesh.kernel->emplace(hedge::point_t(1.f, 1.f, 0.f));

      REQUIRE(mesh.kernel->point_count() == 4);

      THEN( "We can retrieve one of the previously created points." ) {
        REQUIRE(pi1);
        hedge::point_t* p1 = mesh.kernel->get(pi1);
        REQUIRE(p1 != nullptr);
        REQUIRE(p1->position.x == 1.f);
        REQUIRE(p1->position.y == 0.f);
        REQUIRE(p1->position.z == 0.f);
      }
    }

    WHEN("We add three points and three vertices") {
      auto pindex0 = mesh.kernel->emplace(hedge::point_t(0.f, 0.f, 0.f));
      auto pindex1 = mesh.kernel->emplace(hedge::point_t(1.f, 0.f, 0.f));
      auto pindex2 = mesh.kernel->emplace(hedge::point_t(0.f, 1.f, 0.f));

      auto add_vertex = [&mesh](hedge::point_index_t pindex) -> hedge::vertex_index_t {
        hedge::vertex_t vert;
        vert.point_index = pindex;
        return mesh.kernel->emplace(std::move(vert));
      };
      auto vindex0 = add_vertex(pindex0);
      auto vindex1 = add_vertex(pindex1);
      auto vindex2 = add_vertex(pindex2);

      THEN("We can create three edges and a face.") {
        //
      }
    }
  }
}
