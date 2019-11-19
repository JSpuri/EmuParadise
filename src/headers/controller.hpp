#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include <cstdlib>
#include <cstdint>
#include<string>

class Controller {

    public:

        Controller();


        void ToggleStrobe(uint8_t value);

        uint8_t ReadJoypad1();
        void PressButtonJP1(char button);
        void ReleaseButtonJP1(char button);

        uint8_t ReadJoypad2();
        void PressButtonJP2(char button);
        void ReleaseButtonJP2(char button);

    private:

        uint8_t joypad1;
        bool strobe1;
        uint8_t counter1;

        uint8_t joypad2;
        bool strobe2;
        uint8_t counter2;

};

#endif
