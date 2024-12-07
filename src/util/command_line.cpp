#include "command_line.hpp"

#include <filesystem>
#include <iostream>

#include "files/engine_paths.hpp"
#include "util/ArgsReader.hpp"
#include "engine.hpp"

namespace fs = std::filesystem;

static bool perform_keyword(
    util::ArgsReader& reader, const std::string& keyword, CoreParameters& params
) {
    if (keyword == "--res") {
        auto token = reader.next();
        params.resFolder = fs::u8path(token);
    } else if (keyword == "--dir") {
        auto token = reader.next();
        params.userFolder = fs::u8path(token);
    } else if (keyword == "--help" || keyword == "-h") {
        std::cout << "VoxelEngine command-line arguments:\n";
        std::cout << " --help - show help\n";
        std::cout << " --res <path> - set resources directory\n";
        std::cout << " --dir <path> - set userfiles directory\n";
        std::cout << " --headless - run in headless mode\n";
        std::cout << " --test <path> - test script file\n";
        std::cout << std::endl;
        return false;
    } else if (keyword == "--headless") {
        params.headless = true;
    } else if (keyword == "--test") {
        auto token = reader.next();
        params.testFile = fs::u8path(token);
    } else {
        throw std::runtime_error("unknown argument " + keyword);
    }
    return true;
}

bool parse_cmdline(int argc, char** argv, CoreParameters& params) {
    util::ArgsReader reader(argc, argv);
    reader.skip();
    while (reader.hasNext()) {
        std::string token = reader.next();
        if (reader.isKeywordArg()) {
            if (!perform_keyword(reader, token, params)) {
                return false;
            }
        } else {
            std::cerr << "unexpected token" << std::endl;
            return false;
        }
    }
    return true;
}
