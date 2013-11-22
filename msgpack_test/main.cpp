#include <iostream>
#include <string>
#include <list>
#include <set>
#include <map>
#include <vector>

#include <msgpack.hpp>

class myclass {
public:
    std::string m_str;
    std::vector<int> m_vec;
    MSGPACK_DEFINE(m_str, m_vec);
};

int main() {
    // This is target object.
    std::vector<std::string> target;
    target.push_back("Hello,");
    target.push_back("World!");

    // Serialize it.
    msgpack::sbuffer sbuf;  // simple buffer
    msgpack::pack(&sbuf, target);

    // Deserialize the serialized data.
    msgpack::unpacked msg;    // includes memory pool and deserialized object
    msgpack::unpack(&msg, sbuf.data(), sbuf.size());
    msgpack::object obj = msg.get();

    // Print the deserialized object to stdout.
    std::cout << obj << std::endl;    // ["Hello," "World!"]
    

    // Convert the deserialized object to staticaly typed object.
    std::vector<std::string> result;
    obj.convert(&result);

    for (size_t i = 0; i < result.size(); ++i) {
        std::cout << result.at(i) << std::endl;
    }

    // If the type is mismatched, it throws msgpack::type_error.
//    obj.as<int>();  // type is mismatched, msgpack::type_error is thrown
    obj.as<std::vector<std::string> >();

    msgpack::sbuffer buffer;
    msgpack::packer<msgpack::sbuffer> pk(&buffer);
    pk.pack(100);
    pk.pack_array(3);
    pk.pack(std::string("hello, first time1!"));
    pk.pack(22.22);
    pk.pack(std::string("hello, first time2!"));
    pk.pack(std::string("hello, first time3!"));
    pk.pack(99.99);
    pk.pack(std::string("hello, first time!"));
    pk.pack_map(3);
    pk.pack(3);
    pk.pack(std::string("x"));
    pk.pack(3);
    pk.pack(std::string("y"));
    pk.pack(std::string("y"));
    pk.pack(3.4321);

    msgpack::unpacker upr;
    upr.reserve_buffer(buffer.size());
    memcpy(upr.buffer(), buffer.data(), buffer.size());
    upr.buffer_consumed(buffer.size());

    msgpack::unpacked upk;
    while (upr.next(&upk)) {
        std::cout << upk.get() << std::endl;
    }

    std::vector<myclass> vec;
    for (int i = 1; i < 10; i++) {
        myclass mc;
        mc.m_str = "uqee";
        mc.m_vec.push_back(1);
        mc.m_vec.push_back(2);
        vec.push_back(mc);
    }
    msgpack::sbuffer sbuf1;
    msgpack::pack(sbuf1, vec);

    msgpack::unpacked msg1;
    msgpack::unpack(&msg1, sbuf1.data(), sbuf1.size());
    msgpack::object obj1 = msg1.get();
    vec.clear();
    obj1.convert(&vec);
    for (size_t i = 0; i < vec.size(); ++i) {
        std::cout << vec.at(i).m_str << std::endl;
        std::cout << vec.at(i).m_vec[0] << ' ' << vec.at(i).m_vec[1] << std::endl;
    }
    return 0;
}
