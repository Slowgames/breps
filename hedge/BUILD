

cc_library (
    name = "hedge",
    srcs = ["cpp/hedge.cpp"],
    hdrs = ["cpp/hedge.hpp"],
    copts = ["-Ihedge/cpp"],
    deps = ["//vendor:easylogging++", "//vendor:mathfu"]
)

cc_test (
    name = "hedge_test",
    srcs = ["cpp/hedge_test.cpp"],
    deps = [":hedge", "//vendor:catch2", "//vendor:easylogging++"]
)
