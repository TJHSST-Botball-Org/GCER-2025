#include <libkipr>
#include <iostream>

const int LEFT_MOTOR_PIN = 3;
const int RIGHT_MOTOR_PIN = 0;

const float LEFT_TICKS_PER_INCH = 228.2623717;
const float RIGHT_TICKS_PER_INCH = 238.9998057;


// Print function
template <typename T>
void print(const T& value) {
    std::cout << value << std::endl;
}

float left_target_speed = speed_in_inches_per_sec*LEFT_TICKS_PER_INCH
float left_spool_up_time = left_target_speed/10.0;
float left_spool_up_distance = 0.5*10.0*left_spool_up_time*left_spool_up_time;
float left_spool_down_distance = left_target_speed*left_spool_up_time - left_spool_up_distance;

void drive_linear(float distance_in_inches, float speed_in_inches_per_sec) {
    print("Expecting left" + std::to_string(distance_in_inches*LEFT_TICKS_PER_INCH));
    print("Expecting right" + std::to_string(distance_in_inches*RIGHT_TICKS_PER_INCH));

    move_relative_pos(
        LEFT_MOTOR_PIN, 
        speed_in_inches_per_sec*LEFT_TICKS_PER_INCH, 
        distance_in_inches*LEFT_TICKS_PER_INCH
    );

    move_relative_pos(
        RIGHT_MOTOR_PIN,
        speed_in_inches_per_sec*RIGHT_TICKS_PER_INCH,
        distance_in_inches*RIGHT_TICKS_PER_INCH
    );
}

int main() {
    cmpc(LEFT_MOTOR_PIN);
    cmpc(RIGHT_MOTOR_PIN);

    print("Before left " + gmpc(LEFT_MOTOR_PIN));
    print("Before right " + gmpc(RIGHT_MOTOR_PIN));

    drive_linear(5);

    print("After left " + gmpc(LEFT_MOTOR_PIN));
    print("After right " + gmpc(RIGHT_MOTOR_PIN));
}