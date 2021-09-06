#pragma once

#include <string>
#include <map>
#include <json/json.h>

namespace common {

	namespace uci {

		const int load(const std::string filename, Json::Value &root);

	}
}
