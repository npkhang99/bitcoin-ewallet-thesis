#include "http_server.h"

http_server::http_server() {}

http_server::http_server(const std::string& app_name)
        : _app_name(app_name) {}

void http_server::run() {
    base::Environment::init();
    server_run();
    base::Environment::destroy();
}

void http_server::server_run() {
    app_component component;

    OATPP_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, router);

    auto myController = std::make_shared<controller>();
    myController->addEndpointsToRouter(router);

    OATPP_COMPONENT(std::shared_ptr<oatpp::network::ConnectionHandler>, connection_handler);
    OATPP_COMPONENT(std::shared_ptr<oatpp::network::ServerConnectionProvider>, connection_provider);

    network::Server server(connection_provider, connection_handler);

    OATPP_LOGI(_app_name, "Server running on port %s",
               connection_provider->getProperty("port").getData());

    server.run();
}
