/* Encryption.cpp : This file contains the 'main' function.Program execution begins and ends there.
 * author : Keone Garcia
 * course : CS499 Capstone - Artifact_2 Encryption File
 * date : 01/26/2022
 *
 ** Project Requirements **
 ** Artifact Two is the Encryption File
 ** The artifact originates from course CS405 Secure Coding
 ** The goal of this artifact is to utilize components of XOR key to encrypt or decrypt a source string using a provided key.
 ** Code the XOR Encryption
 ** Code the Loading of Text File
 ** Code the Saving of Test File
 ** Create Data Files of Encryption and Decryption
 ** Use the inputdatafile.txt
 *
 *
 * -- Required Changes --
 *
 * Update Pseudo code descriptions and layout.
 * Include assert functions to verify input and output
 * Update Loops, branches, nesting, and use proper logic.
 * Ensure use of formating, date, logic, spacing, tabs, line breaks, curly braces, and brackets.
 * Consider whitespace, remove unnecessary spacing.
 * Ensure naming conventions of fields/constants/variables/parameters/classes/libraries clear and meaningful.
 * Remove excess comments - Over commenting, while useful can lead to distractions or large files.
 * Add commenting to functionality - Provide necessary comments to specific functions required .
 * Ensure functions have the variables that support them declared near by - Relocate declarations above where the function is called.
 * Ensure Project Requirements are fulfilled and improvements support quality, efficiency, and security.
 * Create ReadMe file and include project outcome and steps.
 *
*/

#include <cassert>	// Evaluate assertion
#include <fstream>	// Input/output file stream class
#include <iomanip>	// IO Manipulators
#include <iostream>	// Input/output stream
#include <sstream>	// String streams
#include <ctime>	// Convert time_t value to string

/*
*  ======== Implement XOR-based encryption ========
*/

/* encrypt or decrypt a source string using the provided key
*<param name = "source">input string to process< / param>
*<param name = "key">key to use in encryption / decryption< / param>
*<returns>transformed string< / returns>
*/
std::string encrypt_decrypt(const std::string& source, const std::string& key)
{
	// get lengths now instead of calling the function every time.
	// this would have most likely been in-lined by the compiler, but design for performance.
	const auto key_length = key.length();
	const auto source_length = source.length();

	// assert that our input data is good
	assert(key_length > 0);		// verify the key_length is greater than 0
	assert(source_length > 0);	// verify the source_length is greater than 0

	std::string output = source;	// output string source

	// loop through the source string char by char
	for (size_t i = 0; i < source_length; ++i)
	{ // TODO: Need to change the next line from output[i] = source[i]
	  // transform each character based on an xor of the key modded constrained to key length using a mod
		output[i] = source[i] ^ key[i % key_length]; // https://www.programmingalgorithms.com/algorithm/xor-encryption/cpp/
	}
	// our output length must equal our source length
	assert(output.length() == source_length);
	//std::cout << output.length() << std::endl;	//use the cout function to show the output and source if needed
	//std::cout << source_length << std::endl;

	// return the transformed string
	return output;		// used to return output
}

std::string read_file(const std::string& filename)
{
	std::string file_text = "John Q. Smith\nThis is my test string";

	// TODO: implement loading the file into a string
	//ifstream only not fstream for cleaner read write tool
	std::ifstream readfile; // create file descriptor fstream for readfile
	readfile.open(filename, std::ios::in); // open function of readfile

	if (readfile.is_open()) { // is file open
		std::string temp;
		while (std::getline(readfile, temp)) { // read object
			std::cout << temp << std::endl; // print data from file
		}
		readfile.close(); // close file object
		file_text = temp;
	}
	//std::cout << file_text << std::endl; // test file_text output		// use cout to test file output if needed
	//std::cout << filename << std::endl;  // test filename output

	return file_text;	// return file
}

/*
============ Implement loading data from text file into string ==================
*/

std::string get_student_name(const std::string& string_data)
{
	std::string student_name;
	// find the first newline
	size_t pos = string_data.find('\n');
	// did we find a newline
	if (pos != std::string::npos)
	{ // we did, so copy that substring as the student name
		student_name = string_data.substr(0, pos);
	}

	return student_name;
}

/*
============== Save string to text file ===============
*/

void save_data_file(const std::string& filename, const std::string& student_name, const std::string& key, const std::string& data)
{
	//  TODO: implement file saving
	std::ofstream MyWriteFile(filename); // file format from https://www.w3schools.com/cpp/cpp_files.asp
	//std::myReadFile.open(filename);

	MyWriteFile << student_name; // Line 1: student name (Firstname, Lastname)
	MyWriteFile << __TIMESTAMP__; // Line 2: timestamp (yyyy-mm-dd)
	MyWriteFile << key;  // Line 3: key used
	MyWriteFile << data; // Line 4+: data
	MyWriteFile.close(); // close()
}

int main()
{
	std::cout << "Encryption Decryption - Keone Garcia 01/26/2022 Capstone!" << std::endl;

	// input file format
	// Line 1: <students name>
	// Line 2: <Lorem Ipsum Generator website used> https://pirateipsum.me/ (could be https://www.lipsum.com/ or one of https://www.shopify.com/partners/blog/79940998-15-funny-lorem-ipsum-generators-to-shake-up-your-design-mockups)

	// Lines 3+: <lorem ipsum generated with 3 paragraphs>

	// Fire in the hole bowsprit Jack Tar gally holystone sloop grog heave to grapple Sea Legs. Gally hearties case shot crimp spirits pillage galleon chase guns skysail yo-ho-ho. Jury mast coxswain measured fer yer chains man-of-war Privateer yardarm aft handsomely Jolly Roger mutiny.
	// Hulk coffer doubloon Shiver me timbers long clothes skysail Nelsons folly reef sails Jack Tar Davy Jones' Locker. Splice the main brace ye fathom me bilge water walk the plank bowsprit gun Blimey wench. Parrel Gold Road clap of thunder Shiver me timbers hempen halter yardarm grapple wench bilged on her anchor American Main.
	// Brigantine coxswain interloper jolly boat heave down cutlass crow's nest wherry dance the hempen jig spirits. Interloper Sea Legs plunder shrouds knave sloop run a shot across the bow Jack Ketch mutiny barkadeer. Heave to gun matey Arr draft jolly boat marooned Cat o'nine tails topsail Blimey.

	const std::string file_name = "inputdatafile.txt";
	const std::string encrypted_file_name = "encrypteddatafile.txt";
	//std::cout << "STUDENT NAME" << student_name;
	const std::string decrypted_file_name = "decrytpteddatafile.txt";
	const std::string source_string = read_file(file_name);
	//std::cout << read_file;
	//std::cout << source_string;
	const std::string key = "password";

	// get the student name from the data file
	const std::string student_name = get_student_name(source_string);

	// encrypt sourceString with key
	const std::string encrypted_string = encrypt_decrypt(source_string, key);

	// save encrypted_string to file
	save_data_file(encrypted_file_name, student_name, key, encrypted_string);

	// decrypt encryptedString with key
	const std::string decrypted_string = encrypt_decrypt(encrypted_string, key);

	// save decrypted_string to file
	save_data_file(decrypted_file_name, student_name, key, decrypted_string);

	std::cout << "Read File: " << file_name << " - Encrypted To: " << encrypted_file_name << " - Decrypted To: " << decrypted_file_name << std::endl;
}

