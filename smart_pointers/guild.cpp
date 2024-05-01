#include<string>
#include<memory>
#include<vector>
#include<iostream>
#include<stdexcept>
#include<set>
#include<algorithm>
#include<map>
#include "guild.h"
#include "license.h"
#include "person.h"

	Guild::Guild(std::string name, unsigned fee, unsigned sal, const std::vector<std::shared_ptr<Person>>& members ):
		name{ name }, fee{ fee }, salary{ sal }, members{} {
		if (name.empty() || fee <= 0 || salary <= 0) {
			throw std::runtime_error("Invalid name/fee/salary. ");
		}
	
		for (const auto& i : members) {
			if (!add_member(i)) {
				throw std::runtime_error("Worker allready in vector.");
			}
		}
	}

	bool Guild::add_member(std::shared_ptr<Person> p) {
		if (members.count(p->get_name()) == 1) {
			return false;
		}
		else {
			this->members[p->get_name()] = p;
			return true;
		}
	
	}

	bool Guild::remove_member(std::string n) {
		if (members.count(n) == 0) {
			return false;
		}
		else {
			members.erase(n);
			return true;
		}
	}

	void Guild::grant_license(std::string n) {
		if (members.count(n) == 1 && members[n]->pay_fee(this->fee)) {
			members[n]->receive_license(std::make_unique<License>(this->name, this->salary));
		}
		else {
			throw std::runtime_error("Not part of guild/not enough money to pay fee.");
		}
	
	}

	bool Guild::offer_job(std::shared_ptr<Person> p) {
		if (members.count(p->get_name()) == 1 ) {
			p->work(salary * 2);
			return true;
		}
    
		if(p->eligible(name)) {
			p->work(this->name);
			return true;
		}

		return false;
	
	}

	std::ostream& Guild::print(std::ostream& o) const {
		o<< "[" << this->name << ", License fee: " << this->fee << ", Job salary: " << this->salary << ", {";

		bool first{ true };
		for (const auto& i : this->members) {
			if (first) {
				first = false;
			}
			else {
				o<< ", ";
			}
			o << i.second->get_name();
		}
		o << "}]";
		return o;
	}

	std::ostream& operator<<(std::ostream& o, const Guild& g) {
		return g.print(o);
	}
