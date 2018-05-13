
typedef struct Light {
	int red, green, blue;
} Light ;

typedef struct RGB {
	int r, g, b;
} RGB;

// Function Declaration
void stable_FadingEffect_a(Light light);
void unstable_flickeringEffect_a(Light light);
void unstable_flickeringEffect_b(Light light);
void unstable_flickeringEffect_c(Light light);
void unstable_flickeringEffect_d(Light light);
void test_lightRotation();
int convertToRGBValue(double percent);
void changeLightColor(Light light, RGB rgb);

// Values
Light stable;
Light unstable;

#define RED (RGB){255, 0, 0}
#define GREEN (RGB){0, 255, 0}
#define BLUE (RGB){0, 0, 255}
#define WHITE (RGB){255, 255, 255}
#define OFF (RGB){0, 0, 0}

// Main
void setup() {
	stable = (Light){11, 10, 9};
	unstable = (Light){6, 5, 3};

	// Pin mode
	int pins[] = {stable.red, stable.green, stable.blue, unstable.red, unstable.green, unstable.blue};
	int numberOfPins = sizeof(pins) / sizeof(pins[0]);
	for (int i = 0; i < numberOfPins; i++) {
		pinMode(pins[i], OUTPUT);
	}

	// Random seed
	randomSeed(analogRead(0));

	Serial.begin(9660);
}

void loop() {
	// Test	
	// test_lightRotation();

	// Launching
	if (millis() <= 2000) {
		unstable_flickeringEffect_c(stable);
		unstable_flickeringEffect_c(unstable);
	} else if (millis() <= 2500) {
		changeLightColor(stable, OFF);
		changeLightColor(unstable, OFF);
	} else {
		// Stable
		stable_FadingEffect_a(stable);

		// Unstable
		// unstable_flickeringEffect_a(unstable);
		// unstable_flickeringEffect_b(unstable);
		// unstable_flickeringEffect_c(unstable);
		unstable_flickeringEffect_d(unstable);

		// Fun 
		// unstable_flickeringEffect_c(stable);
		// unstable_flickeringEffect_c(unstable);
	}
}

// Stable
unsigned long sfa_prevFadeAt = 0;
int sfa_fadeDuration = 10;
bool sfa_isForward = true;
int sfa_currentColorIndex = 0;// Deprecated
int sfa_fromColorIndex = 0;
int sfa_toColorIndex = 1;
// RGB sfa_colors[] = {(RGB){255, 255, 255}, (RGB){0, 0, 255}, (RGB){100, 50, 255}, (RGB){255, 50, 255}, (RGB){255, 50, 100}};
// RGB sfa_colors[] = {(RGB){100, 50, 255}, (RGB){255, 50, 255}, (RGB){255, 50, 100}};
RGB sfa_colors[] = {(RGB){255, 255, 255}, (RGB){0, 0, 255}, (RGB){0, 200, 255}};
RGB sfa_currentColor = sfa_colors[0];


void stable_FadingEffect_a(Light light) {
	int millisecond = millis();
	if (millisecond - sfa_prevFadeAt >= sfa_fadeDuration) {
		RGB fromColor = sfa_colors[sfa_fromColorIndex];
		RGB toColor = sfa_colors[sfa_toColorIndex];

		// Print FROM TO CURRENT
		char buffer[100];
		// int l = sprintf(buffer, "FROM R %d G %d B %d", fromColor.r, fromColor.g, fromColor.b);
		// for (int i = 0; i <= l; i++) {
		// 	Serial.print(buffer[i]);
		// }
		// Serial.println("");
		// l = sprintf(buffer, "TO R %d G %d B %d", toColor.r, toColor.g, toColor.b);
		// for (int i = 0; i <= l; i++) {
		// 	Serial.print(buffer[i]);
		// }
		// Serial.println("");

		int rDelta = toColor.r - fromColor.r;
		int gDelta = toColor.g - fromColor.g;
		int bDelta = toColor.b - fromColor.b;
		int maxDelta = max(max(abs(rDelta), abs(gDelta)), abs(bDelta));

		// int l = sprintf(buffer, "CURRENT R %d G %d B %d | rD %d gD %d bD %d mD %d", sfa_currentColor.r, sfa_currentColor.g, sfa_currentColor.b, rDelta, gDelta, bDelta, maxDelta);
		// for (int i = 0; i <= l; i++) {
		// 	Serial.print(buffer[i]);
		// }
		// Serial.println("");

		int newRed = (int)(sfa_currentColor.r + rDelta / maxDelta);
		int newGreen = (int)(sfa_currentColor.g + gDelta / maxDelta);
		int newBlue = (int)(sfa_currentColor.b + bDelta / maxDelta);
		newRed = newRed < 0 ? 0 : newRed;
		newRed = newRed > 255 ? 255 : newRed;
		newGreen = newGreen < 0 ? 0 : newGreen;
		newGreen = newGreen > 255 ? 255 : newGreen;
		newBlue = newBlue < 0 ? 0 : newBlue;
		newBlue = newBlue > 255 ? 255 : newBlue;

		RGB newColor = {newRed, newGreen, newBlue};

		if (
			(abs(maxDelta) == abs(rDelta) && abs(sfa_currentColor.r - fromColor.r) >= abs(rDelta)) ||
			(abs(maxDelta)  == abs(gDelta) && abs(sfa_currentColor.g - fromColor.g) >= abs(gDelta)) ||
			(abs(maxDelta) == abs(bDelta) && abs(sfa_currentColor.b - fromColor.b) >= abs(bDelta))
			) {

			if (sfa_isForward == true) {
				sfa_fromColorIndex += 1;
				sfa_toColorIndex += 1;
				if (sfa_toColorIndex >= sizeof(sfa_colors) / sizeof(sfa_colors[0])) {
					sfa_toColorIndex = sfa_fromColorIndex - 1;
					sfa_isForward = false;
				}
			} else {
				sfa_fromColorIndex -= 1;
				sfa_toColorIndex -= 1;
				if (sfa_toColorIndex < 0) {
					sfa_fromColorIndex = 0;
					sfa_toColorIndex = 1;
					sfa_isForward = true;
				}
			}
		}

		// Serial.println(abs(bDelta));
		// 	Serial.println(abs(sfa_currentColor.b - fromColor.b));

		changeLightColor(stable, newColor);
		sfa_currentColor = newColor;
		sfa_prevFadeAt = millisecond;
	}
}

// Unstable
unsigned long ufa_prevFlickerAt = 0;
int ufa_flickerDuration = 40;
int ufa_currentSequenceIndex = 0;
int ufa_flickerSequence[] = {180, 30, 89, 23, 255, 200, 90, 150, 60, 230, 180, 45, 90};

void unstable_flickeringEffect_a(Light light) {
	int millisecond = millis();
	if (millisecond - ufa_prevFlickerAt >= ufa_flickerDuration) {
		int brightness = ufa_flickerSequence[ufa_currentSequenceIndex];
		int i = random(0, 5);
		changeLightColor(light, (RGB){brightness, 0, 0});

		ufa_currentSequenceIndex += 1;
		if (ufa_currentSequenceIndex >= sizeof(ufa_flickerSequence) / sizeof(ufa_flickerSequence[0])) {
			ufa_currentSequenceIndex = 0;
		}
		ufa_prevFlickerAt = millisecond;
	}
}

unsigned long ufb_prevFlickerAt = 0;
int ufb_flickerDuration = 80;
bool ufb_isOn = false;
int ufb_flickerBrightnessMinValue = 50;
int ufb_flickerBrightnessMaxValue = 200;
int ufb_flickerOnMinDuration = 100;
int ufb_flickerOnMaxDuration = 400;
int ufb_flickerOffMinDuration = 250;
int ufb_flickerOffMaxDuration = 550;

void unstable_flickeringEffect_b(Light light) {
	int millisecond = millis();
	if (millisecond - ufb_prevFlickerAt >= ufb_flickerDuration) {
		if (ufb_isOn == true) {
			// Turn Off
			changeLightColor(light, OFF);
			ufb_flickerDuration = random(ufb_flickerOffMinDuration, ufb_flickerOffMaxDuration);
			ufb_isOn = false;
		} else {
			// Turn on
			int brightness = random(ufb_flickerBrightnessMinValue, ufb_flickerBrightnessMaxValue);
			RGB color = (RGB){brightness, 0, 0};
			changeLightColor(light, color);
			ufb_flickerDuration = random(ufb_flickerOnMinDuration, ufb_flickerOnMaxDuration);
			ufb_isOn = true;
		}
		ufb_prevFlickerAt = millisecond;
	}
}

unsigned long ufc_prevFlickerAt = 0;
int ufc_flickerDuration = 40;
int ufc_currentSequenceIndex = 0;
int ufc_flickerSequence[] = {180, 30, 89, 23, 255, 200, 90, 150, 60, 230, 180, 45, 90};

void unstable_flickeringEffect_c(Light light) {
	int millisecond = millis();
	if (millisecond - ufc_prevFlickerAt >= ufc_flickerDuration) {
		int brightness = ufc_flickerSequence[ufc_currentSequenceIndex];
		int i = random(0, 5);
		switch (i) {
			case 0:
				changeLightColor(light, (RGB){brightness, 0, 0});
				break;
			case 1:
				changeLightColor(light, (RGB){0, brightness, 0});
				break;
			case 2:
				changeLightColor(light, (RGB){0, 0, brightness});
				break;
			case 3:
				changeLightColor(light, (RGB){0, brightness, brightness});
				break;
			case 4:
				changeLightColor(light, (RGB){brightness, brightness, 0});
				break;
		}

		ufc_currentSequenceIndex += 1;
		if (ufc_currentSequenceIndex >= sizeof(ufc_flickerSequence) / sizeof(ufc_flickerSequence[0])) {
			ufc_currentSequenceIndex = 0;
		}
		ufc_prevFlickerAt = millisecond;
	}
}

unsigned long ufd_prevFlickerAt = 0;
int ufd_flickerDuration = 40;
int ufd_currentSequenceIndex = 0;
int ufd_flickerSequence[] = {180, 30, 89, 23, 255, 200, 90, 150, 60, 230, 180, 45, 90};

void unstable_flickeringEffect_d(Light light) {
	int millisecond = millis();
	if (millisecond - ufd_prevFlickerAt >= ufd_flickerDuration) {
		int brightness = ufd_flickerSequence[ufd_currentSequenceIndex];
		int i = random(0, 5);
		bool randomOnOff = random(0, 8);
		int randomG = random(0, min(brightness * 0.1,  brightness));
		int randomB = random(0, min(brightness * 0.1,  brightness));
		switch (randomOnOff) {
			case 0:
			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
			case 6:
			case 7:
				changeLightColor(light, (RGB){brightness, randomG, 0});
				break;
			case 8:
			case 9:
				changeLightColor(light, OFF);
				break;
			default: break;
		}

		ufd_currentSequenceIndex += 1;
		if (ufd_currentSequenceIndex >= sizeof(ufd_flickerSequence) / sizeof(ufd_flickerSequence[0])) {
			ufd_currentSequenceIndex = 0;
		}
		ufd_prevFlickerAt = millisecond;
	}
}

// Test
void test_lightRotation() {
	changeLightColor(stable, RED);
	changeLightColor(unstable, RED);
	delay(1000);
	changeLightColor(stable, GREEN);
	changeLightColor(unstable, GREEN);
	delay(1000);
	changeLightColor(stable, BLUE);
	changeLightColor(unstable, BLUE);
	delay(1000);
	changeLightColor(stable, WHITE);
	changeLightColor(unstable, WHITE);
	delay(5000);
}

// Helper
int convertToRGBValue(double percent) {
	return (int)(percent * 255);
}

void changeLightColor(Light light, RGB rgb) {
	analogWrite(light.red, rgb.r);
	analogWrite(light.green, rgb.g);
	analogWrite(light.blue, rgb.b);
}
