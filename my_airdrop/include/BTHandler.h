#pragma once

#include <string>
#include <memory>

class BTHandler {
    public :
        virtual ~BTHandler() = default;

        virtual void start_advertising(const std::string &name) = 0;
        virtual void stop_advertising() = 0;

        static std::unique_ptr<BTHandler> create();
};

