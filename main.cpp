#include "PS2Keyboard.h"
#include "mbed.h"
#include "rtos.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "time.h"
#include "uLCD_4DGL.h"

#define NUM_WORDS 429
#define MAX_WORD_LEN 15
#define STRING_LEN 75

#define INITIAL_STR_LEN 1000 // Initial length of the running user string
#define MAX_STR_LEN 1000     // Maximum length of the running user string

PS2Keyboard ps2kb(p6, p5); // CLK, DAT
Serial pc(USBTX, USBRX);
uLCD_4DGL uLCD(p9, p10, p11);
AnalogIn noise(p20);
PwmOut speaker(p25);
Mutex lcd_mutex;

// variables
Timeout delay;
bool gameActive = true;
int timeCount = 0;
int isCorrect[20] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int correctPresses = 0;
int incorrectPresses = 0;
int currentIndex = 0;
int word_count = 0;
double currentWPM = 0.0;
double currentAccuracy = 100.0;
int userPos = 0;
char user_input; // Initialize the user input buffer as a single character

char *random_string =
    (char *)malloc(sizeof(char) * (NUM_WORDS * (MAX_WORD_LEN + 1)));
// //parsing charmap
char charMap[] = {
    '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
    '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', 'q',  '\0', '\0',
    '\0', '\0', 'z',  's',  'a',  'w',  '\0', '\0', '\0', 'c',  'x',  'd',
    'e',  '\0', '\0', '\0', '\0', ' ',  'v',  'f',  't',  'r',  '\0', '\0',
    '\0', 'n',  'b',  'h',  'g',  'y',  '\0', '\0', '\0', '\0', 'm',  'j',
    'u',  '\0', '\0', '\0', '\0', '\0', 'k',  'i',  'o',  '\0', '\0', '\0',
    '\0', '\0', '\0', 'l',  '\0', 'p',  '\0', '\0', '\0', '\0', '\0', '\0',
    '\0', '\0', '\0', '\0', '\0', '\0', '\n'};
const char *word_bank[NUM_WORDS] = {
    "time",        "year",         "people",      "way",
    "day",         "man",          "thing",       "woman",
    "life",        "child",        "world",       "school",
    "state",       "family",       "student",     "group",
    "country",     "problem",      "hand",        "part",
    "place",       "case",         "week",        "company",
    "system",      "program",      "question",    "work",
    "government",  "number",       "night",       "point",
    "home",        "water",        "room",        "mother",
    "area",        "money",        "story",       "fact",
    "month",       "lot",          "right",       "study",
    "book",        "eye",          "job",         "word",
    "business",    "issue",        "side",        "kind",
    "head",        "house",        "service",     "friend",
    "father",      "power",        "hour",        "game",
    "line",        "end",          "member",      "law",
    "car",         "city",         "community",   "name",
    "president",   "team",         "minute",      "idea",
    "kid",         "body",         "information", "back",
    "parent",      "face",         "others",      "level",
    "office",      "door",         "health",      "person",
    "art",         "war",          "history",     "party",
    "result",      "change",       "morning",     "reason",
    "research",    "girl",         "guy",         "moment",
    "air",         "teacher",      "force",       "education",
    "foot",        "boy",          "age",         "policy",
    "process",     "music",        "market",      "sense",
    "nation",      "plan",         "college",     "interest",
    "death",       "experience",   "effect",      "role",
    "effort",      "use",          "class",       "control",
    "care",        "field",        "development", "paper",
    "space",       "event",        "roommate",    "warrior",
    "language",    "bank",         "culture",     "form",
    "education",   "direction",    "leg",         "businessman",
    "consequence", "driver",       "computer",    "personnel",
    "bottle",      "memory",       "solution",    "administration",
    "unit",        "failure",      "equipment",   "dealer",
    "store",       "examination",  "topic",       "movie",
    "unit",        "lady",         "youth",       "extreme",
    "anxiety",     "error",        "reality",     "dream",
    "technique",   "equipment",    "accent",      "break",
    "bread",       "contribution", "restaurant",  "soul",
    "factory",     "offer",        "assumption",  "performance",
    "measurement", "guest",        "imagination", "refrigerator",
    "application", "song",         "union",       "addition",
    "regulation",  "tradition",    "psychology",  "clothing",
    "meal",        "extension",    "soup",        "outcome",
    "intention",   "disaster",     "tragedy",     "ratio",
    "permission",  "grain",        "reception",   "universe",
    "complaint",   "mixture",      "classic",     "provider",
    "fridge",      "income",       "therapy",     "bedroom",
    "ladder",      "guidance",     "injury",      "confusion",
    "passion",     "dirt",         "passenger",   "coffee",
    "philosophy",  "acceptance",   "lawyer",      "full",
    "know",        "since",        "place",       "heard",
    "best",        "hour",         "better",      "true",
    "during",      "hundred",      "five",        "remember",
    "step",        "early",        "hold",        "west",
    "ground",      "interest",     "reach",       "fast",
    "verb",        "sing",         "listen",      "six",
    "table",       "travel",       "less",        "morning",
    "ten",         "simple",       "several",     "vowel",
    "toward",      "war",          "lay",         "against",
    "pattern",     "slow",         "center",      "love",
    "person",      "money",        "serve",       "appear",
    "road",        "map",          "science",     "rule",
    "govern",      "pull",         "cold",        "notice",
    "voice",       "fall",         "power",       "town",
    "fine",        "certain",      "fly",         "unit",
    "lead",        "cry",          "dark",        "machine",
    "note",        "wait",         "plan",        "figure",
    "star",        "box",          "noun",        "field",
    "rest",        "correct",      "able",        "pound",
    "done",        "beauty",       "drive",       "stood",
    "contain",     "front",        "teach",       "week",
    "final",       "gave",         "green",       "oh",
    "quick",       "develop",      "ocean",       "warm",
    "free",        "minute",       "strong",      "special",
    "mind",        "behind",       "clear",       "tail",
    "produce",     "fact",         "street",      "inch",
    "multiply",    "nothing",      "course",      "stay",
    "wheel",       "full",         "force",       "blue",
    "object",      "decide",       "surface",     "deep",
    "moon",        "island",       "foot",        "system",
    "busy",        "test",         "record",      "boat",
    "common",      "gold",         "possible",    "plane",
    "stead",       "dry",          "wonder",      "laugh",
    "thousand",    "ago",          "ran",         "check",
    "game",        "shape",        "equate",      "hot",
    "miss",        "brought",      "heat",        "snow",
    "tire",        "bring",        "yes",         "distant",
    "fill",        "east",         "paint",       "language",
    "among",       "grand",        "ball",        "yet",
    "wave",        "drop",         "heart",       "am",
    "present",     "heavy",        "dance",       "engine",
    "position",    "arm",          "wide",        "sail",
    "material",    "size",         "vary",        "settle",
    "speak",       "weight",       "general",     "ice",
    "matter",      "circle",       "pair",        "include",
    "divide",      "syllable",     "felt",        "perhaps",
    "pick",        "sudden",       "count",       "square",
    "reason",      "length",       "represent",   "art",
    "subject",     "region",       "energy",      "hunt",
    "probable",    "bed",          "brother",     "egg",
    "ride",        "cell",         "believe",     "fraction",
    "forest",      "sit",          "race",        "window",
    "store",       "summer",       "train",       "sleep",
    "prove"};

// thread to count down from 30 seconds and display on the screen
void speakerOff() { speaker = 0; }

void time_update(void const *argument) {
  while (1) {

    timeCount++;

    lcd_mutex.lock();

    uLCD.locate(14, 13);
    uLCD.text_height(2);
    uLCD.text_width(2);
    if (20 <= timeCount && timeCount <= 30) {
      uLCD.color(RED);
    } else
      uLCD.color(WHITE);

    int timeremaining = 30 - timeCount;
    uLCD.printf("%2.0d", timeremaining);
    uLCD.text_height(1);
    uLCD.text_width(1);
    lcd_mutex.unlock();
    Thread::wait(1000);
  }
}

void stat_calculation(void const *argument) {
  while (1) {
    currentWPM = ((double)word_count / timeCount) * 60;
    currentAccuracy =
        (double)100 * correctPresses / (correctPresses + incorrectPresses);
    lcd_mutex.lock();
    uLCD.locate(1, 12);
    uLCD.color(BLUE);
    uLCD.printf("WPM:%2.0f", currentWPM);
    uLCD.locate(1, 14);
    uLCD.printf("Acc:%2.0f %", currentAccuracy);
    lcd_mutex.unlock();
    Thread::wait(300);
  }
}

int typeLetter(bool right) {
  if (user_input != '\0') {
    lcd_mutex.lock();
    if (right)
      uLCD.color(GREEN);
    else
      uLCD.color(RED);
    uLCD.locate(1 + userPos, 2);
    uLCD.printf("%c", user_input);
    uLCD.color(WHITE);
    uLCD.printf("_");
    lcd_mutex.unlock();
  }
}

int redraw() {
  lcd_mutex.lock();
  uLCD.filled_rectangle(0, 0, 128, 80, BLACK);
  uLCD.locate(1, 1);
  uLCD.color(WHITE);
  int i = 0;
  while (random_string[currentIndex + i] !=
         ' ') { // prints out letters of first word up next until the space
    uLCD.printf("%c", random_string[currentIndex + i]);
    i++;
  }
  i++; // start at the index of the next word
  uLCD.locate(1, 4);
  while (random_string[currentIndex + i] !=
         ' ') { // prints out letters of second word up next until the space
    uLCD.printf("%c", random_string[currentIndex + i]);
    i++;
  }
  i++;
  uLCD.locate(1, 6);
  while (random_string[currentIndex + i] !=
         ' ') { // prints out letters of third word up next until the space
    uLCD.printf("%c", random_string[currentIndex + i]);
    i++;
  }
  i++;
  uLCD.locate(1, 8);
  while (random_string[currentIndex + i] !=
         ' ') { // prints out letters of fourth word up next until the space
    uLCD.printf("%c", random_string[currentIndex + i]);
    i++;
  }
  uLCD.locate(1, 2);
  uLCD.printf("_");
  lcd_mutex.unlock();
}
// void addToPlayerString(char c);

int main() {

//initialization
  speaker.write(0);
  speaker.period(1.0 / 100.0);
  PS2Keyboard::keyboard_event_t evt_kb; // Setup keyboard interrupt
  int temp = 0;
  bool error = false;
  uLCD.baudrate(3000000);
  uLCD.cls();

  //////////////////////
  // RANDOM STRING
  ///////////////////
  // Set the random seed
  int seed = noise.read_u16();
  srand(seed);
  // Generate a random string from the word bank

  // Generate a random sequence of words from the word bank
  int i, j, random_word_index;
  for (i = 0; i < NUM_WORDS; i++) {
    random_word_index = rand() % NUM_WORDS;
    strncat(random_string, word_bank[random_word_index % NUM_WORDS], MAX_WORD_LEN);
    strcat(random_string, " ");
  }
  random_string[strlen(random_string) - 1] = '\0';
//   pc.printf("\n\n");
  // Print the random string
//   pc.printf("Random string: %s", random_string);

//   pc.printf("%d", seed);
  redraw();
  uLCD.line(5, 85, 123, 85, WHITE);
  Thread timeUpdate(time_update);
  Thread statCalc(stat_calculation);
  while (timeCount <= 30) {
    if (ps2kb.processing(&evt_kb)) { // Executes if a key is pressed
      temp = evt_kb.scancode[0];
      for (int i = 1; i < evt_kb.length;
           i++) { // Parse keyboard input into a key
        temp <<= 4;
        temp |= evt_kb.scancode[i];
      }
      if (temp < 100) {
        // pc.printf("%c", charMap[temp]);
        user_input =
            charMap[temp]; // Read in a single character from standard input
        // pc.printf("\n charmap: %c \t", charMap[temp]);
        // pc.printf("current letter: %c \t", random_string[currentIndex]);
        // pc.printf("current index: %d\t", currentIndex);

        if (random_string[currentIndex] != ' ') { // if space is not needed
          if (user_input == ' ' ||
              user_input == '\n') { // if space or enter is typed
            while (random_string[currentIndex] !=
                   ' ') { // increment currentIndex until next word
              currentIndex++;
            }
            currentIndex++; // Add one more to get to the beginning of the next
                            // word
            redraw();
            userPos = 0;
            error = false;
            incorrectPresses++;
            speaker = 0.5;
            delay.attach(&speakerOff, 1);
          } else if (userPos <= 13) { // if it isn't space or enter and you
                                      // aren't at the end of the line
            if (random_string[currentIndex] ==
                user_input) { // if correct character is typed
              correctPresses++;
              typeLetter(true);
            //   pc.printf("Correct press! # of presses %d \t", correctPresses);
            } else { // if wrong character is typed
              incorrectPresses++;
              typeLetter(false);
              error = true;
            //   pc.printf("Incorrect press! # of presses %d \t",
                        // incorrectPresses);
              speaker.write(0.5);
              delay.attach(&speakerOff, 1);
            }
            currentIndex++;
            userPos++;
          }
        }

        else {
          if (((user_input == ' ') ||
               (user_input == '\n'))) { // if space is needed and typed
            if (!error) {
              word_count++; // if word is all right, add to word count
            //   pc.printf("correct word typed!!");
            }
            error = false;
            // pc.printf("space matched!\t");
            currentIndex++;
            correctPresses++;
            userPos = 0;
            redraw();
          } else { // if space is needed and other letter is typed
            if (userPos <= 13) {
              typeLetter(false);
              incorrectPresses++;
              userPos++;
              speaker.write(0.3);
              delay.attach(&speakerOff, 0.5);
            }
          }
        }
      }

      Thread::wait(10);
    }
  }
//End of Game Screen
//   pc.printf("done");
  lcd_mutex.lock();
  uLCD.cls();
  uLCD.text_height(1);
  uLCD.text_width(1);
  uLCD.locate(5, 2);
  uLCD.text_height(2);
  uLCD.text_width(2);
  uLCD.color(RED);
  uLCD.printf("GAME");
  uLCD.text_height(1);
  uLCD.text_width(1);
  uLCD.locate(5, 4);
  uLCD.text_height(2);
  uLCD.text_width(2);
  uLCD.printf("OVER");
  uLCD.text_height(1);
  uLCD.text_width(1);
  uLCD.color(WHITE);
  uLCD.locate(1, 7);
  uLCD.printf("Words typed: %d", word_count);
  uLCD.locate(1, 9);
  uLCD.printf("Keys pressed: %d", correctPresses + incorrectPresses);
  uLCD.locate(1, 11);
  uLCD.printf("Accuracy:%3.0f%%", currentAccuracy);
  uLCD.locate(1, 13);
  uLCD.printf("WPM:%3.0f", currentWPM);
  lcd_mutex.unlock();

  statCalc.terminate(); //terminates threads
  timeUpdate.terminate();

  
//END OF GAME RHYTHM
  speaker.period(0.5/261.6);

  speaker=0.5;
  wait(0.2);
  speaker=0;
  wait(0.05);

  speaker=0.5;
  wait(0.2);
  speaker=0;
  wait(0.05);

  speaker=0.5;
  wait(0.2);
  speaker=0;
  wait(0.05);
  
  speaker.period(0.5/293.6);
  speaker=0.5;
  wait(0.2);
  speaker=0;
  wait(0.05);

  speaker.period(0.5/261.6);
  speaker=0.5;
  wait(0.2);
  speaker=0;
  wait(0.05);

  speaker.period(0.5/293.6);
  speaker=0.5;
  wait(0.2);
  speaker=0;
  wait(0.05);

  speaker.period(0.5/329.6);
  speaker=0.5;
  wait(0.7);
  speaker=0;
  wait(0.05);

  speaker.period(0.5/130.8);
  speaker=0.5;
  wait(0.2);
  speaker=0;
  wait(0.1);
//   pc.printf("more done");
    
}