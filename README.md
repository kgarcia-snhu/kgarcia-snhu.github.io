# kgarcia-snhu.github.io

## Southern New Hampshire University Software Engineering ePortfolio

### CS499 Computer Science Capstone
My name is Keone, best pronounced as (Ke Own E). I continue to hold the Presidents List at Southern New Hampshire Univeristy. I’m a first-generation dual major, studying computer science and data analytics. Both of which have concentrations in software engineering and project management. The projects associated with this page are part of the Computer Science program from Southern New Hampshire University. The ePortfolio contains a narrated code review, enhancements to completed work in three categories of (Software Design and Engineering, Algorithms and Data Structure, and Databases), justification narratives, and a professional self-assessment. These artifacts have been developed previously in my years at SNHU and have been further developed to expand the complexity of software and functionality. From my Computer Science Capstone course, I have advanced previous work to showcase my abilities.  

## Table of Content
1. Professional Self-Assessment
2. Code Review
3. Software Design and Engineering: Artifact 1 Thermostat Lab Enhancement
4. Algorithms and Data Structure: Artifact 2 Encryption File Enhancement
5. Databases: Artifact 3 

### Professional Self-Assessment

I’ve been in the computer science program since January of 2018, that puts me at 4 years to complete my degree. While in the program, I have learned that there are many applications for hardware and software to co-exist. When building a computer to meet the needs of a client; cost, performance, and limitations are considered to provide the best possible outcome. When developing code, security policies are used to ensure that code written relinquishes bugs, vulnerabilities, and data loss. Within the computer science program, having the ability to work with diverse programming languages along with comments in code are good practice, and provide avenues to reverse engineer software.

As a first generation dual major working towards a BSCS and BSDA, I aim to work as a software engineer and a data analyst/scientist. Being new to coding, SNHU has provided me with my first steps to developing code. I plan to pursue an internship or a position in the industry that can continue to teach me industry standards so that I may implement each skill as I learn. Ultimately I would love to achieve the status of a data scientist, I hear many that pursue this position start as data analysts and learn code to develop algorithms that satisfy analytical reasoning for reports. Either way, I believe that both a software engineer and data analyst carry opposite duties as one creates data and the other collects and analyzes data. At some point in my career, like many of my professors, I would love to teach students based on my experience in the industry.

### Code Review

Provided is a link to my Code Review video where I address Existing Functionality, Code Analysis, and Enhancements planned.
* Existing Functionality: A walk though of what exisiting code does and how it works.
* Code Analysis: A review of code structure, commenting, efficiencies, and an explanation of findings.
* Enhancements: A walk through of enhancements planned and an explanation of execution or limitations.

### Software Design and Engineering: Artifact 1 Thermostat Lab Enhancement
--------------------------------------------------------------------------------
![CC3220S LaunchPad2](https://user-images.githubusercontent.com/79305154/151455474-1afa7d76-0b53-4522-8b75-0416d99671e1.JPG)
--------------------------------------------------------------------------------

I have chosen to select the artifact Thermostat Lab. The artifact originates from course CS350 Emerging Systems and uses Code Composer Studio with a Simple Link CC3220S Launchpad in C language. The goal of this artifact is to utilize components of the launchpad to simulate a working thermostat for an HVAC system. Indicating when the unit is active using an LED, while providing an output of sensor readings and setpoint parameters sent to be viewed on a mobile app.

* The enhancement plan includes updates to expand the complexity of software by incorporating comments where necessary, using proper syntax, and evaluating proper use of gpiointerrupt, timers, UART. 
* I plan to add or edit the LED output to enhance functionality. While improving code by finding bugs, checking for code clarity, consistency, design, features, and coding standard use. I also plan to create or update a ReadMe.txt file and update naming conventions for readability.

I selected this artifact as I beleive the CC3220S Launchpad adds many possibilites to Software Design and Engineering. Specific to the Thermostat Lab project, many components required setup configuration to communicate with the board dependant on availability of pins, and resources. I enjoyed learning the basics of C language and realized by enhancing this project, I had a whole new experience when developing the changes to enhancement my ideas. I knew that the best course of action would be to take the program, its software and settings back to the begining of development. I worked from scratch to ensure each process to add components of the CC3220S Launchpad drivers were implemented in configuration and in code. To then add my enhancement changes to the gpiointerrupt LED functionality. The specific components of the artifact that showcase my abilities in software design and engineering are in the use of State Machines for use in activating an LED when either the heat or cool function is activated from the callback. I found that on the CC3200S board, there were limiations to the resources available in using LED outputs. I then determined in order to signal an active function of heat or cool wouuld be to have the Red LED blink for either function when active. Indicating when the unit is active using an LED, while providing an output of temperature sensor readings based on the setpoint parameters sent to a wireless thermostat and records measures to a database. 

For example:
---------------------------------------------------------------------------
![State Machines](https://user-images.githubusercontent.com/79305154/151462751-1f7665c2-e387-4661-baa1-28d8dc70c4bd.png)

---------------------------------------------------------------------------  
![State Machines Diagam](https://user-images.githubusercontent.com/79305154/151463072-945c60b9-7c81-4049-b181-baebec582d92.png)

Reflecting on the process of enhancing and modifying this artifact, I gained insight on how to focus on key elements in configuration, component limitations, and code structure. One specific detail that I used for the state machine was to keep it simple. By developing pseudo code, I was able to plan out my changes and implement in code how the program would perform with the CC3220S Launchpad. I have confirmed that pseudocode developed aligns to the description of steps taken to structure and deploy programmed outcomes. I've added comments where necessary within codes to adhere to best practices. I've ensured that there are no errors in logic, using correct loops, branches, and nesting. That loops and cases include defaults to protect the program functionality. I using best practices formatting and use of proper spacing or tabs, white space, line breaks, curly braces, and brackets have been checked. Naming conventions have been reviewed for readability, and tests have been performed. Although I would have perferred that the cool and heat functions had thier own deticated LED output of Red, Green or Yellow. By maintaining access to the drivers of UART, I2C, the porject is able to read the temperature output and record the temperature, setpoint, heat, cool, and seconds parameters. As a result, I would have a Thermostat Application that would adhere to coding standards, best practices, and illustrate the programming skills that have matured through my education at Southern New Hampshire University as a Computer Science, Software Engineering student.


### Algorithms and Data Structure: Artifact 2 Encryption File Enhancement




### Databases: Artifact 3 

