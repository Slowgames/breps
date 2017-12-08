
#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#include "hedge.hpp"


TEST_CASE( "hedge indexes work as expected", "[index_types]") {
  SECTION( "An index can be created and copy assigned" ) {
    hedge::edge_index_t def;
    hedge::edge_index_t e1(1, 1);
    hedge::edge_index_t e2 = e1;

    REQUIRE(def.offset == 0);
    REQUIRE(def.generation == 0);

    REQUIRE(e1.offset == e2.offset);
    REQUIRE(e1.generation == e2.generation);
  }

  SECTION( "An index can be compared with other indexes" ) {
    hedge::edge_index_t def;
    hedge::edge_index_t e1(1, 1);
    hedge::edge_index_t e2 = e1;

    REQUIRE(def != e1);
    REQUIRE(e1 == e2);
  }
}

TEST_CASE( "Edges can be created and updated.", "[edges]" ) {
  SECTION( "Default edge can be updated." ) {
    hedge::edge_t edge;
    hedge::vertex_index_t vert(3, 0);
    edge.vertex_index = vert;
  }
}

TEST_CASE( "Function sets behave as intended", "[function_sets]" ) {
  SECTION( "Edge function sets can be given null input when needed and can be checked." ) {
    hedge::edge_fn_t bad(nullptr, hedge::edge_index_t());
    REQUIRE_FALSE(bad);
  }

  SECTION( "Vertex function sets can be given null input when needed and can be checked." ) {
    hedge::vertex_fn_t bad(nullptr, hedge::vertex_index_t());
    REQUIRE_FALSE(bad);
  }

  SECTION( "Face function sets can be given null input when needed and can be checked." ) {
    hedge::face_fn_t bad(nullptr, hedge::face_index_t());
    REQUIRE_FALSE(bad);
  }
}

TEST_CASE( "hedge can create a basic quad", "[basic_quad]") {

  SECTION( "The default basic_mesh_t is of the expected number of elements" ) {
    hedge::mesh_t mesh;
    REQUIRE(mesh.kernel()->point_count() == 1);
    REQUIRE(mesh.kernel()->vertex_count() == 1);
    REQUIRE(mesh.kernel()->face_count() == 1);
    REQUIRE(mesh.kernel()->edge_count() == 1);
  }
}
