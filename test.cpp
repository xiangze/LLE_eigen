#include "LLEeigen.hpp"
#include "csvget.hpp"

#include "stdio.h"
#include <iostream>
#include <math.h>
#include <vector>

using namespace std;

void setdatamat(const char *filename,MatrixXf & data){
  char buf[256];
  FILE *fp=fopen(filename,"r");
  for(int i=0;i<data.rows();++i){
    vector<string> vecData;
    fgets(buf,256,fp);
    string s=string(buf);
    CSVTokenizerForSTL(s,vecData);
    for(int j=0;j<data.cols();++j)
      data(i,j)=atof(vecData[j].c_str());
  }
  fclose(fp);
}

void setdistmat(MatrixXf & data,MatrixXf & dist){
  for (int i = 0;i < data.rows();i++){
    for (int j = i;j < data.rows();j++){
      dist(i,j)=0;
      for (int k = 0;k < data.cols();k++){
	dist(i,j)+=(data(i,k)-data(j,k))*(data(i,k)-data(j,k));
      }
      dist(i,j)=sqrt(dist(i,j));
      dist(j,i)=dist(i,j);
    }
  }
}



int numneibor=10;

int main(int argc,char **argv){
  int num=17;
  char filename[]="../sw100.txt";
  num=atoi(argv[1]);
  //  num=atoi(argv[1]);
  int indim=3;
  int outdim=2;

  MatrixXf data_withlabel(num,indim+1);
  setdatamat(filename,data_withlabel);

  MatrixXf label=data_withlabel.rightCols(1);
  MatrixXf data=data_withlabel.block(0,0,num,indim);

  MatrixXf dist(num,num);
  MatrixXf out(num,outdim);
  setdistmat(data,dist);

//   cout << "label:" << endl <<label << endl;
//  cout << "input:" << endl << data_withlabel << endl;
//  cout << "input:" << endl << data << endl;

  lle_eigen(dist,data,out,outdim,numneibor);

  MatrixXf out_withlabel(num,outdim+1);
  out_withlabel<< out,label;

  //  cout << "output:" << endl << out_withlabel << endl;
  cout << out_withlabel << endl;
  return 1;
}
