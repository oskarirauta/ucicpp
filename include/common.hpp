#pragma once

#include <string>
#include <vector>
#include <sstream>
#include <chrono>
#include <ctime>
#include <map>

namespace common {

	struct duration {

		private:

			template<typename Rep, typename Period>
			void generate(const std::chrono::duration<Rep, Period> d) {

				std::chrono::milliseconds millis =
					std::chrono::duration_cast<std::chrono::milliseconds>(d);

				this -> timestamp = millis.count();
				auto _hours = std::chrono::duration_cast<std::chrono::hours>(millis);
				this -> hours = _hours.count();
				millis -= _hours;
				auto _minutes = std::chrono::duration_cast<std::chrono::minutes>(millis);
				this -> minutes = _minutes.count();
				millis -= _minutes;
				auto _seconds = std::chrono::duration_cast<std::chrono::seconds>(millis);
				this -> seconds = _seconds.count();
				millis -= _seconds;
				this -> ms = millis.count();
			}

		public:

			int hours, minutes, seconds, ms;
			long timestamp;

			template<typename Clock>
			duration(const std::chrono::time_point<Clock> point,
					const std::chrono::time_point<Clock> since) {

				auto d = point.time_since_epoch() -
						since.time_since_epoch();
				this -> generate(d);
			}

			template<typename Rep, typename Period>
			duration(const std::chrono::duration<Rep, Period> d) {

				this -> generate(d);
			}

	};

	static const char* whitespace = " \t\n\r\f\v";

	uint64_t constexpr mix(char m, uint64_t s) {
		return ((s<<7) + ~(s>>3)) + ~m;
	}

	uint64_t constexpr hash(const char *m) {
		return (*m) ? common::mix(*m,hash(m+1)) : 0;
	}

	bool has_prefix(const std::string str, const std::string prefix);

	bool has_suffix(const std::string str, const std::string suffix);

	std::string str_first(const std::string str, char delim = ' ');

	std::string trim(std::string &str, const std::string trimchars);

	std::string trim(const std::string &str, const std::string trimchars);

	std::vector<std::string> lines(const std::string &str, char delim = '\n',
		const std::string trimchars = "\r");

	std::vector<std::string> split(const std::string &str,
		char delim = ' ', const std::string trimchars = "");

	std::string to_lower(std::string &str);

	std::string to_lower(const std::string &str);

	std::string join_vector(const std::vector<std::string> v, const std::string delimeter = ", ");

	std::string c_tostr(const char *str);

	bool is_number(const std::string& s);

	bool is_hex(std::string const& s);

	// trim from end of string (right)
	inline std::string rtrim_ws(const std::string s, const char* t = whitespace) {
		std::string _s = s;
		_s.erase(_s.find_last_not_of(t) + 1);
		return _s;
	}

	// trim from beginning of string (left)
	inline std::string ltrim_ws(const std::string s, const char* t = whitespace) {
		std::string _s = s;
		_s.erase(0, _s.find_first_not_of(t));
		return _s;
	}

	// trim from both ends of string (right then left)
	inline std::string trim_ws(const std::string s, const char* t = whitespace) {
		return common::ltrim_ws(common::rtrim_ws(s, t), t);
	}

	std::string trim_leading(const std::string str, int count = 1);
	double round(double val);
	std::string memToStr(double amount, bool gigabytes = false);

	template <typename T>
	inline const std::string to_string(const T value, const int precision = 6) {
		std::ostringstream res;
		res.precision(precision);
		res << std::fixed << value;
		return res.str();
	}

	inline const std::string time_str(const std::time_t t) {
/*
		std::time_t _t = t;
		struct tm* timeinfo = localtime(&_t);
		std::string ret(asctime(timeinfo));
		return common::trim(ret, "\t\r\n");
*/
		std::time_t _t = t;
		char buf[64];
		strftime(buf, sizeof buf, "%F %R", std::localtime(&_t));
		std::string ret(buf);
		return ret;
	}

	inline const std::string uptime_str(const std::time_t t, bool longdesc = false) {
		std::time_t _t = t;
		std::string ret;
		int d = _t > 86400 ? _t / 86400 : 0;
		if ( d != 0 ) _t -= d * 86400;
		int h = _t > 3600 ? _t / 3600 : 0;
		if ( h != 0 ) _t -= h * 3600;
		int m = _t > 60 ? _t / 60 : 0;
		if ( m != 0 ) _t -= m * 60;

		if ( longdesc ) {
			if ( d > 0 )
				ret += std::to_string(d) + ( d > 1 ? " days" : " day" );
			if ( d > 0 || h > 0 )
				ret += ( ret.empty() ? "" : " " ) + std::to_string(h) +
					( h != 1 ? " hours" : " hour" );
			if ( d > 0 || h > 0 || m > 0 )
				ret += ( ret.empty() ? "" : " " ) + std::to_string(m) +
					( m != 1 ? " minutes" : " minute" );
			ret += ( ret.empty() ? "" : " " ) + std::to_string(_t) +
				( _t != 1 ? " seconds" : " second");
		} else {
			if ( d > 0 )
				ret += std::to_string(d) + "d";
			if ( d > 0 || h > 0 )
				ret += ( ret.empty() ? "" : " " ) + std::to_string(h) + "h";
			if ( d > 0 || h > 0 || m > 0 )
				ret += ( ret.empty() ? "" : " " ) + std::to_string(m) + "m";
			ret += ( ret.empty() ? "" : " " ) + std::to_string(_t) + "s";
		}

		return ret;
	}

	inline const std::chrono::milliseconds get_millis(void) {
		return std::chrono::duration_cast<std::chrono::milliseconds>
			(std::chrono::system_clock::now().time_since_epoch());
	}

	template<typename K, typename V>
	inline bool map_contains(K key, const std::map<K, V> Map) {

		for (typename std::map<K, V>::const_iterator it = Map.begin(); it != Map.end(); it++ )
			if ( it -> first == key )
				return true;

		return false;
	}

}
