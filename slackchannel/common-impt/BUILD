cc_library(
    name = "slackchannel",
    srcs = ["frame.cpp", "action.cpp", "writer.cpp", "reader.cpp"],
    hdrs = ["common-impt.hpp", "frame.hpp", "action.hpp", "writer.hpp", "reader.hpp"],
    deps = [
        "@boost//:thread",
        "@boost//:asio",
    ],
    visibility = ["//server-impt:__pkg__"]
)
