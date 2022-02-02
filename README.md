# kgarcia-snhu.github.io

## Southern New Hampshire University Software Engineering ePortfolio

## CS499 Computer Science Capstone
My name is Keone, best pronounced as (Ke Own E). I continue to hold the Presidents List at Southern New Hampshire University. I’m a first-generation dual major, studying computer science and data analytics. Both of which have concentrations in software engineering and project management. The projects associated with this page are part of the Computer Science program from Southern New Hampshire University. The ePortfolio contains a narrated code review, enhancements to completed work in three categories of (Software Design and Engineering, Algorithms and Data Structure, and Databases), justification narratives, and a professional self-assessment. These artifacts have been developed previously in my years at SNHU and have been further developed to expand the complexity of software and functionality to showcase my abilities.  

## Table of Content
1. Professional Self-Assessment
2. Code Review
3. Software Design and Engineering: Artifact 1 Thermostat Lab Enhancement
4. Algorithms and Data Structure: Artifact 2 Encryption File Enhancement
5. Databases: Artifact 3 RMA_Orders Database Enhancement 

## Professional Self-Assessment
I’ve been in the computer science program since January of 2018, putting me at 4 years to complete my degree. While in the program, I have learned that there are many applications for hardware and software to co-exist. When building a computer to meet the needs of a client; cost, performance, and limitations are considered to provide the best possible outcome. When developing code, security policies are used to ensure that code written relinquishes bugs, vulnerabilities, and data loss. Within the computer science program, having the ability to work with diverse programming languages along with comments in code are good practice, and provides avenues to reverse engineer software.

As a first generation dual major working towards a BSCS and BSDA, I aim to work as a software engineer and a data analyst/scientist. To outline my career goals in computer science, I would like to share projections from careers at www.computerscience.org. From “the Bureau of Labor Statistics (BLS) projects jobs for computer and information technology occupations will grow significantly — an 11% increase — from 2019 to 2029. This is significantly faster than the projected growth rate for all occupations nationwide. Some computer science jobs, such as computer and information research scientists, are projected to grow even more quickly.” (Red Ventures, 2022)

From this projection, I strongly believe that I am on course to achieve knowledge and skills of high demand in the industry. I understand that both degrees and the concentrations afforded are unique. But I feel that each provides a necessary skill in the computer science industry. Being new to coding, SNHU has provided me with my first steps to developing code. I plan to pursue an internship or a position in the industry that can continue to teach me industry standards so that I may implement each skill as I learn. My dream job is to achieve the status of a data scientist. I would like to pursue this position now that I am learning code to develop algorithms that satisfy analytical reasoning for reports. At some point in my career, I would like to teach students skills and knowledge based on my experience in the industry.

Red Ventures, C. (2022, January 12). Compare top careers in computer science. Get an Education the World Needs. ComputerScience.org. Retrieved January 30, 2022, from
[https://www.computerscience.org/careers/](https://www.computerscience.org/careers/)

## Code Review
Provided is a link to my Code Review video where I address Existing Functionality, Code Analysis, and Enhancements planned.
- link to code review video - [Code Review Video](https://www.screencast.com/t/u37vPUqJPSo)
- link to script - [Code Review Script.pdf](https://github.com/kgarcia-snhu/kgarcia-snhu.github.io/files/7956249/Code.Review.Script.pdf)

 1. Existing Functionality: A walk through of what existing code does and how it works.
 2. Code Analysis: A review of code structure, commenting, efficiencies, and an explanation of findings.
 3. Enhancements: A walk through of enhancements planned and an explanation of execution or limitations.

## Software Design and Engineering: Artifact 1 Thermostat Lab Enhancement
![CC3220S LaunchPad2](https://user-images.githubusercontent.com/79305154/151455474-1afa7d76-0b53-4522-8b75-0416d99671e1.JPG)

I have chosen to select the artifact Thermostat Lab. The artifact originates from course CS350 Emerging Systems and uses Code Composer Studio with a Simple Link CC3220S Launchpad in C language. The goal of this artifact is to utilize components of the launchpad to simulate a working thermostat for an HVAC system. Indicating when the unit is active using an LED, while providing an output of sensor readings and setpoint parameters sent to be viewed on a mobile app.

* The enhancement plan includes updates to expand the complexity of software by incorporating comments where necessary, using proper syntax, and evaluating proper use of gpiointerrupt, timers, UART. 
* I plan to add or edit the LED output to enhance functionality. While improving code by finding bugs, checking for code clarity, consistency, design, features, and coding standard use. I also plan to create or update a ReadMe.txt file and update naming conventions for readability.

I selected this artifact as I believe the CC3220S Launchpad adds many possibilities to Software Design and Engineering. Specific to the Thermostat Lab project, many components required setup configuration to communicate with the board dependent on availability of pins and resources. I enjoyed learning the basics of C language and realized by enhancing this project. I had a whole new experience when developing the changes to enhance my ideas. I knew that the best course of action would be to start at the very beginning regarding the program, its software, and settings. The work performed ensured each process had proper components of the CC3220S Launchpad drivers implemented in configuration and code. The enhancement changes to the gpiointerrupt LED functionality were then introduced to include both the heat and cool functions to activate an LED. The specific components of the artifact that showcase my abilities in software design and engineering are in the use of State Machines to activate an LED when either the heat or cool function is triggered from the callback. I found that on the CC3220S board, there were limitations to the resources available in using LED outputs. It was determined that in order to signal an active function of heat or cool, the Red LED would blink for either function when active. Indicating when the unit is in heat or cool mode, while providing an output of temperature sensor readings based on the setpoint parameters sent to a wireless thermostat, recording measures to a database. 

#### For example:
``` 
/*
========== State Machine's States ==========
 */

/* Increase or decrease setpoint from gpioButtonFxn0 or gpioButtonFxn1 buttons */
enum BF_STATES {BF_SMStart, BF_SMWaitRise, BF_SMWaitFall} BF_STATE;

/*
 * BF_SMStart - Start
 *  BF_SMWaitRise - Button Flag Wait Rise
 *  BF_SMWaitFall - Button Flag Wait Fall
 *  Button Flag callback if gpioButtonFxn0 or gpioButtonFxn1 are selected as 0 or 1
 */

void TickFct_ButtonFlag() {
    switch(BF_STATE) { // Transitions
        case BF_SMStart:
            /* Initialize buttons to 0 */
            Button0_Flag = 0; // Initialize outputs
            Button1_Flag = 0; // Initialize outputs
            BF_STATE = BF_SMWaitRise;
            break;
        case BF_SMWaitRise:
            // Wait for button flag 0(Right) or 1(Left) to be selected
            if (Button0_Flag || Button1_Flag) {
                BF_STATE = BF_SMWaitFall;
            }
            break;
        case BF_SMWaitFall:
            // If no button flag is raised go to BF_SMWaitRiseLow
            if (!(Button0_Flag || Button1_Flag)){
                BF_STATE = BF_SMWaitRise;
            }
            break;
        default:
            BF_STATE = BF_SMStart; // Indicates initial call
            break;
    }

    switch(BF_STATE) { // State actions
        case BF_SMStart:
            break;
        case BF_SMWaitRise:
            break;
        case BF_SMWaitFall:
            // Update setpoint temperature value
            if (Button0_Flag) {
                setpoint -= 1;       // Decrease setpoint by 1° C // <30,25,1,0339> <temperature, setpoint, heat, cool, seconds>
                Button0_Flag = 0;    // Update Button0_Flag
            }
            if (Button1_Flag) {
                setpoint += 1;       // Increase setpoint by 1° C // <30,32,0,0339> <temperature, setpoint, heat, cool, seconds>
                Button1_Flag = 0;    // Update Button1_Flag
            }
            break;
        default:
            break;
    }
}


/*
========== State Machine's States Continued ==========
 */

/* Temperature heat or cool indicator activates LED red ON or OFF indicating heater is on or off */
enum TL_STATES {TL_SMStart, TL_SMOff, TL_SMOn} TL_STATE;

/*
 *  TL_SMStart - Start
 *  TL_SMOff - Temperature Light Off (Heater or Cooling Off)
 *  TL_SMOn - Temperature Light On (Heater or Cooling On)
 */

void TickFct_TmpLED() {
    switch(TL_STATE) { // Transitions
        case TL_SMStart:
            heat = 0;
            cool = 0;
            TL_STATE = TL_SMOff; // default Temperature Light is off
            break;
        case TL_SMOff:
            /* If temperature is less than setpoint proceed to STATE TL_On */
            if (temperature < setpoint) {
                TL_STATE = TL_SMOn;
                GPIO_write(CONFIG_GPIO_LED_0, CONFIG_GPIO_LED_ON);
                heat = 1; // Temperature sensor heat is on (Heater activates) LED red on
            }
            /* If temperature is greater than setpoint proceed to STATE TL_On */
            if (temperature > setpoint) {
                TL_STATE = TL_SMOn;
                GPIO_write(CONFIG_GPIO_LED_0, CONFIG_GPIO_LED_ON);
                cool = 1; // Temperature sensor cooling is off (A/C is turned off) LED red on
            }
            break;
        case TL_SMOn:
            /* If temperature is greater than setpoint proceed to STATE TL_Off */
            if (!(temperature < setpoint)){
                TL_STATE = TL_SMOff;
                GPIO_write(CONFIG_GPIO_LED_0, CONFIG_GPIO_LED_OFF);
                heat = 0; // Temperature sensor heat is off (Heater is turned off) LED red off
            }
            /* If temperature is greater than setpoint proceed to STATE TL_Off */
            if (!(temperature > setpoint)){
                TL_STATE = TL_SMOff;
                GPIO_write(CONFIG_GPIO_LED_0, CONFIG_GPIO_LED_OFF);
                cool = 0; // Temperature sensor cooling is on (A/C activates) LED red off
            }
            break;
        default:
            TL_STATE = TL_SMStart; // Initialize outputs
            break;
    }

    switch(TL_STATE) { // State actions
        case TL_SMStart: // Start
            break;
        case TL_SMOff: // Temperature Light Off
            break;
        case TL_SMOn: // Temperature Light On
            break;
        default:    // default
            break;
    }
}
```

#### Pseudo Code:
![State Machines Diagam](https://user-images.githubusercontent.com/79305154/151463072-945c60b9-7c81-4049-b181-baebec582d92.png)

Reflecting on the process of enhancing and modifying this artifact, I gained insight on how to focus on key elements in configuration, component limitations, and code structure. One specific detail that I used for the state machine was to keep it simple. By developing pseudo code, I was able to plan out my changes and implement in code how the program would perform with the CC3220S Launchpad.

List of Enhancements performed
 1. Pseudocode descriptions and layout structure have been updated to align with the program functionality.
 2. Change LED Output suited for both the Heat and Cool function - The red LED is the only option available and is set to blink.
 3. Verified loops, branches, nesting, and logic to support program functionality.
 4. Ensure Cases use defaults.
 5. Verified formatting, date, spacing, tabs, line breaks, curly braces, and brackets.
 6. Reviewed considerations of whitespace, removing unnecessary spacing.
 7. Ensure naming conventions of fields/constants/variables/parameters/classes/libraries clear and meaningful.
 8. Remove redundant variables - CONFIG_GPIO_LED_1, no longer in use as I2C driver uses the same pins for CONFIG_GPIO_LED_1.
 9. Remove excess comments - Over commenting, while useful can lead to distractions or large files.
 10. Add commenting to functionality - Provide necessary comments to specific functions required .
 11. Add parameters for cooling system displayed from terminal - In UART, add parameters to invoke cooling when temp is greater than setpoint.
 12. Add required drivers - Add the Timer Driver for tick counts, Add the I2C Driver for temperature sensor measures.
 13. Edit existing drivers - Update changes to existing drivers based on parameters set.
 14. Remove non-functional drivers - Remove non-functional drivers, as some share the same pins and cause the project to fail.
 15. Ensure functions have the variables that support them declared near by - Relocate declarations above where the function is called.
 16. Ensure Project Requirements are fulfilled and improvments support quality, efficiency, and security.
 17. Create ReadMe file and include project outcome and steps.

The skills for this artifact demonstrate an ability to use well-founded and innovative techniques, skills, and tools in computing practices for the purpose of implementing computer solutions that deliver value and accomplish industry-specific goals in software engineering and design. In an effort to create industry-standard software design, Code Composer was used to create a template of drivers and resources. Additional settings and configurations helped to develop software functionality that is free of errors. I have added comments where necessary within codes to adhere to best practices. I have made an effort to not over comment on items of this project, as a result, redundancies have been removed. I have ensured that there are no errors in logic, using correct loops, branches, and nesting. That loops and cases include defaults to protect the programs functionality. By developing code and determining the proper settings and resources used, the code developed is more robust and efficient, delivering value and accomplishing industry-specific goals.

This artifact demonstrates the ability to design and evaluate computing solutions that solve a given problem using algorithmic principles and computer science practices and standards appropriate to its solution while managing the trade-offs involved in design choices. I have confirmed that pseudocode developed aligns with the description of steps taken to structure and deploy programmed outcomes. Using logic to solve interruptions, I have confirmed that the state machines function correctly with the software. The approach taken to solve the functions for both the heat and cool LED output was first solved in pseudo code, then after determining the limitations of the drivers, the state machines implemented solved the complex logic in software. Best practices have been enforced, using proper spacing or tabs, white space, line breaks, curly braces, and brackets that have been checked. Naming conventions have been reviewed for readability, and tests have been performed prior to submitting deliverables.

Although I would have preferred that the cool and heat functions had their own dedicated LED output of Red, Green, or Yellow. By maintaining access to the drivers of UART, I2C, the project is able to read the temperature output and record the temperature, setpoint, heat, cool, and seconds parameters. As a result, I have a Thermostat Application that adheres to coding standards, best practices, and illustrates the programming skills that have matured through my education at Southern New Hampshire University as a Computer Science, Software Engineering student.


## Algorithms and Data Structure: Artifact 2 Encryption File Enhancement
![free-encryption](https://user-images.githubusercontent.com/79305154/151493285-aba0dc07-24e5-49bc-b772-884ad848b286.jpg)

I have chosen to select the artifact Encryption_File. The artifact originates from course CS405 Secure Coding and uses Microsoft Visual Studio in C++ language. The goal of this artifact is to showcase a secure transfer of data from one location to another. Mitigating errors in accessing an encrypted file and outputting the contents into a new file using an encryption key. 

* The enhancement plan includes updates to improve the efficiency of the artifact by incorporating comments where necessary, using proper syntax, and evaluating the use of an input data file, encryption key, decryption output, assertion, and debugging. 
* I plan to add or edit the archive and find areas to enhance functionality of the project. While improving code by finding bugs, checking for code clarity, consistency, design, features, tests, and coding standard use. I also plan to create a ReadMe.txt file and update naming conventions for readability.

I selected this artifact and believe that the algorithms, data structures, and tests developed successfully create an encryption file using additional resources. Hidden are the contents unavailable unless accessed using an encryption key. The key used in this artifact consists of an encrypt/decrypt source string using a XOR transformation algorithm found in https://www.programmingalgorithms.com/algorithm/xor-encryption/cpp/. The inputdatafile.txt consists of Lorem Ipsum generated content to support the XOR function https://pirateipsum.me/. I enjoyed reviewing the resources necessary that allowed this program to function correctly. Using C++ language, I was able to evaluate assertions, include input output streams, manipulators, file streams, string streams, and convert time values to strings. The encryption process was difficult at first, after constructing the source key for the encrypt_decrypt string. A For loop is used to create the output of the file by defining the XOR function. To evaluate the output, the program required the ability to read a file, I decided to use an If statement to read the file and return the text. The student’s name is then found and returned then saved. The program creates a new encryption file that is secure and displays the decrypted file information.

#### For example:
![Picture1](https://user-images.githubusercontent.com/79305154/151493319-6e73fdc0-0452-4748-9ab5-55f5e382c8d3.png)
![Picture3](https://user-images.githubusercontent.com/79305154/151493329-4ecd01f5-e900-4f1c-a000-9be8a86430e9.png)
![Picture2](https://user-images.githubusercontent.com/79305154/151493335-48b94d4a-a440-4be8-9f92-07287b47f1fc.png)

#### Pseudo Code:
![Encryption_File Pseudo Code](https://user-images.githubusercontent.com/79305154/151493413-03c5d27c-959d-4412-8102-90617adf980a.png)

Reflecting on the process of enhancing and modifying this artifact, I realize that without the resources referenced in this program, the necessary conversion to transform the encrypted file into a decrypted format would be impossible. I now understand the value of algorithms that use such complex functions like in cryptocurrencies. By focusing on key elements of the XOR encryption key, the functionality and components allowed the encryption to use a key at each value. By following sources to read the inputdatafile.txt, the file was embedded into the project and the code written saved a new file for encryption and decryption to take place.

List of Enhancements performed
 1. Pseudocode descriptions and layout structure have been updated to align with the program functionality.
 2. Verified the inclusion of assert functions to verify input and output
 3. Verified loops, branches, nesting, and logic to support program functionality.
 4. Verified formatting, date, spacing, tabs, line breaks, curly braces, and brackets.
 5. Reviewed considerations of whitespace, removing unnecessary spacing.
 6. Ensured naming conventions of fields/constants/variables/parameters/classes/libraries clear and meaningful.
 7. Removed excess commenting - Over commenting, while useful can lead to distractions or large files.
 8. Added commenting to functionality - Provide necessary comments to specific functions required.
 9. Ensure Project Requirements are fulfilled and improvements support quality, efficiency, and security.
 10. Create ReadMe file and include project outcome and steps. This is a project where I had to create a README file as one did not exist.

The skills for this artifact demonstrate an ability to develop a security mindset that anticipates adversarial exploits in software architecture and designs to expose potential vulnerabilities, mitigate design flaws, and ensure privacy and enhanced security of data and resources. Initially, I had issues with the program accessing the inputdatafile.txt file. When trying to read the file Microsoft Visual Studio prompted an error that the file was not found. After researching online, I found that in order to access external files, MVS requires the file to be embedded within the source folder location. By doing this, I was able to find a solution to address potential design flaws in software, but wonder how other projects tackle issues of the same matter for encryption? By embedding the file, the encrypt_decrypt function key was used at each value to output values saved. I routinely placed sections in code to output the values of strings to determine what is being written as a check and balance. 

#### For example: 
Use cout to test file output if needed
```
//std::cout << file_text << std::endl; // test file_text output   
//std::cout << filename << std::endl;  // test filename output

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
```

Using assert within the program helped to verify variables and the arguments used to determine its output. By listing the output of variables using cout and using assertions, I was able to find and eradicate security vulnerabilities or bugs within the program. I feel that because this program is more of an entry level program to illustrate the use of an encryption key. The ability to define and ensure that all data is explicitly validated has been achieved as the file accessed is embedded within the program. I would believe that software developed to address security protection of file transfers would be far more aggressive in protecting the contents of the file, changing the key regularly so that no one could access the information if they had one part of the decryption process. Lastly, I believe that my experience in developing the Encryption_File project allowed me to see how software accesses information and by using a security mindset, secures the information using an encryption to be accessed only when a decryption key is accessible.

This artifact demonstrates an ability to design and evaluate computing solutions that solve a given problem using algorithmic principles and computer science practices and standards appropriate to its solution, while managing the trade-offs involved in design choices. I have confirmed that pseudocode developed aligns to the description of steps taken to structure and deploy programmed outcomes. Using logic and algorithms to solve conversions of text from encrypted to decrypted format. I have confirmed that the encrypt_decrypt functions work correctly with the program, outputting new files for each. The approach taken to solve the functions for both encryption and decryption output were first mapped in pseudo code to segment each within the program. I then determined the functions to establish within the XOR key, the inputdatafile.txt was imported into the program and set to read the output, generating a new file for encryption and decryption to be saved. Best practices have been enforced, using proper spacing or tabs, white space, line breaks, curly braces, and brackets that have been checked. Naming conventions have been reviewed for readability, testing has been performed, using assertions and line outputs prior to submitting deliverables.

This artifact demonstrates an ability to use well-founded and innovative techniques, skills, and tools in computing practices for the purpose of implementing computer solutions that deliver value and accomplish industry-specific goals. The artifact uses assert arguments to check if output lengths equal source lengths as determined within arithmetic for the argument. Additional checks for file output and variables are used when cout is enabled. Comments in code dictate where these arguments are listed. Checking the location of the file output and variables can be helpful to determine if the program functions as expected. Testing the output of the files and specified variables further enhance this artifact. In using commenting, following coding standards and best practices. The code developed falls in line with industry standard software designs.

As a result, I have an Encryption_File application that adheres to coding standards, best practices, and illustrates the programming skills that have matured through my education at SNHU as a Computer Science software engineering student.


## Databases: Artifact 3 RMA_Orders Database 
![database](https://user-images.githubusercontent.com/79305154/151717149-6b7ef30f-c831-4c72-8013-c9862d182d02.jpg)

I have chosen to select artifact RMA_Orders Database. The artifact originates from course DAD220 Intro to Structured Databases and uses MySQL to perform CRUD actions. The goal of this artifact is to showcase how a software engineer uses existing information to formulate a new database and extracts usable data to generate an accurate report based on any findings and analysis. Once the data is imported, the information is used to provide the client with a report identifying sales to better manage the equipment distributed to customers and any returns or defects.

* The enhancement plan includes updates to improve the efficiency of the artifact by incorporating advanced concepts of MySQL and using proper syntax. Datamining will take place by preparing the database to import multiple CSV files. The data will then have attributes linked to then be extracted and sorted by specific parameters to identify frequencies for overall sales. Performing the planned improvements provides the ability to achieve the desired outcome to import raw data that can be filtered, sorted, and linked according to tables and the variables they may share.
 
* I plan to edit the CSV files and find areas to enhance the functionality of the reports for the project. While improving code by checking for code clarity, consistency, design, features, and coding standard use. I also plan to create a ReadMe.txt file and review naming conventions for readability in database tables, attributes, and variables.

I selected this artifact as I believe by using sources of data formatted in Comma-Separated Values provides a simple form to create a useful database. Once the data is selected, within SQL the database must be created, then the tables associated with the source file are created. The data is then imported into the SQL database and the attributes necessary to link the rows of data help to connect the relationships between each table. For this artifact, I chose to add data from the https://www.oe.netl.doe.gov/OE417_annual_summary.aspx. Here I was able to locate a CSV file that listed the climate zones for each city and state. This information has been added to the database and linked to the customer profile to determine if any relationships to climate zone relates to purchase items and or returns or defects.


Reflecting on the process of enhancing and modifying this artifact, I took many steps to prepare changes to the database structure, the fields and thier attributes. During this process I made many mistakes as naming each table, the fields, the attributes, and keys needed to be hand entered. Using CAPS on commands and proper names as used from the Pseudocode/UML Chat became difficult. I found myself having to DESCRIBE and SHOW TABLES often to verify my entires, as there were times when SELECT * would not supply the required outcome, this was due to inaccurate entries on my part when creating the database and its fields. The same can be said when performing queries to pull data, using proper syntax and with the help of the README.md file created I made sure to use beest practice with my implementation of this artifact. I had fun creating the database and assigning the links to connect the data imported within the tables. The report indicates my findings, identifying where certain products have high volumes of returns. As well as vertain states that show signs of higher percentages when it comes to purchases and returns overall. While I was not able to determine if the inclusion of climate was a benefactor to higher returns. The data indicates the highest frequencies in colder climates.


List of Enhancements performed
1. Add new content to Database - added resource of climate to database for each state found at https://www.energy.gov/eere/buildings/downloads/zerh-tour-zero-homepage-data-table-datafile-1012019 
2. Pseudocode descriptions/ UML Chart and layout structure have been updated to align with the program functionality.
3. Cleaned data within the csv files: From customers, removed (-) from phone numbers, 98868 updates were applied.
	From customers, removed (-) from phone numbers, 98868 updates were applied.
	From customers, fixed State listing for formatting.
	From customers. added NULL to empty phone listings.
	From climate by state, fixed State listing for formatting.
	From climate by state, added NULL to empty city and climate listings.
	Mapped Climate data to Customer Table, updated state from abbreviation to state name.
6. Verified formatting, date, spacing, tabs, line breaks, curly braces, and brackets.
7. Reviewed considerations of whitespace, removing unnecessary spacing.
8. Ensured naming conventions of fields/constants/variables/parameters/classes/libraries clear and meaningful.
9. Ensure Project Requirements are fulfilled and improvments support quality, efficiency, and security.
10. Create ReadMe file and include project outcome and steps.
	
	
### Resources
https://www.mysqltutorial.org/import-csv-file-mysql-table/
https://www.oe.netl.doe.gov/OE417_annual_summary.aspx
https://www.energy.gov/eere/buildings/downloads/zerh-tour-zero-homepage-data-table-datafile-1012019

