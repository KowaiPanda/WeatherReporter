#include<../include/weather.hpp>

std::string name;
std::string readBuffer;
std::string errorMessage = "";
float lat{};
float lon{};
struct details tell;

void Weather::findLoc(std::string c) {

	readBuffer.clear();

	CURL* cityLoc;
	CURLcode Loc;

	std::string url = "http://api.openweathermap.org/geo/1.0/direct?q=&appid=";

	url.insert(47, c);
	url.insert(url.size(), API_ID);
	if ((url.find(" "))!=-1) {
		int sp = url.find(" ");
		url.replace(sp, 1, "%20");
	}

	cityLoc = curl_easy_init();
	if (cityLoc == NULL)
		fprintf(stderr, "API FAIL");

	curl_easy_setopt(cityLoc, CURLOPT_URL, url.c_str());
	curl_easy_setopt(cityLoc, CURLOPT_WRITEFUNCTION, WriteCallback);
	curl_easy_setopt(cityLoc, CURLOPT_WRITEDATA, &readBuffer);

	Loc = curl_easy_perform(cityLoc);
	if (Loc != CURLE_OK)
		fprintf(stderr, "Problem with curl code");

	curl_easy_cleanup(cityLoc);
}

void Weather::findData(float lat,float lon) {

	if (errorMessage != "")
		return;	
	CURL* weatherStats;
	CURLcode Stats;

	readBuffer.clear();

	std::string url = "https://api.openweathermap.org/data/2.5/weather?lat=&lon=&appid=";
	int pos;

	pos = url.find("lat");
	url.insert(pos+4, std::to_string(lat));
	
	pos = url.find("lon");
	url.insert(pos+4, std::to_string(lon));

	url.insert(url.size(), API_ID);

	weatherStats = curl_easy_init();
	if (weatherStats == NULL)
		fprintf(stderr, "API FAIL");

	curl_easy_setopt(weatherStats, CURLOPT_URL, url.c_str());
	curl_easy_setopt(weatherStats, CURLOPT_WRITEFUNCTION, WriteCallback);
	curl_easy_setopt(weatherStats, CURLOPT_WRITEDATA, &readBuffer);

	Stats = curl_easy_perform(weatherStats);
	if (Stats != CURLE_OK)
		fprintf(stderr, "Problem with curl code");

	curl_easy_cleanup(weatherStats);
}

void Weather::assignLoc() {

	json locData = json::parse(readBuffer);
	if (!locData.empty()) {
		lat = locData[0]["lat"];
		lon = locData[0]["lon"];
		tell.city = locData[0]["name"];
		errorMessage = "";
	}
	else {
		errorMessage = "Error: No location data found in API response";
	}
}

void Weather::assignData() {

	if (errorMessage == "") {
		json data = json::parse(readBuffer);

		tell.temp = data["main"]["temp"];
		tell.humidity = data["main"]["humidity"];
		tell.pressure = data["main"]["pressure"];
		tell.feels_like = data["main"]["feels_like"];
		tell.icon = data["weather"][0]["icon"];
		tell.short_desc = data["weather"][0]["main"];
		tell.desc = data["weather"][0]["description"];
	}
	else {
		tell.temp = NULL;
		tell.humidity = NULL;
		tell.pressure = NULL;
		tell.feels_like = NULL;
		tell.icon = "";
		tell.short_desc = "";
		tell.desc = "";
	}
}

details Weather::retrieve(std::string city_name) {

	name = city_name;
	findLoc(name);
	assignLoc();
	findData(lat,lon);
	assignData();
	return tell;
}

size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
	((std::string*)userp)->append((char*)contents, size * nmemb);
	return size * nmemb;
}

std::string weatherDataToJsonString(const details& data) {
    json jsonData = {
        {"location", data.city},
        {"temperature", data.temp},
        {"humidity", data.humidity},
        {"pressure", data.pressure},
        {"description", data.desc},
        {"feels_like", data.feels_like},
        {"short_desc", data.short_desc},
		{"icon", data.icon}
    };
    return jsonData.dump();
}