# ucicpp
OpenWRT UCI config file json parser

This is c++ tool to parse uci config files and store their contents in a more convenient json object (IMHO). I decided to use my favourite json library for c++, which is jsoncpp- so this depends on it. This does not support feature boolean with "true" or "yes" as value. Value must be 1. It identifies numbers without decimal points and interprets them correctly and produces a valid json. Json::Value is very easy to read. Section titles are stored under variable "_\_section\_title\__" and config file under "_\_uci\_package\__".

### example
Example application and configuration file is provided.

### jsoncpp dependency
Get it from https://github.com/open-source-parsers/jsoncpp

### sample output
```
{
	"_uci_package_" : "conf",
	"app" : 
	{
		"_section_title_" : "globals",
		"enabled" : 1,
		"hello" : "world",
		"lista" : 
		[
			"opt1",
			"opt2",
			"opt3"
		]
	},
	"interface" : 
	[
		{
			"_section_title_" : "lan",
			"ipaddr" : "192.168.0.1",
			"methods" : 
			[
				"reboot"
			],
			"netmask" : "255.255.255.0"
		},
		{
			"_section_title_" : "wan",
			"enabled" : 1,
			"ipaddr" : "192.168.8.1",
			"methods" : 
			[
				"ifup wan",
				"ifdown wan; sleep 1; ifup wan",
				"reboot"
			],
			"netmask" : "255.255.255.0"
		}
	],
	"route" : 
	[
		{
			"_section_title_" : "cfg06c8b4",
			"opt1" : 3,
			"opt2" : 4,
			"opt3" : "another word"
		},
		{
			"_section_title_" : "route1",
			"opt1" : 1,
			"opt2" : 2,
			"opt3" : "word"
		}
	],
	"test" : 
	{
		"_section_title_" : "cfg026865",
		"opt1" : 1,
		"opt2" : 2
	}
}
```
