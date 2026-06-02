#pragma once

#include "omc-osc/endpoint.hpp"
#include "omc-osc/message.hpp"
#include "omc-osc/path.hpp"
#include "omc-osc/result.hpp"
#include "omc-osc/value.hpp"

#include <functional>
#include <memory>
#include <vector>

namespace omc::osc {

class Peer {
public:
    using Handler = std::function<void(Peer&, const Message&)>;

    explicit Peer(uint16_t listenPort);
    ~Peer();

    Peer(const Peer&) = delete;
    Peer& operator=(const Peer&) = delete;

    Result route(Path path, Handler handler);
    Result send(const Endpoint& endpoint, Path path, std::vector<Value> args = {});
    Result poll(int timeoutMs);

private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
};

} // namespace omc::osc
