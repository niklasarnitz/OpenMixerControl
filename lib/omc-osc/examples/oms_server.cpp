#include "omc-osc/omc_osc.hpp"

#include "example_complex_message.hpp"

#include <atomic>
#include <csignal>
#include <iostream>

using namespace omc::osc;

static std::atomic<bool> running{true};

int main()
{
    std::signal(SIGINT, [](int) { running = false; });

    Peer oms(9000);
    auto fader = OMC.faders.getValue(faderNumber(1));
    Path complex{"/complex"};

    oms.route(fader, [&](Peer& peer, const Message& msg) {
        auto value = msg.arg<float>(0);
        if (!value) return;

        std::cout << "fader 1 = " << *value << "\n";
        peer.send(msg.remote, fader, {*value});
    });

    oms.route(complex, [&](Peer& peer, const Message& msg) {
        ExampleComplexMessage value;
        if (msg.args.empty() || !readValue(msg.args[0], value)) return;

        std::cout << "complex fader=" << value.fader << " mute=" << value.mute << "\n";
        peer.send(msg.remote, complex, {valueOf(value)});
    });

    std::cout << "oms on 9000\n";
    while (running) oms.poll(100);
}
