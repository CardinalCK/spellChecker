#include <vector>
#include <string>
#include <set>
#include <fstream>
#include <iostream>
#include <limits>
#include <unordered_map>

using namespace std;

void readDict();
void searchDict(string cand);
void generateTerms(string word, set<string>& eDS, int dist, ofstream& outFile);
string extractWord(string& line, size_t pos);
void printVec(vector<string>& vec);


int main(int argc, char** argv) {
	string candidate;
	//unordered_map<string, set<string>> m;
	if (argc == 1) {
		cout << "Please enter a word to check: ";
		cin >> candidate;
	} else {
		if (argc > 2) cout << "Ignoring all but the first argument..." <<  endl;
		candidate = argv[1];
		cout << "You wish to spell check the word " << candidate << endl;
	}
	//if(!dictCheck(candidate)) {
	readDict();
	searchDict(candidate);
	//}
}

string extractWord(string& line, size_t pos) {
	string toReturn;
	while(line[pos] != ' ' && line[pos] != '\n' && !isdigit(line[pos])) {
		toReturn += line[pos];
		pos++;
	}
	return toReturn;
}

void printVec(vector<string>& vec) {
	for(string& str : vec) {
		cout << '\t' + str << endl;
	}
}

void searchDict(string cand) {
	string line;
	ifstream file("terms.txt");
	vector<string> e1candidates;
	vector<string> e2candidates;
	if (file.is_open()) {
		while(getline(file, line)) {
			//cout << " searching lines" << endl;
			size_t ed0 = line.find(cand + '0', 0);
			size_t ed1 = line.find(cand + '1', 0);
			size_t ed2 = line.find(cand + '2', 0);
			//cout << "got eds" << endl;
			if (ed0 != string::npos) {
				cout << "The word " << cand << " is spelled correctly." << endl;
				return;
			}
			if (ed1 != string::npos) {
				e1candidates.push_back(extractWord(line, 0));
			} else if (ed2 != string::npos) {
				e2candidates.push_back(extractWord(line, 0));
			}
		}
		cout << "Done searching lines" << endl;
	}
	file.close();
	cout << "Search Finished!" << endl;
	cout << "The word " << cand << " is not spelled correctly." << endl;
	if(e1candidates.empty() && e2candidates.empty()) {
		cout << "Unfortunately we don't have any suggestions. Sorry." << endl;
		return;
	} else {
		cout << "Here are our suggestions: " << endl;
		if (e1candidates.empty()) {
			printVec(e2candidates);
			cout << endl;
		} else {
			printVec(e1candidates);
			if (!e2candidates.empty()) {
				cout << "Or maybe these?" << endl;
				printVec(e2candidates); 
				cout << endl;
			}
		}
		cout << "Thanks for using this program! See you again!" << endl;
		return;
	}
}


void readDict() {
	ifstream file("terms.txt");
	if (file.good()) {
		cout << "terms.txt already exists. No need to generate terms." << endl;
		return;
	}
	string word;
	ifstream file2("dict.txt");
	ofstream outputFile("terms.txt");
	double count = 0; 
	if (file2.is_open()) {
		while(getline(file2, word)) {
			//For each word in dictionary
			set<string> editDistSet;
			generateTerms(word, editDistSet, 0, outputFile);
			outputFile << "\n";
			count++;
			double perc = (count / 10);
			cout << "\rGenerated terms for word: " << word << " Completion: " << perc << "%: " << string(perc, '|') << string(10, ' ');
			cout.flush();
		}
		file2.close();
		outputFile.close();
	} else {
		cout << "Unable to open dictionary text file. Exiting." << endl;
		exit(EXIT_FAILURE);
	}
}


void generateTerms(string word, set<string>& eDS, int dist, ofstream& outFile) {
	if (word.empty()) return;
	if (dist > 2) return;
	if (eDS.find(word + to_string(dist)) != eDS.end()) {
		return;
	}
	eDS.insert(word + to_string(dist));
	outFile << word << dist << " ";
	string wordCpy;
	//Deletions
	for (int i = 0; i < word.length(); i++) {
		wordCpy = word;
		wordCpy.erase(wordCpy.begin() + i);
		generateTerms(wordCpy, eDS, dist + 1, outFile);
	}
	//Transposes (swaps of adjacent letters)
	for (int i = 0; i < word.length() - 1; i++) {
		wordCpy = word;
		swap(wordCpy[i], wordCpy[i + 1]);
		generateTerms(wordCpy, eDS, dist + 1, outFile);
	}
	//Replaces
	for (int i = 0; i < word.length(); i++) {
		wordCpy = word;
		for (char ch = 'a'; ch <= 'z'; ch++) {
			wordCpy[i] = ch;
			if (wordCpy[i] != word[i]) {
				generateTerms(wordCpy, eDS, dist + 1, outFile);
			}	
		}
	}
	//Inserts
	for (int i = 0; i < word.length() + 1; i++) {
		wordCpy = word;
		for (char ch = 'a'; ch <= 'z'; ch++) {
			wordCpy = word;
			wordCpy.insert(wordCpy.begin() + i, ch);
			generateTerms(wordCpy, eDS, dist + 1, outFile);
		}
	}
}






