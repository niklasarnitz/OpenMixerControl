#include "omc-osc/omc_osc.hpp"

#include "example_complex_message.hpp"

#include <chrono>
#include <iostream>

using namespace omc::osc;

int main()
{
    Endpoint oms{"127.0.0.1", 9000};
    Peer omc(9001);

    auto fader = OMC.faders.getValue(faderNumber(1));
    Path complexPath{"/complex"};

    omc.route(fader, [fader](Peer&, const Message& msg) {
        if (auto value = msg.arg(fader)) {
            std::cout << "rx fader 1 = " << *value << "\n";
        }
    });

    omc.route(complexPath, [](Peer&, const Message& msg) {
        ExampleComplexMessage value;
        if (!msg.args.empty() && readValue(msg.args[0], value)) {
            std::cout << "rx complex fader=" << value.fader << " mute=" << value.mute << "\n";
        }
    });

    ExampleComplexMessage value;
    value.fader = 0.42f;
    value.mute = true;

    omc.send(oms, fader, {0.75f});
    omc.send(oms, complexPath, {valueOf(value)});

    auto end = std::chrono::steady_clock::now() + std::chrono::seconds(1);
    while (std::chrono::steady_clock::now() < end) omc.poll(50);
}
