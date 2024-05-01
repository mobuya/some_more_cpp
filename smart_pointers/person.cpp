#include<string>
#include<memory>
#include<vector>
#include<iostream>
#include<map>
#include "person.h"
#include "license.h"

//base class Person

	Person::Person(std::string name, unsigned int wealth) : name{ name }, wealth{ wealth }, licenses{}  {
		if (name.empty() || wealth < 0) {
			throw std::runtime_error("Invalid parameters for Person.");
		}
	}


	void Person::work(std::string guild) {
		if (!eligible(guild)) {
			throw std::runtime_error("License not valid/usable.");
		}
		else {
			this->licenses[guild]->use();
			work(this->licenses[guild]->get_salary());	
		}

	}

	//virtual void work(unsigned int) = 0;

	void Person::increase_wealth(unsigned int i) { wealth += i; }

	std::string Person::get_name() const { return name; }

	bool Person::pay_fee(unsigned int i) {
		if (wealth < i || i <= 0) {
			return false;
		}
		else {
			wealth -= i;
			return true;
		}

	}

	void Person::receive_license(std::unique_ptr<License> l) {
		if (this->licenses.count(l->get_guildname()) == 1) {
			this->licenses[l->get_guildname()].reset(l.release());
		}
		else {
			this->licenses[l->get_guildname()] = std::move(l);
		}
	}

	void Person::transfer_license(std::string l, std::shared_ptr<Person> p) {
		if (eligible(l)) {
			p->receive_license(std::move(this->licenses[l]));
			licenses.erase(l);
		}
		else {
			throw std::runtime_error("License not valid for transfer.");
		}
	}

	bool Person::eligible(std::string l) const {
		return licenses.count(l) == 1 && licenses.at(l)->valid();
	}

	std::ostream& Person::print(std::ostream& o) const {
		o << this->name << ", " << this->wealth << " Coins, {";
		bool first{ true };
		for (const auto& l : this->licenses) {
			if (first) {
				first = false;
			}
			else {
				o << ", ";
			}
			l.second->print(o);
		}
		o << "}]";
		return o;
	}

	std::ostream& operator<<(std::ostream& o, const Person& p) {
		return p.print(o);
	}


	//subclass Worker:

		Worker::Worker(std::string name, unsigned int i) : Person(name , i) {}

		void Worker::work(unsigned int i) { increase_wealth(i); }

		std::ostream& Worker::print(std::ostream& o) const {
			o << "[Worker ";
			Person::print(o);
			return o;
		}



	//subclass Superworker

		Superworker::Superworker(unsigned int fee, std::string name, unsigned int i) : Person(name, i), fee{fee} {}

		void Superworker::work(unsigned int i) { increase_wealth(i+fee); }

		std::ostream& Superworker::print(std::ostream& o) const {
			o << "[Superworker ";
			Person::print(o);
			return o;
		}
