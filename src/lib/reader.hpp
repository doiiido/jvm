#pragma once

#include <fstream>
#include "bit.hpp"

namespace jvm {

	class ByteCodeReader {
	private:

		//< stream with the .class file
		std::ifstream _class;
		std::vector<uint8_t> bytes;
		uint64_t index;

		/**
		* Check if it is a .class file
		* @retval TRUE Has CAFEBABE as initial word in the file
		* @retval FALSE Has not CAFEBABE as initial word in the file
		*/
		bool isValid();

		/**
		* .class file size
		* @retval Size of the .class file
		*/
		uint64_t size();

	public:

		/**
		* Open a .class file
		* @param  filename The name of the .class we are opening
		*/
		void open(std::string filename);

		/**
		* Get the next byte in the file
		* @retval The next Byte
		*/
		jvm::Byte getNextByte();

		/**
		* Get the next half word in the file
		* @retval The next Half Word
		*/
		jvm::HalfWord getNextHalfWord();

		/**
		* Get the next word in the file
		* @retval The next Word
		*/
		jvm::Word getNextWord();

		/**
		* Close the .class file
		*/
		void close();
	};

}
