/// A library that needs a Python interface.
///
/// This is regular C++ code that will be exported via the Python C API. In a
/// real application this can be any external library.
///
#include <string>


namespace cpplib
{
    /// Return a greeting.
    ///
    std::string hello()
    {
        return "Greetings from C++!";
    }

    /// A personalized greeting.
    ///
    class Greeting
    {
    public:
        Greeting(const std::string& name) : name(name) {}
        std::string hello() const {
            return "Greetings from C++, " + name + "!";
        }

    private:
        const std::string name;
    };
}
