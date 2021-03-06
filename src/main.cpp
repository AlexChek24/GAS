#include <iostream>
#include "database/db_connector.hpp"
#include "sql/query.hpp"
#include "sql/table.hpp"

using namespace gas;

struct resource {
  MARK_AS_TABLE(resource)
  TABLE_FIELD(id, int)
  TABLE_FIELD(name, std::string)
  TABLE_FIELD(description, std::string)
  TABLE_FIELD(size, int)
  TABLE_FIELD(checksum, int)
  TABLE_FIELD(type, int)
  TABLE_FIELD(data, std::string)
};

struct dev_role {
  MARK_AS_TABLE(dev_role)
  TABLE_FIELD(id, int)
  TABLE_FIELD(role, std::string)
};

auto main() -> int {

  auto settings =
      gas::Settings{}
          .host("localhost")
          .port(5432)
          .db_name("game_assets")
          .username("username")
          .password("password");
  Connector connector;
  try {
    if (connector.connect(settings)) {
      auto select = gas::select(columns<resource>{resource::name{}},columns<dev_role>{dev_role::id{}})
          .where(
              {logical::AND{},
               {condition<resource>{resource::type{},
                                    eq{5}}}
              }, {}
          );
      std::cout << select() << '\n';
      if (auto res = connector.exec(select())) {
        std::cout << "Found " << (*res).size() << " meshes:\n";
        for (auto[name] : res->iter<std::string>()) {
          std::cout << name << '\n';
        }
      } else {
        std::cout << "Nothing was found\n";
      }
    } else {
      std::cout << "could not connect to db with settings: " + settings.to_string() << '\n';
    }
  }
  catch (std::exception const &e) {
    std::cerr << e.what() << '\n';
    return 1;
  }
  return 0;

}
