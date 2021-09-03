# ucicpp
OpenWRT UCI config file json parser

This is c++ tool to parse uci config files and store their contents in a more convenient json object. I decided to use my favourite json library for c++, which is jsoncpp- so this depends on it. This is not completely automatic, when loading a configuration, you need to know what you are looking for and how to present it- so this is not a common reader of uci files, this is a reader for your app where you know the structure/design of your application's configuration. It is not perfect, with wrongly constructed config file, you will end up in troubles (arrays in place of dictionaries and so on). Also, this does not support feature boolean with "true" or "yes" as value. Value must be 1. It identifies numbers without decimal points and interprets them correctly and produces a valid json. Json::Value is very easy to read. Section titles are stored under variable "\_section\_title\_".

### example
Example application and configuration file is provided. jsoncpp, is something that you need to install on your own.

### sample output
```
{
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
	"interfaces" : 
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
	"routes" : 
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
	"testing" : 
	{
		"_section_title_" : "cfg026865",
		"opt1" : 1,
		"opt2" : 2
	}
}
```
