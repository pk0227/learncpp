1 — Input and output (I/O) streams
    The iostream library
        -- When you include the iostream header, you gain access to a whole hierarchy of classes responsible for providing I/O functionality
           (including one class that is actually named iostream).
        -- The first thing we may notice about this hierarchy is that it uses multiple inheritance (that thing we told you to avoid if at all 
           possible). However, the iostream library has been designed and extensively tested in order to avoid any of the typical multiple 
           inheritance problems, so we can use it freely without worrying.
    
    Streams
        -- At its most basic, I/O in C++ is implemented with streams. Abstractly, a stream is just a sequence of bytes that can be accessed 
           sequentially. Over time, a stream may produce or consume potentially unlimited amounts of data.
        -- Input streams : are used to hold input from a data producer, such as a keyboard, a file, or a network. For example, the user may 
           press a key on the keyboard while the program is currently not expecting any input. Rather than ignore the users keypress, 
           the data is put into an input stream, where it will wait until the program is ready for it.
        -- Output streams : are used to hold output for a particular data consumer, such as a monitor, a file, or a printer. When writing 
           data to an output device, the device may not be ready to accept that data yet -- for example, the printer may still be warming 
           up when the program writes data to its output stream. The data will sit in the output stream until the printer begins consuming it.
        -- Some devices, such as files and networks, are capable of being both input and output sources.

    Input/output in C++
        -- ios is a typedef for std::basic_ios<char> that defines a bunch of stuff that is common to both input and output streams.
        -- The istream class  : is the primary class used when dealing with input streams. With input streams, the extraction operator (>>) 
           is used to remove values from the stream. This makes sense: when the user presses a key on the keyboard, the key code is placed 
           in an input stream. Your program then extracts the value from the stream so it can be used. 
        -- The ostream class is the primary class used when dealing with output streams. With output streams, the insertion operator (<<) 
           is used to put values in the stream. This also makes sense: you insert your values into the stream, and the data consumer 
           (e.g. monitor) uses them.
        -- The iostream class can handle both input and output, allowing bidirectional I/O.

    Standard streams in C++
        A standard stream is a pre-connected stream provided to a computer program by its environment. 
        C++ comes with four predefined standard stream objects that have already been set up for your use.
            -- cin -- an istream object tied to the standard input (typically the keyboard)
            -- cout -- an ostream object tied to the standard output (typically the monitor)
            -- cerr -- an ostream object tied to the standard error (typically the monitor), providing unbuffered output
            -- clog -- an ostream object tied to the standard error (typically the monitor), providing buffered output
    
        Unbuffered output is typically handled immediately, whereas buffered output is typically stored and written out as a block. 
        Because clog isn’t used very often, it is often omitted from the list of standard streams.

    Stream synchronization and tying
        -- By default, C++ standard streams (cin, cout, cerr, clog) synchronize their underlying buffers with the standard C I/O buffers (stdin, stdout, stderr).
           This ensures that C++ stream operations and C stdio functions (like printf, scanf) can be safely mixed without interleaved output or input reordering.
        -- However, this synchronization incurs a noticeable performance overhead. If your program does not mix C stdio and C++ streams, you can disable it via:
               std::ios_base::sync_with_stdio(false);
        -- Stream tying: By default, std::cin is "tied" to std::cout (and std::wcin to std::wcout).
           This means that any input operation on std::cin automatically flushes std::cout first. This ensures that user prompts (printed via cout)
           appear on screen before the program waits for user input.
        -- To untie cin from cout for maximum performance (e.g. in competitive programming or high-throughput stream processing):
               std::cin.tie(nullptr);

2 — Input with istream
    Manipulator 
        -- is an object that is used to modify a stream when applied with the extraction (>>) or insertion (<<) operators.
            Example : "std::endl", which both prints a newline character and flushes any buffered output.
        -- setw (in the iomanip header) that can be used to limit the number of characters read in from a stream. 
            #include <iomanip>
            char buf[10]{};
            std::cin >> std::setw(10) >> buf;

            NOTE : This program will now only read the first 9 characters out of the stream (leaving room for a terminator). 
                   Any remaining characters will be left in the stream until the next extraction.
    
    Extraction(>>) and whitespace
        -- As a reminder, the extraction operator skips whitespace (blanks, tabs, and newlines).
        -- get() function, which simply gets a character from the input stream. It can also read whitespaces.
        -- get() also has a string version that takes a maximum number of characters to read.
            char strBuf[11]{};
            std::cin.get(strBuf, 11);
            std::cout << strBuf << '\n'; 

            NOTE : It only reads the first 10 characters (it leaves one character for a terminator). 
                   The remaining characters were left in the input stream.

            NOTE : One important thing to note about get() is that it does not read in a newline character! 
                   This can cause some unexpected results:

                    char strBuf[11]{};
                    // Read up to 10 characters
                    std::cin.get(strBuf, 11);
                    std::cout << strBuf << '\n';

                    // Read up to 10 more characters
                    std::cin.get(strBuf, 11);
                    std::cout << strBuf << '\n';

            It won't ask for next 10 more characters. Because the first get() read up to the newline and then stopped. 
            The second get() saw there was still input in the cin stream and tried to read it. But the first character 
            was the newline, so it stopped immediately.
        
        -- There is another function called getline() that works similarly to get(), but will extract (and discard) the delimiter.

        gcount() : To know how many character were extracted by the last call of getline().

        A special version of getline() for std::string
            -- There is a special version of getline() that lives outside the istream class that is used for reading in variables of type 
               std::string. This special version is not a member of either ostream or istream, and is included in the string header.

                std::string strBuf{};
                std::getline(std::cin, strBuf);
                std::cout << strBuf << '\n';
            
        A few more useful istream functions
            -- ignore() discards the first character in the stream.
            -- ignore(int nCount) discards the first nCount characters.
            -- peek() allows you to read a character from the stream without removing it from the stream.
            -- unget() returns the last character read back into the stream so it can be read again by the next call.
            -- putback(char ch) allows you to put a character of your choice back into the stream to be read by the next call.

        Robust input stream recovery
            -- When an extraction fails (e.g., entering letters when an integer is expected), std::cin enters a failed state (failbit is set)
               and leaves the problematic input in the stream buffer.
            -- To recover and prompt the user again, you must perform two mandatory steps:
               1. Clear the error state: std::cin.clear();
               2. Flush the offending characters from the buffer up to the newline:
                  #include <limits>
                  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
3 — Output with ostream and ios
    The insertion operator
        -- Both istream and ostream were derived from a class called ios. One of the jobs of ios (and ios_base) is to control the 
           formatting options for output.
    Formatting
        -- Two ways to change the formatting options:
            Flags -- Boolean variables that can be turned on and off.
            Manipulators -- Objects placed in a stream that affect the way things are input and output.
        
        Flags
        -- To switch a flag on, use the setf() function, with the appropriate flag as a parameter. For example, by default, C++ does 
           not print a + sign in front of positive numbers. By using the std::ios::showpos flag, we can change this behavior:
                std::cout.setf(std::ios::showpos); // turn on the std::ios::showpos flag
                std::cout << 27 << '\n';
        -- It is possible to turn on multiple ios flags at once using the Bitwise OR (|) operator:
                std::cout.setf(std::ios::showpos | std::ios::uppercase); // turn on the std::ios::showpos and std::ios::uppercase flag
                std::cout << 1234567.89f << '\n';
        -- To turn a flag off, use the unsetf() function:
                std::cout.unsetf(std::ios::showpos); // turn off the std::ios::showpos flag
        -- There’s one other bit of trickiness when using setf() that needs to be mentioned. Many flags belong to groups, called format groups.
           A format group is a group of flags that perform similar (sometimes mutually exclusive) formatting options. 
           For example, a format group named “basefield” contains the flags “oct”, “dec”, and “hex”, which controls the base of integral 
           values. By default, the “dec” flag is set. Consequently, if we do this:
                std::cout.setf(std::ios::hex); // try to turn on hex output
                std::cout << 27 << '\n';
           It doesn’t work! The reason why is because setf() only turns flags on -- it isn’t smart enough to turn mutually exclusive flags
           off. Consequently, when we turned std::hex on, std::ios::dec was still on, and std::ios::dec apparently takes precedence. 
           There are two ways to get around this problem.
                std::cout.unsetf(std::ios::dec); // turn off decimal output
                std::cout.setf(std::ios::hex); // turn on hexadecimal output

                OR

                std::cout.setf(std::ios::hex, std::ios::basefield);
        
        Manipulators
        -- The nice thing about manipulators is that they are smart enough to turn on and off the appropriate flags.
                std::cout << std::hex << 27 << '\n'; // print 27 in hex
                std::cout << 28 << '\n'; // we're still in hex
                std::cout << std::dec << 29 << '\n'; // back to decimal
    
    -- In general, using manipulators is much easier than setting and unsetting flags. Many options are available via both flags and 
       manipulators (such as changing the base), however, other options are only available via flags or via manipulators, 
       so it’s important to know how to use both.
        -- Flags live in the std::ios class, 
        -- Manipulators live in the std namespace,
        -- The member functions live in the std::ostream class.

        boolalpha
            Flag        - std::ios::boolalpha	If set, booleans print “true” or “false”. If not set, booleans print 0 or 1
            Manipulator - std::boolalpha        Booleans print “true” or “false”
                        - std::noboolalpha	    Booleans print 0 or 1 (default)
        
        showpos
            Flag        - std::ios::showpos	    If set, prefix positive numbers with a +
            Manipulator - std::showpos	        Prefixes positive numbers with a +
                        - std::noshowpos	    Doesn’t prefix positive numbers with a +
        
        uppercase
            Flag        - std::ios::uppercase	If set, uses upper case letters
            Manipulator - std::uppercase	    Uses upper case letters
                        - std::nouppercase	    Uses lower case letters

        +---------------------+------------------+----------------------------------------------+
        | Group               | Flag             | Meaning                                      |
        +---------------------+------------------+----------------------------------------------+
        | std::ios::basefield | std::ios::dec    | Prints values in decimal (default)           |
        | std::ios::basefield | std::ios::hex    | Prints values in hexadecimal                 |
        | std::ios::basefield | std::ios::oct    | Prints values in octal                       |
        | std::ios::basefield | (none)           | Prints values based on leading characters    |
        +---------------------+------------------+----------------------------------------------+

        +-------------+----------------------------------+
        | Manipulator | Meaning                          |
        +-------------+----------------------------------+
        | std::dec    | Prints values in decimal         |
        | std::hex    | Prints values in hexadecimal     |
        | std::oct    | Prints values in octal           |
        +-------------+----------------------------------+

    Precision, notation, and decimal points
        +-----------------------+-----------------------+---------------------------------------------------------------+
        | Group                 | Flag                  | Meaning                                                       |
        +-----------------------+-----------------------+---------------------------------------------------------------+
        | std::ios::floatfield  | std::ios::fixed       | Uses decimal notation for floating-point numbers              |
        | std::ios::floatfield  | std::ios::scientific  | Uses scientific notation for floating-point numbers           |
        | std::ios::floatfield  | (none)                | Uses fixed for small numbers, scientific for larger numbers   |
        | std::ios::floatfield  | std::ios::showpoint   | Always show decimal point and trailing 0s for floating-point  |
        +-----------------------+-----------------------+---------------------------------------------------------------+

        +-----------------------+---------------------------------------------------------------+
        | Manipulator           | Meaning                                                       |
        +-----------------------+---------------------------------------------------------------+
        | std::fixed            | Use decimal notation for values                               |
        | std::scientific       | Use scientific notation for values                            |
        | std::showpoint        | Show decimal point and trailing 0s for floating-point values  |
        | std::noshowpoint      | Don’t show decimal point and trailing 0s for floating-points  |
        | std::setprecision(n)  | Set floating-point precision (from <iomanip>)                 |
        +-----------------------+---------------------------------------------------------------+

        +-------------------------------+---------------------------------------------------------------+
        | Member function               | Meaning                                                       |
        +-------------------------------+---------------------------------------------------------------+
        | std::ios_base::precision()    | Returns the current precision of floating-point numbers       |
        | std::ios_base::precision(int) | Sets floating-point precision and returns the old precision   |
        +-------------------------------+---------------------------------------------------------------+

        NOTE : If fixed or scientific notation is used, precision determines how many decimal places in the fraction is displayed. 
               If the precision is less than the number of significant digits, the number will be rounded.

            std::cout << std::fixed << '\n';
            std::cout << std::setprecision(3) << 123.456 << '\n';       // 123.456
            std::cout << std::setprecision(4) << 123.456 << '\n';       // 123.4560
            std::cout << std::setprecision(5) << 123.456 << '\n';       // 123.45600
            std::cout << std::setprecision(6) << 123.456 << '\n';       // 123.456000
            std::cout << std::setprecision(7) << 123.456 << '\n';       // 123.4560000

            std::cout << std::scientific << '\n';
            std::cout << std::setprecision(3) << 123.456 << '\n';       // 1.235e+002
            std::cout << std::setprecision(4) << 123.456 << '\n';       // 1.2346e+002
            std::cout << std::setprecision(5) << 123.456 << '\n';       // 1.23456e+002
            std::cout << std::setprecision(6) << 123.456 << '\n';       // 1.234560e+002
            std::cout << std::setprecision(7) << 123.456 << '\n';       // 1.2345600e+002

        NOTE : If neither fixed nor scientific are being used, precision determines how many significant digits should be displayed.
               Again, if the precision is less than the number of significant digits, the number will be rounded.

            std::cout << std::setprecision(3) << 123.456 << '\n';       // 123
            std::cout << std::setprecision(4) << 123.456 << '\n';       // 123.5
            std::cout << std::setprecision(5) << 123.456 << '\n';       // 123.46
            std::cout << std::setprecision(6) << 123.456 << '\n';       // 123.456

            std::cout << std::showpoint << '\n';
            std::cout << std::setprecision(3) << 123.456 << '\n';       // 123.
            std::cout << std::setprecision(4) << 123.456 << '\n';       // 123.5
            std::cout << std::setprecision(5) << 123.456 << '\n';       // 123.46
            std::cout << std::setprecision(6) << 123.456 << '\n';       // 123.456
            std::cout << std::setprecision(7) << 123.456 << '\n';       // 123.4560

        +-----------+-----------+----------------+----------------+
        | Option    | Precision | 12345.0        | 0.12345        |
        +-----------+-----------+----------------+----------------+
        | Normal    | 3         | 1.23e+004      | 0.123          |
        | Normal    | 4         | 1.235e+004     | 0.1235         |
        | Normal    | 5         | 12345          | 0.12345        |
        | Normal    | 6         | 12345          | 0.12345        |
        +-----------+-----------+----------------+----------------+
        | Showpoint | 3         | 1.23e+004      | 0.123          |
        | Showpoint | 4         | 1.235e+004     | 0.1235         |
        | Showpoint | 5         | 12345.         | 0.12345        |
        | Showpoint | 6         | 12345.0        | 0.123450       |
        +-----------+-----------+----------------+----------------+
        | Fixed     | 3         | 12345.000      | 0.123          |
        | Fixed     | 4         | 12345.0000     | 0.1235         |
        | Fixed     | 5         | 12345.00000    | 0.12345        |
        | Fixed     | 6         | 12345.000000   | 0.123450       |
        +-----------+-----------+----------------+----------------+
        | Scientific| 3         | 1.235e+004     | 1.235e-001     |
        | Scientific| 4         | 1.2345e+004    | 1.2345e-001    |
        | Scientific| 5         | 1.23450e+004   | 1.23450e-001   |
        | Scientific| 6         | 1.234500e+004  | 1.234500e-001  |
        +-----------+-----------+----------------+----------------+

    Width, fill characters, and justification
        +------------------------+---------------------+-----------------------------------------------------+
        | Group                  | Flag                | Meaning                                             |
        +------------------------+---------------------+-----------------------------------------------------+
        | std::ios::adjustfield  | std::ios::internal  | Left-justifies the sign, right-justifies the value  |
        | std::ios::adjustfield  | std::ios::left      | Left-justifies the sign and value                   |
        | std::ios::adjustfield  | std::ios::right     | Right-justifies the sign and value (default)        |
        +------------------------+---------------------+-----------------------------------------------------+

        +---------------------+----------------------------------------------------------+
        | Manipulator         |  Meaning                                                 |
        +---------------------+----------------------------------------------------------+
        | std::internal       |  Left-justifies the sign, right-justifies the value      |
        | std::left           |  Left-justifies the sign and the value                   |
        | std::right          |  Right-justifies the sign and the value                  |
        | std::setfill(char)  |  Sets the fill character (from <iomanip>)                |
        | std::setw(int)      |  Sets the field width for input/output (from <iomanip>)  |
        +---------------------+----------------------------------------------------------+

        +---------------------------------+--------------------------------------------------------------+
        | Member function                 |  Meaning                                                     |
        +---------------------------------+--------------------------------------------------------------+
        | std::basic_ostream::fill()      |  Returns the current fill character                          |
        | std::basic_ostream::fill(char)  |  Sets the fill character and returns the old fill character  |
        | std::ios_base::width()          |  Returns the current field width                             |
        | std::ios_base::width(int)       |  Sets the current field width and returns old field width    |
        +---------------------------------+--------------------------------------------------------------+

        std::cout << -12345 << '\n'; // print default value with no field width                         "-12345"
        std::cout << std::setw(10) << -12345 << '\n'; // print default with field width                 "    -12345"
        std::cout << std::setw(10) << std::left << -12345 << '\n'; // print left justified              "-12345    "
        std::cout << std::setw(10) << std::right << -12345 << '\n'; // print right justified            "    -12345"
        std::cout << std::setw(10) << std::internal << -12345 << '\n'; // print internally justified    "-    12345"

        std::cout.fill('*');
        std::cout << -12345 << '\n'; // print default value with no field width                         "-12345"
        std::cout << std::setw(10) << -12345 << '\n'; // print default with field width                 "****-12345"
        std::cout << std::setw(10) << std::left << -12345 << '\n'; // print left justified              "-12345****"
        std::cout << std::setw(10) << std::right << -12345 << '\n'; // print right justified            "****-12345"
        std::cout << std::setw(10) << std::internal << -12345 << '\n'; // print internally justified    "-****12345"

    Modern C++ Alternatives: std::format (C++20) and std::print / std::println (C++23)
        -- While iostream manipulators and flags are flexible and extensible, they have notable drawbacks:
           -- Stateful: Manipulators (like std::hex or std::setprecision) permanently mutate stream state until manually reverted,
              frequently introducing subtle formatting bugs elsewhere in the codebase.
           -- Verbose: Aligning multiple columns or combining widths and precisions requires long chains of operator<< calls.
           -- Performance: Type-checking and virtual dispatch within stream hierarchies make iostream slower than C stdio (printf).
        -- C++20 introduced std::format (in header <format>), providing Python-style type-safe formatting with positional specifiers,
           independent of stream state:
               std::string s = std::format("Hex: {:#x}, Fixed: {:.2f}", 255, 3.14159);
        -- C++23 introduced std::print and std::println (in header <print>), outputting formatted text directly to stdout/stderr with
           higher performance than cout and without stream synchronization penalties:
               std::println("Hello, {}! Value: {:04d}", "world", 42);

4 — Stream classes for strings
    Stream classes
        -- The stream classes for strings that allow you to use the familiar insertions (<<) and extraction (>>) operators 
           to work with strings. 
        -- Like istream and ostream, the string streams provide a buffer to hold data. 
           However, unlike cin and cout, these streams are not connected to an I/O channel (such as a keyboard, monitor, etc…). 
           One of the primary uses of string streams is to buffer output for display at a later time, or to process input line-by-line.
        -- There are six stream classes for strings: 
            -- istringstream (derived from istream), 
            -- ostringstream (derived from ostream),
            -- stringstream (derived from iostream) are used for reading and writing normal characters width strings. 
            
            -- wistringstream, wostringstream, and wstringstream are used for reading and writing wide character strings. 
            To use the stringstreams, you need to #include the sstream header.

            There are two ways to get data into a stringstream:
            Use the insertion (<<) operator:
                std::stringstream os {};
                os << "en garde!\n"; // insert "en garde!" into the stringstream
            
            Use the str(string) function to set the value of the buffer:
                std::stringstream os {};
                os.str("en garde!"); // set the stringstream buffer to "en garde!"
            
            There are similarly two ways to get data out of a stringstream:
            Use the str() function to retrieve the results of the buffer:
                std::stringstream os {};
                os << "12345 67.89\n";
                std::cout << os.str();
            
            Use the extraction (>>) operator:
                std::stringstream os {};
                os << "12345 67.89"; // insert a string of numbers into the stream

                std::string strValue {};
                os >> strValue;

                std::string strValue2 {};
                os >> strValue2;

                // print the numbers separated by a dash
                std::cout << strValue << " - " << strValue2 << '\n';

                std::cout << "whole : " << os.str() << "\n";
        
        NOTE : The >> operator iterates through the string -- each successive use of >> returns the next extractable value in the stream. 
               On the other hand, str() returns the whole value of the stream, even if the >> has already been used on the stream.
            
    Conversion between strings and numbers
        First, let’s take a look at converting numbers into a string:
            std::stringstream os {};

            constexpr int nValue { 12345 };
            constexpr double dValue { 67.89 };
            os << nValue << ' ' << dValue;

            std::string strValue1, strValue2;
            os >> strValue1 >> strValue2;

            std::cout << strValue1 << ' ' << strValue2 << '\n';
        
        Now let’s convert a numerical string to a number:
            std::stringstream os {};
            os << "12345 67.89"; // insert a string of numbers into the stream
            int nValue {};
            double dValue {};

            os >> nValue >> dValue;

            std::cout << nValue << ' ' << dValue << '\n';

    Clearing a stringstream for reuse
        os.str("");                 // erase the buffer using a blank C-style string:
        os.str(std::string{});      // erase the buffer using a blank std::string object:

        When clearing out a stringstream, it is also generally a good idea to call the clear() function:
            os.str(""); // erase the buffer
            os.clear(); // reset error flags

        NOTE : clear() resets any error flags that may have been set and returns the stream back to the ok state.

    Modern C++ stringstream features (C++20)
        -- In C++20, std::stringstream provides the .view() member function. Instead of creating and copying into a brand new std::string
           (as .str() does), .view() returns a std::string_view directly referencing the existing internal buffer. This enables zero-copy
           read-only inspection of buffered string data:
               std::stringstream ss{};
               ss << "Hello " << 42;
               std::string_view sv = ss.view(); // C++20 zero-copy view
        -- String streams also support move semantics (since C++11), allowing efficient buffer transfers without copying large strings.
    
5 — Stream states and input validation
    Stream states
        -- The ios_base class contains several state flags that are used to signal various conditions that may occur when using streams.
        +----------+-----------------------------------------------------------------------+
        | Flag     | Meaning                                                               |
        +----------+-----------------------------------------------------------------------+
        | goodbit  | Everything is okay                                                    |
        | badbit   | A fatal error occurred (e.g., reading past end of file)               |
        | eofbit   | The stream has reached end of file                                    |
        | failbit  | A non-fatal error occurred (e.g., wrong input type from the user)     |
        +----------+-----------------------------------------------------------------------+

    Stream boolean conversion
        -- All stream classes can be evaluated directly in boolean contexts (e.g. if (std::cin) or while (std::cin >> x)).
        -- Prior to C++11, streams implemented a conversion operator to void* to allow conditional checks without permitting accidental arithmetic.
        -- Since C++11, streams provide an explicit operator bool() that returns !fail(). That is, the stream evaluates to true if neither
           failbit nor badbit is set.
        -- The operator! function returns fail(), returning true if either failbit or badbit is set.
        
        -- Although these flags live in ios_base, because ios is derived from ios_base and ios takes less typing than ios_base, 
           they are generally accessed through ios (e.g. as std::ios::failbit).
        -- ios also provides a number of member functions in order to conveniently access these states.
        +------------------+--------------------------------------------------------------+
        | Member function  | Meaning                                                      |
        +------------------+--------------------------------------------------------------+
        | good()           |  Returns true if goodbit is set (stream is okay)             |
        | bad()            |  Returns true if badbit is set (fatal error occurred)        |
        | eof()            |  Returns true if eofbit is set (end of file reached)         |
        | fail()           |  Returns true if failbit is set (non-fatal error occurred)   |
        | clear()          |  Clears all flags and restores the stream to goodbit         |
        | clear(state)     |  Clears all flags and sets the provided state flag           |
        | rdstate()        |  Returns the current stream state flags                      |
        | setstate(state)  |  Sets the provided state flag                                |
        +------------------+--------------------------------------------------------------+
        -- The most commonly dealt with bit is the failbit, which is set when the user enters invalid input.
        -- When cin is unable to extract anything to given data type(alphabetic string passed for integer input), and the 
           failbit will be set.
        -- If an error occurs and a stream is set to anything other than goodbit, further stream operations on that stream 
           will be ignored. This condition can be cleared by calling the clear() function.
        
    Input validation
        -- Input validation is the process of checking whether the user input meets some set of criteria. 
           Input validation can generally be broken down into two types: string and numeric.
        -- C++ provides a number of useful functions that we can use to determine whether specific characters are numbers or letters. 
           The following functions live in the cctype header:
            +----------------------+--------------------------------------------------------------------------+
            | Function             | Meaning                                                                  |
            +----------------------+--------------------------------------------------------------------------+
            | std::isalnum(int)    | Returns non-zero if the parameter is a letter or a digit                 |
            | std::isalpha(int)    | Returns non-zero if the parameter is a letter                            |
            | std::iscntrl(int)    | Returns non-zero if the parameter is a control character                 |
            | std::isdigit(int)    | Returns non-zero if the parameter is a digit                             |
            | std::isgraph(int)    | Returns non-zero if printable and not whitespace                         |
            | std::isprint(int)    | Returns non-zero if printable (including whitespace)                     |
            | std::ispunct(int)    | Returns non-zero if neither alphanumeric nor whitespace                  |
            | std::isspace(int)    | Returns non-zero if the parameter is whitespace                          |
            | std::isxdigit(int)   | Returns non-zero if the parameter is a hexadecimal digit (0–9, a–f, A–F) |
            +----------------------+--------------------------------------------------------------------------+
    String validation
        -- When it comes to variable length inputs, the best way to validate strings (besides using a regular expression library) is 
           to step through each character of the string and ensure it meets the validation criteria. That’s what std::all_of does for us.

    Numeric validation
        -- When dealing with numeric input, the obvious way to proceed is to use the extraction operator to extract input to a numeric type. 
           By checking the failbit, we can then tell whether the user entered a number or not.
        -- Doing input validation in C++ is a lot of work. Fortunately, many such tasks (e.g. doing numeric validation as a string) can be 
           easily turned into functions that can be reused in a wide variety of situations.
        
6 — Basic file I/O
    -- File I/O in C++ works very similarly to normal I/O (with a few minor added complexities). There are 3 basic file I/O classes in C++:
       ifstream (derived from istream), 
       ofstream (derived from ostream), and 
       fstream (derived from iostream). 
       These classes do file input, output, and input/output respectively. To use the file I/O classes, we need to include the fstream header.
    -- Unlike the cout, cin, cerr, and clog streams, which are already ready for use, file streams have to be explicitly set up by the programmer. 
    -- To open a file for reading and/or writing, simply instantiate an object of the appropriate file I/O class, with the name of the file
       as a parameter. Then use the insertion (<<) or extraction (>>) operator to write to or read data from the file. Once we are done, 
       there are several ways to close a file: 
        -- explicitly call the close() function,
        -- just let the file I/O variable go out of scope (the file I/O class destructor will close the file for you).
    
    File output
        -- To do file output in the following example, we’re going to use the ofstream class. This is extremely straightforward:
        -- Note that it is also possible to use the put() function to write a single character to the file.

    File input
        -- Note that ifstream returns a 0 if we’ve reached the end of the file (EOF).
    
    Buffered Output
        Buffered I/O : Output to file streams may not be written immediately; data is stored in a buffer for efficiency.
        Flushing     : Writing the buffer to disk happens when the file is closed or explicitly flushed (ostream::flush or std::flush).
        Risk         : If a program crashes or calls exit() before closing files, buffers may not flush, causing data loss.
        Best Practice: Always close files explicitly before exit().
        Manual Flush : Use ostream::flush() or std::flush to force immediate writing.
        std::endl    : Inserts newline and flushes. Overuse can hurt performance; prefer '\n' when flushing isn’t needed.
    
    File Modes
        -- When opening files, C++ lets us choose how the file should be accessed using file mode flags (from std::ios). 
           If we write to an existing file without specifying a mode, it gets overwritten. To change this behavior—such as appending—
           we pass additional mode flags when opening the file.

            +--------+-----------------------------------------------+
            | Mode   | Meaning                                       |
            +--------+-----------------------------------------------+
            | app    | Append to end of file                         |
            | ate    | Seek to end immediately after opening         |
            | binary | Open in binary mode                           |
            | in     | Open for reading (default for ifstream)       |
            | out    | Open for writing (default for ofstream)       |
            | trunc  | Clear file contents if it exists              |
            +--------+-----------------------------------------------+

        -- We can combine modes using bitwise OR (|).
            Defaults:
                ifstream → std::ios::in
                ofstream → std::ios::out
                fstream → std::ios::in | std::ios::out

        Tip : fstream may fail if we include std::ios::in and the file doesn’t exist.
              To create a new file, open with only std::ios::out.

    Filesystem path support (C++17)
        -- Since C++17, all file stream constructors and open() functions accept std::filesystem::path objects directly (#include <filesystem>),
           allowing seamless, portable cross-platform path handling, directory navigation, and native Unicode filename support:
               std::filesystem::path p{ "data/records.bin" };
               std::ofstream out{ p, std::ios::binary };

    Binary File I/O with read() and write()
        -- In addition to formatted text I/O using << and >>, file streams provide unformatted binary I/O member functions:
           -- ostream::write(const char* s, std::streamsize n): Writes n raw bytes from memory buffer s directly to disk.
           -- istream::read(char* s, std::streamsize n): Reads up to n raw bytes from disk directly into memory buffer s.
        -- Binary mode (std::ios::binary) is mandatory for binary files to prevent automatic newline translation on platforms like Windows.
        -- Pointers to non-char objects must be explicitly cast using reinterpret_cast<char*> or reinterpret_cast<const char*>:
               struct Record { int id; double value; };
               Record r{ 1, 99.5 };
               out.write(reinterpret_cast<const char*>(&r), sizeof(Record));
               in.read(reinterpret_cast<char*>(&r), sizeof(Record));
        -- gcount() can be called after in.read() to determine exactly how many raw bytes were extracted.
        -- WARNING: Only trivially copyable types (primitive types, plain old data structs) may be safely written to and read from disk with read()/write().
           Never write objects containing pointers, dynamic memory (std::string, std::vector), or virtual function tables (polymorphic classes),
           as pointers will become invalid and point to undefined memory upon subsequent program executions.

7 — Random file I/O
    The file pointer
        -- File pointer keeps an internal file pointer that tracks the current read/write position. All reads and writes happen at 
           this position. Normally, when a file is opened, the pointer starts at the beginning. However, if the file is opened in 
           append mode, the pointer is automatically moved to the end so that new writes do not overwrite existing data.
    
    Random file access with seekg() and seekp()
        -- File I/O is normally sequential, but we can jump to any position in a file using random access. This is useful when dealing 
           with fixed-size records and we want to go directly to a specific one without reading everything before it.
        
        Reposition the file pointer using:
            -- seekg() — change read (“get”) position
            -- seekp() — change write (“put”) position
        In file streams, the read and write positions are always the same, so both functions work interchangeably.

        Both functions accept:
            -- An offset (in bytes) — how far to move
            -- A reference point, specified with these flags:

            +-------------+---------------------------------------------------+
            | Flag        | Meaning                                           |
            +-------------+---------------------------------------------------+
            | beg         | Offset from the beginning of the file (default)   |
            | cur         | Offset from the current file pointer position     |
            | end         | Offset from the end of the file                   |
            +-------------+---------------------------------------------------+

        -- Positive offsets move forward; negative offsets move backward.

        NOTE:
            Seeking to arbitrary positions in a text file can behave unpredictably because text newlines are encoded differently on 
            different systems:
                -- Windows: newline = CR + LF (2 bytes)
                -- Unix/Linux: newline = LF (1 byte)
            Because newline sizes differ, moving the file pointer forward or backward by a set number of bytes may land on inconsistent 
            character boundaries.
            Additionally, some systems pad files with extra zero bytes, so seeking relative to the file end may produce unexpected 
            positions.
            For reliable results, avoid random seeking in text files—use it only with binary files.

    -- seekg() and seekp() are better used on binary files. You can open the above file in binary mode via:
        std::ifstream inf {"Sample.txt", std::ifstream::binary};
    -- Two other useful functions are tellg() and tellp(), which return the absolute position of the file pointer. 
       This can be used to determine the size of a file:
            std::ifstream inf {"Sample.txt"};
            inf.seekg(0, std::ios::end); // move to end of file
            std::cout << inf.tellg();
        
        NOTE : The result of the above code on Windows may differ from that on Linux due to differences in newline representation.
                    -- On Windows, a newline is stored as two bytes (CR+LF).
                    -- On Linux/Unix, a newline is stored as one byte (LF).
                If the file contains trailing zero bytes, the reported position may also differ.
    
    Reading and writing a file at the same time using fstream
        -- The fstream class is capable of both reading and writing a file at the same time -- almost! The big caveat here is that 
           it is not possible to switch between reading and writing arbitrarily. Once a read or write has taken place, the only way to 
           switch between the two is to perform an operation that modifies the file position (e.g. a seek). 
           
           If we don’t actually want to move the file pointer (because it’s already in the spot you want), we can always seek to the 
           current position:

                // assume iofile is an object of type fstream
                iofile.seekg(iofile.tellg(), std::ios::beg); // seek to current file position

            If we do not do this, any number of strange and bizarre things may occur.
            (Note: Although it may seem that iofile.seekg(0, std::ios::cur) would also work, it appears some compilers may optimize this 
            away).

            One other bit of trickiness: Unlike ifstream, where we could say while (inf) to determine if there was more to read, 
            this will not work with fstream.

        Other useful file functions
            -- To delete a file, simply use the remove() function.
            -- Also, the is_open() function will return true if the stream is currently open, and false otherwise.

        A warning about writing pointers to disk
            -- Do not write memory addresses to files. The variables that were originally at those addresses may be at different addresses 
               when you read their values back in from disk, and the addresses will be invalid.
