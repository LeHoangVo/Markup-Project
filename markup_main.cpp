/*
Name: Le Hoang Vo
Date: 27th March, 2023
Purpose: create a program that can help user to either:
	convert a file (text file for instance) to a HTML file
	report HTML file data(words, paragraphs and tags)
	change the color of keywords
	or do every of the above functions
*/

#include "markup_header.hpp"
#include<filesystem>

using namespace std::filesystem;
using namespace std;

int main(int argc, char* argv[])
{
	//1.Parse Command Line input
	vector<string> args;
	for (int i = 1; i < argc; i++)
	{
		args.push_back(argv[i]);
	}

	//1.1: Quit if there's no input 
	if (args.empty())
	{
		cout << "Error: text file not specified.\n";
		return 1;
	}

	//1.2: Report "help" if there's "--help" arg
	vector<string>::iterator locator = find(args.begin(), args.end(), "--help");
	if (locator != args.end())
	{
		printInstruction();
		return 1;
	}

	//2. Check for input file and switches
	//if there's a switch (-r -p -rp -pr), it must be at argv[1]
	//infile name must be argv[1] or argv[2]
	string switches;
	ifstream infile;
	string inpfileName;

	//Check the first argv (can be switch or text file)
	//Switch (look for '-')
	//2.1: Check switches
	bool report = false;
	bool paragraph = false;
	if (argv[1][0] == '-')
	{
		switches = args.front();
		if (switches.size() > 1)
		{
			for (char i : switches)
			{
				if (i == '-') {
					//the first char of the switch is always '-' --> do nothing
					//I have this condition to eliminate '-' out of validation, 
					//because I chose to start checking from the beginning of the switch
				}

				else if (i == 'r')
				{
					report = true;
				}

				else if (i == 'p')
				{
					paragraph = true;
				}

				else {
					cout << "Error: unknown switch \"" << switches << "\"" << endl;
					return 1;
				}
			}
		}
		args.erase(args.begin());
		if (args.empty() || args.front() == "--keywords")
		{
			cout << "Error: text file not specified.\n";
			return 1;
		}

	}

	//Input file name validation
	path infilename(args.front());
	infile.open(infilename);
	inpfileName = args.front();
	args.erase(args.begin());
	size_t foundHTML = inpfileName.find("html");

	if (args.empty() && (foundHTML != string::npos))
	{
		cout << "Error: input and output files are the same." << endl;
		return 1;
	}

	//2.2: Check input filename, directory
	if (!infile)
	{
		cout << "Error: text filename \"" << inpfileName << "\" --> No such file or directory\n";
		return EXIT_FAILURE;
	}

	//3. Check for the 3rd arguments (output File name or --keywords tag)
	string outfileName;
	bool keywords = false;

	//Validate conditions for output file name, conversion if there's only input file name
	if (args.empty())
	{
		outfileName = inpfileName;
		outfileName = outfileName.replace(inpfileName.find("txt"), 3, "html");
	}
	else if (args.front() == "--keywords") {
		outfileName = inpfileName;
		outfileName = outfileName.replace(inpfileName.find("txt"), 3, "html");
		keywords = true;
	}
	else {
		outfileName = args.front();
		if (!args.empty())
		{
			args.erase(args.begin());
		}
	}

	if ((inpfileName == outfileName) && (foundHTML != string::npos))
	{
		cout << "Error: input and output files are the same." << endl;
		return 1;
	}

	//Create output file stream with output file name after validation and conversion
	ofstream outfile(outfileName);
	string pageTitle = outfileName.substr(0, outfileName.size() - 5);

	//4. Check for the 4th arguments
	if (!args.empty() && !keywords)
	{
		if (args.front() != "--keywords")
		{
			cout << "Error: too many command line arguments \"" << args.front() << "\"" << endl;
			return 1;
		}
		else
		{
			keywords = true;
			args.erase(args.begin());
		}
	}

	vector<string> keywordsString;

	if (keywords)
	{
		if (args.empty())
		{
			cout << "Error: keyword list is empty" << endl;
			return 1;
		}

		if (args.size() > 12)
		{
			cout << "Error: keyword list is too large (maximum 12)" << endl;
			return 1;

		}

		args.erase(args.begin());

		while (!args.empty())
		{
			keywordsString.push_back(args.front());
			args.erase(args.begin());
		}
	}


	int linecounter = 0;
	int paragraphcounter = 0;

	string line;
	vector<string> orgCharVec;
	bool newlineCheck = true;

	while (getline(infile, line)) {
		linecounter++;
		orgCharVec.push_back(line);

		if (line.size() == 0 && !newlineCheck)
			paragraphcounter++;
		if (line.size() == 0)
			newlineCheck = true;
		else
			newlineCheck = false;
	}
	if (orgCharVec[linecounter - 1].size() != 0)
	{
		paragraphcounter++;
	}

	int tags = 0;

	//Check for the report tags --> print out datas to the console
	if (report)
	{
		cout << "# lines input = " << linecounter << endl;
		cout << "# paragraphs ouptut = " << paragraphcounter << endl;

		for (int i = 0; i < keywordsString.size(); i++)
		{
			bool validKeyword = true;
			for (int j = i + 1; j < keywordsString.size(); j++)
			{
				if (keywordsString[i] == keywordsString[j])
					validKeyword = false;
			}
			if (validKeyword)
			{
				for (string line : orgCharVec)
				{
					size_t foundkeywords = line.find(keywordsString[i]);
					if (foundkeywords != string::npos)
					{
						tags++;
					}
				}
			}
		}

		cout << "# tags = " << tags << endl;

		//Check for keywords count, number
		if (keywords) {
			vector<string> keywordCodes = { "Red", "Green", "Blue", "DarkTurqoise",
									"Magenta","Indigo","DarkRed","DeepPink",
									"OrangeRed", "SpringGreen", "Teal", "DodgerBlue" };
			for (int i = 0; i < keywordsString.size(); i++)
			{
				bool validKeyword = true;
				for (int j = i + 1; j < keywordsString.size(); j++)
				{
					if (keywordsString[i] == keywordsString[j])
						validKeyword = false;
				}
				if (validKeyword)
				{
					int keywordCounter = 0;
					for (string line : orgCharVec)
					{
						size_t foundkeywords = line.find(keywordsString[i]);
						if (foundkeywords != string::npos)
						{
							keywordCounter++;
						}
					}

					cout << "        # " << keywordsString[i] << " (" <<
						keywordCodes[i] << ") = " << keywordCounter << endl;
				}
			}
		}
	}

	//Create a vector to store all the data from the input file,
	//Adjust tags, switches and keywords before putting finished data to output file stream
	vector<string> newchar;

	//DOC HEADER for the HTML file
	newchar.push_back("<!DOCTYPE html>\n");
	newchar.push_back("<html>\n");
	newchar.push_back("<head>\n");
	newchar.push_back("<title>");
	newchar.push_back(pageTitle);
	newchar.push_back("</title>\n");
	newchar.push_back("<style>\n");
	newchar.push_back("</style>\n");
	newchar.push_back("</head>\n");
	newchar.push_back("<body>\n");

	//Check if the switch has "p" --> add <p></p> to HTML file
	if (paragraph)
	{
		newchar.push_back("<p>\n" + orgCharVec[0] + "\n");

		for (int i = 1; i < orgCharVec.size() - 1; i++)
		{
			if (orgCharVec[i].size() == 0)
			{
				newchar.push_back("</p>\n");
				while (orgCharVec[++i].size() == 0)
				{
					newchar.push_back("<br>\n");
				}
				i--; //move back 1 char because the while loop will exit at a non-empty string
					 //--> this will prevent skipping any strings	 
				newchar.push_back("<p>\n");
			}
			else {
				newchar.push_back(orgCharVec[i] + "\n");
			}
		}
		newchar.push_back(" " + orgCharVec[orgCharVec.size() - 1] + "</p>\n");
	}

	else {
		newchar.push_back(orgCharVec[0]);

		for (int i = 1; i < orgCharVec.size() - 1; i++)
		{
			if (i == 1 && orgCharVec[i].size() == 0)
			{
				newchar.push_back("<br>\n");
			}

			if (orgCharVec[i].size() != 0)
			{
				newchar.push_back(" " + orgCharVec[i]);
			}
			else {
				newchar.push_back("<br>\n");
				continue;
			}
			if (orgCharVec[i + 1].size() == 0)
			{
				newchar.push_back("<br>\n");
			}
		}
		newchar.push_back(" " + orgCharVec[orgCharVec.size() - 1]);
	}

	newchar.push_back("</body>\n");
	newchar.push_back("</html>");

	//Put the data to the output(HTML) file
	for (string c : newchar)
		outfile << c;

	return 0;
}