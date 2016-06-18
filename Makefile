
ARDUINO_BIN ?= ~/.programmes/arduino-1.6.9/arduino
TARGET = laumio/laumio

all: verify

verify:
	$(ARDUINO_BIN) -v --verify --board esp8266:esp8266:d1_mini \
	--pref build.path=build \
	--pref target_package=esp8266 \
	--pref target_platform=esp8266 \
	--pref board=d1_mini \
	$(TARGET).ino

upload:
	$(ARDUINO_BIN) -v --upload --board esp8266:esp8266:d1_mini \
	--port $(SERIAL_PORT) --preserve-temp-files \
	--pref build.path=build \
	--pref target_package=esp8266 \
	--pref target_platform=esp8266 \
	--pref board=d1_mini \
	$(TARGET).ino

indent:
	indent -kr --no-tabs laumio/*.ino laumio/*.cpp laumio/*.h

clean:
	rm *~
	rm -R build

mrproper: clean
