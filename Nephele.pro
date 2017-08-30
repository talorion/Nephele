TEMPLATE = subdirs

DISTFILES += \
    LICENSE.txt \
    README.md \
    doc/ToDo.txt \
    doc/git.txt \
    doc/testscript.js \
    doc/jsonResponseDiscretValue-Box.json \
    misc/build_number_generator.sh \
    misc/build_number_generator.bat

SUBDIRS +=    ext
SUBDIRS +=    src
SUBDIRS +=    tests

src.depends           += ext
tests.depends           += src

# build must be last:
#CONFIG += ordered
