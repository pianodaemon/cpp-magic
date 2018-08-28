cc_library(
    name = "slackproto",
    srcs  = ["frame.cpp"],
    hdrs = ["frame.hpp"]
)

cc_binary(
    name = "server",
    srcs = ["main.cpp"],
    deps = [
        ":slackproto"
    ]
)
