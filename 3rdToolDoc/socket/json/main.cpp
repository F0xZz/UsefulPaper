#include <iostream>
#include <json.hpp>
#include <fstream>
using namespace std;
using json = nlohmann::json;
int main()
{
    //read the json file
    std::ifstream i("../ch01json/json.json");
    //resolving the filestream to the json
    json j ;
    i>>j;
    //find the json keyword of the "object"
    json freq2=j["object"];
    //find the keyword of the "pi"
    float p = j["pi"];
    //out the object
    for(auto &el:freq2.items()){
        std::cout<<el.key()<<el.value()<<std::endl;
    }
    //find the object
    //"object":{"point1":[0,5],
    //          "point2":[0,5],
    //          "point3":[0,5]}
    json boud = j["boud"];
    std::cout<<boud.at("point1").at(0)<<std::endl;
    //insert the value
    int a[2];
    a[0]=5;
    a[1]=6;
    j["boud2"]={ {"one",a}, {"two", a} };
    //out the json file
    std::ofstream o("prettey.json");
    o << std::setw(4) << j << std::endl;

    return 0;
}
