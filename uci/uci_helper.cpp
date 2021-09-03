#include <iostream>

extern "C" {
# include <uci.h>
}

#include "common.hpp"
#include "json_helper.hpp"
#include "uci_helper.hpp"

const int common::uci::load(const std::string filename, Json::Value &root,
		const std::map<std::string, std::string> dictionaries,
		const std::map<std::string, std::string> arrays) {

	struct uci_context *ctx = uci_alloc_context();
	struct uci_package *cfg = NULL;
	struct uci_element *e = NULL;
	Json::Value _root;

	if ( uci_load(ctx, filename.c_str(), &cfg) != UCI_OK ) {
		std::cout << "failed to load config file" << std::endl;
		root = _root;
		return -1;
	}

	std::map<std::string, std::string> j_dictionaries;
	std::map<std::string, std::map<std::string, std::string>> j_arrays;

	uci_foreach_element(&cfg -> sections, e) {

		struct uci_section *s = uci_to_section(e);
		struct uci_element *e2 = NULL;

		std::string u_pkg = std::string(s -> package -> e.name);
		std::string u_type = std::string(s -> type);
		std::string u_name = std::string(s -> e.name);
		std::string u_option = "";
		std::string u_value = "";
		std::string j_ifd = "";

		std::string dictname = "";
		std::string arrayname = "";
		std::string prev = "";
		std::string val = "";

		for ( std::map<std::string, std::string>::const_iterator it = dictionaries.begin();
			it != dictionaries.end(); it++ )
			if ( common::to_lower(u_type) == common::to_lower(it -> second)) {
				dictname = it -> first;
				break;
			}

		for ( std::map<std::string, std::string>::const_iterator it = arrays.begin();
			it != arrays.end(); it++ )
			if ( common::to_lower(u_type) == common::to_lower(it -> second)) {
				arrayname = it -> first;
				break;
			}

		if ( dictname.empty() && arrayname.empty()) continue;

		if ( !dictname.empty() && !common::map_contains(common::to_lower(dictname), j_dictionaries))
			j_dictionaries.insert_or_assign(common::to_lower(dictname), "");
		else if ( !arrayname.empty() && !common::map_contains(common::to_lower(arrayname), j_arrays))
			j_arrays.insert_or_assign(common::to_lower(arrayname), std::map<std::string, std::string>());

		uci_foreach_element(&s -> options, e2) {

			struct uci_option *o = uci_to_option(e2);
			struct uci_element *e3 = NULL;
			struct uci_element *e4 = NULL;
			bool sep = false;

			u_option = std::string(o -> e.name);
			u_value = o -> type == UCI_TYPE_STRING ? o -> v.string : "";

			if ( !dictname.empty())
				prev = j_dictionaries[common::to_lower(dictname)];
			else {
				if ( !common::map_contains(common::to_lower(u_name),  j_arrays[common::to_lower(arrayname)]))
					j_arrays[common::to_lower(arrayname)][common::to_lower(u_name)] = "";

				prev = j_arrays[common::to_lower(arrayname)][common::to_lower(u_name)];
			}

			switch ( o -> type ) {
				case UCI_TYPE_STRING:

					if ( !dictname.empty() && prev.empty())
						prev = "\"_section_title_\":\"" + common::to_lower(u_name) + "\"";
					else if ( !arrayname.empty() && prev.empty())
						prev = "\"_section_title_\":\"" + common::to_lower(u_name) + "\"";

					val = common::is_number(u_value) ? u_value : ( "\"" + u_value + "\"" );
					prev += ",\"" + common::to_lower(u_option) + "\":" + val;

					if ( !dictname.empty())
						j_dictionaries[common::to_lower(dictname)] = prev;
					else j_arrays[common::to_lower(arrayname)][common::to_lower(u_name)] = prev;

					break;

				case UCI_TYPE_LIST:

					val = "";
					uci_foreach_element(&o -> v.list, e4)
						val += "\"" + std::string(e4 -> name) + "\",";

					if ( val.empty()) break;

					val.pop_back();
					prev += ",\"" + common::to_lower(u_option) + "\":[" + val + "]";

					if ( !dictname.empty())
						j_dictionaries[common::to_lower(dictname)] = prev;
					else j_arrays[common::to_lower(arrayname)][common::to_lower(u_name)] = prev;

					break;

				default:

					std::cout << "unknown";
					break;

			}
		}
	}

	uci_free_context(ctx);

	std::string json = "";

	for ( std::map<std::string, std::string>::iterator it = j_dictionaries.begin();
		it != j_dictionaries.end(); it++ ) {

		if ( !json.empty()) json += ",";
		json += "\"" + it -> first + "\":{" + it -> second + "}";
	}

	for ( std::map<std::string, std::map<std::string, std::string>>::iterator it = j_arrays.begin();
		it != j_arrays.end(); it++ ) {
		std::string json2 = "";
		for ( std::map<std::string, std::string>::iterator it2 = it -> second.begin();
			it2 != it -> second.end(); it2++ ) {
			json2 += json2.empty() ? "{" : ",{";
			json2 += it2 -> second + "}";
		}
		json += ",\"" + it -> first + "\": [" + json2 + "]";
	}

	if ( common::json::build("{" + json + "}", _root) != 0 ) {
		root = _root;
		return -2;
	}

	root = _root;
	return 0;
}
