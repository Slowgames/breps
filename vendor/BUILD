
package(default_visibility=["//visibility:public"])

cc_library (
    name = "easylogging++",
    hdrs = ["easyloggingpp/easylogging++.h"],
    srcs = ["easyloggingpp/easylogging++.cc"],
    strip_include_prefix = "easyloggingpp",
    includes = ["easyloggingpp"],
    copts = ["-Ivendor/easyloggingpp"]
)

cc_library(
    name = "catch2",
    hdrs = ["catch/catch.hpp"],
    includes = ["catch"],
    strip_include_prefix = "catch",
    copts = ["-Ivendor/catch"]
)

cc_library(
    name = "vectorial",
    includes = ["vectorial/include"],
    textual_hdrs = glob(["vectorial/include/vectorial/*.h"]),
    strip_include_prefix = "vectorial"
)

cc_library(
    name = "mathfu",
    hdrs = [
        "mathfu/include/mathfu/constants.h",
        "mathfu/include/mathfu/glsl_mappings.h",
        "mathfu/include/mathfu/internal/disable_warnings_begin.h",
        "mathfu/include/mathfu/internal/disable_warnings_end.h",
        "mathfu/include/mathfu/internal/matrix_4x4_simd.h",
        "mathfu/include/mathfu/internal/vector_2.h",
        "mathfu/include/mathfu/internal/vector_2_simd.h",
        "mathfu/include/mathfu/internal/vector_3.h",
        "mathfu/include/mathfu/internal/vector_3_simd.h",
        "mathfu/include/mathfu/internal/vector_4.h",
        "mathfu/include/mathfu/internal/vector_4_simd.h",
        "mathfu/include/mathfu/io.h",
        "mathfu/include/mathfu/matrix.h",
        "mathfu/include/mathfu/matrix_4x4.h",
        "mathfu/include/mathfu/quaternion.h",
        "mathfu/include/mathfu/rect.h",
        "mathfu/include/mathfu/utilities.h",
        "mathfu/include/mathfu/vector.h",
    ],
    features = ["-parse_headers"],
    includes = ["mathfu/include"],
    strip_include_prefix = "mathfu/include",
    copts = ["-Ivendor/mathfu/include"],
    deps = [
        ":vectorial",
    ],
)
