#pragma once

typedef u64 strId;

// CRC64_ECMA_182 precomputed table
constexpr u64 crcTable[256] = {
	0x0000000000000000ul, 0x42f0e1eba9ea3693ul, 0x85e1c3d753d46d26ul, 0xc711223cfa3e5bb5ul, 0x493366450e42ecdful, 0x0bc387aea7a8da4cul, 0xccd2a5925d9681f9ul, 0x8e224479f47cb76aul, 0x9266cc8a1c85d9beul, 0xd0962d61b56fef2dul, 0x17870f5d4f51b498ul, 0x5577eeb6e6bb820bul, 0xdb55aacf12c73561ul, 0x99a54b24bb2d03f2ul, 0x5eb4691841135847ul, 0x1c4488f3e8f96ed4ul, 0x663d78ff90e185eful, 0x24cd9914390bb37cul, 0xe3dcbb28c335e8c9ul, 0xa12c5ac36adfde5aul, 0x2f0e1eba9ea36930ul, 0x6dfeff5137495fa3ul, 0xaaefdd6dcd770416ul, 0xe81f3c86649d3285ul, 0xf45bb4758c645c51ul, 0xb6ab559e258e6ac2ul, 0x71ba77a2dfb03177ul, 0x334a9649765a07e4ul, 0xbd68d2308226b08eul, 0xff9833db2bcc861dul, 0x388911e7d1f2dda8ul, 0x7a79f00c7818eb3bul, 0xcc7af1ff21c30bdeul, 0x8e8a101488293d4dul, 0x499b3228721766f8ul, 0x0b6bd3c3dbfd506bul, 0x854997ba2f81e701ul, 0xc7b97651866bd192ul, 0x00a8546d7c558a27ul, 0x4258b586d5bfbcb4ul, 0x5e1c3d753d46d260ul, 0x1cecdc9e94ace4f3ul, 0xdbfdfea26e92bf46ul, 0x990d1f49c77889d5ul, 0x172f5b3033043ebful, 0x55dfbadb9aee082cul, 0x92ce98e760d05399ul, 0xd03e790cc93a650aul, 0xaa478900b1228e31ul, 0xe8b768eb18c8b8a2ul, 0x2fa64ad7e2f6e317ul, 0x6d56ab3c4b1cd584ul, 0xe374ef45bf6062eeul, 0xa1840eae168a547dul, 0x66952c92ecb40fc8ul, 0x2465cd79455e395bul, 0x3821458aada7578ful, 0x7ad1a461044d611cul, 0xbdc0865dfe733aa9ul, 0xff3067b657990c3aul, 0x711223cfa3e5bb50ul, 0x33e2c2240a0f8dc3ul, 0xf4f3e018f031d676ul, 0xb60301f359dbe0e5ul, 0xda050215ea6c212ful, 0x98f5e3fe438617bcul, 0x5fe4c1c2b9b84c09ul, 0x1d14202910527a9aul, 0x93366450e42ecdf0ul, 0xd1c685bb4dc4fb63ul, 0x16d7a787b7faa0d6ul, 0x5427466c1e109645ul, 0x4863ce9ff6e9f891ul, 0x0a932f745f03ce02ul, 0xcd820d48a53d95b7ul, 0x8f72eca30cd7a324ul, 0x0150a8daf8ab144eul, 0x43a04931514122ddul, 0x84b16b0dab7f7968ul, 0xc6418ae602954ffbul, 0xbc387aea7a8da4c0ul, 0xfec89b01d3679253ul, 0x39d9b93d2959c9e6ul, 0x7b2958d680b3ff75ul, 0xf50b1caf74cf481ful, 0xb7fbfd44dd257e8cul, 0x70eadf78271b2539ul, 0x321a3e938ef113aaul, 0x2e5eb66066087d7eul, 0x6cae578bcfe24bedul, 0xabbf75b735dc1058ul, 0xe94f945c9c3626cbul, 0x676dd025684a91a1ul, 0x259d31cec1a0a732ul, 0xe28c13f23b9efc87ul, 0xa07cf2199274ca14ul, 0x167ff3eacbaf2af1ul, 0x548f120162451c62ul, 0x939e303d987b47d7ul, 0xd16ed1d631917144ul, 0x5f4c95afc5edc62eul, 0x1dbc74446c07f0bdul, 0xdaad56789639ab08ul, 0x985db7933fd39d9bul, 0x84193f60d72af34ful, 0xc6e9de8b7ec0c5dcul, 0x01f8fcb784fe9e69ul, 0x43081d5c2d14a8faul, 0xcd2a5925d9681f90ul, 0x8fdab8ce70822903ul, 0x48cb9af28abc72b6ul, 0x0a3b7b1923564425ul, 0x70428b155b4eaf1eul, 0x32b26afef2a4998dul, 0xf5a348c2089ac238ul, 0xb753a929a170f4abul, 0x3971ed50550c43c1ul, 0x7b810cbbfce67552ul, 0xbc902e8706d82ee7ul, 0xfe60cf6caf321874ul, 0xe224479f47cb76a0ul, 0xa0d4a674ee214033ul, 0x67c58448141f1b86ul, 0x253565a3bdf52d15ul, 0xab1721da49899a7ful, 0xe9e7c031e063acecul, 0x2ef6e20d1a5df759ul, 0x6c0603e6b3b7c1caul, 0xf6fae5c07d3274cdul, 0xb40a042bd4d8425eul, 0x731b26172ee619ebul, 0x31ebc7fc870c2f78ul, 0xbfc9838573709812ul, 0xfd39626eda9aae81ul, 0x3a28405220a4f534ul, 0x78d8a1b9894ec3a7ul, 0x649c294a61b7ad73ul, 0x266cc8a1c85d9be0ul, 0xe17dea9d3263c055ul, 0xa38d0b769b89f6c6ul, 0x2daf4f0f6ff541acul, 0x6f5faee4c61f773ful, 0xa84e8cd83c212c8aul, 0xeabe6d3395cb1a19ul, 0x90c79d3fedd3f122ul, 0xd2377cd44439c7b1ul, 0x15265ee8be079c04ul, 0x57d6bf0317edaa97ul, 0xd9f4fb7ae3911dfdul, 0x9b041a914a7b2b6eul, 0x5c1538adb04570dbul, 0x1ee5d94619af4648ul, 0x02a151b5f156289cul, 0x4051b05e58bc1e0ful, 0x87409262a28245baul, 0xc5b073890b687329ul, 0x4b9237f0ff14c443ul, 0x0962d61b56fef2d0ul, 0xce73f427acc0a965ul, 0x8c8315cc052a9ff6ul, 0x3a80143f5cf17f13ul, 0x7870f5d4f51b4980ul, 0xbf61d7e80f251235ul, 0xfd913603a6cf24a6ul, 0x73b3727a52b393ccul, 0x31439391fb59a55ful, 0xf652b1ad0167feeaul, 0xb4a25046a88dc879ul, 0xa8e6d8b54074a6adul, 0xea16395ee99e903eul, 0x2d071b6213a0cb8bul, 0x6ff7fa89ba4afd18ul, 0xe1d5bef04e364a72ul, 0xa3255f1be7dc7ce1ul, 0x64347d271de22754ul, 0x26c49cccb40811c7ul, 0x5cbd6cc0cc10fafcul, 0x1e4d8d2b65facc6ful, 0xd95caf179fc497daul, 0x9bac4efc362ea149ul, 0x158e0a85c2521623ul, 0x577eeb6e6bb820b0ul, 0x906fc95291867b05ul, 0xd29f28b9386c4d96ul, 0xcedba04ad0952342ul, 0x8c2b41a1797f15d1ul, 0x4b3a639d83414e64ul, 0x09ca82762aab78f7ul, 0x87e8c60fded7cf9dul, 0xc51827e4773df90eul, 0x020905d88d03a2bbul, 0x40f9e43324e99428ul, 0x2cffe7d5975e55e2ul, 0x6e0f063e3eb46371ul, 0xa91e2402c48a38c4ul, 0xebeec5e96d600e57ul, 0x65cc8190991cb93dul, 0x273c607b30f68faeul, 0xe02d4247cac8d41bul, 0xa2dda3ac6322e288ul, 0xbe992b5f8bdb8c5cul, 0xfc69cab42231bacful, 0x3b78e888d80fe17aul, 0x7988096371e5d7e9ul, 0xf7aa4d1a85996083ul, 0xb55aacf12c735610ul, 0x724b8ecdd64d0da5ul, 0x30bb6f267fa73b36ul, 0x4ac29f2a07bfd00dul, 0x08327ec1ae55e69eul, 0xcf235cfd546bbd2bul, 0x8dd3bd16fd818bb8ul, 0x03f1f96f09fd3cd2ul, 0x41011884a0170a41ul, 0x86103ab85a2951f4ul, 0xc4e0db53f3c36767ul, 0xd8a453a01b3a09b3ul, 0x9a54b24bb2d03f20ul, 0x5d45907748ee6495ul, 0x1fb5719ce1045206ul, 0x919735e51578e56cul, 0xd367d40ebc92d3fful, 0x1476f63246ac884aul, 0x568617d9ef46bed9ul, 0xe085162ab69d5e3cul, 0xa275f7c11f7768aful, 0x6564d5fde549331aul, 0x279434164ca30589ul, 0xa9b6706fb8dfb2e3ul, 0xeb46918411358470ul, 0x2c57b3b8eb0bdfc5ul, 0x6ea7525342e1e956ul, 0x72e3daa0aa188782ul, 0x30133b4b03f2b111ul, 0xf7021977f9cceaa4ul, 0xb5f2f89c5026dc37ul, 0x3bd0bce5a45a6b5dul, 0x79205d0e0db05dceul, 0xbe317f32f78e067bul, 0xfcc19ed95e6430e8ul, 0x86b86ed5267cdbd3ul, 0xc4488f3e8f96ed40ul, 0x0359ad0275a8b6f5ul, 0x41a94ce9dc428066ul, 0xcf8b0890283e370cul, 0x8d7be97b81d4019ful, 0x4a6acb477bea5a2aul, 0x089a2aacd2006cb9ul, 0x14dea25f3af9026dul, 0x562e43b4931334feul, 0x913f6188692d6f4bul, 0xd3cf8063c0c759d8ul, 0x5dedc41a34bbeeb2ul, 0x1f1d25f19d51d821ul, 0xd80c07cd676f8394ul, 0x9afce626ce85b507ul
};

// CRC64_ECMA_182
constexpr u64 crc64(const u8* bytes, u32 length) {
	//constexpr u64 polynom = 0x42f0e1eba9ea3693;
	constexpr u64 initial = 0x0;

	u64 c = initial;
	for (u32 i = 0; i < length; i++) {
		u8 idx = static_cast<u8>(c >> 56) ^ bytes[i];
		c = (c << 8) ^ crcTable[idx];
	}
	return c;
}
// CRC64_ECMA_182
constexpr u64 crc64(const char* bytes, u32 length) {
	//constexpr u64 polynom = 0x42f0e1eba9ea3693;
	constexpr u64 initial = 0x0;

	u64 c = initial;
	for (u32 i = 0; i < length; i++) {
		u8 idx = static_cast<u8>(c >> 56) ^ bytes[i];
		c = (c << 8) ^ crcTable[idx];
	}
	return c;
}

// String Hashing
constexpr strId hashString(const char* cstr, size_t length) {
	return crc64(cstr, static_cast<u32>(length));
}

// String Id literals
constexpr strId operator""sid(const char* cstr, size_t length) {
	return crc64(cstr, static_cast<u32>(length));
}
constexpr strId operator""sid(const wchar_t* cstr, size_t length) {
	return crc64(reinterpret_cast<const u8*>(cstr), static_cast<u32>(length) * sizeof(wchar_t));
}
constexpr strId operator""sid(const char16_t* cstr, size_t length) {
	return crc64(reinterpret_cast<const u8*>(cstr), static_cast<u32>(length) * sizeof(char16_t));
}
constexpr strId operator""sid(const char32_t* cstr, size_t length) {
	return crc64(reinterpret_cast<const u8*>(cstr), static_cast<u32>(length) * sizeof(char32_t));
}


/* ==== Parsing ==== */

// Return true if the character represents a digit in base 10, false otherwise
inline constexpr bool isBase10Digit(const char c) {
	return c >= 48 && c <= 57;
}

// Converts an ascii character to the digit it represents
// Expects a correct input
inline constexpr u8 charToDigit(const char c) {
	return (c <= 57) ? c - 48 : ((c <= 90) ? c - 55 : c - 87);
	//return (c >= 97) ? c - 87 : ((c >= 65) ? c - 55 : c - 48);
	//return (c >= 48 && c <= 57) ? c - 48 : ((c >= 65 && c <= 70) ? c - 55 : ((c >= 97 && c <= 102) ? c - 87 : c));
}

template<typename T>
constexpr T parseSignedIntegerLiteral(const char* cstr, size_t length) {
	T value = 0;
	size_t offset = 0;
	bool negative = false;

	// Sign detection
	if (cstr[offset] == '-') {
		negative = true;
		offset++;
	} else if (cstr[offset] == '+') {
		negative = false;
		offset++;
	}

	// Base detection
	u32 base = 10;
	if (cstr[offset] == '0') {
		base = 8;
		offset++;
	}
	if (cstr[offset] == 'x' || cstr[offset] == 'X') {
		base = 16;
		offset++;
	} else if (cstr[offset] == 'b' || cstr[offset] == 'B') {
		base = 2;
		offset++;
	}

	while (cstr[offset] == '0') offset++; // Skip leading zeros

	// Parse
	while (offset < length) {
		if (cstr[offset] == '\'') {
			offset++;
			continue; // Skip c++14 digit separator
		}
		// Break when reaching an integer-suffix, we expect the type of value to handle it
		if (cstr[offset] == 'u' || cstr[offset] == 'U' || cstr[offset] == 'l' || cstr[offset] == 'L') break;
		value *= base;
		value += charToDigit(cstr[offset]);
		offset++;
	}

	if (negative) {
		value = -value;
	}

	return value;
}

template<typename T>
constexpr T parseUnsignedIntegerLiteral(const char* cstr, size_t length) {
	T value = 0;
	size_t offset = 0;

	// Skip positive sign
	if (cstr[offset] == '+') {
		offset++;
	}

	// Base detection
	u32 base = 10;
	if (cstr[offset] == '0') {
		base = 8;
		offset++;
	}
	if (cstr[offset] == 'x' || cstr[offset] == 'X') {
		base = 16;
		offset++;
	} else if (cstr[offset] == 'b' || cstr[offset] == 'B') {
		base = 2;
		offset++;
	}

	while (cstr[offset] == '0') offset++; // Skip leading zeros

	// Parse
	while (offset < length) {
		if (cstr[offset] == '\'') {
			offset++;
			continue; // Skip c++14 digit separator
		}
		// Break when reaching an integer-suffix, we expect the type of value to handle it
		if (cstr[offset] == 'u' || cstr[offset] == 'U' || cstr[offset] == 'l' || cstr[offset] == 'L') break;
		value *= base;
		value += charToDigit(cstr[offset]);
		offset++;
	}

	return value;
}

// Lightweight integer literal parser
// expects a properly formed (c++ style) integer literal with the exception of a potential sign
template<typename T>
constexpr T parseIntegerLiteral(const char* cstr, size_t length) {}
template<>
constexpr u8 parseIntegerLiteral<u8>(const char* cstr, size_t length) { return parseUnsignedIntegerLiteral<u8>(cstr, length); }
template<>
constexpr u16 parseIntegerLiteral<u16>(const char* cstr, size_t length) { return parseUnsignedIntegerLiteral<u16>(cstr, length); }
template<>
constexpr u32 parseIntegerLiteral<u32>(const char* cstr, size_t length) { return parseUnsignedIntegerLiteral<u32>(cstr, length); }
template<>
constexpr u64 parseIntegerLiteral<u64>(const char* cstr, size_t length) { return parseUnsignedIntegerLiteral<u64>(cstr, length); }
template<>
constexpr i8 parseIntegerLiteral<i8>(const char* cstr, size_t length) { return parseSignedIntegerLiteral<i8>(cstr, length); }
template<>
constexpr i16 parseIntegerLiteral<i16>(const char* cstr, size_t length) { return parseSignedIntegerLiteral<i16>(cstr, length); }
template<>
constexpr i32 parseIntegerLiteral<i32>(const char* cstr, size_t length) { return parseSignedIntegerLiteral<i32>(cstr, length); }
template<>
constexpr i64 parseIntegerLiteral<i64>(const char* cstr, size_t length) { return parseSignedIntegerLiteral<i64>(cstr, length); }
