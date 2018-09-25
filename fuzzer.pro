TEMPLATE = subdirs

SUBDIRS += \
    app \
    lib \
    tests

lib.subdir = src/lib
app.subdir = src/app
tests.subdir = src/tests

app.depends = lib
tests.depends = lib
