#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>

using namespace std;

vector<string> splitStrings2(string str,  char dl){
  string word = "";

  // to count the number of split strings
  int num = 0;

  // adding delimiter character at the end
  // of 'str'
  str = str + dl;

  // length of 'str'
  int l = str.size();

  // traversing 'str' from left to right
  vector<string> substr_list;
  for (int i = 0; i < l; i++) {

      // if str[i] is not equal to the delimiter
      // character then accumulate it to 'word'
      if (str[i] != dl)
          word = word + str[i];

      else {

          // if 'word' is not an empty string,
          // then add this 'word' to the array
          // 'substr_list[]'
          if ((int)word.size() != 0)
              substr_list.push_back(word);

          // reset 'word'
          word = "";
      }
  }

  // return the splitted strings
  return substr_list;
  }

  int  toParseOut(string path){
    int res;
    ifstream myfile;
    myfile.open(path.c_str());
    string strOneLine;
    int nbVar;
    int nbClause;
    int nbLine=0;
    int numLines=0;
    string satiOrNotSati;


    while (myfile){
         getline(myfile, strOneLine);
         vector<string> lineSplitted = splitStrings2(strOneLine, ' ');

         if (nbLine==0){
           satiOrNotSati = lineSplitted.at(1);
           if (satiOrNotSati.compare("SATISFIABLE") == 0){
             res = 1;
           }
          else{
             res = 0;
           }
           //cout << satiOrNotSati << "  "<< res<< "\n";
         }


         nbLine++;

    }
    myfile.close();
    return res;

}
