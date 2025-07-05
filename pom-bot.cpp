#include <kipr/kipr.h>
#include <iostream>

bool is_arm_down = false;

// WHEEL ADJUSTMENTS

// Making it go straight:
const float RIGHT_WHEEL_ADJUSTMENT = 0.87; // Veering to left? Pull right side back, decrease value
                                                  // Veering to right? Push right side forward, increase value

const float ARM_DOWN_RIGHT_WHEEL_ADJUSTMENT = 0.86;

// Making it go the right distance
const float FORWARD_DISTANCE_ADJUSTMENT = 0.227920228; // Make robot move 5 inches.
                                             // If not enough distance, increase this value
                                             // If too much, decrease this value

const float BACKWARD_DISTANCE_ADJUSTMENT = 0.227920228; // Make robot move 5 inches.
                                              // If not enough distance, increase this value
                                              // If too much, decrease this value


// Making it go the right distance when the claw is down
const float ARM_DOWN_FORWARD_DISTANCE_ADJUSTMENT = 0.233918128; // Make robot move 5 inches.
                                             // If not enough distance, increase this value
                                             // If too much, decrease this value

const float ARM_DOWN_BACKWARD_DISTANCE_ADJUSTMENT = 0.233918128; // Make robot move 5 inches.
                                              // If not enough distance, increase this value
                                              // If too much, decrease this value

// Making it turn the right amount
// Make the robot turn 90 degrees. Adjust values accordingly
const float CLOCKWISE_TURNING_ADJUSTMENT = 1; // Turn too much --> decrease value
                                              // Turn too little --> decrease value

const float COUNTER_CLOCKWISE_TURNING_ADJUSTMENT = 1.03; // Turn too much --> decrease value
                                                      // Turn too little --> decrease value

// Making it turn the right amount
// Make the robot turn 90 degrees. Adjust values accordingly
const float ARM_DOWN_CLOCKWISE_TURNING_ADJUSTMENT = 0.965; // Turn too much --> decrease value
                                              // Turn too little --> decrease value

const float ARM_DOWN_COUNTER_CLOCKWISE_TURNING_ADJUSTMENT = 0.96; // Turn too much --> decrease value
                                                      // Turn too little --> decrease value


// PORTS
const int ARM_PORT = 0;
const int CLAW_PORT = 1;
const int LEFT_MOTOR_PORT = 3;
const int RIGHT_MOTOR_PORT = 0;
const int LEFT_TOPHAT_PORT = 2;
const int RIGHT_TOPHAT_PORT = 0;
const int LEFT_TOPHAT_THRESHOLD = 3600;
const int RIGHT_TOPHAT_THRESHOLD = 3600;

// POSITIONS
const int RAISED_POSITION = 660; //FIX THE NUMBER
const int FULLY_RAISED = 600; 
const int HALF_LOWERED = 1770;
const int LOWERED_POSITION = 2020; //FIX THE NUMBER
const int CLOSED_POSITION = 800; //FIX THE NUMBER
const int OPEN_POSITION = 2047; //FIX THE NUMBER

const double PI = 3.141592654;
                                            
void slowly_set_servo_position(int pin, int position, int wait_delay_ms=10) {
    int initial_pos = get_servo_position(pin);

    while (initial_pos > position ? get_servo_position(pin) > position : get_servo_position(pin) < position) {
        set_servo_position(pin, get_servo_position(pin) + (initial_pos > position ? -10 : 10) );
        msleep(wait_delay_ms);
    }
}

void raise_arm()
{
    is_arm_down = false;
    slowly_set_servo_position(ARM_PORT, RAISED_POSITION);
}

void fully_raise_arm()
{
    is_arm_down = false;
    slowly_set_servo_position(ARM_PORT, FULLY_RAISED);
}


void half_lower_arm()
{
    is_arm_down = false;
    slowly_set_servo_position(ARM_PORT, HALF_LOWERED);
}

void lower_arm()
{
    is_arm_down = true;
    slowly_set_servo_position(ARM_PORT, LOWERED_POSITION);
}

void close_claw()
{
    set_servo_position(CLAW_PORT, CLOSED_POSITION);
    msleep(1000);
}

void open_claw()
{
    set_servo_position(CLAW_PORT, OPEN_POSITION);
    msleep(1000);
}

void p(std::string output)
{
    std::cout << output << std::endl;
}

void wait_for_button()
{
    std::cout << "Waiting for button." << std::endl;
    while (analog(5)>1000)
    {
        msleep(1);
    }
}

void stop() {
    freeze(LEFT_MOTOR_PORT);
    freeze(RIGHT_MOTOR_PORT);
    msleep(100);
}

void move_linear(float distance_in_inches, float speed_in_inches_per_sec) {
    cmpc(LEFT_MOTOR_PORT);
    cmpc(RIGHT_MOTOR_PORT);

    int direction = distance_in_inches < 0 ? -1 : 1;

    move_at_velocity(
        LEFT_MOTOR_PORT, 
        1000*(speed_in_inches_per_sec/5)*direction
    );

    move_at_velocity(
        RIGHT_MOTOR_PORT,
        1000*(speed_in_inches_per_sec/5)*(is_arm_down ? ARM_DOWN_RIGHT_WHEEL_ADJUSTMENT : RIGHT_WHEEL_ADJUSTMENT)*direction
    );

    if (direction == 1) 
        msleep(1000*abs(distance_in_inches)*(is_arm_down ? ARM_DOWN_FORWARD_DISTANCE_ADJUSTMENT : FORWARD_DISTANCE_ADJUSTMENT));
    else
        msleep(1000*abs(distance_in_inches)*(is_arm_down ? ARM_DOWN_BACKWARD_DISTANCE_ADJUSTMENT : BACKWARD_DISTANCE_ADJUSTMENT));
    
    stop();
}

void move_forward_until_black_line() {
    /* Moves forward until rear tophats align with black tape. 
    Not 100% accurate, but probably better than nothing. */
    
    cmpc(LEFT_MOTOR_PORT);
    cmpc(RIGHT_MOTOR_PORT);

    move_at_velocity(
        LEFT_MOTOR_PORT, 
        1000
    );

    move_at_velocity(
        RIGHT_MOTOR_PORT,
        1000*(is_arm_down ? ARM_DOWN_RIGHT_WHEEL_ADJUSTMENT : RIGHT_WHEEL_ADJUSTMENT)
    );

    while (!(analog(LEFT_TOPHAT_PORT) > LEFT_TOPHAT_THRESHOLD && analog(RIGHT_TOPHAT_PORT) > RIGHT_TOPHAT_THRESHOLD)) {
        // While not BOTH on black line

        if (analog(LEFT_TOPHAT_PORT) > LEFT_TOPHAT_THRESHOLD && !(analog(RIGHT_TOPHAT_PORT) > RIGHT_TOPHAT_THRESHOLD)) {
            p("Right white");
            
            // Only left side on the black line. Move only the right side forward.
            freeze(LEFT_MOTOR_PORT);

            move_at_velocity(
                RIGHT_MOTOR_PORT,
                1000*(is_arm_down ? ARM_DOWN_RIGHT_WHEEL_ADJUSTMENT : RIGHT_WHEEL_ADJUSTMENT)
            );
        } else if (!(analog(LEFT_TOPHAT_PORT) > LEFT_TOPHAT_THRESHOLD) && (analog(RIGHT_TOPHAT_PORT) > RIGHT_TOPHAT_THRESHOLD)) {
            p("Left white");    

            // Only right side on the black line. Move only the left side forward.
            move_at_velocity(
                LEFT_MOTOR_PORT, 
                1000
            );

            freeze(RIGHT_TOPHAT_PORT);
        } else {
            /* Neither on the black line */
            p("Both white");
            move_at_velocity(
                LEFT_MOTOR_PORT, 
                1000
            );

            move_at_velocity(
                RIGHT_MOTOR_PORT,
                1000*(is_arm_down ? ARM_DOWN_RIGHT_WHEEL_ADJUSTMENT : RIGHT_WHEEL_ADJUSTMENT)
            );
        }

        msleep(200);
    }
    
    stop();
}

void turn(float direction, float speed_in_inches_per_sec, float degrees) { 
    /* Direction is 1 or -1, 1 for clockwise, -1 for counter-c */

    cmpc(LEFT_MOTOR_PORT);
    cmpc(RIGHT_MOTOR_PORT);

    move_at_velocity(
        LEFT_MOTOR_PORT, 
        1000*direction
    );

    move_at_velocity(
        RIGHT_MOTOR_PORT,
        1000*RIGHT_WHEEL_ADJUSTMENT*direction*-1
    );

    if (direction == 1) 
        msleep(13.25*degrees*(is_arm_down ? ARM_DOWN_CLOCKWISE_TURNING_ADJUSTMENT : CLOCKWISE_TURNING_ADJUSTMENT));
    else
        msleep(13.25*degrees*(is_arm_down ? ARM_DOWN_COUNTER_CLOCKWISE_TURNING_ADJUSTMENT : COUNTER_CLOCKWISE_TURNING_ADJUSTMENT));

    stop();
}

int main()
{


    /*wait_for_light(0);
    shut_down_in(119);
*/
	enable_servos();

 	//disable_servo(0);
    
    //Calibration
	//move_linear(6,30,0);
    //move_linear(6, -30, 0);
    //turn_around_left_wheel(90, 0);
    
    ////wait_for_button()();
    // Slide 3
    
    
    p("Slide 3");
    raise_arm();
    turn(1,3,90);
    
    // Slide 4
    ////wait_for_button()();
    p("Slide 4");
    move_linear(-16,5);

   	// Slide 5
    ////wait_for_button()();
    p("Slide 5");
    turn(-1,3,90);
    

    // Slide 6
    ////wait_for_button()();
    p("Slide 6");
    move_linear(-14,5);    
    
    
    p("Unit 2");
    
    
    
    /*
    bb    bb bbb    bb bb bbbbbbbb     bbbbbb  
    bb    bb bbbb   bb bb    bb             bb 
    bb    bb bb bb  bb bb    bb         bbbbb  
    bb    bb bb  bb bb bb    bb        bb      
     bbbbbb  bb   bbbb bb    bb        bbbbbbb 
                                           
    */


    p("Moving to the right, to get the poms.");
    
   // Slide 8
    p("Slide 8");
    open_claw();
    lower_arm();

    ////wait_for_button()();
    // Slide 9
    p("Slide 9");
    move_linear(24.5,5);

    
    
    // Slide 10
    p("Slide 10");
    p("Picking up the first set of poms");
    close_claw();
    raise_arm();
    
    
    
    // Slide 11
    p("Slide 11");
    p("Move forward a bit to line up with the trays");
    move_linear(7.5,5);
    
    
   
    // Slide 12
    p("Slide 12");
    p("Turn towards the trays");
    turn(1,5,90);
    

    
    // Slide 13
	p("Slide 13");
    p("First set: Move toward the trays");
    move_linear(7.5,5);
    
    
    // Slide 14
    p("Slide 14");
    p("Dropping the poms");
    half_lower_arm();
    wait_for_button();
    open_claw();
    wait_for_button();
    
      //Slide 17
    p("Slide 17");
    p("Raising arm back up");
    raise_arm();
    close_claw();




    p("\n\n\n\nGetting the second set of poms, vertical.");

    p("Moving back 6 inches to get back on the center line.");
    move_linear(-7.5, 5);

    p("Rotate 90 degrees counter clockwise so that we are facing the right side of the board");
    turn(-1, 5, 90);
    wait_for_button();

    p("Back up a bit");
    move_linear(-1.5, 5);
    wait_for_button();
    
    p("Lower the arm");
    open_claw();
    lower_arm();
    wait_for_button();

    p("Sweep 26.5 degrees counter clockwise to get the red pom");
    turn(-1, 5, 26.5);
    wait_for_button();
    
    p("Move forward 2 inches. This is so that we push the other poms further");
    move_linear(2, 5);
    wait_for_button();

    p("Grab all three poms.");
    close_claw();
    raise_arm();
    wait_for_button();

    p("Turn CW until facing RIGHT. PLEASE EDIT THIS NUMBER TO HOW MUCH IS NECESSARY");
    turn(1, 5, 32);
    wait_for_button();

    p("Adjust left and right position until good. THE REAR OF THE CHASSIS MUST BE 23.63 INCHES FROM THE BLACK TAPE");
    move_linear(4.76, 5);
    wait_for_button();

    p("Turn CW until facing tray. PLEASE EDIT THIS NUMBER TO HOW MUCH IS NECESSARY");
    turn(1, 5, 90);
    wait_for_button();

    p("Move towards the tray. PLEASE EDIT THIS NUMBER BASED ON WHAT IS NECESSARY");
    move_linear(6, 5);
    wait_for_button();

    p("Drop the vertical poms");
    half_lower_arm();
    p("STOP. Draw a dot where the robot is right now. Figure out how much further the robot needs to move forward in order for the arm to be over the tray.");
    wait_for_button();

    open_claw();
    wait_for_button();

    raise_arm();
    close_claw();
    p("STOP. Ensure that the right side of the chassis is 27.5 inches from the inside edge of the black tape. THIS MUST BE INCORRECT IN ORDER FOR FOLLOWING CODE TO WORK. Please edit the code on line 335-336 in order to fix this part.");
    wait_for_button();





    p("\n\n\n\nGetting the final pom set");

    p("Moving back 7.35 inches to get back on the center line. ");
    move_linear(-4.85, 5);
    wait_for_button();

    p("Rotate 90 degrees counter clockwise so that we are facing the right side of the board. The right side chassis should be 2 inches south of the center line. If not, adjust the number on line 363");
    turn(-1, 5, 90);
    wait_for_button();

    // We cannot go over the bump with the arm lowered, but if we go over the 
    // bump then start grabbing stuff then we would go too far.
    // We will first pickup the orange pom, go over the bump, drop it, and then
    // Get the rest.


    // p("Move east for 1.36 inches so that we can reach the orange pom");
    // move_linear(1.25, 5);
    // wait_for_button();

    p("Get the orange pom");
    open_claw();
    lower_arm();
    wait_for_button();

    close_claw();
    raise_arm();
    wait_for_button();

    p("Get over the bump. Move east for 3.83 inches");
    move_linear(3.83, 5);
    wait_for_button();

    p("Drop orange pom, then continue moving");
    half_lower_arm();
    wait_for_button();

    open_claw();
    wait_for_button();

    lower_arm();
    wait_for_button();

    move_linear(6.2, 5);
    wait_for_button();

    p("Grab the poms.");
    close_claw();
    raise_arm();
    wait_for_button();

    p("Back up 5 inches.");
    move_linear(-5, 5);
    p("STOP. Ensure that the rear of the chassis is 29 inches away from the black tape. If not, adjust the value above");
    wait_for_button();

    p("Turn 90 degrees CW until facing towards the trays");
    turn(1, 5, 117);
    wait_for_button();

    p("Move towards the trays");
    move_linear(7.5, 5);
    wait_for_button();

    p("Drop the last set poms poms");
    half_lower_arm();
    p("STOP. Draw a dot where the robot is right now. Figure out how much further the robot needs to move forward in order for the arm to be over the tray.");
    wait_for_button();

    open_claw();
    wait_for_button();

    raise_arm();
    close_claw();
    wait_for_button();



    p("\n\n\n\n\nGetting the pickle.");
    
    p("Move back 12 inches");
    p("The robot should be close to touching the north wall by now.");
    move_linear(-12, 5);
    wait_for_button();

    p("Rotate to face east.");
    turn(-1, 5, 140);
    wait_for_button();

    p("Move forward until color sensors align with center line. Rear should be roughly on the LEFT EDGE of the black line at this point");
    move_forward_until_black_line();
    wait_for_button();

    p("Move forward 5.7 inches so that we align with the pickle");
    move_linear(5.7, 5);
    wait_for_button();

    p("Turn towards north.");
    turn(-1, 5, 90);
    wait_for_button();

    p("Move forward 2.5 inches until we can get the pickle.");
    move_linear(2.5, 5);
    wait_for_button();

    p("Get the pickle");
    open_claw();
    wait_for_button();

    lower_arm();
    p("STOP. Is the claw aligned? How much more do we need to move?");
    wait_for_button();

    close_claw();
    wait_for_button();

    raise_arm();
    wait_for_button();

    // The plan now is to
    // 1. Back up 2.63 inches so that we don't hit the north wall when we rotate later
    // 1. move west,
    // 2. Get over the bump
    // 3. line up with the black line
    // 4. Move back a bit because the line is too far
    // 5. Turn south, drop it off
    
    p("Back up 2.63 inches so that we don't hit the north wall later");
    move_linear(-2.63, 5);
    wait_for_button();

    p("Turn west");
    turn(-1, 5, 90);
    wait_for_button();

    p("Go forward, line up with the black line");
    move_forward_until_black_line();
    wait_for_button();

    p("Move back 1.45 inches because we gone too far");
    move_linear(-1.45, 5);
    wait_for_button();

    p("Turn south. Drop off the pickle");
    turn(-1, 5, 90);
    wait_for_button();

    move_linear(12, 5); // What line 454 should be.
    half_lower_arm();
    p("STOP. Is the arm at the right place? If not, please change the distance on line 454, above.");
    wait_for_button();

    open_claw();
    wait_for_button();





    p("Getting the tomato");

    p("Move back 12 inches");
    p("The robot should be close to touching the north wall by now.");
    move_linear(-12, 5);
    wait_for_button();

    p("Rotate to face east.");
    turn(-1, 5, 90);
    wait_for_button();

    p("Move forward until color sensors align with center line. Rear should be roughly on the LEFT EDGE of the black line at this point");
    move_forward_until_black_line();
    wait_for_button();
    
    p("Move an extra 1.61 inches to align on the east-west axis with the tomato.");
    move_linear(1.61, 5);
    wait_for_button();
    
    p("Turn north");
    turn(-1, 5, 90);
    wait_for_button();

    p("Move forward 2.5 inches to align with north-south axis");
    move_linear(2.5, 5);
    wait_for_button();

    p("Get the tomato");
    lower_arm();
    p("STOP. Is the arm aligned? Adjust values so that it is. Claw cannot touch PVC. Right side of claw must be around right side of tomato.");
    wait_for_button();

    close_claw();
    wait_for_button();

    raise_arm();
    wait_for_button();

    p("\nReverse the process.\n");

    p("Back up 2.63 inches so that we don't hit the north wall later");
    move_linear(-2.63, 5);
    wait_for_button();

    p("Turn west");
    turn(-1, 5, 90);
    wait_for_button();

    p("Go forward, line up with the black line");
    move_forward_until_black_line();
    wait_for_button();

    p("Go forward 4.22 inches to align on the east-west axis");
    move_linear(4.22, 5);
    wait_for_button();


    p("Turn south. Drop off the tomato");
    turn(-1, 5, 90);
    wait_for_button();

    move_linear(12, 5); // What line 454 should be.
    half_lower_arm();
    p("STOP. Is the arm at the right place? If not, please change the distance on line 454, above.");
    wait_for_button();

    open_claw();

    return 0;
    
}