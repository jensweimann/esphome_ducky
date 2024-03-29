#define HID_CUSTOM_LAYOUT
#define LAYOUT_GERMAN
#define BAUD_RATE 115200
#define OUTER_LED 13
#define INNER_LED 8
#define ESP_RECEIVE_BUFFER_SIZE 255
#define USB_RECEIVE_BUFFER_SIZE 8
#define EspSerial Serial1
#define UsbSerial Serial
#define WRITE_CHAR_DELAY 10
//-----------------------------
#include <Arduino.h>
#include <HID-Project.h>
//#include <SPI.h>
//#include <SD.h>

//-----------------------------
const char ESP_START_MARKER = 0x02;  // START OF TEXT
const char ESP_END_MARKER = 0x03;    // END OF TEXT

enum State : byte { NONE, ON, OFF, SLEEP };

char espReceivedChars[ESP_RECEIVE_BUFFER_SIZE];
boolean espRecvInProgress = false;
uint16_t espReceiveIndex = 0;
boolean espNewData = false;

char usbReceivedChars[USB_RECEIVE_BUFFER_SIZE];
uint16_t usbReceiveIndex = 0;
boolean usbNewData = false;

uint16_t defaultDelay = 0;
const boolean DEBUG = false;
uint16_t startRam = 0;
State lastPusblishedState = NONE;

uint16_t getFreeRam() {
  uint16_t size = 2560;
  byte *buf;
  while ((buf = (byte *)malloc(--size)) == NULL)
    ;
  free(buf);
  return size;
}

void setup() {
  if (DEBUG) {
    startRam = getFreeRam();
  }
  pinMode(OUTER_LED, OUTPUT);
  digitalWrite(OUTER_LED, HIGH);
  pinMode(INNER_LED, OUTPUT);
  digitalWrite(INNER_LED, HIGH);

  // disable arduino pro micro leds
  pinMode(LED_BUILTIN_RX, INPUT);
  pinMode(LED_BUILTIN_TX, INPUT);

  UsbSerial.begin(BAUD_RATE);
  EspSerial.begin(BAUD_RATE);

  Keyboard.begin();
  Mouse.begin();
  Consumer.begin();
  System.begin();
  digitalWrite(INNER_LED, LOW);
  digitalWrite(OUTER_LED, LOW);
  // processSD();
  EspSerial.println("setup done");
}

// https://forum.arduino.cc/index.php?topic=396450
void receiveFromEspSerial() {
  while (EspSerial.available() > 0 && espNewData == false) {
    char rc = EspSerial.read();
    if (espRecvInProgress == true) {
      if (rc != ESP_END_MARKER) {
        espReceivedChars[espReceiveIndex] = rc;
        espReceiveIndex++;
        if (espReceiveIndex >= ESP_RECEIVE_BUFFER_SIZE) {
          espReceiveIndex = ESP_RECEIVE_BUFFER_SIZE - 1;
        }
      } else {
        espReceivedChars[espReceiveIndex] = '\0';  // terminate the string
        espRecvInProgress = false;
        espReceiveIndex = 0;
        espNewData = true;
      }
    }

    else if (rc == ESP_START_MARKER) {
      espRecvInProgress = true;
    }
  }
}

void receiveFromUsbSerial() {
  while (UsbSerial.available() > 0 && usbNewData == false) {
    char rc = UsbSerial.read();
    if (rc != '\n') {
      usbReceivedChars[usbReceiveIndex] = rc;
      usbReceiveIndex++;
      if (usbReceiveIndex >= USB_RECEIVE_BUFFER_SIZE) {
        usbReceiveIndex = USB_RECEIVE_BUFFER_SIZE - 1;
      }
    } else {
      usbReceivedChars[usbReceiveIndex] = '\0';  // terminate the string
      usbReceiveIndex = 0;
      usbNewData = true;
    }
  }
}

void Press(String b) {
  if (b.length() == 1)
    Keyboard.press(tolower(b[0]));
  else if (b.equals("ENTER"))
    Keyboard.press(KEY_RETURN);
  else if (b.equals("CTRL") || b.equals("CONTROL"))
    Keyboard.press(KEY_LEFT_CTRL);
  else if (b.equals("SHIFT"))
    Keyboard.press(KEY_LEFT_SHIFT);
  else if (b.equals("ALT"))
    Keyboard.press(KEY_LEFT_ALT);
  else if (b.equals("ALTGR"))
    Keyboard.press(KEY_RIGHT_ALT);
  else if (b.equals("GUI") || b.equals("WIN") || b.equals("CMD"))
    Keyboard.press(KEY_LEFT_GUI);
  else if (b.equals("UP") || b.equals("UPARROW"))
    Keyboard.press(KEY_UP_ARROW);
  else if (b.equals("DOWN") || b.equals("DOWNARROW"))
    Keyboard.press(KEY_DOWN_ARROW);
  else if (b.equals("LEFT") || b.equals("LEFTARROW"))
    Keyboard.press(KEY_LEFT_ARROW);
  else if (b.equals("RIGHT") || b.equals("RIGHTARROW"))
    Keyboard.press(KEY_RIGHT_ARROW);
  else if (b.equals("DELETE") || b.equals("DEL"))
    Keyboard.press(KEY_DELETE);
  else if (b.equals("PAGEUP"))
    Keyboard.press(KEY_PAGE_UP);
  else if (b.equals("PAGEDOWN"))
    Keyboard.press(KEY_PAGE_DOWN);
  else if (b.equals("HOME"))
    Keyboard.press(KEY_HOME);
  else if (b.equals("ESC") || b.equals("ESCAPE"))
    Keyboard.press(KEY_ESC);
  else if (b.equals("BACKSPACE"))
    Keyboard.press(KEY_BACKSPACE);
  else if (b.equals("INSERT"))
    Keyboard.press(KEY_INSERT);
  else if (b.equals("TAB"))
    Keyboard.press(KEY_TAB);
  else if (b.equals("END"))
    Keyboard.press(KEY_END);
  else if (b.equals("CAPSLOCK"))
    Keyboard.press(KEY_CAPS_LOCK);
  else if (b.equals("F1"))
    Keyboard.press(KEY_F1);
  else if (b.equals("F2"))
    Keyboard.press(KEY_F2);
  else if (b.equals("F3"))
    Keyboard.press(KEY_F3);
  else if (b.equals("F4"))
    Keyboard.press(KEY_F4);
  else if (b.equals("F5"))
    Keyboard.press(KEY_F5);
  else if (b.equals("F6"))
    Keyboard.press(KEY_F6);
  else if (b.equals("F7"))
    Keyboard.press(KEY_F7);
  else if (b.equals("F8"))
    Keyboard.press(KEY_F8);
  else if (b.equals("F9"))
    Keyboard.press(KEY_F9);
  else if (b.equals("F10"))
    Keyboard.press(KEY_F10);
  else if (b.equals("F11"))
    Keyboard.press(KEY_F11);
  else if (b.equals("F12"))
    Keyboard.press(KEY_F12);
  else if (b.equals("SPACE"))
    Keyboard.press(KEY_SPACE);
  else if (b.equals("APP") || b.equals("MENU"))
    Keyboard.press(KEY_MENU);
  else if (b.equals("PRINTSCREEN"))
    Keyboard.press(KEY_PRINT);
  else if (b.equals("SCROLLOCK"))
    Keyboard.press(KEY_SCROLL_LOCK);
  else if (b.equals("BREAK"))
    Keyboard.press(KEY_PAUSE);
  else if (b.equals("NUMLOCK"))
    Keyboard.press(KEY_NUM_LOCK);
  //---------------------------------------------------------
  else if (b.equals("VOLUP"))
    Consumer.write(MEDIA_VOL_UP);
  else if (b.equals("VOLDWN"))
    Consumer.write(MEDIA_VOL_DOWN);
  else if (b.equals("VOLMUT") || b.equals("MUTE"))
    Consumer.write(MEDIA_VOL_MUTE);
  else if (b.equals("PLAY_PAUSE"))
    Consumer.write(MEDIA_PLAY_PAUSE);
  else if (b.equals("PLAY"))
    Consumer.write(HID_CONSUMER_PLAY);
  else if (b.equals("PAUSE"))
    Consumer.write(HID_CONSUMER_PAUSE);
  else if (b.equals("STOP"))
    Consumer.write(MEDIA_STOP);
  else if (b.equals("NEXT"))
    Consumer.write(MEDIA_NEXT);
  else if (b.equals("PREV"))
    Consumer.write(MEDIA_PREVIOUS);
  //---------------------------------------------------------
  else if (b.equals("WAKE"))
    System.write(SYSTEM_WAKE_UP);
  else if (b.equals("SLEEP"))
    System.write(SYSTEM_SLEEP);
  else if (b.equals("POWER_DOWN"))
    System.write(SYSTEM_POWER_DOWN);
  //---------------------------------------------------------
  else if (b.equals("MOUSE_LEFT") || b.equals("CLICK"))
    Mouse.click();
  else if (b.equals("MOUSE_RIGHT"))
    Mouse.click(MOUSE_RIGHT);
  else if (b.equals("MOUSE_MIDDLE"))
    Mouse.click(MOUSE_MIDDLE);
  else if (b.equals("MOUSE_PREV"))
    Mouse.click(MOUSE_PREV);
  else if (b.equals("MOUSE_NEXT"))
    Mouse.click(MOUSE_NEXT);
  else if (b.equals("MOUSE_LEFT_DOWN")) {
    if (!Mouse.isPressed(MOUSE_LEFT)) {
      Mouse.press(MOUSE_LEFT);
    }
  } else if (b.equals("MOUSE_LEFT_UP")) {
    if (Mouse.isPressed(MOUSE_LEFT)) {
      Mouse.release(MOUSE_LEFT);
    }
  } else if (b.equals("MOUSE_RIGHT_DOWN")) {
    if (!Mouse.isPressed(MOUSE_RIGHT)) {
      Mouse.press(MOUSE_RIGHT);
    }
  } else if (b.equals("MOUSE_RIGHT_UP")) {
    if (Mouse.isPressed(MOUSE_RIGHT)) {
      Mouse.release(MOUSE_RIGHT);
    }
  } else if (b.equals("MOUSE_MIDDLE_DOWN")) {
    if (!Mouse.isPressed(MOUSE_MIDDLE)) {
      Mouse.press(MOUSE_MIDDLE);
    }
  } else if (b.equals("MOUSE_MIDDLE_UP")) {
    if (Mouse.isPressed(MOUSE_MIDDLE)) {
      Mouse.release(MOUSE_MIDDLE);
    }
  }
}

void Line(String _line, String last) {
  int16_t firstSpace = _line.indexOf(" ");
  if (firstSpace == -1) {
    Press(_line);
  } else {
    String subString = _line.substring(0, firstSpace);
    if (subString.equals("STRING")) {
      if (DEBUG) {
        UsbSerial.print("Going to type ");
        UsbSerial.print(
            _line.substring(firstSpace + 1, _line.length()).length());
        UsbSerial.println(" chars...");
      }
      for (uint16_t i = firstSpace + 1; i < _line.length(); i++) {
        Keyboard.write(_line[i]);
        delay(WRITE_CHAR_DELAY);
      }
      if (DEBUG) UsbSerial.println("typing done");
    } else if (subString.equals("SERIAL") && UsbSerial) {
      for (uint16_t i = firstSpace + 1; i < _line.length(); i++) {
        UsbSerial.print(_line[i]);
      }
      UsbSerial.println();
    } else if (subString.equals("DELAY")) {
      uint16_t delaytime = _line.substring(firstSpace + 1).toInt();
      delay(delaytime);
    } else if (subString.equals("DEFAULTDELAY"))
      defaultDelay = _line.substring(firstSpace + 1).toInt();
    else if (subString.equals("REM")) {
      // nothing :/
    } else if (subString.equals("MOUSE_MOVE")) {
      uint16_t comma = _line.indexOf(",");
      int point_x = _line.substring(firstSpace + 1, comma).toInt();
      int point_y = _line.substring(comma + 1).toInt();
      if (DEBUG) {
        UsbSerial.print("Move to x=");
        UsbSerial.print(point_x);
        UsbSerial.print(", y=");
        UsbSerial.println(point_y);
      }
      Mouse.move(point_x, point_y);
    } else if (subString.equals("MOUSE_SCROLL") ||
               subString.equals("MOUSE_WHEEL") || subString.equals("SCROLL")) {
      int steps = _line.substring(firstSpace + 1).toInt();
      Mouse.move(0, 0, steps);
    }
    //---------------------------------------------------------
    else if (subString.equals("REPLAY") || subString.equals("REPEAT")) {
      uint16_t replaynum = _line.substring(firstSpace + 1).toInt();
      while (replaynum) {
        Line(last, "");
        --replaynum;
      }
    } else {
      String remain = String(_line);

      while (remain.length() > 0) {
        int16_t latest_space = remain.indexOf(" ");
        if (latest_space == -1) {
          Press(remain);
          remain = "";
        } else {
          Press(remain.substring(0, latest_space));
          remain = remain.substring(latest_space + 1);
        }
        delay(WRITE_CHAR_DELAY);
      }
    }
  }

  Keyboard.releaseAll();
  Consumer.releaseAll();
  // Mouse.releaseAll();
  System.releaseAll();
  delay(defaultDelay);
}

void handleDuckyCommand() {
  String bufferStr = String(espReceivedChars);
  String last = "";
  bufferStr.replace("\r", "\n");
  bufferStr.replace("\n\n", "\n");
  if (bufferStr.length() > 0) {
    EspSerial.println("busy");
    digitalWrite(INNER_LED, HIGH);
    while (bufferStr.length() > 0) {
      int16_t latest_return = bufferStr.indexOf("\n");
      if (latest_return == -1) {
        if (DEBUG) {
          UsbSerial.print("run1: '");
          UsbSerial.print(bufferStr);
          UsbSerial.println("'");
        }
        Line(bufferStr, last);
        bufferStr = "";
      } else {
        String temp = bufferStr.substring(0, latest_return);
        if (temp.length() > 0) {
          if (DEBUG) {
            UsbSerial.print("run2: '");
            UsbSerial.print(temp);
            UsbSerial.println("'");
          }
          Line(temp, last);
          last = temp;
        }
        if (DEBUG) {
          UsbSerial.println("bufferStr length: " + String(bufferStr.length()));
          UsbSerial.println("Select next substring from: " +
                            String(latest_return + 1));
        }
        bufferStr = bufferStr.substring(latest_return + 1);
        if (DEBUG) {
          UsbSerial.println("resulting bufferStr length: " +
                            String(bufferStr.length()));
        }
      }
    }
    UsbSerial.println("done");
    EspSerial.println("ready");
    defaultDelay = 0;
    last = "";
    digitalWrite(INNER_LED, LOW);

    if (DEBUG) {
      UsbSerial.println("done");
      UsbSerial.println("Free ram start: " + String(startRam) + " Bytes.");
      UsbSerial.println("Free ram now: " + String(getFreeRam()) + " Bytes.");
    }
  }
}

void handleNewData() {
  if (espNewData == true) {
    handleDuckyCommand();
    if (DEBUG) {
      EspSerial.print("ESP_INPUT=");
      EspSerial.println(espReceivedChars);
    }
    espNewData = false;
  }
  if (usbNewData == true) {
    EspSerial.print("USB_INPUT=");
    EspSerial.println(usbReceivedChars);
    usbNewData = false;
  }
}

String getStateText(State state) {
  switch (state) {
    case ON:
      return "STATE=ON";
    case OFF:
      return "STATE=OFF";
    case SLEEP:
      return "STATE=SLEEP";
    default:
      return "STATE=NONE";
  }
}

void publishUsbDeviceState() {
  State state = NONE;
  if (USBDevice.configured()) {
    if (USBDevice.isSuspended()) {
      state = SLEEP;
    } else {
      state = ON;
    }
  } else {
    state = OFF;
  }
  if (lastPusblishedState != state) {
    lastPusblishedState = state;
    EspSerial.println(getStateText(state));
    EspSerial.println("ready");
  }
}

void loop() {
  receiveFromEspSerial();
  receiveFromUsbSerial();
  handleNewData();
  publishUsbDeviceState();
}
