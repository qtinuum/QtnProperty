#QtnProperty
This is user and programmist friendly properties for Qt framework
#Overview
There are some limitations of standard Qt property system.
This project is an attempt to make better properties for programmist and user.
The key features are:

* Properties hierarchy (properties can be organized in hierarchy at any depth)
* Property widget to observe and edit properties in uniform way
* Signals before and after property has changed
* Property description - short text which help user to understand meaning and purpose of the property
* Property state - property can be disabled or hidden at any moment
* Serialization via QDataStream
* Set/Get property value to/from QVariant and QString
* Scripting support
* Delegates to customize look and feel properties in property widget
* PEG (property/enum generator) - it's tool like Qt moc which generates properties hierarchy from QML like files.

Here are screenshots of the Demo application:
![Demo_screenshot_linux](Docs/img/Demo1.png)
![Demo_screenshot_win](Docs/img/DemoWin.png)

#How to build
**Requirements:**

1. Qt 5.2 framework
2. Flex 2.5.37 and Bison 2.7 (for Windows can be found [here](http://sourceforge.net/projects/winflexbison/))

**To build:**
  
    mkdir path_to_build
    cd path_to_build
    qmake path_to_QtnProperty/Property.pro -r
    make

Generated libraries and executables will be placed into the 'path\_to\_build/bin' folder.
Precomiled libraries and Demo application you can download [here](http://sourceforge.net/projects/qtnproperty/)
  
**To run tests and demo:**

    cd path_to_build/bin
    ./QtnPropertyTests
    ./QtnPropertyDemo

QtnProperty project consisit of five submodules:

1. **QtnPropertyCore** library - property classes (non GUI stuff)
2. **QtnPropertyWidget** library - QtnPropertyWidget, QtnPropertyView and property delegates (GUI stuff)
3. **QtnPEG** tool - executable to generate C++ code for propert sets from simple QML like files (*.pef files)
4. **QtnPropertyTests** - tests for QtnPropertyCore library
5. **QtnPropertyDemo** - demo application

#How to use

1. Write *.pef file with propertyset declaration. For example TextEditor.pef:
  
```C++
#include "Core/PropertyCore.h"

property_set TextEditor
{
    Bool enableWrapping
    {
        description = "Enable/disable text wrapping";
        value = true;
    }
    
    Bool replaceTabsWithSpaces
    {
        description = "Automatically replace tabs with spaces";
        value = false;
            
        slot propertyDidChange
        {
            tabSize.switchState(QtnPropertyStateImmutable, !replaceTabsWithSpaces);
        }
    }
    
    UInt tabSize
    {
        description = "Number of spaces to be placed.";
        state = QtnPropertyStateImmutable;
        value = 4;
    }
}
```
    
2. Generate C++ classes by running command (to use *.pef files in your project see [PEG.pri](PEG.pri))

    ./QtnPEG TextEditor.pef
    
3. Include generated TextEditor.peg.h and TextEditor.peg.cpp files into 
your project.
4. Now you can use QtnPropertySetTextEditor class (defined in generated files) in your C++ code like this:
```C++
    QtnPropertySetTextEditor params;
    params.enableWrapping = false;
    if (params.replaceTabsWithSpaces)
        document.replaceTabsWithSpaces(params.tabSize);
```

Video of GUI testing using Froglogic (c) Squish test framework is [here](https://www.youtube.com/watch?v=lCmM13vPWBU).

