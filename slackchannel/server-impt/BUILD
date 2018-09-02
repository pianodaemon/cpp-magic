cc_library(
    name = "implementation",
    hdrs = ["server-impt.hpp", "server.hpp", "session.hpp"],
    srcs = ["server.cpp", "session.cpp"],
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
