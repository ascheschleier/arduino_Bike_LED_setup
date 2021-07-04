boolean controlFade = true;
//int _brightness = 200;


void controller() {
  static boolean longpress[BTN_AMOUNT];
  static boolean verylongpress[BTN_AMOUNT];
  static int dbl[BTN_AMOUNT];
  static unsigned long timer[BTN_AMOUNT];
  static int oldVal = 0;
  static int softVal = 0;
  static int totalVal = 0;
  static int potiTlrnc = 15;
  static int frames = 0;


  int val = analogRead(poti1Pin);
  softVal += val;
  frames++;

  EVERY_N_MILLISECONDS( 40 ) {
    totalVal = (softVal / frames);
    frames = 0;
    softVal = 0;
  }

  if (totalVal > oldVal + potiTlrnc || totalVal < oldVal - potiTlrnc) { //make sure the poti was moved
    //    Serial.println("-----------------------------");
    //    Serial.print("** Soft Value 1 = ");
    //    Serial.print(totalVal);
    oldVal = totalVal;

    if (controlFade) {
      gFade = map(totalVal, potiTlrnc, 1023, 100, 0);
      if (gFade < 0) gFade = 0;
      //      Serial.println(" ");
      //      Serial.print("gFAde set to = ");
      //      Serial.print(gFade);
      //      Serial.print("_brightness set to = ");
      //      Serial.print(_brightness);
    } else {
      gTimeout = map(totalVal, potiTlrnc, 1023, maxTimeout, minTimeout);
      if (gTimeout < 0) gTimeout = 0;
      /*
        _brightness = map(totalVal,15, 1023, 0, 255);
        if(_brightness < 0) _brightness = 0;
        FastLED.setBrightness(_brightness);
      */
      //      Serial.println(" ");
      //      Serial.print("_brightness set to = ");
      //      Serial.print(_brightness);
      //      Serial.print("gFAde set to = ");
      //      Serial.print(gFade);
    }
  }

  //  Serial.print("** Value 1 = ");
  //  Serial.print(val);
  //  Serial.print("** BTN 1 = ");
  //  Serial.print(analogRead(push1Pin));
  //  Serial.print("** BTN 2 = ");
  //  Serial.println(analogRead(push2Pin));
  //
  for (int i = 0; i < BTN_AMOUNT; i++ ) {
    btn[i].read();

    //this fires every time the button is pressed down
    //    if (btn[i].wasPressed()) {
    //      Serial.print(i + 1);
    //      Serial.println(" Button down");
    //    }

    //if the button was released, check if it was held down for a very long time, for just a short time or it was released immediately
    if (btn[i].wasReleased()) {
      if (verylongpress[i]) {
        verylongpress[i] = false;
        longpress[i] = false;
        dbl[i] = 0;
        /*  uncomment for very long press = random mode
        if (i == 0) {
          if (random_mode_on) {
            random_mode_on = false;
            Serial.println("Random mode OFF");
          } else {
            random_mode_on = true;
            nextPattern();
            nextPallete();
            Serial.println("Random mode ON");
          }
        }
        */
      } else if (longpress[i]) {
        longpress[i] = false;
        dbl[i] = 0;

        Serial.print(i);
        Serial.println(" pressed long");

        if (i == 1) {
          controlFade = true;
          Serial.print(controlFade);
          Serial.println(" is the state of the boolean");
        }
        if (i == 0)  {
          controlFade = false;
          Serial.print(controlFade);
          Serial.println(" is the state of the boolean");
        }

      } else {
        dbl[i]++;
        longpress[i] = false;
        timer[i] = millis();
      }

      if (millis() > 5000) {
        just_turned_on = false;
      }
    }

    //set the flags for the two timed presses. just a notification here for longpress, since we may want to wait if the press goes on to be a very long press
    if (btn[i].pressedFor(600) && !longpress[i]) {
      longpress[i] = true;
      Serial.print(i);
      Serial.println(" Button longpress detected, release to trigger or hold for very long press");
    }
    if (btn[i].pressedFor(5000) && !verylongpress[i]) {
      verylongpress[i] = true;
      Serial.print(i);
      Serial.println(" pressed VERY long");
    }

    // if more than one press in a short time (DBL_PRESS_TIME) is detected, trigger the double click action, normal press otherwise
    if (dbl[i] > 0) {
      if (millis() > timer[i] + DBL_PRESS_TIME) {
        if (dbl[i] > 1) {
          Serial.print(i);
          Serial.print(" Button pressed ");
          Serial.print(dbl[i]);
          Serial.println(" times");
          if (i == 0) nextPallete();
          if (i == 1 && dbl[i] == 2) previousPallete();
          if (i == 1 && dbl[i] == 3) {
            if (safety_lights_on) {
              safety_lights_on = false;
            } else {
              safety_lights_on = true;
            }

            Serial.print(" Safety Light is =  ");
            Serial.println(safety_lights_on);
          }
          if (i == 0 && dbl[i] == 3) {
            if (random_mode_on) {
              random_mode_on = false;
              Serial.println("Random mode OFF");
            } else {
              random_mode_on = true;
              nextPattern();
              nextPallete();
              Serial.println("Random mode ON");
            }
          }
        } else {
          Serial.print(i);
          Serial.println(" Button trigger");

          if (i == 0) nextPattern();
          if (i == 1) previousPattern();
        }
        dbl[i] = 0;
      }
    }
  }
}
