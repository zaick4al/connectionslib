To link library in next projects:
```CMake
#setup your project
add_subdirectory(connectionlib)
#add_executable
target_link_libraries(${PROJECT_NAME} PRIVATE ConnectionsLib
)
target_include_directories(${PROJECT_NAME} PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/connectionlib/src
    #...
```
Use example:
```cpp
#ifndef MYOBJECT_H
#define MYOBJECT_H
#include <iostream>
#include "cobject.h"

class MyObject : public CObject {
public:
    CEvent<> someStuff;
    CEvent<int> valueChanged;
    explicit MyObject() {
        
    }
    void changeValue(int p_value) {
        valueChanged.emit(p_value);
        someStuff.emit();
    }
    void onValueChanged(int p_value){
        std::cout << p_value << std::endl;
    }
    void onSomeStuff(){
        std::cout << "some stuff" << std::endl;
    }
};

#endif
```
```cpp
#include <iostream>
#include <stdio.h>
#include <memory>
#include "src/object.h"

int main(int argc, char *argv[]) {
    auto obj1 = std::make_shared<MyObject>();
    auto obj2 = std::make_shared<MyObject>();
    auto obj2Weak = std::weak_ptr<MyObject>(obj2);
    obj1->someStuff.connect([obj2Weak](){
        if (auto p_obj = obj2Weak.lock()) {
            p_obj->onSomeStuff();
        }
    });
    obj1->valueChanged.connect([obj2Weak](int p_value){
        if (auto p_obj = obj2Weak.lock()) {
            p_obj->onValueChanged(p_value);
        }
    });
    obj1->changeValue(5);
    return 0;
}
```
