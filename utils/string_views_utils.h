//
// Created by Артемий Дегтярев on 11.01.2024.
//

#ifndef STRING_VIEWS_UTILS_H
#define STRING_VIEWS_UTILS_H
#include <string>
#include <vector>
#include <__fwd/string_view.h>
#include <__ranges/split_view.h>
#include <__ranges/views.h>

namespace tstl::views {
    inline std::vector<std::string_view> split(std::string_view view, std::string_view delim) {
        std::vector<std::string_view> res;
        for ( auto word: view | std::views::split(delim) )
            res.push_back( std::string_view( word ) );

        return res;
    }

}

#endif //STRING_VIEWS_UTILS_H
