#include <cstdio>
#include <iostream>
#include <vector>
#include "reader.hpp"
#include "constant_pool.hpp"
#include "bit.hpp"

#define W(x) std::cerr << "\033[35m" << #x << "=" << x << "\033[0m" << "\n";

/**
 * Read the constant pool data
 * @param file The file to extract the data
 * @param count How many constants to extract
 */
void read_cp (jvm::Reader& file, uint32_t count) {
	for (int i = 0; i < count; ++i) {
		auto aux = file.getNextByte();
		auto tag = aux.value.number;

		switch (tag) {
			case jvm::CP_TAGS::Class: {
				std::cout << "\t Tag: Class"                      << std::endl;
				auto name_index = file.getNextHalfWord();
				break;
			}
			case jvm::CP_TAGS::FieldRef: {
				std::cout << "\t Tag: Field Reference" << std::endl;
				auto name_index = file.getNextHalfWord();
				auto name_type_index = file.getNextHalfWord();
				break;
			}
			case jvm::CP_TAGS::MethodRef: {
				std::cout << "\t Tag: Method Reference"           << std::endl;
				auto name_index = file.getNextHalfWord();
				auto name_type_index = file.getNextHalfWord();
				break;
			}
			case jvm::CP_TAGS::InterfaceMethodRef: {
				std::cout << "\t Tag: Interface Method Reference" << std::endl;
				auto name_index = file.getNextHalfWord();
				auto name_type_index = file.getNextHalfWord();
				break;
			}
			case jvm::CP_TAGS::String: {
				std::cout << "\t Tag: String"                     << std::endl;
				auto string_index = file.getNextHalfWord();
				break;
			}
			case jvm::CP_TAGS::Integer: {
				std::cout << "\t Tag: Integer"                    << std::endl;
				auto bytes = file.getNextHalfWord();
				break;
			}
			case jvm::CP_TAGS::Float: {
				std::cout << "\t Tag: Float"                      << std::endl;
				auto bytes = file.getNextHalfWord();
				break;
			}
			case jvm::CP_TAGS::Long: {
				std::cout << "\t Tag: Long"                       << std::endl;
				auto high_bytes = file.getNextWord();
				auto low_bytes = file.getNextWord();
				break;
			}
			case jvm::CP_TAGS::Double: {
				std::cout << "\t Tag: Double"                     << std::endl;
				auto high_bytes = file.getNextWord();
				auto low_bytes = file.getNextWord();
				break;
			}
			case jvm::CP_TAGS::NameAndType: {
				std::cout << "\t Tag: Name And Type"              << std::endl;
				auto name_index = file.getNextHalfWord();
				auto descriptor_index = file.getNextHalfWord();
				break;
			}
			case jvm::CP_TAGS::Utf8: {
				std::cout << "\t Tag: UTF-8"                      << std::endl;
				auto len = file.getNextHalfWord().value.number;
				auto bytes = std::vector<jvm::Byte>(len);

				for (int i = 0; i < len; i++) {
					bytes[i] = file.getNextByte();
				}

				break;
			}
			case jvm::CP_TAGS::MethodHandle: {
				std::cout << "\t Tag: Method Handle"              << std::endl;
				auto reference_kind = file.getNextByte();
				auto reference_index = file.getNextHalfWord();
				break;
			}
			case jvm::CP_TAGS::MethodType: {
				std::cout << "\t Tag: Method Type"                << std::endl;
				auto descriptor_index = file.getNextHalfWord();
				break;
			}
			case jvm::CP_TAGS::InvokeDynamic: {
				std::cout << "\t Tag: Invoke Dynamic"             << std::endl;
				auto bootstrap_method_attr_index = file.getNextHalfWord();
				auto name_and_type_index = file.getNextHalfWord();
			}
			default:
				throw "Invalid conversion, file is wrong";
		}
	}
	
}

void print_access (uint32_t flag) {
	switch (flag) {
		case jvm::FLAGS::PUBLIC:
			std::cout << "\t Access: Public"					<< std::endl;
			break;
		case jvm::FLAGS::PRIVATE:
			std::cout << "\t Access: Private"					<< std::endl;
			break;
		case jvm::FLAGS::PROTECTED:
			std::cout << "\t Access: Protected"					<< std::endl;
			break;
		case jvm::FLAGS::STATIC:
			std::cout << "\t Access: Static"					<< std::endl;
			break;
		case jvm::FLAGS::FINAL:
			std::cout << "\t Access: Final"						<< std::endl;
			break;
		case jvm::FLAGS::SUPER:
			std::cout << "\t Access: Super"						<< std::endl;
			break;
		case jvm::FLAGS::VOLATILE:
			std::cout << "\t Access: Volatile"					<< std::endl;
			break;
		case jvm::FLAGS::TRANSIENT:
			std::cout << "\t Access: Transient"					<< std::endl;
			break;
		case jvm::FLAGS::INTERFACE:
			std::cout << "\t Access: Interface"					<< std::endl;
			break;
		case jvm::FLAGS::ABSTRACT:
			std::cout << "\t Access: Abstract"					<< std::endl;
			break;
		case jvm::FLAGS::SYNTHETIC:
			std::cout << "\t Access: Synthetic"					<< std::endl;
			break;
		case jvm::FLAGS::ANNOTATION:
			std::cout << "\t Access: Annotation"				<< std::endl;
			break;
		case jvm::FLAGS::ENUM:
			std::cout << "\t Access: Enum"						<< std::endl;
			break;
		default:
			throw "Invalid conversion, file is wrong";
	}
}

/**
 * Iniciate reading the .class file
 */
void init (std::string filename) {
	auto file = jvm::Reader();

	file.open(filename);

	std::cout << "> .class" << std::endl;

	auto min_version = file.getNextHalfWord();
	std::cout << "Min Version: " << min_version.value.number << std::endl;

	auto max_version = file.getNextHalfWord();
	std::cout << "Max Version: " << max_version.value.number << std::endl;

	auto cp_count = file.getNextHalfWord();
	std::cout << "CP count: " << cp_count.value.number - 1 << std::endl;

	if (cp_count.value.number != 0) {
		read_cp(file, cp_count.value.number);
	}

	auto access_flags = file.getNextHalfWord();

	read_access(access_flags.value.number);

	auto this_class = file.getNextHalfWord();
	auto super_class = file.getNextHalfWord();
	auto interfaces_count = file.getNextHalfWord();
}

int main (int argc, char *argv[ ]) {
	if (argc < 2) {
        std::cout << "Número de argumentos insuficientes" << std::endl;
        return 0;
    }

    try {
		init(argv[1]);
	} catch (std::string error) {
	    std::cout << error << std::endl;
	}
}
