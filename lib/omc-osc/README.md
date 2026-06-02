# omc-osc

tiny c++ wrapper around liblo for omc/oms osc stuff.

the point is: dont type paths as strings all over the app.

```cpp
using namespace omc::osc;

Peer omc(9001);
Endpoint oms{"127.0.0.1", 9000};

auto fader = OMC.faders.getValue(faderNumber(1));

omc.route(fader, [](Peer&, const Message& msg) {
    auto value = msg.arg<float>(0);
});

omc.send(oms, fader, {0.75f});
```

complex values inherit from `Serializable`.

```cpp
class MyThing : public Serializable {
public:
    Blob serialize() const override;
    bool deserialize(const Blob& blob) override;
};
```

build:

```sh
make
```

examples:

```sh
./build/oms_server
./build/omc_client
```

not fancy yet. just enough to keep osc paths and values sane while the real oms api settles.
