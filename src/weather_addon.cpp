#include<napi.h>
#include<../include/weather.hpp>

Napi::String GetWeatherWrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1 || !info[0].IsString()) {
        throw Napi::TypeError::New(env, "Location string required");
    }

    std::string location = info[0].As<Napi::String>().Utf8Value();
    
    Weather weatherService;
    details weatherData = weatherService.retrieve(location);
    return Napi::String::New(env, weatherDataToJsonString(weatherData));
}

Napi::Object InitModule(Napi::Env env, Napi::Object exports) {
    exports.Set(
        Napi::String::New(env, "getWeather"),
        Napi::Function::New(env, GetWeatherWrapper)
    );
    return exports;
}

NODE_API_MODULE(weatherService, InitModule)