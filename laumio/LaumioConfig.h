#ifndef LAUMIOCONFIG_H
#define LAUMIOCONFIG_H

#include <array>
#include <string.h>

class LaumioConfig {
  public: // Helper classes
	class Param {
	  public:
		virtual int size() = 0;
		virtual char *buffer() = 0;
	};
	template <int N> class Config : public Param {
	  public:
		int size() final { return N; }
		char *buffer() final { return _buffer; }

	  protected:
		char _buffer[N];
	};
	class ConfigBool : public Config<1> {
	  public:
		ConfigBool(bool b = false) { setValue(b); }
		bool value() { return this->_buffer[0] != 0; }
		void setValue(bool b) { this->_buffer[0] = b; }
		operator bool() { return value(); }
	};
	template <int N> class ConfigString : public Config<N> {
	  public:
		ConfigString(const char *const s = "") { setValue(s); }
		char *value() { return this->_buffer; }
		void setValue(const char *const s) {
			auto len = strlen(s);
			if (len > N - 1)
				len = N - 1;
			memcpy(this->_buffer, s, len);
			this->_buffer[len] = 0;
		}
		operator char *() { return value(); }
	};

  public: // The actual config
	/* Note: increment VERSION if the config is modified to
	 *       invalidate older versions. Do not increment if you only
	 *       add data at the end.
	 */
	const unsigned char VERSION{1};

	ConfigString<32> hostname;

	ConfigBool connect_enabled;
	ConfigString<32> connect_essid;
	ConfigString<32> connect_password;

	ConfigBool ap_enabled;
	ConfigString<32> ap_essid;
	ConfigString<32> ap_password;

	ConfigBool apfallback_enabled{true};

	ConfigBool mqtt_enabled;
	ConfigString<32> mqtt_host;
	ConfigString<32> mqtt_user;
	ConfigString<32> mqtt_password;

  public:
	void readFromEEPROM();
	void saveToEEPROM();

  private:
	std::array<Param *, 11> paramList{{
	    &connect_enabled,
	    &connect_essid,
	    &connect_password,
	    &ap_enabled,
	    &ap_essid,
	    &ap_password,
	    &apfallback_enabled,
	    &mqtt_enabled,
	    &mqtt_host,
	    &mqtt_user,
	    &mqtt_password,
	}};
};

#endif
