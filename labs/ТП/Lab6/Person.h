#pragma once
#include <string>
#include <iostream>

using namespace std;

class Person
{
public:
    Person();
    virtual void getData();
    virtual bool isOutStanding();
    void getName() { cout << "Name: " <<  name << endl; }
    void getSurName() { cout << "Surname: " <<  surname << endl; }
    void getAge() { cout << "Age: " <<  age << endl; }
    void setName();
    void setSurname();
    void setAge();
    void setData(string _name, string _surname, int _age) { name = _name; surname = _surname; age = _age; }
    
protected:
    string name;
    string surname;
    int age;
};

class Student : public Person {
public:
    Student();
    void getData();
    bool isOutStanding();
    void getGpa() { cout << "Gpa: " << gpa << endl; }
    
private:
    double gpa;
};

class Professor : public Person {
public:
    Professor();
    void getData();
    bool isOutStanding();
    void getNumPubs() { cout << "NumPubs: " << numPubs << endl; }
private:
    int numPubs;
};
