Name: Christopher Manna
Student ID: 7813690460

What have I done:
    I have completed all of the project including the extra credit. All functionale should work.

What my code files are and what each one of them does:

    Utilities.cpp, Utilities.h
        Utilities.cpp and Utilities.h contains all libaries,defintions, and functions that overlap between the files.
        It contains definitins for char credientials such as FAIL_NO_USR and FAIL_NO_PASS for serverC, serverM and client to use when determining authentication.
        Lastly, functions were created to create and bind socket, and to create an address as this is needed by all files and is redundant to retype

    ServerEE.cpp
        Compares requested course to text file, if there is a match grabs necessary information and puts it into a string that will be printed on the clients screen.
        Function for both singular and multicourse querry

    ServerCS.cpp
        idential to serverEE with cs.txt instead of ee.txt

    ServerC.cpp
        compares credentials to cred text file, if correct sends PASS_CRED('2'), if only username correct sends FAIL_NO_PASS('1'), if username incorrect sends FAIL_NO_USR('0').

    serverM.cpp
        Accepts one client at a time. If client disconnect, will wait for next client to accept. Will not terminate until cntrl ^c is pressed.
        Behaves as expected from project description.
        Continues to wait for more course querries until client shutdowns or is forcefully terminated.
        if during course querry a class department code is not EE or CS will send back cannot find course.

    Client.cpp

        client asks for credentials, if valid in 3 attempts one can querry for a coursecode and category or multiple courses

        NOTE: white space space is not removed from inputs
        if username has extra space will be added to username

        has error checking mechanism for coursecode input and catagory.
        CourseCode must be two capital letters followed by 3 numbers and no trailing whitespace
        Category must have first letter capital and no trailing white space.
        For extra credit each coursecode has to be valid as stated above with 1 white space inbetween.
        NOTE: can send FF100. ServerM will take care of locating if department exists not the clients responsibility
        If improper input will ask for user to re-enter


Format of all messages exchanged:

    credentials
    client sends credentials to serverM with format: username,password
    ServerM encrypts and sends to serverC, format: enc_username,enc_password
    ServerC sends back a single char. In Utilities.h there is defintions for FAIL_NO_USR ('0'), FAIL_NO_PASS('1'), PASS_CRED('2')
    ServerM fowards this to client.

    course querry
    client will send a 1,course,ctg or 2,course course course...
    ServerM looks at first char from client to deterimine if it's a singular course (1) or multi course querry(2)
    If singular will forward message to appropriate server, If multi will parse courses into an array and then forward courses to corresponding channel
    Both serverEE/CS also look for 1 or 2 at beginning of message to determine if it a multi or singular course querry.
    ServerEE/CS sends out exact string for client to print out to serverM. ServerM forwards to client on singular.
    On multi, serverM will combine strings in original order of querry after receiving from both serverEE and serverCS before sending to client.



Idosyncrasy:
    If cntrl^c is pressed on serverM and then cntrl^c is pressed on client. Zombie process is made and main server cannot bind for approximately a minute. If instead after serverM is abruptly turned off, one just enters through the client's inputs, the client will eventually close and no zombie process is created.

    If one inputs a comma into the client durign a cin, the program will break. All the servers use commas to parse between various parts of incoming messages. As a result an input of a comma will cause unforseen erros.

Reused code:
    I did not resuse code verbatum; however there were a few sources I took inspiration from.

    https://www.geeksforgeeks.org/tcp-server-client-implementation-in-c/
        - I used this website to refresh on how to create TCP sockets (Utilities.cpp, all other files send/recv commands etc..)

    https://www.geeksforgeeks.org/udp-server-client-implementation-c/
        - In addition I used it to refresh on how to create UDP sockets (Utilities.cpp,all other files send/recv commands etc..)

    https://www.tutorialspoint.com/cplusplus-program-to-implement-caesar-cypher#:~:text=It%20is%20a%20mono%2Dalphabetic,to%20as%20the%20Shift%20Cipher.
        - I used this to learn how to do a shift cipher. (serverM, functionL: string encrypt_msg(string msg))