/**
 * @file path.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <graphics/path.h>
#include <mach-o/dyld.h>
#include <sstream>

namespace Graphics {
namespace PathUtil {

std::string prependExecutableDirectory(std::string localPath) {
    char buff[PATH_MAX + 1];
    uint32_t size = PATH_MAX;
    int stat = _NSGetExecutablePath(buff, &size);
    assert(!stat && "_NSGetExecutablePath failed");
    buff[size] = 0;
    std::string exe_path(buff);

    std::string exe_dir = exe_path.substr(0, exe_path.rfind("/"));

    std::stringstream path_str;
    path_str << exe_dir << "/" << localPath;
    std::string path = path_str.str();

    return path;
}

}}
