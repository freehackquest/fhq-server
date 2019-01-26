# Contributing

### Pull request

Don't big pull requests

### Code Format

#### Name Variables
 
 - use "Hungarian Notation" exclude variable "i" in cicles
 - use lowercamelcase

Examples:

 - `int nMaxSize;`
 - `std::string sName;`
 - `SomeClass *pSize = new SomeClass();`

#### Intents

Intents: 4 whitespace === tab (but don't use tab)

#### Format of if, for and etc.

Examples:

```
if (some != some2) {
    // some  
}

for (int i = 0; i < 10; i++) {
    // some
}
```

Name of methods of functions must be in lowercamelcase, example:

```
void someMethodWhichDo(int nSize) {
    // some
}
void someMethodWhichDo(const std::string &sStr) {
    // some
}

```

#### Format of classes

- name of classes format in camelcase
- class members with prefix "m_" and then Hungarian Notation
- private methods must has suffix "_"
- overrided not need add "override" attribute;

Example:

```
class SomeClass 
  : public ISomeClass {
public:
    SomeClass();
    void someMethod1(const std::string &sStr);
    virtual std::string someMethod3();

private:
    void _someMethod1();
    int m_nSize;
}
```


# https://tproger.ru/translations/stanford-cpp-style-guide/



```
// bad
if (size == 0) return;
else
    for (int i = 0; i < 10; i++) cout << "ok" << endl;

// nice
if (size == 0) {
    return;
} else {
    for (int i = 0; i < 10; i++) {
        cout << "ok" << endl;
    }
}
```
