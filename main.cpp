#include <iostream>
#include <stdexcept>
#include <limits>
int func(){
	int max_val = std::numeric_limits<int>::max();
	int a=0;
	int i=0;
	int count = 1;
	std::cin >> a;
	if (std::cin.fail() && !std::cin.eof()){
		throw 1;
	}
	else if (std::cin.eof()){
		throw 2;
	}
	if (a==0){
		std::cout << 0;
		return 0;
}
	int c =a;
	while (a!=0){ 
		if (a<c){
			c=a;
			i=0;
			i++;
	}
		else if (a==c){
			i++;
	}
		std::cin >> a;
		count++;
		if (count==max_val){
			throw 2;
		}
		if (std::cin.fail()){
			throw 1;
		}
}
	return i;
}
int main(){
	try{
		int res=func();
		std::cout << res << "\n";
	}
	catch (int k){
		if (k==1){
			std::cerr << "Error" << "\n";
			return 1;
		}
		else if (k==2){
			std::cerr << "The length is incorrect" << "\n";
			return 2;
		}
	}
	return 0;
}
