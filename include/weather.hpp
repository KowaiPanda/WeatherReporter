#ifndef WEATHER_HPP
#define WEATHER_HPP
#include<iostream>
#include<string>
#include<curl_simple_https.h>
#include<nlohmann/json.hpp>
using json = nlohmann::json;

#define API_ID "320cfca321adaf5ec92e63aa69b95d6a"

struct details {
		std::string city;
		float temp{};
		float feels_like{};
		int pressure{};
		int humidity{};
		std::string icon{};
		std::string short_desc{};
		std::string desc{};
};

class Weather {
    public:
	void findLoc(std::string c);
	void findData(float lat,float lon);
	void assignLoc();
	void assignData();
	details retrieve(std::string city_name);
};

std::size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp);

std::string weatherDataToJsonString(const details& data);
#endif