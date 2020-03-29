#include <vector>
#include <string>

namespace Stardust::Network {
	typedef unsigned char byte;

	struct PacketOut {
		short ID;
		std::vector<byte> bytes;
	};
	struct PacketIn {
		short ID;
		short int pos;
		std::vector<byte> bytes;
	};

	inline std::vector<byte> encodeVarInt(int value) {
		std::vector<byte> vec;

		while (value > 127) {
			vec.push_back(((byte)(value & 127)) | 128);

			value >> 7;
		}
		vec.push_back((byte)value & 127);
		return vec;
	}

	inline void encodeVarInt(int value, std::vector<byte>& p) {
		

		while (value > 127) {
			p.push_back(((byte)(value & 127)) | 128);

			value >> 7;
		}
		p.push_back((byte)value & 127);
	}

	inline unsigned int decodeVarInt(std::vector<byte> input) {
		unsigned int result = 0;
  		unsigned int bits = 0;

		int pos = 0;
		
		while (input[pos] & 0x80) {
    		byte b = input[pos];
    		result += ((b & 0x7F) << bits);
    		pos++;
    		bits += 7;
  		}
		  
  		byte b = input[pos++];
  		result += ((b & 0x7F) << bits);

		return result;
	}

	inline unsigned int decodeVarInt(PacketIn& p) {
		unsigned int result = 0;
  		unsigned int bits = 0;
		
		while (p.bytes[p.pos] & 0x80) {
    		byte b = p.bytes[p.pos];
    		result += ((b & 0x7F) << bits);
    		p.pos++;
    		bits += 7;
  		}

  		byte b = p.bytes[p.pos++];
  		result += ((b & 0x7F) << bits);

		return result;
	}
	inline void encodeBool(bool v, PacketOut& p) {
		p.bytes.push_back((byte)v);
	}

	inline bool decodeBool(PacketIn& p) {
		return (bool)p.bytes[p.pos++];
	}

	inline void encodeByte(byte v, PacketOut& p) {
		p.bytes.push_back(v);
	}

	inline byte decodeByte(PacketIn& p) {
		return p.bytes[p.pos++];
	}

	inline void encodeShort(short int v, PacketOut& p) {
		p.bytes.push_back((v >> 8) & 0xff);
		p.bytes.push_back(v & 0xff);
	}

	inline void encodeShort(short int v, std::vector<byte>& p) {
		p.push_back((v >> 8) & 0xff);
		p.push_back(v & 0xff);
	}

	inline short int decodeShort(PacketIn& p) {
		short int res = 0;
		for (int i = 0; i != 2; ++i) {
			res |= (byte)p.bytes[p.pos] << (8 - i * 8);
			p.pos++;
		}
		return res;
	}

	inline void encodeInt(long int v, PacketOut& p) {
		p.bytes.push_back((v >> 24) & 0xff);
		p.bytes.push_back((v >> 16) & 0xff);
		p.bytes.push_back((v >> 8) & 0xff);
		p.bytes.push_back(v & 0xff);
	}

	inline long int decodeInt(PacketIn& p) {
		long int res = 0;
		for (int i = 0; i != 4; ++i) {
			res |= (byte)p.bytes[p.pos] << (24 - i * 8);
			p.pos++;
		}
		return res;
	}

	inline void encodeLong(long long int v, PacketOut& p) {
		p.bytes.push_back((v >> 56) & 0xff);
		p.bytes.push_back((v >> 48) & 0xff);
		p.bytes.push_back((v >> 40) & 0xff);
		p.bytes.push_back((v >> 32) & 0xff);
		p.bytes.push_back((v >> 24) & 0xff);
		p.bytes.push_back((v >> 16) & 0xff);
		p.bytes.push_back((v >> 8) & 0xff);
		p.bytes.push_back(v & 0xff);
	}

	inline long long int decodeLong(PacketIn& p) {
		long long int res = 0;
		for (int i = 0; i != 8; ++i) {
			res |= (byte)p.bytes[p.pos] << (56 - i * 8);
			p.pos++;
		}
		return res;
	}

	inline void encodeString(std::string str, PacketOut& p) {
		std::vector<byte> prePend = encodeVarInt(str.size() + 1);

		for (int i = 0; i < prePend.size(); i++) {
			p.bytes.push_back(prePend[i]);
		}

		for (int i = 0; i < str.size(); i++) {
			p.bytes.push_back(str[i]);
		}
		p.bytes.push_back((byte)'\0');
	}

	inline std::string decodeString(PacketIn& p) {
		std::string res = "";

		int size = decodeVarInt(p);

		for (int i = 0; i < size - 1; i++) { //Don't include null char
			char a = (char)p.bytes[p.pos++];
			res += a;
		}

		p.pos++;//Skip null char

		return res;
	}

	inline void encodeFloat(float v, PacketOut& p) {
		long int l = reinterpret_cast<long int>(&v);
		p.bytes.push_back((l >> 24) & 0xff);
		p.bytes.push_back((l >> 16) & 0xff);
		p.bytes.push_back((l >> 8) & 0xff);
		p.bytes.push_back(l & 0xff);
	}

	inline float decodeFloat(PacketIn& p) {
		long int res = 0;
		for (int i = 0; i != 4; ++i) {
			res |= (byte)p.bytes[p.pos] << (24 - i * 8);
			p.pos++;
		}

		float* f = reinterpret_cast<float*>(&res);

		return *f;
	}

	inline void encodeDouble(double v, PacketOut& p) {
		long long int* l = reinterpret_cast<long long int*>(&v);
		p.bytes.push_back(((*l) >> 56) & 0xff);
		p.bytes.push_back(((*l) >> 48) & 0xff);
		p.bytes.push_back(((*l) >> 40) & 0xff);
		p.bytes.push_back(((*l) >> 32) & 0xff);
		p.bytes.push_back(((*l) >> 24) & 0xff);
		p.bytes.push_back(((*l) >> 16) & 0xff);
		p.bytes.push_back(((*l) >> 8) & 0xff);
		p.bytes.push_back((*l) & 0xff);
	}

	inline float decodeDouble(PacketIn& p) {
		long long int res = 0;
		for (int i = 0; i != 8; ++i) {
			res |= (byte)p.bytes[p.pos] << (56 - i * 8);
			p.pos++;
		}

		double* f = reinterpret_cast<double*>(&res);

		return *f;
	}


}