cc_library(
    name = "slackchannel",
    srcs = ["frame.cpp", "reader.cpp"],
    hdrs = ["frame.hpp", "reader.hpp"],
    deps = [
        "@boost//:asio",
    ],
)

cc_binary(
    name = "server",
    srcs = ["main.cpp"],
    deps = [
            "//misc:toolbox",
            ":slackchannel"
    ]
)
