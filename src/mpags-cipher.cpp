#include "CipherMode.hpp"
#include "CipherFactory.hpp"
#include "CaesarCipher.hpp"
#include "PlayfairCipher.hpp"
#include "VigenereCipher.hpp"

#include "ProcessCommandLine.hpp"
#include "TransformChar.hpp"

#include <cctype>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

int main(int argc, char* argv[])
{
    // Convert the command-line arguments into a more easily usable form
    const std::vector<std::string> cmdLineArgs{argv, argv + argc};

    // Options that might be set by the command-line arguments
    ProgramSettings settings{false, false, "", "", {}, {}, CipherMode::Encrypt};

    // Process command line arguments
    const bool cmdLineStatus{processCommandLine(cmdLineArgs, settings)};

    // Any failure in the argument processing means we can't continue
    // Use a non-zero return value to indicate failure
    if (!cmdLineStatus) {
        return 1;
    }

    // Handle help, if requested
    if (settings.helpRequested) {
        // Line splitting for readability
        std::cout
            << "Usage: mpags-cipher [-h/--help] [--version] [-i <file>] [-o <file>] [--multi-cipher <nciphers>] [-c <cipher>] [-k <key>] [--encrypt/--decrypt]\n\n"
            << "Encrypts/Decrypts input alphanumeric text using classical ciphers\n"
            << "Available options:\n\n"
            << "  -h|--help          Print this help message and exit\n"
            << "  --version          Print version information\n"
            << "  -i FILE            Read text to be processed from FILE\n"
            << "                     Stdin will be used if not supplied\n"
            << "  -o FILE            Write processed text to FILE\n"
            << "                     Stdout will be used if not supplied\n"
            << "  --multi-cipher NC  Specify the number of ciphers to be used - defaults to 1\n"
            << "  -c CIPHER          Specify the cipher to be used to perform the encryption/decryption\n"
            << "                     CIPHER can be caesar or playfair or vigenere - caesar is the default\n"
            << "  -k KEY             Specify the cipher KEY\n"
            << "                     A null key, i.e. no encryption, is used if not supplied\n"
            << "  --encrypt          Will use the cipher to encrypt the input text (default behaviour)\n"
            << "  --decrypt          Will use the cipher to decrypt the input text\n"
            << std::endl;
        // Help requires no further action, so return from main
        // with 0 used to indicate success
        return 0;
    }

    // Handle version, if requested
    // Like help, requires no further action,
    // so return from main with zero to indicate success
    if (settings.versionRequested) {
        std::cout << "0.4.0" << std::endl;
        return 0;
    }

    // Initialise variables
    char inputChar{'x'};
    std::string inputText;

    // Read in user input from stdin/file
    if (!settings.inputFile.empty()) {
        // Open the file and check that we can read from it
        std::ifstream inputStream{settings.inputFile};
        if (!inputStream.good()) {
            std::cerr << "[error] failed to create istream on file '"
                      << settings.inputFile << "'" << std::endl;
            return 1;
        }

        // Loop over each character from the file
        while (inputStream >> inputChar) {
            inputText += transformChar(inputChar);
        }

    } else {
        // Loop over each character from user input
        // (until Return then CTRL-D (EOF) pressed)
        while (std::cin >> inputChar) {
            inputText += transformChar(inputChar);
        }
    }

    std::string outputText;

    std::vector <std::unique_ptr<Cipher>> myCiphers;

    for (size_t i{0}; i<settings.cipherType.size(); ++i){
        myCiphers.push_back( CipherFactory::makeCipher(settings.cipherType[i], settings.cipherKey[i]) );
    }
    
    if (settings.cipherMode==CipherMode::Encrypt){
        for (const auto& v : myCiphers){
            inputText = v->applyCipher(inputText, settings.cipherMode);
        }
    }
    else{
        for (auto it = myCiphers.rbegin(); it != myCiphers.rend(); ++it) {
          inputText = (*it)->applyCipher(inputText, settings.cipherMode);  
        }
    }
    
    outputText=inputText;

    // Output the encrypted/decrypted text to stdout/file
    if (!settings.outputFile.empty()) {
        // Open the file and check that we can write to it
        std::ofstream outputStream{settings.outputFile};
        if (!outputStream.good()) {
            std::cerr << "[error] failed to create ostream on file '"
                      << settings.outputFile << "'" << std::endl;
            return 1;
        }

        // Print the encrypted/decrypted text to the file
        outputStream << outputText << std::endl;

    } else {
        // Print the encrypted/decrypted text to the screen
        std::cout << outputText << std::endl;
    }

    // No requirement to return from main, but we do so for clarity
    // and for consistency with other functions
    return 0;
}
