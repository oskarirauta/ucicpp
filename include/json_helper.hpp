#pragma once

#include <string>
#include <json/json.h>

namespace common {

	namespace json {

		inline const std::string dump(const Json::Value root, bool nice = true) {

			Json::StreamWriterBuilder builder;
			if ( !nice ) {
				builder.settings_["commentStyle"] = "None";
				builder.settings_["indentation"] = "";
			}
			return Json::writeString(builder, root);
		}

		inline const int build(const std::string src, Json::Value &root) {

			Json::Value _root;

			if ( src.empty()) {
				root = _root;
				return 0;
			}

			JSONCPP_STRING err;
			const auto rawJsonLength = static_cast<int>(src.length());
			Json::CharReaderBuilder builder;
			const std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
			if ( !reader -> parse(src.c_str(), src.c_str() + rawJsonLength,
						&_root, &err))
				return -1;

			root = _root;
			return 0;
		}

	}
}
