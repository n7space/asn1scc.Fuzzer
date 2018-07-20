*-msvc*: QMAKE_CXXFLAGS += -WX
else: QMAKE_CXXFLAGS += -Werror -Wextra -Wpedantic
