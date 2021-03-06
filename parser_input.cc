#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>


using namespace std;

struct dataInput {
  vector<vector<int> > clauses;
  int nbVar;
};

vector<string> splitStrings(string str,  char dl){
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

dataInput toParseInput(string path){
  dataInput dInp;
  vector<vector<int> > res;
  ifstream myfile;
  myfile.open(path.c_str());
  string strOneLine;
  int nbVar;
  int nbClause;
  int nbLine=0;


  while (myfile){
        getline(myfile, strOneLine);
        vector<int> oneline;
        vector<string> lineSplitted = splitStrings(strOneLine, ' ');

        if (nbLine==0){

          nbVar = atoi(lineSplitted.at(2).c_str());
          nbClause = atoi(lineSplitted.at(3).c_str());
          //cout << "strOneLine "<<strOneLine << endl;
        }
        else if (nbLine>0){
          if (lineSplitted.size()>0){
            for (int i=0; i<lineSplitted.size();i++){
              if (atoi(lineSplitted[i].c_str())!=0){

                //cout<<" atoi i " << atoi(lineSplitted[i].c_str()) << " | ";
                oneline.push_back(atoi(lineSplitted[i].c_str()));
              }
            }
            //cout<<"size of one clause "<< oneline.size() << "\n";
            //cout << "\n";
            res.push_back(oneline);
          }

        }
        nbLine++;
    }
  dInp.clauses = res;
  dInp.nbVar = nbVar;


  myfile.close();
  //cout<<"size of vec clauses "<< res.size() << "\n";
  return dInp;

}
