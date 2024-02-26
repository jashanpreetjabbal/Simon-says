/*Simon Says Game with Arduino

  This program simulates the classic Simon Says game using Arduino. The game
  generates a random sequence of lights and sounds, and the player must replicate
  the sequence by pressing the corresponding buttons.

  The circuit:
  - LED attached from LED_PIN_L to ground
  - LED attached from LED_PIN_R to ground
  - Speaker attached to SPEAKER_PIN
  - Pushbutton attached to BUTTON_PIN_L from +5V
  - Pushbutton attached to BUTTON_PIN_R from +5V
  - 10K resistor attached to BUTTON_PIN_L from ground
  - 10K resistor attached to BUTTON_PIN_R from ground

  Instructions:
  - Connect the circuit based on the provided instructions.
  - Upload this code to your Arduino board.
  - Open the Serial Monitor to view the game patterns and user input.

  Author: Avipreet Singh, Jashanpreet, Falak
  Date: November 30, 2023
*/

// Constants won't change. They're used here to set pin numbers:
const int BUTTON_PIN_L = 7;     // Left button pin
const int BUTTON_PIN_R = 8;     // Right button pin
const int SPEAKER_PIN = 9;      // Speaker pin
const int LED_PIN_L = 12;       // Left LED pin
const int LED_PIN_R = 13;       // Right LED pin

static int Level = 1;  // Initial game level

// Function prototypes
void CallLights(char gamepattern[], int level);
void ButtonCheck(char userpattern[], int level);
int WinCheck(char gamepattern[], char userpattern[], int level);

void setup() {
    // Initialize pin modes:
    pinMode(LED_PIN_L, OUTPUT);
    pinMode(LED_PIN_R, OUTPUT);
    pinMode(SPEAKER_PIN, OUTPUT);
    pinMode(BUTTON_PIN_L, INPUT);
    pinMode(BUTTON_PIN_R, INPUT);

    // Seed the random number generator:
    randomSeed(analogRead(0));

    // Initialize serial communication:
    Serial.begin(9600);
}

void loop() {
    // Arrays to store game and user patterns:
    char gamepattern[Level];
    char userpattern[Level];

    // Generate and display the game pattern:
    CallLights(gamepattern, Level);

    // Play a tone as a signal for the player to start:
    tone(SPEAKER_PIN, 1000, 1000);
    delay(1000);

    // Allow the player to input their pattern:
    ButtonCheck(userpattern, Level);

    // Print the generated game pattern:
    Serial.print("Game Pattern: ");
    for (int i = 0; i < Level; i++) {
        Serial.print(gamepattern[i]);
    }
    Serial.println();

    // Print the player's input pattern:
    Serial.print("User Pattern: ");
    for (int i = 0; i < Level; i++) {
        Serial.print(userpattern[i]);
    }
    Serial.println();

    // Check if the player's input matches the game pattern:
    if (WinCheck(gamepattern, userpattern, Level) == 1) {
        // Player wins, play a winning tone:
        tone(SPEAKER_PIN, 2000, 500);
        delay(1000);  // Wait for 1 second after winning
        Level = Level + 1;  // Move to the next level
    } else {
        // Player loses, play a losing tone:
        tone(SPEAKER_PIN, 100, 2000);
        delay(1000);  // Wait for 1 second after losing
        Level = 1;  // Restart the game at level 1
    }

    delay(1000);  // Add delay before starting the next round
}

void CallLights(char gamepattern[], int level) {
    // Generate and display the game pattern of lights:
    for (int i = 0; i < level; i++) {
        int randomnumber = random(2);
        if (randomnumber == 0) {
            // Activate the left LED:
            digitalWrite(LED_PIN_L, HIGH);
            delay(500);  // LED ON for 0.5 seconds
            digitalWrite(LED_PIN_L, LOW);
            gamepattern[i] = 'L';  // Store the pattern
        } else {
            // Activate the right LED:
            digitalWrite(LED_PIN_R, HIGH);
            delay(500);  // LED ON for 0.5 seconds
            digitalWrite(LED_PIN_R, LOW);
            gamepattern[i] = 'R';  // Store the pattern
        }
        delay(500);  // Add delay between LED sequences
    }
}

void ButtonCheck(char userpattern[], int level) {
    // Check the buttons for user input:
    int buttonState_L, buttonState_R;
    unsigned long time1;

    for (int i = 0; i < level; i++) {
        time1 = millis();

        // Wait for a button press or timeout after a certain period:
        while (millis() - time1 < 3000) {
            // Set button states inside the loop:
            buttonState_L = digitalRead(BUTTON_PIN_L);
            buttonState_R = digitalRead(BUTTON_PIN_R);

            if (buttonState_L == HIGH || buttonState_R == HIGH) {
                break;  // Exit the loop when a button is pressed
            }
        }

        // Check which button was pressed:
        if (buttonState_L == HIGH) {
            userpattern[i] = 'L';  // Store the input
        } else if (buttonState_R == HIGH) {
            userpattern[i] = 'R';  // Store the input
        }

        delay(200);  // Add a small delay to debounce the button
    }
}

int WinCheck(char gamepattern[], char userpattern[], int level) {
    // Check if the user's input matches the game pattern:
    for (int i = 0; i < level; i++) {
        if (gamepattern[i] != userpattern[i]) {
            return 0;  // Return 0 if patterns don't match
        }
    }
    return 1;  // Return 1 if patterns match
}
