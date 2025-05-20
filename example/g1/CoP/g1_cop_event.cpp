#include <iostream>
#include <string>

#include <unitree/robot/channel/channel_subscriber.hpp>
#include <unitree/robot/g1/loco/g1_loco_client.hpp>
#include <unitree/robot/g1/audio/g1_audio_client.hpp>

enum fsm_states {
    ZERO_TORQUE         = 0,
    DAMPING             = 1,
    SQUAD               = 2,
    SIT                 = 3,
    LOCK_STAND          = 4,
    MAIN_MOTION_CONTROL = 200
};

unitree::robot::g1::LocoClient setupLocoClient() {
    unitree::robot::g1::LocoClient client;

    client.Init();
    client.SetTimeout(10.f);

    return client;
}

unitree::robot::g1::AudioClient setupAudioClient() {
    unitree::robot::g1::AudioClient client;

    client.Init();
    client.SetTimeout(10.f);

    return client;
}

void start() {
    unitree::robot::g1::LocoClient client = setupLocoClient();

    fsm_states state;
    state = DAMPING;
    client.SetFsmId(state);
    std::cout << "Setting damping mode" << std::endl;
    unitree::common::Sleep(1);
    state = LOCK_STAND;
    client.SetFsmId(state);
    std::cout << "Setting lock stand mode" << std::endl;
    unitree::common::Sleep(10);
    state = MAIN_MOTION_CONTROL;
    client.SetFsmId(state);
    std::cout << "Setting main motion control mode" << std::endl;
    unitree::common::Sleep(5);
}

void sit() {
    unitree::robot::g1::LocoClient client = setupLocoClient();

    fsm_states state;
    state = SIT;
    client.SetFsmId(state);
    std::cout << "Setting sit mode" << std::endl;
    unitree::common::Sleep(10);
    state = DAMPING;
    client.SetFsmId(state);
    std::cout << "Setting damping mode" << std::endl;
    unitree::common::Sleep(5);
}

void say() {
    unitree::robot::g1::AudioClient client = setupAudioClient();

    std::string in;
    std::getline(std::cin, in);

    int32_t ret;
    ret = client.TtsMaker(in, 1);
    std::cout << ret << std::endl;
}

void eyecolor() {
    unitree::robot::g1::AudioClient client = setupAudioClient();

    std::string in;
    std::getline(std::cin, in);

    uint8_t r, g, b;
    std::stringstream stream;

    stream << std::hex << in.substr(0, 2);
    stream >> r;
    stream.clear();

    stream << std::hex << in.substr(2, 4);
    stream >> g;
    stream.clear();

    stream << std::hex << in.substr(4, 6);
    stream >> b;
    stream.clear();

    client.LedControl(r, g, b);
    std::cout << r << ' ' << g << ' ' << b << std::endl;
}

int main(int argc, char const *argv[]) {
    if (argc < 2) {
        std::cout << "Usage: audio_client_example [NetWorkInterface(eth0)]" << std::endl;
        exit(0);
    }

    std::string networkInterface = argv[1];
    unitree::robot::ChannelFactory::Instance()->Init(0, networkInterface.c_str());

    std::unordered_map<std::string, void(*)()> functionMap = {
        {"--start", &start},
        {"--sit", &sit},
        {"--say", &say},
        {"--eyecolor", &eyecolor}
    };

    for (int i = 2; i < argc; ++i) {
        std::string arg = argv[i];

        auto it = functionMap.find(arg);
        if (it != functionMap.end()) {
            it->second();
        } else {
            std::cerr << "Error: Function '" << arg << "' does not exist." << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    return 0;
}