#include "headers/controller.hpp"

Controller::Controller() {

    this->joypad1 = 0;
    this->counter1 = 8;
    this->strobe1 = false;

    this->joypad2 = 0;
    this->counter2 = 8;
    this->strobe2 = false;

}

void Controller::ToggleStrobe(uint8_t value) {

    if(value & 0x01){

        this->strobe1 = true;
        this->counter1 = 8;

        this->strobe2 = true;
        this->counter2 = 8;
    }
    else{

        this->strobe1 = false;
        this->strobe2 = false;
    }
}

uint8_t Controller::ReadJoypad1() {

    if(this->strobe1)
        return this->joypad1 & 0x01;

    else if(this->counter1 > 0){

        uint8_t button = this->joypad1 & 0x1;
        this->joypad1 = this->joypad1 >> 1;
        this->counter1 -= 1;

        return button;
    }

    return 0x01;
}

void Controller::ReleaseButtonJP1(char button) {

    printf("JOYPAD 1: %02x\n", this->joypad1);
    if(this->strobe1){
        switch(button){

            case 'A':
                this->joypad1 &= 0xFE;
                break;
            case 'B':
                this->joypad1 &= 0xFD;
                break;
            case 's':                   // Select
                this->joypad1 &= 0xFB;
                break;
            case 'S':                   // Start
                this->joypad1 &= 0xF7;
                break;
            case 'U':
                this->joypad1 &= 0xEF;
                break;
            case 'D':
                this->joypad1 &= 0xDF;
                break;
            case 'L':
                this->joypad1 &= 0xBF;
                break;
            case 'R':
                this->joypad1 &= 0x7F;
                break;
        }
    }
}
void Controller::PressButtonJP1(char button) {

    printf("JOYPAD 1: %02x\n", this->joypad1);
    if(this->strobe1){
        switch(button){

            case 'A':
                this->joypad1 |= 0x01;
                break;
            case 'B':
                this->joypad1 |= 0x02;
                break;
            case 's':                   // Select
                this->joypad1 |= 0x04;
                break;
            case 'S':                   // Start
                this->joypad1 |= 0x08;
                break;
            case 'U':
                this->joypad1 |= 0x10;
                break;
            case 'D':
                this->joypad1 |= 0x20;
                break;
            case 'L':
                this->joypad1 |= 0x40;
                break;
            case 'R':
                this->joypad1 |= 0x80;
                break;
        }
    }
}

uint8_t Controller::ReadJoypad2() {

    if(this->strobe2)
        return this->joypad2 & 0x01;

    else if(this->counter2 > 0){

        uint8_t button = this->joypad2 & 0x1;
        this->joypad2 = this->joypad2 >> 1;
        this->counter2 -= 1;

        return button;
    }

    return 0x01;
}

void Controller::ReleaseButtonJP2(char button) {

    if(this->strobe2){
        switch(button){

            case 'A':
                this->joypad2 &= 0xFE;
                break;
            case 'B':
                this->joypad2 &= 0xFD;
                break;
            case 's':                   // Select
                this->joypad2 &= 0xFB;
                break;
            case 'S':                   // Start
                this->joypad2 &= 0xF7;
                break;
            case 'U':
                this->joypad2 &= 0xEF;
                break;
            case 'D':
                this->joypad2 &= 0xDF;
                break;
            case 'L':
                this->joypad2 &= 0xBF;
                break;
            case 'R':
                this->joypad2 &= 0x7F;
                break;
        }
    }

}
void Controller::PressButtonJP2(char button) {

    if(this->strobe2){
        switch(button){

            case 'A':
                this->joypad2 |= 0x01;
                break;
            case 'B':
                this->joypad2 |= 0x02;
                break;
            case 's':                   // Select
                this->joypad2 |= 0x04;
                break;
            case 'S':                   // Start
                this->joypad2 |= 0x08;
                break;
            case 'U':
                this->joypad2 |= 0x10;
                break;
            case 'D':
                this->joypad2 |= 0x20;
                break;
            case 'L':
                this->joypad2 |= 0x40;
                break;
            case 'R':
                this->joypad2 |= 0x80;
                break;
        }
    }

}

