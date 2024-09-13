#ifndef FIELD_GENERATOR_H
#define FIELD_GENERATOR_H

#include <utility>
#include <vector>

#include "field_info.h"
#include "polish_notation/data_structures/queue/queue.h"
#include "polish_notation/token/token.h"

namespace polish_notation::renderer {
::std::pair<bool, ::std::vector<::std::vector<char>>> tryGetGeneratedField(
    const ::polish_notation::data_structures::queue::Queue<
        ::polish_notation::token::Token>& qPostfix,
    FieldInfo fieldInfo);
} // namespace polish_notation::renderer

#endif // FIELD_GENERATOR_H