
#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#include "hedge.hpp"


TEST_CASE( "hedge indexes work as expected", "[index_types]") {
  SECTION( "An index can be created and copied" ) {
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
    hedge::vertex_index_t v1(1, 1);

    REQUIRE(def != e1);
    REQUIRE(e1 == e2);
    //REQUIRE(v1 == e1);
  }

  SECTION( "An index is sufficiently type-safe" ) {
    auto test_fn = [](hedge::edge_index_t a, hedge::edge_index_t b) -> bool {
      return a == b;
    };

    hedge::edge_index_t e1;
    hedge::edge_index_t e2;

    REQUIRE(test_fn(e1, e2));
  }
}

TEST_CASE( "hedge can create a basic quad", "[basic_quad]") {

  SECTION( "The default basic_mesh_t is of the expected number of elements" ) {
    hedge::mesh_t mesh;
    REQUIRE(mesh.point_count() == 1);
    REQUIRE(mesh.vertex_count() == 1);
    REQUIRE(mesh.face_count() == 1);
    REQUIRE(mesh.edge_count() == 1);
  }
}
