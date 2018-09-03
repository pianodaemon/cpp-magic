cc_library(
    name = "implementation",
    hdrs = ["server-impt.hpp", "server.hpp", "session.hpp", "monitor.hpp"],
    srcs = ["server.cpp", "session.cpp", "monitor.cpp"],
    deps = [
        "@boost//:asio",
        "//misc:toolbox",
        "//common-impt:slackchannel"
    ]
)

cc_binary(
    name = "server",
    srcs = ["main.cpp"],
    deps = [
        ":implementation"
    ]
)
