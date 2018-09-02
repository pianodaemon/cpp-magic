cc_library(
    name = "implementation",
    hdrs = ["server.hpp"],
    srcs = ["server.cpp"],
    deps = [
        "@boost//:asio",
    ]
)

cc_binary(
    name = "server",
    srcs = ["main.cpp"],
    deps = [
            ":implementation",
            "//misc:toolbox",
            "//common-impt:slackchannel"
    ]
)
