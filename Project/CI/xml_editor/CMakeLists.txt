cmake_minimum_required(VERSION 3.30)
project(xml_editor)

set(CMAKE_CXX_STANDARD 20)

add_executable(xml_editor
        check_consistency/tag.h
        check_consistency/check_xml_consistency.h
        check_consistency/check_xml_consistency.cpp
        main.cpp
        "formatting/formatting.cpp"
        "formatting/formatting.h"
        "converting_json/converting_xml_to_json.cpp"
        "converting_json/converting_xml_to_json.h"
        "minifying/minifying.cpp"
        "minifying/minifying.h"
        "compress/compressing.cpp"
        "compress/compressing.h"
        "decompress/decompress.cpp"
        "decompress/decompress.h"
        "representing xml/representing.cpp"
        "representing xml/representing.h"
        "representing xml/Graph.h"
        "network analysis/User.cpp"
        "network analysis/User.h"
        "network analysis/Post.cpp"
        "network analysis/Post.h"
        "network analysis/NetworkAnalysis.cpp"
        "network analysis/NetworkAnalysis.h"
)
