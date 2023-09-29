/*
Name: Le Hoang Vo
Date: 27th March, 2023
Puspose: implement functions  
*/

#include "markup_header.hpp"

using namespace std;

void printInstruction() {
    cout << "usage: markup[--help][-rp] textFilename[htmlFilename][--keywords Args...]" << endl << endl;

    cout << "    --help          display the help text for the program." << endl;
    cout << "    -r              report convertion metrics" << endl;
    cout << "    -p              use <p> tag for paragraph breaks" << endl << endl;
    cout << "    textFilename    the name of the ASCII text file to process" << endl;
    cout << "    htmlFilename    the name of the HTML file receiving the output." << endl;
    cout << "                    If not provided the text filename will be used" << endl;
    cout << "                    with its extension changed to \".html\"" << endl << endl;
    cout << "    --keywords          keywords to highlight";
}