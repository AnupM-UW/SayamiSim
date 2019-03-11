#include <stdio.h>
#include "stopwatch.h"
#include "robot.h"

using namespace elma;
using namespace std;
using json = nlohmann::json;

struct stype {
    string from;
    string to;
    string when;
};

namespace ns {
    // a simple struct to model a person
    struct person {
        std::string name;
        std::string address;
        int age;
    };

    void to_json(json& j, const person& p) {
        j = json{{"name", p.name}, {"address", p.address}, {"age", p.age}};
    }

    void from_json(const json& j, person& p) {
        j.at("name").get_to(p.name);
        j.at("address").get_to(p.address);
        j.at("age").get_to(p.age);
    }
} // namespace ns    

void runtests() {
    using namespace std;
    Stopwatch sWatch = Stopwatch();

    StopWatchUser sUser = StopWatchUser();

    elma::Manager m;

    elma::Channel link("link");

    m.schedule(sWatch, 10_ms)
    .schedule(sUser, 10_ms)
    .add_channel(link)
    .init();
    sWatch.watch("reset", [](Event& evt) {
        cout<<"Reset watch multihandler"<<endl;
    }, 2);
    sWatch.watch("reset", [](Event& evt) {
        cout<<"Reset watch multihandler higher"<<endl;
    }, 4);

    m.run(900_ms);

    cout<<"Expected milliseconds: "<<sUser.expected_milliseconds()<<" seconds: "<<sUser.expected_milliseconds()/(double)1000<<endl;
    cout<<"Actual seconds: "<<sWatch.seconds()<<endl;

    std::cout << "\n*************\n\n";

    Robot politebot = Robot("What a very nice robot.");

    json jr = politebot.to_json();
    cout<<"PolitBot: "<<jr.dump(4)<<endl;

    std::cout << "\n*************\n\n";

    Manager m2;
    m2.schedule(politebot, 500_ms)
    .init()
    .start()
    .emit(Event("start"));


    std::cout << "\n***\nStart a robot, detect intruder. Check state\n";
    std::cout << politebot.current().name() << std::endl;
    m2.emit(Event("intruder detected"));
    std::cout << "Pardon me sir, I believe there is an INTRUDER\n";
    std::cout << politebot.current().name() << std::endl;

    std::cout << "\n***\nStart a robot, and drain battery. Check state\n";
    m2.start()
    .emit(Event("start"))
    .emit(Event("battery low"));
    std::cout <<"Final State: "<< politebot.current().name() << std::endl;

    std::cout << "\n***\nStart a robot, and drain battery. Check state\n";
    m2.start()
    .emit(Event("start"))
    .emit(Event("battery low"));
    std::cout << "Battery drained and find recharge station\n";
    m2.emit(Event("found recharge station"));
    std::cout <<"Intermediate state: "<< politebot.current().name() << std::endl;
    std::cout << "Found recharge station and fully charged\n";
    m2.emit(Event("battery full"));
    std::cout << "Final State: "<<politebot.current().name() << std::endl;

    // json j;

    // // add a number that is stored as double (note the implicit conversion of j to an object)
    // j["pi"] = 3.141;

    // // add a Boolean that is stored as bool
    // j["happy"] = true;

    // // add a string that is stored as std::string
    // j["name"] = "Niels";

    // // add another null object by passing nullptr
    // j["nothing"] = nullptr;

    // // add an object inside the object
    // j["answer"]["everything"] = 42;

    // // add an array that is stored as std::vector (using an initializer list)
    // j["list"] = { 1, 0, 2 };

    // // add another object (using an initializer list of pairs)
    // j["object"] = { { {"currency", "USD"}, {"value", 42.99} } };

    // std::cout<<j.dump()<<std::endl;

    // j = 
    //     {
    //         { 
    //             "tas", {
    //                 {
    //                     { "first", "Justin" },
    //                     { "last", "Vrana" }
    //                 },
    //                 {
    //                     { "first", "Kung-Hung" },
    //                     { "last", "Lu" },
    //                     { "aka",  "Henry" }
    //                 }
    //             },
    //         },
    //         { 
    //             "weeks", { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 } 
    //         },
    //         { 
    //             "version", 2.0
    //         }
    //     };

    // stype s1 {"fk", "tj", "buy"};
    // stype s2 {"tj", "ao", "sell"};
    // stype s3 {"ao", "fk", "hold"};

    // stype stypes[3] = {s1, s2, s3};

    // json jsons[3];
    // for (int i = 0; i < 3; ++i) {
    //     json js;
    //     js["from"] = stypes[i].from;
    //     js["to"] = stypes[i].to;
    //     js["when"] = stypes[i].when;
    //     jsons[i] = js;
    // }

    // // j["myvals"] = {{{"a1", "sauce"}, {"h57", "sauce"}}, {{"Leah", "better"}}};
    // j["myvals"] = jsons; //{ json1 };

    // // json array_not_object = json::array({ {"currency", "USD"}, {"value", 42.99} });
    // std::cout<<j.dump()<<std::endl;

    // std::cout << "\n*************\n\n";

    // // create a person
    // ns::person p {"Ned Flanders", "744 Evergreen Terrace", 60};

    // // conversion: person -> json
    // j = p;

    // std::cout << j << std::endl;
    // // {"address":"744 Evergreen Terrace","age":60,"name":"Ned Flanders"}

}

//// GTEST_API_ 
int main(int argc, char **argv) {
    //testing::InitGoogleTest(&argc, argv);
    runtests();
    //return RUN_ALL_TESTS();
}
