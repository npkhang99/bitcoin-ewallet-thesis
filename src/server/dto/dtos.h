#ifndef BITCOIN_EWALLET_THESIS_DTOS_H
#define BITCOIN_EWALLET_THESIS_DTOS_H

#include <oatpp/core/data/mapping/type/Object.hpp>
#include <oatpp/core/macro/codegen.hpp>
#include <oatpp/core/Types.hpp>

#include OATPP_CODEGEN_BEGIN(DTO)

class message_dto : public oatpp::DTO {

    DTO_INIT(message_dto, DTO /* Extends */)

    DTO_FIELD(Int32, statusCode);   // Status code field
    DTO_FIELD(String, message);     // Message field

};

#include OATPP_CODEGEN_END(DTO)

#endif //BITCOIN_EWALLET_THESIS_DTOS_H
