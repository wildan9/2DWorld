/* by Wildan R. */

#define GETTERSETTER(type, method, name) inline type Get##method() const { return name; } inline void Set##method(type value) { name = value; }