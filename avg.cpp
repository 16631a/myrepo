#include <iostream>

int main()
{
      int avg=0,value=0,i=0;
      std::cout<<"enter 5 no one by one:\n";
      for(i=0;i<6;i++)
      {
       std::cin >> value;
       avg +=value;
      }
      return avg;
}
