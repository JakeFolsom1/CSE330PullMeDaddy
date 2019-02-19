#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <cstring>
#include <omp.h>
#include <time.h>

using namespace std;
int main(int argc, char* argv[]) {
	clock_t timer = clock();
	string pattern = argv[1];
	static string checkForPlus = "+";
	size_t pos = 0;
	string patterns[100];
	bool isKleeneStar = false;
	int n = 0;
	int j = 0; //j represents how many .txt files we have
	while ((pos = pattern.find(checkForPlus)) != string::npos)
	{
		patterns[n] = pattern.substr(0, pos);
		pattern.erase(0, pos + checkForPlus.length());
		n++;
	}
	patterns[n] = pattern; //I have no idea what is happening in my code
	string patt = "";
	/*
	if (pattern.find('*', 0)) { //Find a star operator in the input
	int patternIndex = pattern.find('*', 0);
	patt = pattern.substr(0, patternIndex);
	isKleeneStar = true;
	cout << patt;
	}
	*/
#pragma omp parallel for schedule(dynamic) private(j) shared(argc) num_threads(argc-1)

	for (j = 2; j < argc; j++) // For each file, run my logic
	{
		vector<size_t> positions; // holds all the positions that pattern occurs within str
		int matched = 0;
		int i = 0;
		string input = "";
		ifstream inFile;
		inFile.open(argv[j]);
		int lineNo = 0;
		while (getline(inFile, input))
		{
			for (int k = 0; k <= n; k++)
			{
				pattern = patterns[k];
				size_t position = input.find(pattern, 0);
				while (position != string::npos)
				{
					positions.push_back(position);
					position = input.find(pattern, position + 1);
					matched++;
				}

				while (matched != 0)
				{	
					#pragma omp critical 
					{
						cout << argv[j] << ", ";
						cout << lineNo + 1 << ", ";
						cout << positions[i] + 1 << ", ";
						cout << positions[i] + pattern.length() << ":";
						cout << " " << pattern << endl;
						matched--;
						i++;
					}
				}
			}
			lineNo++;
		}
		inFile.close();
	}
	printf("Time taken: %.2fs\n", (double)(clock() - timer) / CLOCKS_PER_SEC);
}
