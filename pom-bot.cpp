#include <kipr/kipr.h>
#include <iostream>

// WHEEL ADJUSTMENTS

// Making it go straight:
const float RIGHT_WHEEL_ADJUSTMENT = 0.875; // Veering to left? Pull right side back, decrease value
                                                  // Veering to right? Push right side forward, increase value

// Making it go the right distance
const float FORWARD_DISTANCE_ADJUSTMENT = 0.222222222; // Make robot move 5 inches.
                                             // If not enough distance, increase this value
                                             // If too much, decrease this value

const float BACKWARD_DISTANCE_ADJUSTMENT = 0.222222222; // Make robot move 5 inches.
                                              // If not enough distance, increase this value
                                              // If too much, decrease this value

// Making it turn the right amount
// Make the robot turn 90 degrees. Adjust values accordingly
const float CLOCKWISE_TURNING_ADJUSTMENT = 1; // Turn too much --> decrease value
                                              // Turn too little --> decrease value

const float COUNTER_CLOCKWISE_TURNING_ADJUSTMENT = 1.03; // Turn too much --> decrease value
                                                      // Turn too little --> decrease value


// PORTS
const int ARM_PORT = 0;
const int CLAW_PORT = 1;
const int LEFT_MOTOR_PORT = 3;
const int RIGHT_MOTOR_PORT = 0;

// POSITIONS
const int RAISED_POSITION = 660; //FIX THE NUMBER
const int FULLY_RAISED = 600; 
const int HALF_LOWERED = 1400;
const int LOWERED_POSITION = 2020; //FIX THE NUMBER
const int CLOSED_POSITION = 800; //FIX THE NUMBER
const int OPEN_POSITION = 2047; //FIX THE NUMBER

const double PI = 3.141592654;
                                            


// const float ADDITIONAL_TURN_PERCENT = 0.01;
// const float LEFT_TICKS_PER_INCH_TURN = LEFT_TICKS_PER_INCH;
// const float RIGHT_TICKS_PER_INCH_TURN = RIGHT_TICKS_PER_INCH;

//_____________________________________________________________________________________________

// const double LEFT_TICKS_PER_INCH_BACKWARDS = 237.5;
// const double RIGHT_TICKS_PER_INCH_BACKWARDS = LEFT_TICKS_PER_INCH_BACKWARDS*1.216;


// const double ARM_DOWN_LEFT_TICKS_PER_INCH = LEFT_TICKS_PER_INCH;
// const double ARM_DOWN_RIGHT_TICKS_PER_INCH = RIGHT_TICKS_PER_INCH;

// const double ARM_DOWN_LEFT_TICKS_PER_INCH_BACKWARDS = LEFT_TICKS_PER_INCH_BACKWARDS;
// const double ARM_DOWN_RIGHT_TICKS_PER_INCH_BACKWARDS = RIGHT_TICKS_PER_INCH_BACKWARDS;

// //const double TURN_AROUND_LEFT_WHEEL_ARM_RAISED_OFFSET = -140;
// //const double TURN_AROUND_LEFT_WHEEL_ARM_LOWERED_OFFSET = -70;

// //const double TURN_AROUND_RIGHT_WHEEL_ARM_RAISED_OFFSET = -130;
// //const double TURN_AROUND_RIGHT_WHEEL_ARM_LOWERED_OFFSET = -140;

// const double TURN_AROUND_LEFT_WHEEL_ARM_RAISED_OFFSET = 0;
// const double TURN_AROUND_LEFT_WHEEL_ARM_LOWERED_OFFSET = 0;

// const double TURN_AROUND_RIGHT_WHEEL_ARM_RAISED_OFFSET = 0;
// const double TURN_AROUND_RIGHT_WHEEL_ARM_LOWERED_OFFSET = 0;

void slowly_set_servo_position(int pin, int position, int wait_delay_ms=5) {
    int initial_pos = get_servo_position(pin);

    while (initial_pos > position ? get_servo_position(pin) > position : get_servo_position(pin) < position) {
        set_servo_position(pin, get_servo_position(pin) + (initial_pos > position ? -10 : 10) );
        msleep(wait_delay_ms);
    }
}

void raise_arm()
{
    slowly_set_servo_position(ARM_PORT, RAISED_POSITION);
}

void fully_raise_arm()
{
    slowly_set_servo_position(ARM_PORT, FULLY_RAISED);
}


void half_lower_arm()
{
    slowly_set_servo_position(ARM_PORT, HALF_LOWERED);
}

void lower_arm()
{
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
        1000*(speed_in_inches_per_sec/5)*RIGHT_WHEEL_ADJUSTMENT*direction
    );

    if (direction == 1) 
        msleep(1000*abs(distance_in_inches)*FORWARD_DISTANCE_ADJUSTMENT);
    else
        msleep(1000*abs(distance_in_inches)*BACKWARD_DISTANCE_ADJUSTMENT);
    
    stop();
}

void turn(float direction, float speed_in_inches_per_sec, float degrees) { //direction is 1 or -1, 1 for clockwise, -1 for counter-c
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
        msleep(13.25*degrees*CLOCKWISE_TURNING_ADJUSTMENT);
    else
        msleep(13.25*degrees*COUNTER_CLOCKWISE_TURNING_ADJUSTMENT);

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
    move_linear(-13.1,5);    
    
    
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
    move_linear(29,5);

    p("Backing up 4 inches because the pile of poms is too far back.");
    move_linear(-2, 5);

    
    
    // Slide 10
    p("Slide 10");
    p("Picking up the first set of poms");
    close_claw();
    raise_arm();
    
    
    
    // Slide 11
    p("Slide 11");
    p("Move forward a bit to line up with the trays");
    move_linear(6,5);
    
    
   
    // Slide 12
    p("Slide 12");
    p("Turn towards the trays");
    turn(1,5,90);
    

    
    // Slide 13
	p("Slide 13");
    p("Move toward the trays");
    move_linear(6.5,5);
    
    
    // Slide 14
    p("Slide 14");
    p("Dropping the poms");
    half_lower_arm();
    open_claw();
    
      //Slide 17
    p("Slide 17");
    p("Raising arm back up");
    raise_arm();
    close_claw();

    
    
    p("Unit 3");
    //wait_for_button()();
    
    
    






    // UNIT 3 
    //Slide 18
    p("Slide 18");
    move_linear(-3,5);
    //wait_for_button()();

    //Slide 19
    p("Slide 19");
    turn(-1,5,90);
    move_linear(-3,5);
    //wait_for_button()();
    //Slide 20
    p("Slide 20");
    move_linear(2.5,5); 
    //wait_for_button()();

    //Slide 21
    p("Slide 21");
    open_claw();
    lower_arm();
    //wait_for_button()();
    
    //Slide 22
    p("Slide 22");
    turn(-1,5,45);
    //wait_for_button()();
    
    //Slide 23
    p("Slide 23");
    move_linear(2.3,5);
//wait_for_button()();
    //Slide 24
    p("Slide 24");
    turn(-1,5,25);
    //move_linear(6, X); //FIX THE NUMBER
    
    move_linear(2,5);
//wait_for_button()();
    //Slide 25
    p("Slide 25");
    close_claw();
    raise_arm();
//wait_for_button()();
    
    //Slide 26
    p("Slide 26. Turning towards the tray to drop off the vertical set of poms");
    turn(1,5,45);
    move_linear(3.5,5);
    turn(1,5,90);
    
    
    //wait_for_button()();
    //Slide 27
    p("Slide 27");
    move_linear(9,5); // DROPPING THE VERTICAL SET OF POMS
    move_linear(-2,5);
    //wait_for_button()();

    //Slide 28
    p("Slide 28");
    half_lower_arm();
    open_claw();

    
    
    // GETTING THE PICKLES

    
    
    move_linear(-8.5,5);
    raise_arm();
    
    close_claw();
    
    p("We are turning to get to the pickles"); 
   turn(-1,5,90);
    
    move_linear(9,5);
    
    turn(-1,5,90);
    
    // BACK UP AND RAM INTO THE WALL
    move_linear(-3,5);
    
    move_linear(13,5);
    
    move_linear(-1.75,5);
    
    
    for(int servoPos = get_servo_position(ARM_PORT); servoPos>450; servoPos-=3){
        set_servo_position(ARM_PORT, servoPos);
        msleep(1);
    }
    msleep(1000);
    set_servo_position(CLAW_PORT, 1300);
    msleep(1000);
    lower_arm();
    msleep(500);
    close_claw();
    raise_arm();
    
    
    // DROP OFF THE PICKLE
    
    // Move forward a bit so that we can align
    move_linear(3,5);
    turn(-1,5,90);
    msleep(7000);
    
   
    move_linear(7.5,5);
    
    turn(-1,5,90);

    // Move forward towards the trays
    move_linear(11,5);
    move_linear(-2,5);
    
    // Drop the pickle
    half_lower_arm();
    open_claw();

    move_linear(2.1,5);

    // Wait for the other robot.
    msleep(13000);


    /* GET THE THIRD POM SET */

    raise_arm();

    close_claw();

    // Back up away from the trays
    move_linear(-5.5,5);

    

    // Turn towards the third pom set.
    turn(-1,5,90);

    // Move towards the third pom set.
    move_linear(11,5);
    
    open_claw();
    lower_arm();
    
    move_linear(4,5);
    move_linear(-2,5);

    // Grab the poms.
    open_claw();
    close_claw();
    raise_arm();




    /* PUT THE LAST POM SET IN THE THIRD TRAY. */

    // Back up. This is moving left.
    move_linear(-8,5);
    
    // Turn towards the trays, then move towards them.
    turn(1,5,90);
    move_linear(8.5,5);
    move_linear(-3.5,5);

    // Drop the poms.
    half_lower_arm();
    open_claw();


    
    return 0;
  
    
    // UNIT 5: GETTING THE PICKLE
    
    p("Slide 37. Move claw to 1300. This makes the claw as small as possible so it doesn't bump into anything but also wide enough to not hit the pickle.");
    set_servo_position(CLAW_PORT, 1300);
    msleep(1000);
   
    
    p("Slide 32. Turn around right wheel -90 degrees");
    turn(1,5,90);
    //wait_for_button();
    
    p("Slide 33. Back up 6 inches. This is uh towards the right side.");
   	move_linear(-9,5);
    //wait_for_button();
    
    p("Slide 34. Turn around right wheel -90 degrees so that we are facing the pickle");
   	turn(1,5,90);
    //wait_for_button();
    
    p("Slide 35. Move forward until bump into wall. Then back up.");
   	move_linear(4,5);
    move_linear(-2,5);
    //wait_for_button();
    
    p("Slide 38. Lower arm");
    lower_arm();
    set_servo_position(ARM_PORT, 0);
    msleep(500);
    
    p("Slide 39. Close claw. Raise arm. Back out.");
    close_claw();
    raise_arm();
    move_linear(-5,5);
    
    p("Slide 40. Turn around -90 degrees around the right wheel");
    turn(-1,5,90);
    
    p("Slide 41. Move forward 4.5 inches.");
    move_linear(4.5,5);
    
    p("Slide 42. Turn around left wheel -90 degrees.");
    turn(-1,5,90);
    
    p("Slide 43. Move forward so that the pickle is over the trays");
    move_linear(3,5);
    
    p("Slide 44. Drop the pickle.");
    half_lower_arm();
    open_claw();
    
    // TOMATO
    
    p("Back up towards the tomato so that we are aligned.");
    move_linear(-12,5);
    
    
    //return 0;
    
    

    // BLOCK OF CODE THAT GRABS AND THEN PLACES THE PICKEL
    
  //wait_for_button();
    raise_arm();
     //wait_for_button();
    close_claw();
      // wait_for_button();
    
    move_linear(-8,5);
       //wait_for_button();
    turn(1,5,90);
      wait_for_button();
    move_linear(-2.5,5);
       //wait_for_button();
    turn(1,5,90);
       //wait_for_button();
    set_servo_position(ARM_PORT, 400);
    msleep(1000);
       //wait_for_button();
	move_linear(3,5);
       //wait_for_button();
    move_linear(-2,5);
       //wait_for_button();
    set_servo_position(CLAW_PORT, 600);
    msleep(1000);
       //wait_for_button();
    lower_arm();
       //wait_for_button();
    close_claw();
    
    p("RAISING ARM");
    raise_arm();
    p("OOK");
    
   
	turn(-1,5,90);
    move_linear(7.5,5);
    turn(-1,5,90);
    move_linear(7.5,5);
    half_lower_arm();
    open_claw();
    
    // dropped pickle
    p("dropped pickle");
    
    
    // BLOCK OF CODE THAT GETS THE TOMATO
    
    //wait_for_button();
    raise_arm();
     //wait_for_button();
    close_claw();
      // wait_for_button();
    
    move_linear(-8,5);
       //wait_for_button();
    turn(1,5,90);
      // wait_for_button();
    move_linear(-2.5,5);
       //wait_for_button();
    turn(1,5,90);
       //wait_for_button();
    set_servo_position(ARM_PORT, 400);
    msleep(1000);
       //wait_for_button();
	move_linear(3,5);
       //wait_for_button();
    move_linear(-4,5);
       //wait_for_button();
    set_servo_position(CLAW_PORT, 600);
    msleep(1000);
       //wait_for_button();
    lower_arm();
       //wait_for_button();
    close_claw();
    
    p("RAISING ARM");
    raise_arm();
    p("OOK");
   
	turn(-1,5,90);
    move_linear(7.5,5);
    turn(-1,5,90);
    move_linear(7.5,5);
    half_lower_arm();
    open_claw();
    
    
    //return 0;
    
    
    p("unit 5");
    // UNIT 5
    
    close_claw();
    raise_arm();
    move_linear(-11.3,5);
    
    turn(1,5,95);
    
    move_linear(26.5,5);
    turn(-1,5,180);
    
    //wait_for_button()();
    turn(1,5,5);
    open_claw();
    lower_arm();
    
    move_linear(5,5);
    
    /*
    
   	// UNIT 3 
    
    //wait_for_button()();
    // Slide 16
    p("Slide 16");
    raise_arm();
    
    //wait_for_button()();
    //Slide 17
    p("Slide 17");
    move_linear(6, -6.5);
	
    //wait_for_button()();
    //Slide 18
    p("Slide 18");
    turn(-1,5,90);
	    
    //wait_for_button()();
    //Slide 19
    p("Slide 19");
    move_linear(6, -3);
    
    //wait_for_button()();
    // Slide 20
    p("Slide 20");
    lower_arm();
    close_claw();
    
    //wait_for_button()();
    // Slide 21
    p("Slide 21");
    raise_arm();
    
    //wait_for_button()();
    // Slide 22
    p("Slide 22");
    move_linear(6, 13.75, 0);
    
    //wait_for_button()();
    // Slide 23
    p("Slide 23");
    turn(-1,5,90);
    
    
    
    return 0;
    
    //wait_for_button()();
    // Slide 20
    p("Slide 20");
    lower_arm();
    
    //wait_for_button()();
    // Slide 21
    p("Slide 21");
    
    return 0;
    
    
    
    //Slide 20
    p("Slide 20");
    move_linear(6, X); //FIX THE NUMBER

    //Slide 21
    p("Slide 21");
    turn(1,5,90);
    
    //Slide 22
    p("Slide 22");
    close_claw();
    
    //Slide 23
    p("Slide 23");
    turn(-1,5,90);

    //Slide 24
    p("Slide 24");
    move_linear(6, X); //FIX THE NUMBER

    //Slide 25
    p("Slide 25");
    turn(1,5,90);

    //Slide 26
    p("Slide 26");
    move_linear(6, X); //FIX THE NUMBER
    lower_arm();
    
    //Slide 27
    p("Slide 27");
    open_claw();
   
*/
    
}