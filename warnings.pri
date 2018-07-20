*-msvc*: QMAKE_CXXFLAGS += -WX
else:*-clang*: QMAKE_CXXFLAGS += -Werror
else:*-g++*: QMAKE_CXXFLAGS += -Werror
