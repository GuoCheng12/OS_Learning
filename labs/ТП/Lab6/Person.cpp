#include "Person.h"

Person::Person() : name(""), surname(""), age(15)
{
}

bool Person::isOutStanding() {
    return true;
}

void Person::getData() {
    
}

void Person::setName() {
    cout << "Enter name: ";
    cin >> name;
}

void Person::setSurname() {
    cout << "Enter surname: ";
    cin >> surname;
}

void Person::setAge() {
    cout << "Enter age: ";
    cin >> age;
}

Student::Student()
{
    Person();
}

void Student::getData() {
    Person::setName();
    Person::setSurname();
    Person::setAge();
    cout << "Gga: ";
    cin >> gpa;
}

bool Student::isOutStanding() {
    return (gpa > 3.5) ? true : false;
}

Professor::Professor() {
    Person();
}

void Professor::getData() {
    Person::setName();
    Person::setSurname();
    Person::setAge();
    cout << "Count of pubs: ";
    cin >> numPubs;
}

bool Professor::isOutStanding() {
    return (numPubs > 100) ? true : false;
}
