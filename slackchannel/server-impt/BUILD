cc_library(
    name = "slackchannel",
    srcs = ["frame.cpp"],
    hdrs = ["frame.hpp"]
)

cc_binary(
    name = "server",
    srcs = ["main.cpp"],
    deps = [
            "//misc:toolbox",
            ":slackchannel"
    ]
)
