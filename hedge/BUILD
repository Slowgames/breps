

cc_library (
    name = "hedge",
    srcs = ["cpp/hedge.cc"],
    hdrs = ["cpp/hedge.hpp"],
    copts = ["-Ihedge/cpp"],
    deps = ["//vendor:easylogging++", "//vendor:glm"]
)

cc_test (
    name = "hedge_test",
    srcs = ["cpp/hedge_test.cc"],
    deps = [":hedge", "//vendor:catch2", "//vendor:easylogging++"]
)
