#define DECLARE_ENUM(E) \
struct E \
{ \
public: \
    E(int value = 0) : _value((__Enum)value) { \
    } \
    E& operator=(int value) { \
         _value = (__Enum)value; \
        return *this; \
    } \
    operator int() const { \
        return  _value; \
    } \
\
    enum __Enum {

#define END_ENUM() \
    }; \
\
public: \
    __Enum _value; \
};

#define UNUSED(x) (void)(x)
