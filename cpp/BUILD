

cc_library (
    name = "hedge",
    srcs = ["hedge/hedge.cpp"],
    hdrs = ["hedge/hedge.hpp"],
    copts = ["-Icpp/hedge"],
    deps = ["//vendor:easylogging++", "//vendor:mathfu"],
    visibility = ["//visibility:public"]
)

cc_test (
    name = "hedge_test",
    srcs = ["hedge/hedge_test.cpp"],
    deps = [":hedge", "//vendor:catch2", "//vendor:easylogging++"]
)
