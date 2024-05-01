#include<string>
#include<memory>
#include<vector>
#include<iostream>
#include<map>
#include "license.h"

License::License(std::string name, unsigned int salary){
		if(name.empty() || salary <= 0)
			throw std::runtime_error("Invalid name/salary.");
		
		this->name = name;
		this->salary = salary;
		this->counter = 0;
	
	}
	
	std::string License::get_guildname() const {return name;}
	
	unsigned int License::get_salary() const {return salary;}
	
	bool License::valid() const {return counter<=3;}
	
	bool License::use(){
		if(!valid()) {
			return false;
		}
		else {
			counter++;
			return true;
		}
	}
	
	std::ostream& License::print(std::ostream& o) const {
		o<<"[License for " << name << ", Salary: " << salary << ", Used: " << counter << "]";
	return o;
	}
	
	std::ostream& operator<<(std::ostream& o, const License& l){
		return l.print(o);
	}