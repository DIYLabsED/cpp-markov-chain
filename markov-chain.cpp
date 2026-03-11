#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <cstdlib>
#include <string>

using namespace std;
using MarkovChain = map<string, vector<string>>;

MarkovChain train(string ftext);
void printChain(MarkovChain chain);
string* generate(MarkovChain chain, unsigned int wordCount);

// Takes in a filepath to a text file, returns a trained markov chain
MarkovChain train(string ftext){

  MarkovChain chain;

  ifstream trainingText(ftext);
  if(!trainingText.is_open()){
    cerr << "Could not open training text file: " << ftext << endl;
    return chain;
  }

  string prevWord, word;

  // Build chain
  while(trainingText >> word){
    if(!prevWord.empty()){
        chain[prevWord].push_back(word);
    }
    prevWord = word;

  }

  trainingText.close();
  printChain(chain);
  return chain;

}

void printChain(MarkovChain chain){

  if(chain.empty()) return;

  cout << endl << endl << "Markov Chain:" << endl;

  for(auto word : chain){

    cout << word.first << " - ";

    for(auto nextWords : word.second){
      cout << nextWords << ", ";
    }

    cout << endl;

  }

  cout << endl << endl;

}

string* generate(MarkovChain chain, unsigned int wordCount){

  if(chain.empty() || wordCount == 0) return nullptr;

  string* result = new string[wordCount];

  // Pick a random starting word
  auto it = chain.begin();
  advance(it, rand() % chain.size());
  string currentWord = it->first;

  result[0] = currentWord;

  for(int i = 1; i < wordCount; i++){

    const vector<string>& nextWords = chain.at(currentWord);
    if(nextWords.empty()) break;
    // Pick next word at random
    currentWord = nextWords[rand() % nextWords.size()];

    result[i] = currentWord;

  }

  return result;

}


int main(int argc, char* argv[]){

  if(argc != 3){
    cerr << "Pass in a filepath to a text file, and an integer word count" << endl;
    return 0;
  }

  srand(42);

  unsigned int wordCount = stoi(argv[2]);

  MarkovChain chain = train(argv[1]);
  string* result = generate(chain, wordCount);

  for(int i = 0; i < wordCount; i++){
    cout << result[i] << " ";
  }

  return 0;

}