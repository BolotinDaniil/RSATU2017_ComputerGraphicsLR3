#include "obj.h"

void loadObj (const char *fileName, vector<glm::vec3> *verteces, vector<int> *indeces)
{
  ifstream file; //�������� ����.
  file.open(fileName);
   if(!file.fail())
    {
      string str;
      glm::vec3 nVector;
      int i[4] = {0,0, 0, 0};
      while(getline(file, str))
      {
        str.erase(str.find_last_not_of(" \n\r\t")+1);
        if(str.find_first_of("#") != -1) //������� ����������
        continue;
            
        string prefix = str.substr(0, str.find_first_of(" ")); //����� ������
        if(prefix.compare("v")==0)
        {
          str = str.substr(str.find_first_of("v")+2);//�������� ������ ���� x y z
          
          istringstream s(str);
          s >> nVector.x;  //�������� ��������� � 3 ������ ������
          s >> nVector.y;
          s >> nVector.z;
          verteces->push_back(nVector); //��������� � ��������� ������
          
        }
        if(prefix.compare("f")==0) //����� ��������� �������, ��� ������������� ��� �� ���� ��� ��� �� �� �����.
        {
          str = str.substr(str.find_first_of("f")+2);
          istringstream s(str);
          s >> i[0];
          s >> i[1];
          s >> i[2];
          s >> i[3];
          if(i[3]==0)
          {
          indeces->push_back(i[0]);i[0]=0;
          indeces->push_back(i[1]);i[1]=0;
          indeces->push_back(i[2]);i[2]=0;
          }
          else
          {
          indeces->push_back(i[0]);i[0]=0;
          indeces->push_back(i[1]);i[1]=0;
          indeces->push_back(i[2]);i[2]=0;
          indeces->push_back(i[3]);i[3]=0;
          }
          
        }
      }  
          
    }
}