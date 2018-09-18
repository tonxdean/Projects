
#include <string>
#include <iostream>

std::string reverse_word(const std::string& word);

int main()
{
	std::string a = "a";
	reverse_word(a);
	
	
	
	
}
std::string reverse_word(const std::string& word)
{
	std::string ret = "";
	for(unsigned int i=0;i<word.size();i++)
	{
		ret+=word[word.size()-1-i];
		
		
	}
	std::cout<<ret<<std::endl;
	return ret;
	
	
}