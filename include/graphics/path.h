/**
 * @file path.h
 *
 * @brief Path utilities
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#ifndef __PATH_H
#define __PATH_H

#include <string>

namespace Graphics {
namespace PathUtil {

/**
 * @brief Inserts the full path to the directory containing the executable
 * before a 'local' path. This is useful for loading content distributed
 * alongside the executable.
 */
std::string prependExecutableDirectory(std::string localPath);

}}

#endif
