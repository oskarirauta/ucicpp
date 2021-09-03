#include <iostream>
#include <string>
#include <json/json.h>

#include "common.hpp"
#include "json_helper.hpp"
#include "uci_helper.hpp"

static const std::string config_file = "./example/conf";
static const bool nice = true;

int main() {

	Json::Value root;
	int ret = common::uci::load(config_file, root,
		{
			{ "app", "app" },
			{ "testing", "test" }
		}, {
			{ "interfaces", "interface" },
			{ "routes", "route" }
		});

	if ( ret != 0 ) {

		if ( ret == -1 )
			std::cout << "failed to load config file " << config_file << std::endl;

		if ( ret == -2 )
			std::cout << "json parse error" << std::endl;

		return -1;
	}

	std::cout << "json:\n" << common::json::dump(root, nice) << std::endl;;
	return 0;
}
