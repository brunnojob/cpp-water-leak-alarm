# cpp-water-leak-alarm

C++17 leak signal detector with a three-sample confirmation window to reduce false alarms. Input values represent a 12-bit moisture sensor reading; sustained values at or above 650 trigger an alert.

Build: `c++ -std=c++17 -O2 -Wall -Wextra -Werror src/main.cpp -o leak-detector`. Run `./leak-detector 100 700 720 710`.

Project by [Brunno Dev](https://brunnodev.store).