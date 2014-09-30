#include <iostream>
#include <dmtx.hpp>

int main(int argc, char** argv) {
    using dmtx::pack;
    
    std::uint8_t data[256];
    dmtx::image img(data, 16, 16, pack::k8);
    dmtx::decoder dec(img);
    while (auto reg = dec.next_region())
    {
        if (auto msg = dec.decode(reg))
        {
            std::cout << "output: \"";
            std::cout.write((char const*)msg->output, msg->outputIdx);
            std::cout << "\"\n";
        }
    }
    dmtx::decoder(img).swap(dec);
    
	return 0;
}
