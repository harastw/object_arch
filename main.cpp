#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <iterator>
#include <string>
#include <vector>
#include <list>
#include <array>

enum { _name = 0, _x, _y, _type, _time };

namespace {
  constexpr const auto Path = "file.txt";
  constexpr auto TotalFields = 5;
}
  
void printvs(const std::vector<std::string>& obj) {
  for (auto el : obj)
    std::cout << el << std::endl;
}

class SomeClass {
private:
  std::string name, x, y, typeName, creationTime;
public:
  SomeClass(const std::array<std::string,TotalFields>& args) {
    name = args[_name];
    x = args[_x];
    y = args[_y];
    typeName = args[_type];
    creationTime = args[_time];
  }

  [[nodiscard]] const std::string GetName() const {
    return name;
  }
  
  [[nodiscard]] const std::string GetInfo() const {
    return name + " " + x + " " + y + " " + creationTime + " " + typeName; }
};

class Server {
private:
  std::list<SomeClass> mainList;
  void InitTokens();
public:
  Server() { InitTokens(); }
  void Update(SomeClass someObj);
  [[nodiscard]] const std::list<SomeClass> * const GetListServer() const { return &mainList; }
};

class UserInterface {
private:
  Server server;
  [[nodiscard]] const bool FunCallFromInputP(const std::string& input);
  [[nodiscard]] const std::string GetInput() const;
  [[nodiscard]] std::array<std::string,TotalFields> UltimateSetter() const;
  void AddElement();
  void Print() const;
public:
  UserInterface();
  void RunUserSession();
};

int main() {
  auto server = Server();
  auto session = UserInterface();
  session.RunUserSession();
  return EXIT_SUCCESS;
}

void Server::InitTokens() {  
  std::fstream resFile(Path);
  if (!resFile) {
    std::cout << "Unable to open " << Path << std::endl;
    return;
  }
  
  std::array<std::string,TotalFields> fields;
  int i = 0;
  for (std::size_t i = 0; !resFile.eof(); i = (i + 1) % TotalFields) {
    resFile >> fields[i];

    if (i == (TotalFields - 1)) {
      SomeClass newObj(fields);
      mainList.push_front(newObj);
    }
  }
}

void Server::Update(SomeClass someObj) {
  mainList.push_back(someObj);
  std::fstream resFile(Path);
  for (auto obj : mainList)
    resFile << obj.GetInfo();
  resFile.close();
}

UserInterface::UserInterface() {
  server = Server();
}

std::array<std::string,TotalFields> UserInterface::UltimateSetter() const {
  std::string name, x, y, createTime, typeName;
  std::cin >> name >> x >> y >> createTime >> typeName;
  std::array<std::string,5> array = {name, x, y, createTime, typeName};
  return array;
}

void UserInterface::AddElement() {
  auto initArgs = UltimateSetter();
  auto obj = SomeClass(initArgs);
  server.Update(obj);
}

void UserInterface::Print() const {
  for (auto el : *server.GetListServer())
    std::cout << el.GetInfo() << std::endl;
}

const bool UserInterface::FunCallFromInputP(const std::string& input) {
  if (input == "add")
    AddElement();
  else if (input == "print")
    Print();
  else if (input == "q")
    return false;
  else
    std::cout << "incorrect input" << std::endl;
  return true;
}

const std::string UserInterface::GetInput() const {
  std::cout << "go next?\n\"add\" - add element" << std::endl
	    << "\"print\" - print" << std::endl
	    << "\"q\" - exit" << std::endl;
  std::string userInput;
  std::cin >> userInput;
  return userInput;
}

void UserInterface::RunUserSession() {
  while (FunCallFromInputP(GetInput())) {}
}
