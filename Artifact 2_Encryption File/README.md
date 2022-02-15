# CS-405-Secure Coding
## Secure Coding-Vulnerabilities-Risks-Standards

### Summarize the project and what problem it was solving?

Secure coding is software development that defends against active security threats. Although many of the existing secure coding best practices are specific to programming languages and domains, there is also language- and domain‐neutral knowledge to be obtained by software developers.

The artifact is focused on best practices covering secure coding in C/C++. Providing skills necessary to identify and prevent software vulnerabilities found in insecure code. The objective of this project is to give a solid foundation in developing secure software and creating policies and standards to make the principles predictable and repeatable.

Specific to Encryption_File, when working through each section. I had not been familiar with the function XOR, by reviewing the resource of https://www.programmingalgorithms.com/algorithm/xor-encryption/cpp. I began to understand the structure necessary for the XOR function to work in the code given for this assignment. By adding (^ key[i % keyLen];) this allowed the encryption to use a key at each value for “password”, (const std::string key = "password"). I then followed sources to read the inputextfile, I struggled to access the file even though in code it stated to return text_file. I later found that for the file to be viewed it needed to be embedded into the project folder created in Microsoft Visual Studio source/repo/project_name. This corrected the issue, and the last step was to add code to save the file for encryption and decryption to take place.

/* =============== Include ============== /*

#include <cassert>	// Evaluate assertion
#include <fstream>	// Input/output file stream class
#include <iomanip>	// IO Manipulators
#include <iostream>	// Input/output stream
#include <sstream>	// String streams
#include <ctime>	// Convert time_t value to string

/*  ======== Implement XOR-based encryption ======== */

/* encrypt or decrypt a source string using the provided key
*<param name = "source">input string to process< / param>
*<param name = "key">key to use in encryption / decryption< / param>
*<returns>transformed string< / returns>
*/
  
// When loading the file ensure the file is embedded within the source file location of the project. Otherwise MVS may not validate that the file exists.
// test that the file is available using the following commands
  
//std::cout << file_text << std::endl; // test file_text output
//std::cout << filename << std::endl;  // test filename output
  
// When saving the file use ofstream MyWriteFile

/* ============== Save string to text file =============== */

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
  
// Consider using cout to verify the student name, read_file, and source_String  
//std::cout << "STUDENT NAME" << student_name;
//std::cout << read_file;
//std::cout << source_string;
  
// The program should Read File: inputdatafile.txt - Encrypted To: encrypteddatafile.txt - Decrypted To: decrytpteddatafile.txt
