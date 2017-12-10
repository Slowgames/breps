
package(default_visibility=["//visibility:public"])

cc_library (
    name = "easylogging++",
    hdrs = ["easyloggingpp/easylogging++.h"],
    srcs = ["easyloggingpp/easylogging++.cc"],
    copts = ["-Ivendor/easyloggingpp"]
)

cc_library(
    name = "catch2",
    hdrs = ["catch/catch.hpp"],
    strip_include_prefix = "catch",
    copts = ["-Ivendor/catch"]
)

cc_library(
    name = "glm",
    hdrs = glob(["glm-0.9.8.5/glm/**/*.hpp"]),
    strip_include_prefix = "glm-0.9.8.5",
    includes = ["glm-0.9.8.5/glm", "glm-0.9.8.5/glm/detail"],
    copts = ["-Ivendor/glm-0.9.8.5", "-Ivendor/glm-0.9.8.5/glm", "-DGLM_FORCE_DEPTH_ZERO_TO_ONE"],
)
