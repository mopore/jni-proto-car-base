#include "jni_controller_display.h"

#define TITEL_COL 0
#define TITEL_Y 0

#define LABELS_Y 20
#define VALUES_Y LABELS_Y + 10

#define COORDINATE_X_COL 0
#define COORDINATE_Y_COL 35
#define COORDINATE_WIDTH 30

#define BAT_COL 70
#define TEXT_WIDTH 30

#define IP_COL 0
#define LABEL_IP_Y 47
#define VALUE_IP_Y LABEL_IP_Y + 10

#define UDP_COL 102
#define LABEL_UDP_Y 47
#define VALUE_UDP_Y LABEL_UDP_Y + 10

#define NO_CONNECTION "<No Connection>"

#define LABEL_TITLE "JNI CAR BASE"
#define LABEL_IP "IP:"
#define LABEL_BAT "BAT:"
#define LABEL_X "X:"
#define LABEL_Y "Y:"
#define LABEL_UDP "UDP:"

#define TEXT_USB "USB"
#define TEXT_LOW "LOW"
#define TEXT_MED "MED"
#define TEXT_HIGH "HIGH"
#define TEXT_OK "OK"
#define TEXT_NO "n/a"

static Adafruit_SH1107 display = Adafruit_SH1107(64, 128, &Wire);
static GFXcanvas1 xCanvas = GFXcanvas1(COORDINATE_WIDTH, 10);
static GFXcanvas1 yCanvas = GFXcanvas1(COORDINATE_WIDTH, 10);
static GFXcanvas1 batCanvas = GFXcanvas1(TEXT_WIDTH, 10);
static GFXcanvas1 ipCanvas = GFXcanvas1(128, 10);
static GFXcanvas1 udpCanvas = GFXcanvas1(TEXT_WIDTH, 10);

ControllerDisplay::ControllerDisplay() {
}


void ControllerDisplay::setup() {
	Serial.println("128x64 OLED FeatherWing test");
	delay(250); // wait for the OLED to power up
	display.begin(0x3C, true); // Address 0x3C default

	Serial.println("OLED begun");

	// Show image buffer on the display hardware.
	// Since the buffer is intialized with an Adafruit splashscreen
	// internally, this will display the splashscreen.
	display.display();
	delay(1000);

	// General setup
	display.clearDisplay();
	display.display();
	// Set rotation to be upside down in landscape relative to display buttons.
	display.setRotation(3);
	display.setTextSize(1);
	display.setTextColor(SH110X_WHITE);

	// Draw title
	display.setCursor(TITEL_COL, TITEL_Y);
	display.print(LABEL_TITLE);
	display.drawLine(TITEL_COL, (TITEL_Y + 12), 128, (TITEL_Y + 12), SH110X_WHITE);

	// Draw coordinate labels
	display.setCursor(COORDINATE_X_COL, LABELS_Y);
	display.print(LABEL_X);
	display.setCursor(COORDINATE_Y_COL, LABELS_Y);
	display.print(LABEL_Y);

	// Draw battery label
	display.setCursor(BAT_COL,LABELS_Y);
	display.print(LABEL_BAT);

	// Draw IP label
	display.setCursor(IP_COL, LABEL_IP_Y);
	display.print(LABEL_IP);

	// Draw UDP label
	display.setCursor(UDP_COL, LABEL_UDP_Y);
	display.print(LABEL_UDP);

	display.display();
}


void ControllerDisplay::loop10Hz() {

	// Draw the X coordinate
	xCanvas.fillScreen(0);
	xCanvas.setCursor(0,0);
	xCanvas.printf("%d%%", carInput.x);
	display.drawBitmap(
		COORDINATE_X_COL, VALUES_Y, 
		xCanvas.getBuffer(), 
		COORDINATE_WIDTH, 10, 
		SH110X_WHITE, SH110X_BLACK
	);

	// Draw the Y coordinate
	yCanvas.fillScreen(0);
	yCanvas.setCursor(0,0);
	yCanvas.printf("%d%%", carInput.y);
	display.drawBitmap(
		COORDINATE_Y_COL, VALUES_Y, 
		yCanvas.getBuffer(), 
		COORDINATE_WIDTH, 10, 
		SH110X_WHITE, SH110X_BLACK
	);

	// Draw the battery status
	batCanvas.fillScreen(0);
	batCanvas.setCursor(0,0);
	if (powerStatus.usbPowerPresent) {
		batCanvas.printf(TEXT_USB);
	}
	else if (powerStatus.batteryVoltage < 3.3) {
		batCanvas.printf(TEXT_LOW);
	}
	else if (powerStatus.batteryVoltage < 3.5) {
		batCanvas.printf(TEXT_MED);
	}
	else {
		batCanvas.printf(TEXT_HIGH);
	}
	display.drawBitmap(
		BAT_COL, VALUES_Y, 
		batCanvas.getBuffer(), 
		TEXT_WIDTH, 10, 
		SH110X_WHITE, SH110X_BLACK
	);

	// Draw the IP address
	ipCanvas.fillScreen(0);
	ipCanvas.setCursor(0,0);
	if (connectionStatus.isWifiConnected) {
		auto ip = getWifiStatusIP_v4();
		ipCanvas.print(ip);
	}
	else {
		ipCanvas.printf(NO_CONNECTION);
	}
	display.drawBitmap(
		IP_COL, VALUE_IP_Y, 
		ipCanvas.getBuffer(), 
		128, 10, 
		SH110X_WHITE, SH110X_BLACK
	);

	// Draw the UDP status
	udpCanvas.fillScreen(0);
	udpCanvas.setCursor(0,0);
	if (connectionStatus.isUdpWorking) {
		udpCanvas.printf(TEXT_OK);
	}
	else {
		udpCanvas.printf(TEXT_NO);
	}
	display.drawBitmap(
		UDP_COL, VALUE_UDP_Y, 
		udpCanvas.getBuffer(), 
		TEXT_WIDTH, 10, 
		SH110X_WHITE, SH110X_BLACK
	);

	display.display();
}
