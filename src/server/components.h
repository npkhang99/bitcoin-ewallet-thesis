#ifndef BITCOIN_EWALLET_THESIS_COMPONENTS_H
#define BITCOIN_EWALLET_THESIS_COMPONENTS_H

#include <oatpp/parser/json/mapping/ObjectMapper.hpp>

#include <oatpp/web/server/HttpConnectionHandler.hpp>
#include <oatpp/network/tcp/server/ConnectionProvider.hpp>

#include <oatpp/core/macro/component.hpp>

using namespace oatpp;

class app_component {
public:

    /**
     *  Create ConnectionProvider component which listens on the port
     */
    OATPP_CREATE_COMPONENT(std::shared_ptr<network::ServerConnectionProvider>, serverConnectionProvider)([] {
        return network::tcp::server::ConnectionProvider::createShared({
            "localhost", 8080, network::Address::IP_4
        });
    }());

    /**
     *  Create Router component
     */
    OATPP_CREATE_COMPONENT(std::shared_ptr<web::server::HttpRouter>, httpRouter)([] {
        return web::server::HttpRouter::createShared();
    }());

    /**
     *  Create ConnectionHandler component which uses Router component to route requests
     */
    OATPP_CREATE_COMPONENT(std::shared_ptr<network::ConnectionHandler>, serverConnectionHandler)([] {
        OATPP_COMPONENT(std::shared_ptr<web::server::HttpRouter>, router); // get Router component
        return web::server::HttpConnectionHandler::createShared(router);
    }());

    /**
     *  Create ObjectMapper component to serialize/deserialize DTOs in Contoller's API
     */
    OATPP_CREATE_COMPONENT(std::shared_ptr<data::mapping::ObjectMapper>, apiObjectMapper)([] {
        return parser::json::mapping::ObjectMapper::createShared();
    }());

};

#endif //BITCOIN_EWALLET_THESIS_COMPONENTS_H
