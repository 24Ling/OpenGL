#pragma once
#include <vector>
#include <functional>
#include <string>
#include <iostream>

class Test
{
public:
	Test() {}
	virtual ~Test() {};


	virtual void OnUpdate() {}
	virtual void OnImguiRender() {}
	virtual void OnRender() {}

};

class TestMenu : public Test
{
public:

	TestMenu(Test*& currentTestPointer);
	~TestMenu();

	void OnImguiRender() override;

	template<typename T>
	void RegisterTest(const std::string& name)
	{
		std::cout << "Register: " << name << std::endl;

		m_Tests.push_back(std::make_pair(name, []() { return new T(); }));
	}

private:
	Test*& m_CurrentTest;
	std::vector <std::pair<std::string, std::function<Test* ()>>> m_Tests;
};