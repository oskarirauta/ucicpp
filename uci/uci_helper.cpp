extern "C" {
# include <uci.h>
}

#include "common.hpp"
#include "json_helper.hpp"
#include "uci_helper.hpp"

const int common::uci::load(const std::string filename, Json::Value &root) {

	struct uci_context *ctx = uci_alloc_context();
	struct uci_package *cfg = NULL;
	struct uci_element *e = NULL;
	Json::Value _root;

	if ( uci_load(ctx, filename.c_str(), &cfg) != UCI_OK ) {
		//std::cout << "failed to load config file" << std::endl;
		root = _root;
		return -1;
	}

	std::map<std::string, std::map<std::string, std::string>> u_content;
	std::string u_pkg = "";

	uci_foreach_element(&cfg -> sections, e) {

		struct uci_section *s = uci_to_section(e);
		struct uci_element *e2 = NULL;

		std::string _u_pkg = common::to_lower(std::string(s -> package -> e.name));
		std::string u_type = common::to_lower(std::string(s -> type));
		std::string u_name = common::to_lower(std::string(s -> e.name));
		std::string u_option = "";
		std::string u_value = "";

		std::string prev = "";
		std::string val = "";

		if ( u_pkg.empty() && !_u_pkg.empty())
			u_pkg = _u_pkg;

		if ( !common::map_contains(u_type, u_content))
			u_content.insert_or_assign(u_type, std::map<std::string, std::string>());

		uci_foreach_element(&s -> options, e2) {

			struct uci_option *o = uci_to_option(e2);
			struct uci_element *e3 = NULL;
			struct uci_element *e4 = NULL;
			bool sep = false;

			u_option = common::to_lower(std::string(o -> e.name));
			u_value = o -> type == UCI_TYPE_STRING ? o -> v.string : "";

			if ( !common::map_contains(u_name, u_content[u_type]))
				u_content[u_type][u_name] = "";

			prev = u_content[u_type][u_name];

			switch ( o -> type ) {
				case UCI_TYPE_STRING:

					if ( prev.empty())
						prev = "\"_section_title_\":\"" + u_name + "\"";

					val = common::is_number(u_value) ? u_value : ( "\"" + u_value + "\"" );
					prev += ",\"" + u_option + "\":" + val;

					u_content[u_type][u_name] = prev;

					break;

				case UCI_TYPE_LIST:

					val = "";
					uci_foreach_element(&o -> v.list, e4)
						val += "\"" + std::string(e4 -> name) + "\",";

					if ( val.empty()) break;

					val.pop_back();

					if ( prev.empty())
						prev = "\"_section_title_\":\"" + u_name + "\"";
					prev += ",\"" + u_option + "\":[" + val + "]";

					u_content[u_type][u_name] = prev;

					break;

				default:

					// std::cout << "unknown";
					break;

			}
		}
	}

	uci_free_context(ctx);

	std::string json = "";

	for ( std::map<std::string, std::map<std::string, std::string>>::iterator it = u_content.begin();
		it != u_content.end(); it++ ) {
		std::string json2 = "";
		for ( std::map<std::string, std::string>::iterator it2 = it -> second.begin();
			it2 != it -> second.end(); it2++ ) {
			json2 += json2.empty() ? "{" : ",{";
			json2 += it2 -> second + "}";
		}
		if ( !json.empty()) json += ",";
		json += "\"" + it -> first + "\":";
		if ( it -> second.size() > 1 ) json +=  "[" + json2 + "]";
		else json += json2;
	}

	if ( !u_pkg.empty())
		json = "\"_uci_package_\":\"" + u_pkg + "\"," + json;

	if ( common::json::build("{" + json + "}", _root) != 0 ) {
		root = _root;
		return -2;
	}

	root = _root;
	return 0;
}
